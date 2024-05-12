#pragma once

template<typename T>
class BehaviorState;

// BehaviorStateMachine class
template<typename T>
class BehaviorStateMachine 
{
public:
    BehaviorStateMachine(T* pComponent) 
        : m_pComponent(pComponent), m_pCurrentState(nullptr)
    {}

    ~BehaviorStateMachine() 
    {
        if (m_pCurrentState) {
            m_pCurrentState->Exit(*this);
            delete m_pCurrentState;
            m_pCurrentState = nullptr;
        }
    }

    // Update the current state
    void Update() 
    {
        if (m_pCurrentState)
        {
            m_pCurrentState->Update(*this);
        }
    }


    // Fixed Update the current state
    void FixedUpdate()
    {
        if (m_pCurrentState)
        {
            m_pCurrentState->FixedUpdate(*this);
        }
    }

    // Late Update the current state
    void LateUpdate()
    {
        if (m_pCurrentState)
        {
            m_pCurrentState->LateUpdate(*this);
        }
    }

    // Change state
    void SetState(BehaviorState<T>* newState) 
    {
        if (m_pCurrentState)
        {
            m_pCurrentState->Exit(*this);
            delete m_pCurrentState;
            m_pCurrentState = nullptr;
        }
        m_pCurrentState = newState;
        if (m_pCurrentState)
        {
            m_pCurrentState->Entry(*this);
        }
    }

    // Get the component associated with this state machine
    T* GetComponent() const { return m_pComponent; }

private:
    T* m_pComponent;
    BehaviorState<T>* m_pCurrentState;
};