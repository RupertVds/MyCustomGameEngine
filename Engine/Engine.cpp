#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Engine.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Timer.h"
#include <thread>
#include <algorithm>
#include "ServiceLocator.h"
#pragma warning (push)
#pragma warning (disable: 4996)
//#include <steamtypes.h>
#pragma warning (pop)

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

Engine::Engine(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	auto& renderer = Renderer::GetInstance();

	Uint32 flags{};
	if (Renderer::FULLSCREEN)
	{
		flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	else
	{
		flags = SDL_WINDOW_OPENGL;
	}

	g_window = SDL_CreateWindow(
		"2GD18 Rupert Vanderstappen",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		renderer.WIDTH,
		renderer.HEIGHT,
		flags
	);


	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	renderer.Init(g_window);
	ResourceManager::GetInstance().Init(dataPath);
}

Engine::~Engine()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void Engine::Run(const std::function<void()>& load)
{
	ServiceLocator::RegisterSoundSystem(nullptr);

	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& timer = Timer::GetInstance();
	
	constexpr bool useVsync{ true };
	constexpr bool capFps{ true };
	constexpr float targetFps{ 144.f };
	constexpr double targetFrameDuration = 1.0 / (targetFps / 2);
	float lag = 0.0f;

	SDL_GL_SetSwapInterval(useVsync);   

	bool doContinue = true;
	while (doContinue)
	{
		timer.Update();
		lag += timer.GetDeltaTime();

		doContinue = input.ProcessInput();

		while (lag >= timer.GetFixedTimeStep())
		{
			sceneManager.FixedUpdate();
			lag -= timer.GetFixedTimeStep();
		}

		sceneManager.Update();

		sceneManager.LateUpdate();

		renderer.Render();

		if (capFps)
		{
			// Use vsync or manual fps limiting with target fps
			if (!useVsync)
			{
				// Calculate the sleep duration to achieve the target frame rate
				const double sleepDuration = targetFrameDuration - timer.GetDeltaTime();;

				// If the actual frame duration is less than the target, sleep for the remaining time
				if (sleepDuration > 0.0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(sleepDuration * 1000)));
				}
			}
		}
	}
}
