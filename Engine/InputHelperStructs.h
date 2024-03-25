#pragma once
#include <functional>
#include <memory>
#include "Command.h"

// Enum to represent different input modes
// This allows the user to pass these in when binding input
// So I can handle different input modes internally
enum class InputMode
{
	Release,
	Hold,
	Press
};

struct InputBinding
{
	Command* command;
	InputMode mode = InputMode::Press;
};

enum XInputBindings : unsigned short
{
	GAMEPAD_DPAD_UP          = 0x0001,
	GAMEPAD_DPAD_DOWN        = 0x0002,
	GAMEPAD_DPAD_LEFT        = 0x0004,
	GAMEPAD_DPAD_RIGHT       = 0x0008,
	GAMEPAD_START            = 0x0010,
	GAMEPAD_BACK             = 0x0020,
	GAMEPAD_LEFT_THUMB       = 0x0040,
	GAMEPAD_RIGHT_THUMB      = 0x0080,
	GAMEPAD_LEFT_SHOULDER    = 0x0100,
	GAMEPAD_RIGHT_SHOULDER   = 0x0200,
	GAMEPAD_A                = 0x1000,
	GAMEPAD_B                = 0x2000,
	GAMEPAD_X                = 0x4000,
	GAMEPAD_Y                = 0x8000,
};