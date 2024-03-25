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

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);

	std::shared_ptr<GameObject> fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent<RenderComponent>();
	fpsObject->AddComponent<TextComponent>(" ", font);
	fpsObject->AddComponent<FPSComponent>();
	fpsObject->SetLocalPosition(glm::vec3{ 20, 0, 0 });
	fpsObject->SetParent(SceneManager::GetInstance().GetRootObject());

	//std::shared_ptr<Achievements> achievements = std::make_shared<Achievements>();


	const int playerHealth{ 3 };
	std::shared_ptr<GameObject> playerOneObject = std::make_shared<GameObject>();
	playerOneObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("player_1.png"));
	playerOneObject->AddComponent<PlayerMovementComponent>(200.f);
	playerOneObject->AddComponent<HealthComponent>(playerHealth);
	playerOneObject->SetLocalPosition(glm::vec3{ 1280 / 2 - 50, 720 / 2, 0 });

	std::shared_ptr<GameObject> playerOneLivesText = std::make_shared<GameObject>();
	playerOneLivesText->AddComponent<RenderComponent>();
	playerOneLivesText->AddComponent<TextComponent>("# lives: " + std::to_string(playerHealth), font);
	playerOneLivesText->SetLocalPosition(glm::vec3{ 20, 100, 0 });

	std::shared_ptr<PlayerLivesObserver> playerLivesObserver = std::make_shared<PlayerLivesObserver>(playerOneLivesText->GetComponent<TextComponent>());

	playerOneObject->AddObserver(playerLivesObserver);



	InputManager::GetInstance().BindInput(SDL_SCANCODE_W, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 }), InputMode::Hold });
	InputManager::GetInstance().BindInput(SDL_SCANCODE_A, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 }), InputMode::Hold });
	InputManager::GetInstance().BindInput(SDL_SCANCODE_S, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 }), InputMode::Hold });
	InputManager::GetInstance().BindInput(SDL_SCANCODE_D, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 }), InputMode::Hold });
	InputManager::GetInstance().BindInput(SDL_SCANCODE_SPACE, InputBinding{ playerOneObject->AddCommand<TakeDamageCommand>(), InputMode::Press });

	std::shared_ptr<GameObject> playerTwoObject = std::make_shared<GameObject>();
	playerTwoObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("player_2.png"));
	playerTwoObject->AddComponent <PlayerMovementComponent>(400.f);
	playerTwoObject->AddComponent<HealthComponent>(3);
	playerTwoObject->SetLocalPosition(glm::vec3{ 1280 / 2 + 50, 720 / 2, 0 });
	////playerTwoObject->SetParent(playerOneObject);

	InputManager::GetInstance().AddController();
	InputManager::GetInstance().BindInput(0, GAMEPAD_DPAD_UP, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 }), InputMode::Hold });
	InputManager::GetInstance().BindInput(0, GAMEPAD_DPAD_LEFT, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 }), InputMode::Hold });
	InputManager::GetInstance().BindInput(0, GAMEPAD_DPAD_DOWN, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 }), InputMode::Hold });
	InputManager::GetInstance().BindInput(0, GAMEPAD_DPAD_RIGHT, InputBinding{ playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 }), InputMode::Hold });

	InputManager::GetInstance().AddController();
	InputManager::GetInstance().BindInput(1, GAMEPAD_DPAD_UP, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 }), InputMode::Hold });
	InputManager::GetInstance().BindInput(1, GAMEPAD_DPAD_LEFT, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 }), InputMode::Hold });
	InputManager::GetInstance().BindInput(1, GAMEPAD_DPAD_DOWN, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 }), InputMode::Hold });
	InputManager::GetInstance().BindInput(1, GAMEPAD_DPAD_RIGHT, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 }), InputMode::Hold });

	scene.Add(playerOneObject);
	scene.Add(playerTwoObject);
	scene.Add(fpsObject);
	scene.Add(playerOneLivesText);
	scene.Add(SceneManager::GetInstance().GetRootObject());
}

int main(int, char*[]) 
{
	Engine engine("../Data/");
	engine.Run(load);
    return 0;
}
