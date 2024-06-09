#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include <stdexcept>
//#include "XInputController.h"
#include <string>
#include <algorithm>

bool InputManager::ProcessInput()
{
    //// Save previous state
    //m_PreviousControllerState = m_CurrentControllerState;

    // Process general keyboard input
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            return false;
        }
        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                return false;
            }
        }
    }

    // Process Command based Keyboard Input
    // Update current key state
    m_CurrentKeyState = SDL_GetKeyboardState(NULL);

    for (auto const& [key, binding] : m_KeyboardBindings)
    {
        switch (binding.mode)
        {
        case InputMode::Release:
            if (m_PreviousKeyState[key] && !m_CurrentKeyState[key])
            {
                binding.command->Execute();
            }
            break;
        case InputMode::Hold:
            if (m_CurrentKeyState[key])
            {
                binding.command->Execute();
            }
            break;
        case InputMode::Press:
            if (!m_PreviousKeyState[key] && m_CurrentKeyState[key])
            {
                binding.command->Execute();
            }
            break;
        }
    }

    // Update previous key state
    memcpy(m_PreviousKeyState, m_CurrentKeyState, SDL_NUM_SCANCODES);

    std::for_each(m_Controllers.begin(), m_Controllers.end(), [](const std::unique_ptr<XInputController>& controller) { controller->ProcessInput(); });

    // process event for IMGUI
    //ImGui_ImplSDL2_ProcessEvent(&e);

    return true;
}

XInputController* InputManager::AddController()
{
    auto controller = std::make_unique<XInputController>(static_cast<int>(m_Controllers.size()));

    m_Controllers.emplace_back(std::move(controller));
    return m_Controllers.back().get();
}

void InputManager::BindInput(SDL_Scancode key, InputBinding inputBinding)
{
    m_KeyboardBindings[key] = std::move(inputBinding);
}

void InputManager::BindInput(int controllerIndex, unsigned short button, InputBinding inputBinding)
{
    //m_ControllerBindings[button] = std::move(inputBinding);
    for (auto& controller : m_Controllers)
    {
        if (controller->GetIndex() == controllerIndex)
        {
            controller->BindInput(button, inputBinding);
            return;
        }
    }
    //throw std::runtime_error("Invalid Controller Index: " + std::to_string(controllerIndex) + "!");
}

void InputManager::UnbindInput(SDL_Scancode key)
{
    m_KeyboardBindings.erase(key);
}

void InputManager::UnbindInput(int controllerIndex, unsigned short button)
{
    for (auto& controller : m_Controllers)
    {
        if (controller->GetIndex() == controllerIndex)
        {
            controller->UnbindInput(button);
            return;
        }
    }
}

void InputManager::UnbindAllForObject(GameObject* object)
{
    if (!object) return; // Null check

    std::cout << "Unbinding all for GameObject: " << object << "\n";

    if (!m_KeyboardBindings.empty()) {
        UnbindAllForObjectInMap(m_KeyboardBindings, object);
    }

    for (auto& controller : m_Controllers) {
        if (controller) { // Null check
            std::cout << "Unbinding all for GameObject in controller: " << controller->GetIndex() << "\n";
            controller->UnbindAllForObject(object);
        }
    }
}

void InputManager::ClearAllBindings()
{
    // Clear all keyboard bindings
    m_KeyboardBindings.clear();

    // Clear all controller bindings
    for (auto& controller : m_Controllers)
    {
        if (controller)
        {
            controller->ClearAllBindings();
        }
    }
}
