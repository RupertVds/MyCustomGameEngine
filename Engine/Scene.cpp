#include "Scene.h"
#include <algorithm>
#include <imgui.h>
#include <iostream>


unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) 
	: 
	m_Name(name),
	m_RootObject{std::make_unique<GameObject>("name")}
{
	//m_RootObject->SetParent(nullptr); // Root object has no parent
}

Scene::~Scene() = default;


void Scene::Add(std::unique_ptr<GameObject>&& object)
{
	// Set root object as parent (set parent takes ownership)
	//object->SetParent(m_RootObject.get());
	m_RootObject->AddChild(std::move(object));

	// Make it so that the object that is now a child of the rootobject doesn't get managed by this passed unique ptr anymore
	// so that the child object stays valid
}

void Scene::Update()
{
	if (m_IsMarkedForDeletion) return;
	// Update all objects in the scene
	try
	{
		m_RootObject->Update();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
}

void Scene::FixedUpdate()
{
	if (m_IsMarkedForDeletion) return;

	try
	{
		m_RootObject->FixedUpdate();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
}

void Scene::LateUpdate()
{
	if (m_IsMarkedForDeletion) return;

	try
	{
		m_RootObject->LateUpdate();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
}

void Scene::Render() const
{
	if (m_IsMarkedForDeletion) return;

	// Render all objects in the scene
	m_RootObject->Render();
}

void Scene::RenderImGui()
{
	ImGui::NewFrame();
	m_RootObject->RenderImGui();
	ImGui::EndFrame();
}

void Scene::DeleteObjectsByName(const std::string& name)
{
    // Start the recursive deletion from the root object
    DeleteObjectsByNameRecursive(m_RootObject.get(), name);
}

void Scene::DeleteObjectsByNameRecursive(GameObject* currentObject, const std::string& name)
{
	if (!currentObject)
		return;

	// If the current object's name matches the name to be deleted, delete it
	if (currentObject->GetName() == name)
	{
		currentObject->DeleteSelf();
		return;
	}

	// Recursively traverse child objects
	for (int i = 0; i < currentObject->GetChildCount(); ++i)
	{
		auto child = currentObject->GetChildAtIndex(i);
		DeleteObjectsByNameRecursive(child, name);
	}
}