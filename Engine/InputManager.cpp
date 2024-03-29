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
    ImGui_ImplSDL2_ProcessEvent(&e);

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
    throw std::runtime_error("Invalid Controller Index: " + std::to_string(controllerIndex) + "!");
}
