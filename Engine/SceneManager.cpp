#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

void SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene.second->Update();
	}
}

void SceneManager::FixedUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene.second->FixedUpdate();
	}
}

void SceneManager::LateUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene.second->LateUpdate();
	}
}

void SceneManager::Render() const
{
	for (const auto& scene : m_Scenes)
	{
		scene.second->Render();
	}
}

void SceneManager::RenderImGui()
{
	for (const auto& scene : m_Scenes)
	{
		scene.second->RenderImGui();
	}
}

Scene& SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	//m_Scenes.push_back(scene);
	m_Scenes[name] = scene;
	//scene->Add(m_RootObject);
	return *scene;
}
