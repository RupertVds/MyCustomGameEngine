#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include "Component.h"
#include "Exceptions.h"

class Texture2D;
class Component;
class Transform;

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
	const glm::vec3& GetPosition();
	const Transform& GetTransform() const;

	//=======================================
	// TEMPLATED COMPONENT SYSTEM
	//=======================================
	template <typename T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must be a subclass of Component");

		// Check if a component of the same type already exists
		for (const auto& existingComponent : m_Components) {
			if (dynamic_cast<T*>(existingComponent.get()) != nullptr) {
				throw TooManyComponentsException();
			}
		}

		// Construct component based on arguments and templated type
		auto newComponent = std::make_unique<T>(std::forward<Args>(args)...);

		// Allow raw pointer return
		T* rawPtr = newComponent.get();

		// Set the owner before adding the component to the container
		Component* component = static_cast<Component*>(newComponent.get());
		component->SetOwner(this);
		component->OnInit();

		m_Components.emplace_back(std::move(newComponent));
		return rawPtr;
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
	Transform m_Transform{};
	std::vector<std::unique_ptr<Component>> m_Components;
	//std::shared_ptr<Texture2D> m_texture{};
};
