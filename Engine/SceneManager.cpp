#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

void SceneManager::Update()
{
	std::vector<std::string> sceneNames;
	sceneNames.reserve(m_Scenes.size());
	for (const auto& scenePair : m_Scenes)
	{
		sceneNames.push_back(scenePair.first);
	}

	// Iterate over the copied keys and update the scenes
	for (const auto& sceneName : sceneNames)
	{
		// Find the scene by name in the original unordered_map
		auto it = m_Scenes.find(sceneName);
		if (it != m_Scenes.end() && it->second)
		{
			it->second->Update();
		}
	}
}

void SceneManager::FixedUpdate()
{
	// Create a copy of the keys (scene names) to avoid modifying the container while iterating
	std::vector<std::string> sceneNames;
	sceneNames.reserve(m_Scenes.size());
	for (const auto& scenePair : m_Scenes)
	{
		sceneNames.push_back(scenePair.first);
	}

	// Iterate over the copied keys and update the scenes
	for (const auto& sceneName : sceneNames)
	{
		// Find the scene by name in the original unordered_map
		auto it = m_Scenes.find(sceneName);
		if (it != m_Scenes.end() && it->second)
		{
			it->second->FixedUpdate();
		}
	}
}

void SceneManager::LateUpdate()
{
	// Create a copy of the keys (scene names) to avoid modifying the container while iterating
	std::vector<std::string> sceneNames;
	sceneNames.reserve(m_Scenes.size());
	for (const auto& scenePair : m_Scenes)
	{
		sceneNames.push_back(scenePair.first);
	}

	// Iterate over the copied keys and update the scenes
	for (const auto& sceneName : sceneNames)
	{
		// Find the scene by name in the original unordered_map
		auto it = m_Scenes.find(sceneName);
		if (it != m_Scenes.end() && it->second)
		{
			it->second->LateUpdate();
		}
	}

	// Collect scenes marked for deletion
	std::vector<std::string> scenesToDelete;
	for (const auto& scenePair : m_Scenes)
	{
		if (scenePair.second->GetIsMarkedForDeletion())
		{
			scenesToDelete.push_back(scenePair.first);
		}
	}

	// Erase scenes marked for deletion
	for (const auto& sceneName : scenesToDelete)
	{
		m_Scenes.erase(sceneName);
	}
}

void SceneManager::Render() const
{
	std::vector<std::string> sceneNames;
	sceneNames.reserve(m_Scenes.size());
	for (const auto& scenePair : m_Scenes)
	{
		sceneNames.push_back(scenePair.first);
	}

	// Iterate over the copied keys and update the scenes
	for (const auto& sceneName : sceneNames)
	{
		// Find the scene by name in the original unordered_map
		auto it = m_Scenes.find(sceneName);
		if (it != m_Scenes.end() && it->second)
		{
			it->second->Render();
		}
	}
}

void SceneManager::RenderImGui()
{
	std::vector<std::string> sceneNames;
	sceneNames.reserve(m_Scenes.size());
	for (const auto& scenePair : m_Scenes)
	{
		sceneNames.push_back(scenePair.first);
	}

	// Iterate over the copied keys and update the scenes
	for (const auto& sceneName : sceneNames)
	{
		// Find the scene by name in the original unordered_map
		auto it = m_Scenes.find(sceneName);
		if (it != m_Scenes.end() && it->second)
		{
			it->second->RenderImGui();
		}
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

void SceneManager::DestroyScene(const std::string& name)
{
	// delete scene
	// will probably need to let reference be notified
	m_Scenes.erase(name);
}

void SceneManager::DestroyAllScenes()
{
	for (auto& scene : m_Scenes)
	{
		scene.second->GetRoot()->DeleteSelf();
		scene.second->SetIsMarkedForDeletion(true);
	}
}
