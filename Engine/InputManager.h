#pragma once
#include "Singleton.h"
#include "XInputController.h"
#include <iostream>

class InputManager final : public Singleton<InputManager>
{
public:
	virtual ~InputManager() {
		ClearAllBindings();
	}
	// The main update method of the input manager
	// This is where everything comes together
	bool ProcessInput();

	XInputController* AddController();

	// Register a keyboard command to be executed when a specific key event occurs
	void BindInput(SDL_Scancode key, InputBinding inputBinding);

	// Register a controller command to be executed when a specific button event occurs
	void BindInput(int controllerIndex, unsigned short button, InputBinding inputBinding);

	void UnbindInput(SDL_Scancode key);
	void UnbindInput(int controllerIndex, unsigned short button);

	void UnbindAllForObject(GameObject* object);
private:
	friend class Singleton<InputManager>;
	InputManager() = default;

	std::unordered_map<SDL_Scancode, InputBinding> m_KeyboardBindings{};
	Uint8 m_PreviousKeyState[SDL_NUM_SCANCODES];
	const Uint8* m_CurrentKeyState = nullptr;

	std::vector<std::unique_ptr<XInputController>> m_Controllers;

	void ClearAllBindings();

	template<typename T>
	void UnbindAllForObjectInMap(std::unordered_map<T, InputBinding>& bindings, GameObject* object);
};

template<typename T>
inline void InputManager::UnbindAllForObjectInMap(std::unordered_map<T, InputBinding>& bindings, GameObject* object)
{
	if (!object) return; // Null check

	std::cout << "Unbinding all inputs for GameObject: " << object << "\n";

	for (auto it = bindings.begin(); it != bindings.end(); ) {
		if (it->second.gameObject == object) {
			std::cout << "Erasing binding for key: " << it->first << "\n";
			//delete it->second.command;  // Ensure the command is deleted if it is no longer needed
			it = bindings.erase(it);
		}
		else {
			++it;
		}
	}
}