#include "XInputController.h"
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <xinput.h>
#include <unordered_map>

// PIMPL
class XInputController::XInputControllerImpl final
{
public:

    XInputControllerImpl(int index)
        : m_Index{ index }
    {}
    ~XInputControllerImpl() = default;
    XInputControllerImpl(XInputControllerImpl&& other) = default;
    XInputControllerImpl& operator=(XInputControllerImpl&& other) = default;

    void ProcessInput()
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
    // Register a controller command to be executed when a specific button event occurs
    void BindInput(unsigned short button, InputBinding inputBinding)
    {
        m_ControllerBindings[button] = std::move(inputBinding);
    }

    int GetIndex() const { return m_Index; }
private:
    int m_Index;
    std::unordered_map<WORD, InputBinding> m_ControllerBindings;
    XINPUT_STATE m_PreviousControllerState{};
    XINPUT_STATE m_CurrentControllerState{};
};

XInputController::XInputController(int index)
    :
    m_pImpl{std::make_unique<XInputControllerImpl>(index)}
{
}

XInputController::~XInputController()
{
}

void XInputController::ProcessInput()
{
    m_pImpl->ProcessInput();
}

void XInputController::BindInput(unsigned short button, InputBinding inputBinding)
{
    m_pImpl->BindInput(button, inputBinding);
}

int XInputController::GetIndex() const
{
    return m_pImpl->GetIndex();
}
