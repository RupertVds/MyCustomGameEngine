#pragma once
#include <unordered_map>
#include "InputHelperStructs.h"
#include <memory>

class XInputController final
{
public:
	XInputController(int index);
	~XInputController();
	XInputController(XInputController&& other) = default;
	XInputController& operator=(XInputController&& other) = default;

	void ProcessInput();
	// Register a controller command to be executed when a specific button event occurs
	void BindInput(unsigned short button, InputBinding inputBinding);

	int GetIndex() const;
private:
	class XInputControllerImpl;
	std::unique_ptr<XInputControllerImpl> m_pImpl;
};

