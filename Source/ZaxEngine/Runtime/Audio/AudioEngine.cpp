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

	void AudioEngine::PlaySound(const char* soundFile, bool loop) {
		if (initResult != MA_SUCCESS) {
			std::cerr << "Audio engine not initialized" << std::endl;
			return;
		}

		ma_uint32 flags = 0;
		if (loop) {
			flags |= MA_SOUND_FLAG_LOOPING;
		}
		ma_sound* sound = new ma_sound();
		ma_result result = ma_sound_init_from_file(&engine, soundFile, flags, nullptr, nullptr, sound);
		if (result != MA_SUCCESS) {
			std::cerr << "Failed to load sound for looping (" << soundFile << "): " << result << std::endl;
			return;
		}
		
		if (loop) {
			ma_sound_set_looping(sound, MA_TRUE);
			result = ma_sound_start(sound);
			if (result != MA_SUCCESS) {
				std::cerr << "Failed to start looping sound (" << soundFile << "): " << result << std::endl;
				ma_sound_uninit(sound);
			} else {
				// Store the sound to keep it alive for looping
				loopingSounds.push_back(sound);
				std::cout << "Looping sound started successfully: " << soundFile << std::endl;
			}
		} else {
			result = ma_sound_start(sound);
			if (result != MA_SUCCESS) {
				std::cerr << "Failed to play sound: " << result << std::endl;
			}
		}
	}

	void AudioEngine::Shutdown() {
		if (initResult == MA_SUCCESS) {
			// Clean up all looping sounds
			for (auto sound : loopingSounds) {
				ma_sound_uninit(sound);
				delete sound;
			}
			loopingSounds.clear();
			
			ma_engine_uninit(&engine);
			initResult = MA_ERROR;
		}
	}
	
}
