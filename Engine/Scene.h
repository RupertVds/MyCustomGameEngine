#pragma once
#include "SceneManager.h"
#include "GameObject.h"


class GameObject;

class Scene final
{
	friend Scene& SceneManager::CreateScene(const std::string& name);
public:
	void Add(std::shared_ptr<GameObject> object);

	void Update();
	void FixedUpdate();
	void LateUpdate();
	void Render() const;
	void RenderImGui();

	~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

private: 
	explicit Scene(const std::string& name);

	std::string m_Name;
	std::vector <std::shared_ptr<GameObject>> m_Objects{};

	static unsigned int m_IdCounter; 
};
