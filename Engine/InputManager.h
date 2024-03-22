#pragma once
#include "Singleton.h"
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//#include <xinput.h>
#include "InputHelperStructs.h"
#include "XInputController.h"
//class XInputController;

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
	void BindInput(int controllerIndex, WORD button, InputBinding inputBinding);
private:
	std::unordered_map<SDL_Scancode, InputBinding> m_KeyboardBindings{};
	Uint8 m_PreviousKeyState[SDL_NUM_SCANCODES];
	const Uint8* m_CurrentKeyState;

	std::vector<std::unique_ptr<XInputController>> m_Controllers;
	 
	//std::unordered_map<WORD, InputBinding> m_ControllerBindings;
	//XINPUT_STATE m_PreviousControllerState{};
	//XINPUT_STATE m_CurrentControllerState{};
};