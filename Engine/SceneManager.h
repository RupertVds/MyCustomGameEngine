#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include <unordered_map>
#include <iostream>

class GameObject;
class Scene;
class SceneManager final : public Singleton<SceneManager>
{
public:
	Scene& CreateScene(const std::string& name);
	void DestroyScene(const std::string& name);
	void DestroyAllScenes();

	void Update();
	void FixedUpdate();
	void LateUpdate();

	void Render() const;
	void RenderImGui();

	std::shared_ptr<Scene> GetSceneByName(const std::string& name) const 
	{ 
		if (m_Scenes.find(name) != m_Scenes.end())
		{
			return m_Scenes.at(name);
		}
		else
		{
			std::cout << "SCENE: " << name << " NOT FOUND\n";
		}
		return nullptr;
	}
	
private:
	friend class Singleton<SceneManager>;
	SceneManager() = default;
	//std::vector<std::shared_ptr<Scene>> m_Scenes;
	std::unordered_map<std::string, std::shared_ptr<Scene>> m_Scenes;
};