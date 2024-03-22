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