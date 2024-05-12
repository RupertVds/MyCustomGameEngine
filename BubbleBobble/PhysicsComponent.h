#pragma once

#include "Component.h"
#include <SDL.h>
#include "Renderer.h"
#include "PhysicsSystem.h"

class PhysicsComponent : public Component {
public:
    PhysicsComponent(GameObject* pOwner,
        float width, float height, float density, 
        const b2Vec2& initialVelocity = { 0, 0 })
        : 
        Component(pOwner), 
        m_Width(width), m_Height(height), m_Density(density), 
        m_InitialVelocity(initialVelocity)
    {
        // Perform additional initialization for this component
        // For example, create a dynamic body in the physics world
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.fixedRotation = true;
        bodyDef.bullet = true;
        bodyDef.position.Set(GetOwner()->GetLocalPosition().x, GetOwner()->GetLocalPosition().y);
        m_pBody = PhysicsSystem::GetInstance().GetWorld()->CreateBody(&bodyDef);

        // Create other fixtures, shapes, joints, etc., as needed
        // Example: Create a fixture for a rectangular shape
        b2PolygonShape shape;

        shape.SetAsBox(m_Width / 2, m_Height / 2); 

        
        m_pBody->CreateFixture(&shape, m_Density);

        // Apply initial velocity if provided
        m_pBody->SetLinearVelocity(m_InitialVelocity);
    }

    virtual ~PhysicsComponent() {}

    void Update() override {
        // Perform any per-frame updates for this component
        // For example, update the GameObject's position based on the physics simulation
        b2Vec2 position = m_pBody->GetPosition();
        GetOwner()->SetLocalPosition(glm::vec3(position.x, position.y, GetOwner()->GetLocalPosition().z));
    }

    void Render() const override
    {
#if _DEBUG
        // Debug render the collider of the physics component
        b2Fixture* fixture = m_pBody->GetFixtureList();
        SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set outline color to red
        while (fixture != nullptr) {
            b2Shape* shape = fixture->GetShape();
            if (shape->GetType() == b2Shape::e_polygon) {
                b2PolygonShape* polyShape = static_cast<b2PolygonShape*>(shape);
                SDL_Point* points = new SDL_Point[polyShape->m_count + 1]; // Add one for the final connection
                for (int i = 0; i < polyShape->m_count; ++i) {
                    b2Vec2 vertex = m_pBody->GetWorldPoint(polyShape->m_vertices[i]);
                    points[i] = { static_cast<int>(vertex.x), static_cast<int>(vertex.y) };
                }
                // Connect the final point to the first point
                b2Vec2 firstVertex = m_pBody->GetWorldPoint(polyShape->m_vertices[0]);
                points[polyShape->m_count] = { static_cast<int>(firstVertex.x), static_cast<int>(firstVertex.y) };

                // Render the lines
                SDL_RenderDrawLines(Renderer::GetInstance().GetSDLRenderer(), points, polyShape->m_count + 1);
                delete[] points;
            }
            fixture = fixture->GetNext();
            
        }
#endif
    }

    b2Body* GetBody() const { return m_pBody; }
    float GetHeight() const { return m_Height; }
    float GetWidth() const { return m_Width; }

private:
    b2Body* m_pBody;
    float m_Width; // Width of the collider
    float m_Height; // Height of the collider
    float m_Density; // Density of the collider
    b2Vec2 m_InitialVelocity;
};
