#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"

bool InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				return false;
			}
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		// process event for IMGUI
		ImGui_ImplSDL2_ProcessEvent(&e);


	}

	return true;
}