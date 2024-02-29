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

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	
	auto backgroundObject = std::make_shared<GameObject>();
	backgroundObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("background.tga"));
	glm::vec2 bOSize = backgroundObject->GetComponent<RenderComponent>()->GetTexture()->GetSize();
	backgroundObject->SetPosition(640 / 2 - bOSize.x / 2, 480 / 2 - bOSize.y / 2);
	scene.Add(backgroundObject);


	auto fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent<RenderComponent>();
	fpsObject->AddComponent<TextComponent>(" ", font);
	fpsObject->AddComponent<FPSComponent>();
	glm::vec2 fpsOSize = fpsObject->GetComponent<RenderComponent>()->GetTexture()->GetSize();
	fpsObject->SetPosition(640 / 2 - fpsOSize.x / 2, 480 / 2 - fpsOSize.y / 2);
	scene.Add(fpsObject);


	//std::vector<std::unique_ptr<GameObject>> objects;

	//auto textObject = std::make_shared<GameObject>();
	//textObject->AddComponent<TextComponent>("Programming 4 Assignment", font);
	//textObject->SetPosition(200, 40);
	//scene.Add(textObject);
	//auto fpsObject = std::make_shared<GameObject>();
	//fpsObject->AddComponent<FPSComponent>(std::make_unique<FPSComponent>(fpsObject.get(), font));
	//scene.Add(fpsObject);

	//auto spriteObject = std::make_shared<GameObject>();
	//spriteObject->AddComponent<SpriteComponent>(std::make_unique<SpriteComponent>(spriteObject.get(), ResourceManager::GetInstance().LoadTexture("logo.tga")));
	//spriteObject->AddComponent<TextComponent>(std::make_unique<TextComponent>(spriteObject.get(), "Programming 4 Assignment", font));
	//spriteObject->SetPosition(100, 100);
	//scene.Add(spriteObject);
}

int main(int, char*[]) 
{
	Engine engine("../Data/");
	engine.Run(load);
    return 0;
}