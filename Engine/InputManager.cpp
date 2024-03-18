#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"

bool InputManager::ProcessInput()
{
    // Save previous state
    m_PreviousControllerState = m_CurrentControllerState;

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
        case Release:
            if (m_PreviousKeyState[key] && !m_CurrentKeyState[key])
            {
                binding.command();
            }
            break;
        case Hold:
            if (m_CurrentKeyState[key])
            {
                binding.command();
            }
            break;
        case Press:
            if (!m_PreviousKeyState[key] && m_CurrentKeyState[key])
            {
                binding.command();
            }
            break;
        }
    }

    // Update previous key state
    memcpy(m_PreviousKeyState, m_CurrentKeyState, SDL_NUM_SCANCODES);


    // Process command based gamepad input

    CopyMemory(&m_PreviousControllerState, &m_CurrentControllerState, sizeof(XINPUT_STATE));
    ZeroMemory(&m_CurrentControllerState, sizeof(XINPUT_STATE));
    DWORD dwResult = XInputGetState(0, &m_CurrentControllerState);
    if (dwResult != ERROR_SUCCESS)
    {
        // Handle controller disconnected or other error
        // I may want to implement error handling here
        //return true;
    }

    auto buttonChanges = m_CurrentControllerState.Gamepad.wButtons ^ m_PreviousControllerState.Gamepad.wButtons;
    auto buttonsPressedThisFrame = buttonChanges & m_CurrentControllerState.Gamepad.wButtons;
    auto buttonsReleasedThisFrame = buttonChanges & (~m_CurrentControllerState.Gamepad.wButtons);

    for (auto const& [button, binding] : m_ControllerBindings)
    {
        switch (binding.mode)
        {
        case Release:
            if (buttonsReleasedThisFrame & button)
            {
                binding.command();
            }
            break;
        case Hold:
            if (m_CurrentControllerState.Gamepad.wButtons & button)
            {
                binding.command();
            }
            break;
        case Press:
            if (buttonsPressedThisFrame & button)
            {
                binding.command();
            }
            break;
        }
    }

    // process event for IMGUI
    ImGui_ImplSDL2_ProcessEvent(&e);

    return true;
}

void InputManager::BindInput(SDL_Scancode key, std::function<void()> command, InputMode mode)
{
    m_KeyboardBindings[key] = { command, mode };
}

void InputManager::BindInput(WORD button, std::function<void()> command, InputMode mode)
{
    m_ControllerBindings[button] = { command, mode };
}
