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


void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	
	//std::shared_ptr<GameObject> backgroundObject = std::make_unique<GameObject>();
	//backgroundObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("background.tga"));
	//glm::vec2 bOSize = backgroundObject->GetComponent<RenderComponent>()->GetTexture()->GetSize();
	//backgroundObject->SetLocalPosition(glm::vec3{ 640 / 2 - bOSize.x / 2, 480 / 2 - bOSize.y / 2, 0 });
	//backgroundObject->SetParent(SceneManager::GetInstance().GetRootObject());

	std::shared_ptr<GameObject> fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent<RenderComponent>();
	fpsObject->AddComponent<TextComponent>(" ", font);
	fpsObject->AddComponent<FPSComponent>();
	fpsObject->SetLocalPosition(glm::vec3{ 0, 0, 0 });
	fpsObject->SetParent(SceneManager::GetInstance().GetRootObject());

	std::shared_ptr<GameObject> playerOneObject = std::make_shared<GameObject>();
	playerOneObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("player_1.png"));
	playerOneObject->AddComponent<PlayerMovementComponent>(200.f);
	playerOneObject->SetLocalPosition(glm::vec3{ 1280 / 2 - 50, 720 / 2, 0 });

	MoveCommand* moveUpCommand = playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 });
	MoveCommand* moveLeftCommand = playerOneObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 });
	MoveCommand* moveDownCommand = playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 });
	MoveCommand* moveRightCommand = playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 });

	InputManager::GetInstance().BindInput(SDL_SCANCODE_W, [moveUpCommand]() { moveUpCommand->Execute(); }, InputManager::InputMode::Hold);
	InputManager::GetInstance().BindInput(SDL_SCANCODE_A, [moveLeftCommand]() { moveLeftCommand->Execute(); }, InputManager::InputMode::Hold);
	InputManager::GetInstance().BindInput(SDL_SCANCODE_S, [moveDownCommand]() { moveDownCommand->Execute(); }, InputManager::InputMode::Hold);
	InputManager::GetInstance().BindInput(SDL_SCANCODE_D, [moveRightCommand]() { moveRightCommand->Execute(); }, InputManager::InputMode::Hold);

	std::shared_ptr<GameObject> playerTwoObject = std::make_shared<GameObject>();
	playerTwoObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("player_2.png"));
	playerTwoObject->AddComponent <PlayerMovementComponent>(400.f);
	playerTwoObject->SetLocalPosition(glm::vec3{ 1280 / 2 + 50, 720 / 2, 0 });
	//playerTwoObject->SetParent(playerOneObject);

	moveUpCommand = playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 });
	moveLeftCommand = playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 });
	moveDownCommand = playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 });
	moveRightCommand = playerTwoObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 });

	InputManager::GetInstance().BindInput(XINPUT_GAMEPAD_DPAD_UP, [moveUpCommand]() { moveUpCommand->Execute(); }, InputManager::InputMode::Hold);
	InputManager::GetInstance().BindInput(XINPUT_GAMEPAD_DPAD_LEFT, [moveLeftCommand]() { moveLeftCommand->Execute(); }, InputManager::InputMode::Hold);
	InputManager::GetInstance().BindInput(XINPUT_GAMEPAD_DPAD_DOWN, [moveDownCommand]() { moveDownCommand->Execute(); }, InputManager::InputMode::Hold);
	InputManager::GetInstance().BindInput(XINPUT_GAMEPAD_DPAD_RIGHT, [moveRightCommand]() { moveRightCommand->Execute(); }, InputManager::InputMode::Hold);

	scene.Add(playerOneObject);
	scene.Add(playerTwoObject);
	scene.Add(fpsObject);
	scene.Add(SceneManager::GetInstance().GetRootObject());
}

int main(int, char*[]) 
{
	Engine engine("../Data/");
	engine.Run(load);
    return 0;
}
