#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <SDL.h>
#include "Engine.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "CircularMovementComponent.h"
#include "GameCommands.h"
#include "InputManager.h"
#include "XInputController.h"
#include "HealthComponent.h"
#include "PlayerLivesObserver.h"
#include "PlayerPointsObserver.h"
#include "ServiceLocator.h"

void load() {
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	auto& inputManager = InputManager::GetInstance();

	ServiceLocator::RegisterSoundSystem(nullptr);
	
#if _DEBUG
	ServiceLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SDLSoundSystem>()));
#else
	ServiceLocator::RegisterSoundSystem(std::make_unique<SDLSoundSystem>());
#endif

	std::shared_ptr<GameObject> fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent<RenderComponent>();
	fpsObject->AddComponent<TextComponent>(" ", font);
	fpsObject->AddComponent<FPSComponent>();
	fpsObject->SetLocalPosition(glm::vec3{ 20, 0, 0 });	
	fpsObject->SetParent(SceneManager::GetInstance().GetRootObject());

	std::shared_ptr<GameObject> controlsText = std::make_shared<GameObject>();
	controlsText->AddComponent<RenderComponent>();
	controlsText->AddComponent<TextComponent>("Use WASD to move green player, press 1-9 to play some different sounds\nUse the D-Pad to move blue player, X,Y,B,A\nPress 0 to load and play a longer audio file to test threading (no obvious fps drop)", font);
	controlsText->SetLocalPosition(glm::vec3{ 20, 30, 0 });

	// Player one
	std::shared_ptr<GameObject> playerOneObject = std::make_shared<GameObject>();
	playerOneObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("player_1.png"));
	playerOneObject->AddComponent<PlayerMovementComponent>(200.f);
	playerOneObject->SetLocalPosition(glm::vec3{ 1280 / 2 - 50, 720 / 2, 0 });
	playerOneObject->SetScale({ 2.f, 2.f, 2.f });

	inputManager.BindInput(SDL_SCANCODE_W, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 }), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_A, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 }), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_S, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 }), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_D, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 }), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_0, InputBinding{ playerOneObject->AddCommand<PlaySFX>("Mega_Man_3_The_Passing_of_the_Blue_Crown_OC_ReMix.mp3", 0.1f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_1, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_1.wav", 0.2f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_2, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_2.wav", 0.2f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_3, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_3.wav", 0.2f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_4, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_4.wav", 0.2f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_5, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_5.wav", 0.2f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_6, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_6.wav", 0.2f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_7, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_7.wav", 0.2f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_8, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_8.wav", 0.2f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_9, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_9.wav", 0.2f), InputMode::Press});

	// Player two
	std::shared_ptr<GameObject> playerTwoObject = std::make_shared<GameObject>();
	playerTwoObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("player_2.png"));
	playerTwoObject->AddComponent <PlayerMovementComponent>(400.f);
	playerTwoObject->SetLocalPosition(glm::vec3{ 1280 / 2 + 50, 720 / 2, 0 });

	inputManager.AddController();
	inputManager.BindInput(0, GAMEPAD_DPAD_UP, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 }), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_DPAD_LEFT, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 }), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_DPAD_DOWN, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 }), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_DPAD_RIGHT, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 }), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_X, InputBinding{ playerTwoObject->AddCommand<PlaySFX>("BubbleBobble_SFX_1.wav", 0.2f), InputMode::Press });
	inputManager.BindInput(0, GAMEPAD_Y, InputBinding{ playerTwoObject->AddCommand<PlaySFX>("BubbleBobble_SFX_2.wav", 0.2f), InputMode::Press });
	inputManager.BindInput(0, GAMEPAD_B, InputBinding{ playerTwoObject->AddCommand<PlaySFX>("BubbleBobble_SFX_3.wav", 0.2f), InputMode::Press });
	inputManager.BindInput(0, GAMEPAD_A, InputBinding{ playerTwoObject->AddCommand<PlaySFX>("BubbleBobble_SFX_4.wav", 0.2f), InputMode::Press });

	inputManager.AddController();
	inputManager.BindInput(1, GAMEPAD_DPAD_UP, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 }), InputMode::Hold });
	inputManager.BindInput(1, GAMEPAD_DPAD_LEFT, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 }), InputMode::Hold });
	inputManager.BindInput(1, GAMEPAD_DPAD_DOWN, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 }), InputMode::Hold });
	inputManager.BindInput(1, GAMEPAD_DPAD_RIGHT, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 }), InputMode::Hold });
	inputManager.BindInput(1, GAMEPAD_X, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_1.wav", 0.2f), InputMode::Press });
	inputManager.BindInput(1, GAMEPAD_Y, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_2.wav", 0.2f), InputMode::Press });
	inputManager.BindInput(1, GAMEPAD_B, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_3.wav", 0.2f), InputMode::Press });
	inputManager.BindInput(1, GAMEPAD_A, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_4.wav", 0.2f), InputMode::Press });
	
	scene.Add(fpsObject);
	scene.Add(controlsText);
	scene.Add(playerOneObject);
	scene.Add(playerTwoObject);
	scene.Add(SceneManager::GetInstance().GetRootObject());
}

int main(int, char* []) {
	Engine engine{"../Data/"};
	engine.Run(load);
	return 0;
}