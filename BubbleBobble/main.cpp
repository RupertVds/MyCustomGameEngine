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
#include "GameObjectCommand.h"
#include "InputManager.h"
#include "XInputController.h"
#include "HealthComponent.h"
#include "PlayerLivesObserver.h"
#include "PlayerPointsObserver.h"
#include "ServiceLocator.h"
#include "SDL_mixer.h"

void load() {
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	auto fontSecret = ResourceManager::GetInstance().LoadFont("Lingua.otf", 12);
	auto& inputManager = InputManager::GetInstance();

	ServiceLocator::RegisterSoundSystem(nullptr);
	ServiceLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SDLSoundSystem>()));

	std::shared_ptr<GameObject> fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent<RenderComponent>();
	fpsObject->AddComponent<TextComponent>(" ", font);
	fpsObject->AddComponent<FPSComponent>();
	fpsObject->SetLocalPosition(glm::vec3{ 20, 0, 0 });
	fpsObject->SetParent(SceneManager::GetInstance().GetRootObject());

	std::shared_ptr<GameObject> controlsText = std::make_shared<GameObject>();
	controlsText->AddComponent<RenderComponent>();
	controlsText->AddComponent<TextComponent>("Use WASD to move green player, press 1-4 to play some different sounds\nUse the D-Pad to move blue player, X,Y,B,A", font);
	controlsText->SetLocalPosition(glm::vec3{ 20, 30, 0 });

	std::shared_ptr<GameObject> secretText = std::make_shared<GameObject>();
	secretText->AddComponent<RenderComponent>();
	
	secretText->AddComponent<TextComponent>("Press 5 for a little secret :)", fontSecret);
	secretText->SetLocalPosition(glm::vec3{ 20, 700, 0 });


	const int playerHealth{ 3 };
	// Player one
	std::shared_ptr<GameObject> playerOneObject = std::make_shared<GameObject>();
	playerOneObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("player_1.png"));
	playerOneObject->AddComponent<PlayerMovementComponent>(200.f);
	playerOneObject->AddComponent<HealthComponent>(playerHealth);
	playerOneObject->AddComponent<PlayerPointsComponent>();
	playerOneObject->SetLocalPosition(glm::vec3{ 1280 / 2 - 50, 720 / 2, 0 });

	//std::shared_ptr<GameObject> playerOneLivesText = std::make_shared<GameObject>();
	//playerOneLivesText->AddComponent<RenderComponent>();
	//playerOneLivesText->AddComponent<TextComponent>("# lives: " + std::to_string(playerHealth), font);
	//playerOneLivesText->SetLocalPosition(glm::vec3{ 20, 200, 0 });

	//std::shared_ptr<GameObject> playerOnePointsText = std::make_shared<GameObject>();
	//playerOnePointsText->AddComponent<RenderComponent>();
	//playerOnePointsText->AddComponent<TextComponent>("Score: 0", font);
	//playerOnePointsText->SetLocalPosition(glm::vec3{ 20, 230, 0 });

	//std::shared_ptr<PlayerLivesObserver> playerLivesObserver = std::make_shared<PlayerLivesObserver>(playerOneLivesText->GetComponent<TextComponent>());
	//std::shared_ptr<PlayerPointsObserver> playerPointsObserver = std::make_shared<PlayerPointsObserver>(playerOnePointsText->GetComponent<TextComponent>());

	//playerOneObject->AddObserver(playerLivesObserver);
	//playerOneObject->AddObserver(playerPointsObserver);

	inputManager.BindInput(SDL_SCANCODE_W, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 }), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_A, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 }), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_S, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 }), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_D, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 }), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_1, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_1.wav", 0.1f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_2, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_2.wav", 0.1f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_3, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_3.wav", 0.1f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_4, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_4.wav", 0.1f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_5, InputBinding{ playerOneObject->AddCommand<PlaySFX>("fart.wav", 0.1f), InputMode::Press});
	//inputManager.BindInput(SDL_SCANCODE_C, InputBinding{ playerOneObject->AddCommand<TakeDamageCommand>(), InputMode::Press });
	//inputManager.BindInput(SDL_SCANCODE_Z, InputBinding{ playerOneObject->AddCommand<AddScore>(10), InputMode::Press });
	//inputManager.BindInput(SDL_SCANCODE_X, InputBinding{ playerOneObject->AddCommand<AddScore>(100), InputMode::Press });

	// Player two

	std::shared_ptr<GameObject> playerTwoObject = std::make_shared<GameObject>();
	playerTwoObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("player_2.png"));
	playerTwoObject->AddComponent <PlayerMovementComponent>(400.f);
	playerTwoObject->AddComponent<HealthComponent>(playerHealth);
	playerTwoObject->AddComponent<PlayerPointsComponent>();
	playerTwoObject->SetLocalPosition(glm::vec3{ 1280 / 2 + 50, 720 / 2, 0 });

	//std::shared_ptr<GameObject> playerTwoLivesText = std::make_shared<GameObject>();
	//playerTwoLivesText->AddComponent<RenderComponent>();
	//playerTwoLivesText->AddComponent<TextComponent>("# lives: " + std::to_string(playerHealth), font);
	//playerTwoLivesText->SetLocalPosition(glm::vec3{ 20, 260, 0 });

	//std::shared_ptr<GameObject> playerTwoPointsText = std::make_shared<GameObject>();
	//playerTwoPointsText->AddComponent<RenderComponent>();
	//playerTwoPointsText->AddComponent<TextComponent>("Score: 0", font);
	//playerTwoPointsText->SetLocalPosition(glm::vec3{ 20, 290, 0 });

	//std::shared_ptr<PlayerLivesObserver> playerLivesObserverTwo = std::make_shared<PlayerLivesObserver>(playerTwoLivesText->GetComponent<TextComponent>());
	//std::shared_ptr<PlayerPointsObserver> playerPointsObserverTwo = std::make_shared<PlayerPointsObserver>(playerTwoPointsText->GetComponent<TextComponent>());

	//playerTwoObject->AddObserver(playerLivesObserverTwo);
	//playerTwoObject->AddObserver(playerPointsObserverTwo);

	inputManager.AddController();
	inputManager.BindInput(0, GAMEPAD_DPAD_UP, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 }), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_DPAD_LEFT, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 }), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_DPAD_DOWN, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 }), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_DPAD_RIGHT, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 }), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_X, InputBinding{ playerTwoObject->AddCommand<PlaySFX>("BubbleBobble_SFX_1.wav", 0.1f), InputMode::Press });
	inputManager.BindInput(0, GAMEPAD_Y, InputBinding{ playerTwoObject->AddCommand<PlaySFX>("BubbleBobble_SFX_2.wav", 0.1f), InputMode::Press });
	inputManager.BindInput(0, GAMEPAD_B, InputBinding{ playerTwoObject->AddCommand<PlaySFX>("BubbleBobble_SFX_3.wav", 0.1f), InputMode::Press });
	inputManager.BindInput(0, GAMEPAD_A, InputBinding{ playerTwoObject->AddCommand<PlaySFX>("BubbleBobble_SFX_4.wav", 0.1f), InputMode::Press });
	//inputManager.BindInput(0, GAMEPAD_X, InputBinding{ playerTwoObject->AddCommand<TakeDamageCommand>(), InputMode::Press });
	//inputManager.BindInput(0, GAMEPAD_A, InputBinding{ playerTwoObject->AddCommand<AddScore>(10), InputMode::Press });
	//inputManager.BindInput(0, GAMEPAD_B, InputBinding{ playerTwoObject->AddCommand<AddScore>(100), InputMode::Press });

	inputManager.AddController();
	inputManager.BindInput(1, GAMEPAD_DPAD_UP, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 }), InputMode::Hold });
	inputManager.BindInput(1, GAMEPAD_DPAD_LEFT, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 }), InputMode::Hold });
	inputManager.BindInput(1, GAMEPAD_DPAD_DOWN, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 }), InputMode::Hold });
	inputManager.BindInput(1, GAMEPAD_DPAD_RIGHT, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 }), InputMode::Hold });
	inputManager.BindInput(1, GAMEPAD_X, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_1.wav", 0.1f), InputMode::Press });
	inputManager.BindInput(1, GAMEPAD_Y, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_2.wav", 0.1f), InputMode::Press });
	inputManager.BindInput(1, GAMEPAD_B, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_3.wav", 0.1f), InputMode::Press });
	inputManager.BindInput(1, GAMEPAD_A, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_4.wav", 0.1f), InputMode::Press });
	//inputManager.BindInput(1, GAMEPAD_X, InputBinding{ playerOneObject->AddCommand<TakeDamageCommand>(), InputMode::Press });
	//inputManager.BindInput(1, GAMEPAD_A, InputBinding{ playerOneObject->AddCommand<AddScore>(10), InputMode::Press });
	//inputManager.BindInput(1, GAMEPAD_B, InputBinding{ playerOneObject->AddCommand<AddScore>(100), InputMode::Press });

	scene.Add(fpsObject);
	scene.Add(controlsText);
	scene.Add(secretText);

	scene.Add(playerOneObject);
	//scene.Add(playerOneLivesText);
	//scene.Add(playerOnePointsText);

	scene.Add(playerTwoObject);
	//scene.Add(playerTwoLivesText);
	//scene.Add(playerTwoPointsText);
	scene.Add(SceneManager::GetInstance().GetRootObject());
}

int main(int, char* []) {
	Engine engine{"../Data/"};
	engine.Run(load);
	return 0;
}