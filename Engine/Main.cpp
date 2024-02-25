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
#include "TextComponent.h"
#include "FPSComponent.h"
#include "Scene.h"

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto textObject = std::make_shared<GameObject>();
	textObject->AddComponent<TextComponent>("Programming 4 Assignment", font);
	textObject->SetPosition(100, 0);
	scene.Add(textObject);

	auto fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent<FPSComponent>(font);
	scene.Add(fpsObject);
}

int main(int, char*[]) 
{
	Engine engine("../Data/");
	engine.Run(load);
    return 0;
}