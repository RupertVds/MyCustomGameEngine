#pragma once
#include "Singleton.h"
#include "XInputController.h"

class InputManager final : public Singleton<InputManager>
{
public:
	// The main update method of the input manager
	// This is where everything comes together
	bool ProcessInput();

	XInputController* AddController();

	// Register a keyboard command to be executed when a specific key event occurs
	void BindInput(SDL_Scancode key, InputBinding inputBinding);

	// Register a controller command to be executed when a specific button event occurs
	void BindInput(int controllerIndex, unsigned short button, InputBinding inputBinding);
private:
	friend class Singleton<InputManager>;
	InputManager() = default;

	std::unordered_map<SDL_Scancode, InputBinding> m_KeyboardBindings{};
	Uint8 m_PreviousKeyState[SDL_NUM_SCANCODES];
	const Uint8* m_CurrentKeyState = nullptr;

	std::vector<std::unique_ptr<XInputController>> m_Controllers;
};