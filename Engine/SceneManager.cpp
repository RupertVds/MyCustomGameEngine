#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

std::shared_ptr<GameObject> SceneManager::m_RootObject = std::make_shared<GameObject>();

void SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

void SceneManager::FixedUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene->FixedUpdate();
	}
}

void SceneManager::LateUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene->LateUpdate();
	}
}

std::shared_ptr<GameObject> const SceneManager::GetRootObject() const
{
	return m_RootObject;
}

void SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

Scene& SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	scene->Add(m_RootObject);
	return *scene;
}
