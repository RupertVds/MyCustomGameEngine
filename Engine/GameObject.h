#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include "Component.h"
#include "Exceptions.h"

class Texture2D;
class Transform;
class Component;

class GameObject final
{
public:
	//=======================================
	// RULE OF ZERO/FIVE/SIX
	//=======================================
	GameObject() = default;
	virtual ~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;
public:
	//=======================================
	// UPDATE AND RENDER METHODS
	//=======================================
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
	virtual void Render() const;

	//=======================================
	// MUTATORS AND ACCESSORS
	//=======================================
	void SetPosition(float x, float y);
	const glm::vec3& GetPosition() const;
	const Transform& GetTransform() const;
	bool IsMarkedForDeletion() const;
	void DeleteSelf();

	//=======================================
	// TEMPLATED COMPONENT SYSTEM
	//=======================================
	//template <typename T>
	//void AddComponent(std::unique_ptr<Component> component)
	//{
	//	if (HasComponent<T>()) throw TooManyComponentsException();
	//	m_Components.emplace_back(std::move(component));
	//}

	template <typename T, typename... Args>
	void AddComponent(Args&&... args)	
	{
		if (HasComponent<T>()) throw TooManyComponentsException();
		std::unique_ptr<T> component{ std::make_unique<T>(this, std::forward<Args>(args)...) };
		m_Components.emplace_back(std::move(component));
	}

	template <typename T>
	void RemoveComponent() {
		m_Components.erase(std::remove_if(
			m_Components.begin(),
			m_Components.end(),
			[](const auto& component) {
				return dynamic_cast<T*>(component.get()) != nullptr;
			}),
			m_Components.end());
	}

	template <typename T>
	T* GetComponent() const {
		for (const auto& component : m_Components) {
			if (auto result = dynamic_cast<T*>(component.get()))
				return result;
		}
		return nullptr;
	}

	template <typename T>
	bool HasComponent() const {
		return GetComponent<T>() != nullptr;
	}


private:
	bool m_IsMarkedForDeletion{ false };
	Transform m_Transform{};
	std::vector<std::unique_ptr<Component>> m_Components;
};
