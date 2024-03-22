#pragma once
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <xinput.h>
#include <unordered_map>
#include "InputHelperStructs.h"

class XInputController final
{
public:
	XInputController() = default;
	XInputController(int index);
	~XInputController() = default;

	void ProcessInput();
	// Register a controller command to be executed when a specific button event occurs
	void BindInput(WORD button, InputBinding inputBinding);

	inline int GetIndex() const { return m_Index; }
private:
	int m_Index;
	std::unordered_map<WORD, InputBinding> m_ControllerBindings;
	XINPUT_STATE m_PreviousControllerState{};
	XINPUT_STATE m_CurrentControllerState{};
};

