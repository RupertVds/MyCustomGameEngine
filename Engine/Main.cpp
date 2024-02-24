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
	to->SetPosition(300, 300);
	try
	{
		to->AddComponent<TextComponent>("Programming 4 Assignment", font);
		to->AddComponent<FPSComponent>(font);
		to->AddComponent<FPSComponent>(font);
	}
	catch (const BaseException& e) 
	{
		e.what();
	}


	scene.Add(to);
}

int main(int, char*[]) 
{
	Engine engine("../Data/");
	engine.Run(load);
    return 0;
}