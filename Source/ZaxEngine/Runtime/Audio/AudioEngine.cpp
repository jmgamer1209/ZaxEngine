#define MINIAUDIO_IMPLEMENTATION 
#include "AudioEngine.h"
#include <iostream>

namespace ZaxEngine::Audio {
	
	AudioEngine* AudioEngine::instance = nullptr;

	AudioEngine::AudioEngine() : initResult(MA_SUCCESS) {
	}

	AudioEngine::~AudioEngine() {
		Shutdown();
	}

	AudioEngine& AudioEngine::GetInstance() {
		if (instance == nullptr) {
			instance = new AudioEngine();
			instance->Init();
		}
		return *instance;
	}

	void AudioEngine::Init() {
		ma_engine_config config = ma_engine_config_init();
		initResult = ma_engine_init(&config, &engine);
		
		if (initResult != MA_SUCCESS) {
			std::cerr << "Failed to initialize audio engine: " << initResult << std::endl;
		} else {
			std::cout << "Audio engine initialized successfully" << std::endl;
		}
	}

	/// <summary>
	/// 因为 ma_engine_play_sound 没有处理 loop 音频，所以这里在此函数实现的基础上，增加 loop 音频处理
	/// </summary>
	/// <param name="pEngine"></param>
	/// <param name="pFilePath"></param>
    /// <param name="loop">是否是循环音乐</param>
	/// <param name="pNode">输出节点，类似于音频后处理</param>
	/// <param name="nodeInputBusIndex"></param>
	/// <returns></returns>
	ma_result MA_PlaySound(ma_engine* pEngine, const char* pFilePath, bool loop, ma_node* pNode, ma_uint32 nodeInputBusIndex)
	{
        ma_result result = MA_SUCCESS;
        ma_sound_inlined* pSound = NULL;
        ma_sound_inlined* pNextSound = NULL;

        if (pEngine == NULL || pFilePath == NULL) {
            return MA_INVALID_ARGS;
        }

        /* Attach to the endpoint node if nothing is specified. */
        if (pNode == NULL) {
            pNode = ma_node_graph_get_endpoint(&pEngine->nodeGraph);
            nodeInputBusIndex = 0;
        }

        /*
        We want to check if we can recycle an already-allocated inlined sound. Since this is just a
        helper I'm not *too* concerned about performance here and I'm happy to use a lock to keep
        the implementation simple. Maybe this can be optimized later if there's enough demand, but
        if this function is being used it probably means the caller doesn't really care too much.

        What we do is check the atEnd flag. When this is true, we can recycle the sound. Otherwise
        we just keep iterating. If we reach the end without finding a sound to recycle we just
        allocate a new one. This doesn't scale well for a massive number of sounds being played
        simultaneously as we don't ever actually free the sound objects. Some kind of garbage
        collection routine might be valuable for this which I'll think about.
        */
        ma_spinlock_lock(&pEngine->inlinedSoundLock);
        {
            ma_uint32 soundFlags = 0;

            // 1.找到一个可以回收的 sound，如果没有，就分配一个新的
            for (pNextSound = pEngine->pInlinedSoundHead; pNextSound != NULL; pNextSound = pNextSound->pNext) {
                if (ma_sound_at_end(&pNextSound->sound) && !ma_sound_is_looping(&pNextSound->sound)) {     // loop 检查
                    /*
                    The sound is at the end which means it's available for recycling. All we need to do
                    is uninitialize it and reinitialize it. All we're doing is recycling memory.
                    */
                    pSound = pNextSound;
                    ma_atomic_fetch_sub_32(&pEngine->inlinedSoundCount, 1);
                    break;
                }
            }
            if (pSound != NULL) {
                /*
                We actually want to detach the sound from the list here. The reason is because we want the sound
                to be in a consistent state at the non-recycled case to simplify the logic below.
                */
                if (pEngine->pInlinedSoundHead == pSound) {
                    pEngine->pInlinedSoundHead = pSound->pNext;
                }

                if (pSound->pPrev != NULL) {
                    pSound->pPrev->pNext = pSound->pNext;
                }
                if (pSound->pNext != NULL) {
                    pSound->pNext->pPrev = pSound->pPrev;
                }

                /* Now the previous sound needs to be uninitialized. */
                ma_sound_uninit(&pNextSound->sound);
            }
            else {
                /* No sound available for recycling. Allocate one now. */
                pSound = (ma_sound_inlined*)ma_malloc(sizeof(*pSound), &pEngine->allocationCallbacks);
            }

            // 2.加载并设置 sound
            if (pSound != NULL) {   /* Safety check for the allocation above. */
                /*
                At this point we should have memory allocated for the inlined sound. We just need
                to initialize it like a normal sound now.
                */
                soundFlags |= MA_SOUND_FLAG_ASYNC;                 /* For inlined sounds we don't want to be sitting around waiting for stuff to load so force an async load. */
                soundFlags |= MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT; /* We want specific control over where the sound is attached in the graph. We'll attach it manually just before playing the sound. */
                soundFlags |= MA_SOUND_FLAG_NO_PITCH;              /* Pitching isn't usable with inlined sounds, so disable it to save on speed. */
                soundFlags |= MA_SOUND_FLAG_NO_SPATIALIZATION;     /* Not currently doing spatialization with inlined sounds, but this might actually change later. For now disable spatialization. Will be removed if we ever add support for spatialization here. */
                if (loop)
                {
                    soundFlags |= MA_SOUND_FLAG_LOOPING;
                }

                result = ma_sound_init_from_file(pEngine, pFilePath, soundFlags, NULL, NULL, &pSound->sound);
                if (result == MA_SUCCESS) {
                    /* Now attach the sound to the graph. */
                    result = ma_node_attach_output_bus(pSound, 0, pNode, nodeInputBusIndex);
                    if (result == MA_SUCCESS) {
                        /* At this point the sound should be loaded and we can go ahead and add it to the list. The new item becomes the new head. */
                        pSound->pNext = pEngine->pInlinedSoundHead;
                        pSound->pPrev = NULL;

                        pEngine->pInlinedSoundHead = pSound;    /* <-- This is what attaches the sound to the list. */
                        if (pSound->pNext != NULL) {
                            pSound->pNext->pPrev = pSound;
                        }
                    }
                    else {
                        ma_free(pSound, &pEngine->allocationCallbacks);
                    }
                }
                else {
                    ma_free(pSound, &pEngine->allocationCallbacks);
                }
            }
            else {
                result = MA_OUT_OF_MEMORY;
            }
        }
        ma_spinlock_unlock(&pEngine->inlinedSoundLock);

        if (result != MA_SUCCESS) {
            return result;
        }

        // 3.播放 sound
        result = ma_sound_start(&pSound->sound);
        if (result != MA_SUCCESS) {
            /* Failed to start the sound. We need to mark it for recycling and return an error. */
            ma_atomic_exchange_32(&pSound->sound.atEnd, MA_TRUE);
            return result;
        }

        ma_atomic_fetch_add_32(&pEngine->inlinedSoundCount, 1);
        return result;
	}

	void AudioEngine::PlaySound(const char* soundFile, bool loop) {
        MA_PlaySound(&this->engine, soundFile, loop, nullptr, 0);
	}

	void AudioEngine::Shutdown() {
		if (initResult == MA_SUCCESS) {
			ma_engine_uninit(&engine);
			initResult = MA_ERROR;
		}
	}
	
}
