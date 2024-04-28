#include "SoundSystem.h"
#include "SDL_mixer.h"

SoundSystem::~SoundSystem()
{
}

SDLSoundSystem::~SDLSoundSystem()
{
	m_IsRunning = false;
	if (m_EventThread.joinable()) {
        m_EventThread.join();
	}

    // Free all loaded sound chunks
    for (auto& pair : m_LoadedChunks) {
        Mix_FreeChunk(pair.second);
    }

    // Clear the map
    m_LoadedChunks.clear();

    // Shutdown SDL_mixer
    Mix_CloseAudio();
}

