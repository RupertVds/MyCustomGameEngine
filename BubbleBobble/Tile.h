#pragma once
#include <SDL.h>
#include <Box2D/Box2D.h>
#include <box2d\b2_draw.h>
#include <Renderer.h>

class Tile 
{
public:
    Tile(float x, float y, float width, float height, b2World* world) 
    {
        // Define the position and size of the tile
        b2BodyDef bodyDef;
        bodyDef.position.Set(x, y);
        m_pBody = world->CreateBody(&bodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(width / 2.0f, height / 2.0f); // Half-width and half-height

        // Assign the shape to the member variable
        m_Shape = shape;

        // Create the collision fixture
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.f;
        fixtureDef.restitution = 0.0f; // No bounciness

        m_pBody->CreateFixture(&fixtureDef);

        // Create the sensor fixture for contact data
        b2FixtureDef sensorFixtureDef;
        sensorFixtureDef.shape = &shape;
        sensorFixtureDef.isSensor = true; // Set as a sensor

        m_pBody->CreateFixture(&sensorFixtureDef);
    }

    void Render() const
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


    ~Tile() {
        // Destroy the Box2D body when the tile is destroyed
        //m_pBody->GetWorld()->DestroyBody(m_pBody);
    }

private:
    b2Body* m_pBody; // Box2D body representing the tile
    b2PolygonShape m_Shape; // Shape of the tile
};