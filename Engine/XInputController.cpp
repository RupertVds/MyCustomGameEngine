#include "XInputController.h"
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <xinput.h>
#include <unordered_map>
#include <iostream>

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
            std::cout << "A CONTROLLER IS MISSING \n";
            return;
        }

        auto buttonChanges = m_CurrentControllerState.Gamepad.wButtons ^ m_PreviousControllerState.Gamepad.wButtons;
        auto buttonsPressedThisFrame = buttonChanges & m_CurrentControllerState.Gamepad.wButtons;
        auto buttonsReleasedThisFrame = buttonChanges & (~m_CurrentControllerState.Gamepad.wButtons);

        for (auto const& [button, binding] : m_ControllerBindings)
        {
            if (binding.command == nullptr)
            {
                continue;
            }

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

    void UnbindInput(unsigned short button)
    {
        m_ControllerBindings.erase(button);
    }

    void UnbindAllForObject(GameObject* object)
    {
        for (auto it = m_ControllerBindings.begin(); it != m_ControllerBindings.end(); ) {
            if (it->second.gameObject == object) {
                std::cout << "Erasing controller binding for button: " << it->first << "\n";
                it = m_ControllerBindings.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    int GetIndex() const { return m_Index; }

    void ClearAllBindings()
    {
        m_ControllerBindings.clear();
    }

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

void XInputController::UnbindInput(unsigned short button)
{
    m_pImpl->UnbindInput(button);
}

void XInputController::UnbindAllForObject(GameObject* object)
{
    if (!object) return; // Null check

    std::cout << "Unbinding all for GameObject in XInputController: " << m_pImpl->GetIndex() << "\n";
    m_pImpl->UnbindAllForObject(object);
}

void XInputController::ClearAllBindings()
{
    m_pImpl->ClearAllBindings();
}

int XInputController::GetIndex() const
{
    return m_pImpl->GetIndex();
}
