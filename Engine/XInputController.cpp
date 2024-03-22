#include "XInputController.h"

XInputController::XInputController(int index)
    :
    m_Index{ index }
{
}

void XInputController::ProcessInput()
{
    CopyMemory(&m_PreviousControllerState, &m_CurrentControllerState, sizeof(XINPUT_STATE));
    ZeroMemory(&m_CurrentControllerState, sizeof(XINPUT_STATE));
    DWORD dwResult = XInputGetState(m_Index, &m_CurrentControllerState);
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
        case InputMode::Release:
            if (buttonsReleasedThisFrame & button)
            {
                binding.command->Execute();
            }
            break;
        case InputMode::Hold:
            if (m_CurrentControllerState.Gamepad.wButtons & button)
            {
                binding.command->Execute();
            }
            break;
        case InputMode::Press:
            if (buttonsPressedThisFrame & button)
            {
                binding.command->Execute();
            }
            break;
        }
    }
}

void XInputController::BindInput(WORD button, InputBinding inputBinding)
{
    m_ControllerBindings[button] = std::move(inputBinding);
}
