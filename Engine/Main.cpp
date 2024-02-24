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
	auto to = std::make_shared<GameObject>();
	to->AddComponent<TextComponent>("Programming 4 Assignment", font);
	to->AddComponent<TextComponent>("Test", font);
	to->GetComponent<TextComponent>()->SetPosition(20, 40);
	to->AddComponent<FPSComponent>(font);
	to->GetComponent<FPSComponent>()->SetPosition(80, 100);
	scene.Add(to);
}

int main(int, char*[]) 
{
	Engine engine("../Data/");
	engine.Run(load);
    return 0;
}