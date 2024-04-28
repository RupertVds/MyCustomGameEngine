#include "SoundSystem.h"

SoundSystem::~SoundSystem()
{
}

SDLSoundSystem::~SDLSoundSystem()
{
	running_ = false;
	if (eventThread_.joinable()) {
		eventThread_.join();
	}

    // Free all loaded sound chunks
    for (auto& pair : loadedChunks_) {
        Mix_FreeChunk(pair.second);
    }

    // Clear the map
    loadedChunks_.clear();

    // Shutdown SDL_mixer
    Mix_CloseAudio();
}

