#pragma once
#include "miniaudio/miniaudio.h"
#include <vector>
#undef PlaySound

namespace ZaxEngine::Audio
{
	class AudioEngine
	{
	public:
		static AudioEngine& GetInstance();
		void Init();
		void PlaySound(const char* soundFile, bool loop = false);
		void Shutdown();

		ma_engine* GetEngine() { return &engine; }

	private:
		static AudioEngine* instance;
		ma_engine engine;
		ma_result initResult;
		
		AudioEngine();
		~AudioEngine();
	};
}


