#pragma once
#include "Singleton.h"
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <xinput.h>
#include <functional>

class InputManager final : public Singleton<InputManager>
{
public:
	// Enum to represent different input modes
	// This allows the user to pass these in when binding input
	// So I can handle different input modes internally
	enum InputMode 
	{
		Release,
		Hold,
		Press
	};

	// The main update method of the input manager
	// This is where everything comes together
	bool ProcessInput();

	// Register a keyboard command to be executed when a specific key event occurs
	void BindInput(SDL_Scancode key, std::function<void()> command, InputMode mode = InputMode::Press);

	// Register a controller command to be executed when a specific button event occurs
	void BindInput(WORD button, std::function<void()> command, InputMode mode = InputMode::Press);
private:
	struct InputBinding 
	{
		std::function<void()> command;
		InputMode mode = InputMode::Press;
	};

	std::unordered_map<SDL_Scancode, InputBinding> m_KeyboardBindings{};
	Uint8 m_PreviousKeyState[SDL_NUM_SCANCODES];
	const Uint8* m_CurrentKeyState;

	std::unordered_map<WORD, InputBinding> m_ControllerBindings;
	XINPUT_STATE m_PreviousControllerState{};
	XINPUT_STATE m_CurrentControllerState{};
};