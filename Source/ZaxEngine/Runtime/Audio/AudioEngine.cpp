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

	void AudioEngine::PlaySound(const char* soundFile) {
		if (initResult != MA_SUCCESS) {
			std::cerr << "Audio engine not initialized" << std::endl;
			return;
		}
		
		ma_result result = ma_engine_play_sound(&engine, soundFile, nullptr);
		if (result != MA_SUCCESS) {
			std::cerr << "Failed to play sound: " << result << std::endl;
		}
	}

	void AudioEngine::Shutdown() {
		if (initResult == MA_SUCCESS) {
			ma_engine_uninit(&engine);
			initResult = MA_ERROR;
		}
	}
	
}
