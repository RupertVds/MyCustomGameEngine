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

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	
	auto backgroundObject = std::make_shared<GameObject>();
	backgroundObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("background.tga"));
	glm::vec2 bOSize = backgroundObject->GetComponent<RenderComponent>()->GetTexture()->GetSize();
	backgroundObject->SetLocalPosition(glm::vec3{ 640 / 2 - bOSize.x / 2, 480 / 2 - bOSize.y / 2, 0 });
	scene.Add(backgroundObject);
	auto fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent<RenderComponent>();
	fpsObject->AddComponent<TextComponent>(" ", font);
	fpsObject->AddComponent<FPSComponent>();
	fpsObject->SetLocalPosition(glm::vec3{ 0, 0, 0 });
	scene.Add(fpsObject);

	auto spriteObject = std::make_shared<GameObject>();
	spriteObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("test_sprite.png"));
	spriteObject->AddComponent<CircularMovementComponent>(60.f, 1.f, glm::vec3{ 640 / 2 - 110, 480 / 2, 0});
	scene.Add(spriteObject);

	auto spriteObject2 = std::make_shared<GameObject>();
	spriteObject2->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("test_sprite.png"));
	spriteObject2->AddComponent<CircularMovementComponent>(100.f, 5.f, glm::vec3{0, 0, 0});
	spriteObject2->SetParent(spriteObject);
	scene.Add(spriteObject2);

	auto spriteObject3 = std::make_shared<GameObject>();
	spriteObject3->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("test_sprite.png"));
	spriteObject3->AddComponent<CircularMovementComponent>(40.f, 4.f, glm::vec3{ 0, 0, 0 });
	spriteObject3->SetParent(spriteObject2);
	scene.Add(spriteObject3);

	spriteObject2->SetParent(nullptr);
	spriteObject2->SetParent(spriteObject);
	//spriteObject2->DeleteSelf();
}

int main(int, char*[]) 
{
	Engine engine("../Data/");
	engine.Run(load);
    return 0;
}