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
	// Render all objects in the scene
	m_RootObject->Render();
}

void Scene::RenderImGui()
{
	ImGui::NewFrame();
	m_RootObject->RenderImGui();
	ImGui::EndFrame();
}