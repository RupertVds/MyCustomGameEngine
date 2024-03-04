#include "Scene.h"
#include "GameObject.h"

#include <algorithm>


unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_Objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{	
	// Recursively remove the object and its children
	//RemoveRecursive(object);

	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object), m_Objects.end());
}

void Scene::RemoveRecursive(std::shared_ptr<GameObject> object)
{
	// Remove all children recursively
	for (auto& child : object->GetChildren())
	{
		RemoveRecursive(std::shared_ptr<GameObject>(child));
	}

	// Remove the object from the scene's object list
	if (object != nullptr)
	{
		m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object), m_Objects.end());
	}
}

void Scene::RemoveAll()
{
	m_Objects.clear();
}

void Scene::Update()
{
	for(auto& object : m_Objects)
	{
		if (!object->IsMarkedForDeletion())
		{
			object->Update();
		}
	}

	for (auto& object : m_Objects)
	{
		if (object->IsMarkedForDeletion())
		{
			Remove(object);
		}
	}
}

void Scene::FixedUpdate()
{
	for (auto& object : m_Objects)
	{
		if (!object->IsMarkedForDeletion())
		{
			object->FixedUpdate();
		}
	}
}

void Scene::LateUpdate()
{
	for (auto& object : m_Objects)
	{
		if (!object->IsMarkedForDeletion())
		{
			object->LateUpdate();
		}
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		if (!object->IsMarkedForDeletion())
		{
			object->Render();
		}
	}
}

