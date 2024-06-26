#pragma once
#include "SceneManager.h"
#include "GameObject.h"


class GameObject;

class Scene final
{
	friend Scene& SceneManager::CreateScene(const std::string& name);
public:
	void Add(std::unique_ptr<GameObject>&& object);
	GameObject* GetRoot() const { return m_RootObject.get(); }

	void Update();
	void FixedUpdate();
	void LateUpdate();
	void Render() const;
	void RenderImGui();

	bool GetIsMarkedForDeletion() const { return m_IsMarkedForDeletion; }
	void SetIsMarkedForDeletion(bool isMarkedForDeletion) { m_IsMarkedForDeletion = isMarkedForDeletion; }

	void DeleteObjectsByName(const std::string& name);

	~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

private: 
	explicit Scene(const std::string& name);
	void DeleteObjectsByNameRecursive(GameObject* currentObject, const std::string& name);

	std::string m_Name;
	bool m_IsMarkedForDeletion{};
	std::unique_ptr<GameObject> m_RootObject{};

	static unsigned int m_IdCounter; 
};
