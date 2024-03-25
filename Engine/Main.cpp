#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

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
#include "Achievements.h"
#include "PlayerLivesObserver.h"
#include "PlayerPointsObserver.h"

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	auto& inputManager = InputManager::GetInstance();

	std::shared_ptr<GameObject> fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent<RenderComponent>();
	fpsObject->AddComponent<TextComponent>(" ", font);
	fpsObject->AddComponent<FPSComponent>();
	fpsObject->SetLocalPosition(glm::vec3{ 20, 0, 0 });
	fpsObject->SetParent(SceneManager::GetInstance().GetRootObject());

	//std::shared_ptr<Achievements> achievements = std::make_shared<Achievements>();
	std::shared_ptr<GameObject> controlsText = std::make_shared<GameObject>();
	controlsText->AddComponent<RenderComponent>();
	controlsText->AddComponent<TextComponent>("Use WASD to move green player, C to inflict damage, Z and X to pickup points\nUse the D-Pad to move blue player, X to inflict damage, A and B to pick up points", font);
	controlsText->SetLocalPosition(glm::vec3{ 20, 30, 0 });


	const int playerHealth{ 3 };
	// Player one
	std::shared_ptr<GameObject> playerOneObject = std::make_shared<GameObject>();
	playerOneObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("player_1.png"));
	playerOneObject->AddComponent<PlayerMovementComponent>(200.f);
	playerOneObject->AddComponent<HealthComponent>(playerHealth);
	playerOneObject->AddComponent<PlayerPointsComponent>();
	playerOneObject->SetLocalPosition(glm::vec3{ 1280 / 2 - 50, 720 / 2, 0 });

	std::shared_ptr<GameObject> playerOneLivesText = std::make_shared<GameObject>();
	playerOneLivesText->AddComponent<RenderComponent>();
	playerOneLivesText->AddComponent<TextComponent>("# lives: " + std::to_string(playerHealth), font);
	playerOneLivesText->SetLocalPosition(glm::vec3{ 20, 200, 0 });

	std::shared_ptr<GameObject> playerOnePointsText = std::make_shared<GameObject>();
	playerOnePointsText->AddComponent<RenderComponent>();
	playerOnePointsText->AddComponent<TextComponent>("Score: 0", font);
	playerOnePointsText->SetLocalPosition(glm::vec3{ 20, 230, 0 });

	std::shared_ptr<PlayerLivesObserver> playerLivesObserver = std::make_shared<PlayerLivesObserver>(playerOneLivesText->GetComponent<TextComponent>());
	std::shared_ptr<PlayerPointsObserver> playerPointsObserver = std::make_shared<PlayerPointsObserver>(playerOnePointsText->GetComponent<TextComponent>());
	std::shared_ptr<Achievements> achievements = std::make_shared<Achievements>();

	playerOneObject->AddObserver(playerLivesObserver);
	playerOneObject->AddObserver(playerPointsObserver);
	playerOneObject->AddObserver(achievements);

	inputManager.BindInput(SDL_SCANCODE_W, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 }), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_A, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 }), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_S, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 }), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_D, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 }), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_C, InputBinding{ playerOneObject->AddCommand<TakeDamageCommand>(), InputMode::Press });
	inputManager.BindInput(SDL_SCANCODE_Z, InputBinding{ playerOneObject->AddCommand<AddScore>(10), InputMode::Press });
	inputManager.BindInput(SDL_SCANCODE_X, InputBinding{ playerOneObject->AddCommand<AddScore>(100), InputMode::Press });

	// Player two

	std::shared_ptr<GameObject> playerTwoObject = std::make_shared<GameObject>();
	playerTwoObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("player_2.png"));
	playerTwoObject->AddComponent <PlayerMovementComponent>(400.f);
	playerTwoObject->AddComponent<HealthComponent>(playerHealth);
	playerTwoObject->AddComponent<PlayerPointsComponent>();
	playerTwoObject->SetLocalPosition(glm::vec3{ 1280 / 2 + 50, 720 / 2, 0 });

	std::shared_ptr<GameObject> playerTwoLivesText = std::make_shared<GameObject>();
	playerTwoLivesText->AddComponent<RenderComponent>();
	playerTwoLivesText->AddComponent<TextComponent>("# lives: " + std::to_string(playerHealth), font);
	playerTwoLivesText->SetLocalPosition(glm::vec3{ 20, 260, 0 });

	std::shared_ptr<GameObject> playerTwoPointsText = std::make_shared<GameObject>();
	playerTwoPointsText->AddComponent<RenderComponent>();
	playerTwoPointsText->AddComponent<TextComponent>("Score: 0", font);
	playerTwoPointsText->SetLocalPosition(glm::vec3{ 20, 290, 0 });

	std::shared_ptr<PlayerLivesObserver> playerLivesObserverTwo = std::make_shared<PlayerLivesObserver>(playerTwoLivesText->GetComponent<TextComponent>());
	std::shared_ptr<PlayerPointsObserver> playerPointsObserverTwo = std::make_shared<PlayerPointsObserver>(playerTwoPointsText->GetComponent<TextComponent>());

	playerTwoObject->AddObserver(playerLivesObserverTwo);
	playerTwoObject->AddObserver(playerPointsObserverTwo);

	inputManager.AddController();
	inputManager.BindInput(0, GAMEPAD_DPAD_UP, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 }), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_DPAD_LEFT, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 }), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_DPAD_DOWN, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 }), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_DPAD_RIGHT, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 }), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_X, InputBinding{ playerTwoObject->AddCommand<TakeDamageCommand>(), InputMode::Press });
	inputManager.BindInput(0, GAMEPAD_A, InputBinding{ playerTwoObject->AddCommand<AddScore>(10), InputMode::Press });
	inputManager.BindInput(0, GAMEPAD_B, InputBinding{ playerTwoObject->AddCommand<AddScore>(100), InputMode::Press });
	
	inputManager.AddController();
	inputManager.BindInput(1, GAMEPAD_DPAD_UP, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 }), InputMode::Hold });
	inputManager.BindInput(1, GAMEPAD_DPAD_LEFT, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 }), InputMode::Hold });
	inputManager.BindInput(1, GAMEPAD_DPAD_DOWN, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 }), InputMode::Hold });
	inputManager.BindInput(1, GAMEPAD_DPAD_RIGHT, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 }), InputMode::Hold });
	inputManager.BindInput(1, GAMEPAD_X, InputBinding{ playerOneObject->AddCommand<TakeDamageCommand>(), InputMode::Press });
	inputManager.BindInput(1, GAMEPAD_A, InputBinding{ playerOneObject->AddCommand<AddScore>(10), InputMode::Press });
	inputManager.BindInput(1, GAMEPAD_B, InputBinding{ playerOneObject->AddCommand<AddScore>(100), InputMode::Press });

	scene.Add(fpsObject);
	scene.Add(controlsText);

	scene.Add(playerOneObject);
	scene.Add(playerOneLivesText);
	scene.Add(playerOnePointsText);

	scene.Add(playerTwoObject);
	scene.Add(playerTwoLivesText);
	scene.Add(playerTwoPointsText);
	scene.Add(SceneManager::GetInstance().GetRootObject());
}

int main(int, char*[]) 
{
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	else
	{
		std::cout << "Successfully initialized steam." << std::endl;
		g_SteamAchievements = new CSteamAchievements(g_Achievements, 4);
	}

	Engine engine("../Data/");
	engine.Run(load);

	// Shutdown Steam
	SteamAPI_Shutdown();
	// Delete the SteamAchievements object
	if (g_SteamAchievements)
		delete g_SteamAchievements;
    return 0;
}
