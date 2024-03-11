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
	
	std::shared_ptr<GameObject> backgroundObject = std::make_unique<GameObject>();
	backgroundObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("background.tga"));
	glm::vec2 bOSize = backgroundObject->GetComponent<RenderComponent>()->GetTexture()->GetSize();
	backgroundObject->SetLocalPosition(glm::vec3{ 640 / 2 - bOSize.x / 2, 480 / 2 - bOSize.y / 2, 0 });
	backgroundObject->SetParent(SceneManager::GetInstance().GetRootObject());

	std::shared_ptr<GameObject> fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent<RenderComponent>();
	fpsObject->AddComponent<TextComponent>(" ", font);
	fpsObject->AddComponent<FPSComponent>();
	fpsObject->SetLocalPosition(glm::vec3{ 0, 0, 0 });
	fpsObject->SetParent(SceneManager::GetInstance().GetRootObject());

	std::shared_ptr<GameObject> rotationPointObject = std::make_shared<GameObject>();
	rotationPointObject->SetLocalPosition(glm::vec3{ 640 / 2, 480 / 2, 0 });

	std::shared_ptr<GameObject> spriteObject = std::make_shared<GameObject>();
	spriteObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("test_sprite.png"));
	spriteObject->SetLocalPosition(glm::vec3{ 640 / 2, 480 / 2, 0 });
	spriteObject->AddComponent<CircularMovementComponent>(60.f, 1.f);
	spriteObject->SetParent(rotationPointObject);

	std::shared_ptr<GameObject> spriteObject2 = std::make_shared<GameObject>();
	spriteObject2->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("test_sprite.png"));
	spriteObject2->AddComponent<CircularMovementComponent>(100.f, 1.f);
	spriteObject2->SetParent(spriteObject);

	std::shared_ptr<GameObject> spriteObject3 = std::make_shared<GameObject>();
	spriteObject3->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("test_sprite.png"));
	spriteObject3->AddComponent<CircularMovementComponent>(75.f, 1.f);
	spriteObject3->SetParent(spriteObject2);

	std::shared_ptr<GameObject> spriteObject4 = std::make_shared<GameObject>();
	spriteObject4->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("test_sprite.png"));
	spriteObject4->AddComponent<CircularMovementComponent>(50.f, 1.f);
	spriteObject4->SetParent(spriteObject2);

	std::shared_ptr<GameObject> spriteObject5 = std::make_shared<GameObject>();
	spriteObject5->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("test_sprite.png"));
	spriteObject5->AddComponent<CircularMovementComponent>(15.f, 3.f);
	spriteObject5->SetParent(SceneManager::GetInstance().GetRootObject());

	scene.Add(backgroundObject);
	scene.Add(fpsObject);
	scene.Add(rotationPointObject);
	scene.Add(SceneManager::GetInstance().GetRootObject());
}

int main(int, char*[]) 
{
	Engine engine("../Data/");
	engine.Run(load);
    return 0;
}
