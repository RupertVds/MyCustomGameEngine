#include "Scene.h"
#include <algorithm>
#include <imgui.h>


unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) 
	: 
	m_Name(name),
	m_RootObject{std::make_unique<GameObject>()}
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
	m_RootObject->Update();

	// Erase-remove idiom to remove objects marked for deletion
	//m_Objects.erase(std::remove_if(m_Objects.begin(), m_Objects.end(),
	//	[](auto& obj) {
	//		return obj->IsMarkedForDeletion();
	//	}), m_Objects.end());
}

void Scene::FixedUpdate()
{
	m_RootObject->FixedUpdate();
}

void Scene::LateUpdate()
{
	m_RootObject->LateUpdate();
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