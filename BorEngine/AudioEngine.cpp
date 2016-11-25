#include "AudioEngine.h"
#include "BorEngineErrors.h"

namespace BorEngine {

	void SoundEffect::play(int loops)
	{
		//channel: -1 picks the first channel open (avaiable)
		//loops: -1 loops forever, 0 plays once, 1 loops once (after the first play)
		// Mix_PlayChannel returns the channel on wich it's playing
		if (Mix_PlayChannel(-1, p_chunk, loops) == -1)
		{
			if (Mix_PlayChannel(0, p_chunk, loops) == -1)
			{
				fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
			}
		}
	}
	void Music::play(int loops)
	{
		// here if loops==0 it means that music will be played 0 times!
		if (Mix_PlayMusic(p_music, loops) == -1)
		{
			fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
		}
	}
	void Music::pause()
	{
		Mix_PauseMusic();
	}
	void Music::stop()
	{
		Mix_HaltMusic();
	}
	void Music::resume()
	{
		Mix_ResumeMusic();
	}

	AudioEngine::AudioEngine()
	{

	}

	AudioEngine::~AudioEngine()
	{
		destroy();
	}

	void AudioEngine::init() {
		if (p_isInitialized)
		{
			fatalError("Tried to initialize AudioEngine twice!\n");
		}

		if (Mix_Init(MIX_INIT_OGG) == -1)
		{
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
		}

		p_isInitialized = true;
	}

	void AudioEngine::destroy()
	{
		if (p_isInitialized)
		{
			p_isInitialized = false;

			for (auto& it : p_effectMap)
			{
				Mix_FreeChunk(it.second);
			}
			for (auto& it : p_musicMap)
			{
				Mix_FreeMusic(it.second);
			}

			p_effectMap.clear();
			p_musicMap.clear();

			Mix_CloseAudio();
			Mix_Quit();
		}
	}

	SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath)
	{
		auto it = p_effectMap.find(filePath);
		SoundEffect effect;
		if (it == p_effectMap.end())
		{
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr)
			{
				fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
			}
			effect.p_chunk = chunk;
			p_effectMap[filePath] = chunk;
		}
		else
		{
			effect.p_chunk = it->second;
		}
		return effect;
	}

	Music AudioEngine::loadMusic(const std::string& filePath)
	{
		auto it = p_musicMap.find(filePath);
		Music music;
		if (it == p_musicMap.end())
		{
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			if (mixMusic == nullptr)
			{
				fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
			}
			music.p_music = mixMusic;
			p_musicMap[filePath] = mixMusic;
		}
		else
		{
			music.p_music = it->second;
		}
		return music;
	}

}
