#pragma once
#include "Singleton.h"
#include "Timer.h"

#pragma warning(push, 0)  // Disable warnings
#include "Box2D/Box2D.h"  // Include Box2D headers
#pragma warning(pop)      // Restore previous warning level

class PhysicsSystem final : public Singleton<PhysicsSystem>
{
public:
    virtual ~PhysicsSystem()
    { 
        delete m_pWorld;
    }

    void Initialize(const b2Vec2& gravity = { 0, 9.81f })
    {
        m_pWorld = new b2World(gravity);
    }
    virtual void Update()
    {
        // Step the physics simulation further
        m_pWorld->Step(Timer::GetInstance().GetFixedTimeStep(), VELOCITY_ITERATIONS, POSITION_ITERATIONS);
    }

    inline b2World* GetWorld() const { return m_pWorld; }
    inline static float GetScale() { return SCALE; }

private:
    friend class Singleton<PhysicsSystem>;
    PhysicsSystem() = default;
    b2World* m_pWorld{};
    const int VELOCITY_ITERATIONS{ 8 };
    const int POSITION_ITERATIONS{ 3 };
    // 1 pixel is 1 cm in box2d
    static const float SCALE;
};