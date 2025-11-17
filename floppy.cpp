#include "raylib.h"
#include <vector>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;
const int GRAVITY = 600;
const int JUMP_STRENGTH = -350;


class Bird
{
private:
    Vector2 position;
    float velocity;
    float radius;
    Color color;

public:
    Bird(float r = 20, Color c = GOLD)
    {
        radius = r;
        color = c;
        velocity = 0;
        position = {100, SCREEN_HEIGHT / 2.0f};
    }

    Vector2 GetPosition() 
    { 
        return position; 
    }

    float GetVelocity() 
    { 
        return velocity; 
    }

    float GetRadius()
    { 
        return radius; 
    }

    void SetPosition(Vector2 pos) 
    { 
        position = pos; 
    }

    void SetVelocity(float vel) 
    { 
        velocity = vel; 
    }

    void Jump()
    {
        velocity = JUMP_STRENGTH;
    }

    void Update(float dt)
    {
        velocity += GRAVITY * dt;
        position.y += velocity * dt;
    }

    void Draw() const
    {
        DrawCircleV(position, radius, color);
        DrawCircle(position.x + 10, position.y - 5, 5, WHITE);
        DrawCircle(position.x + 12, position.y - 5, 2, BLACK);
        // Draw a beak
        Vector2 v1 = {position.x + 10, position.y + 5};
        Vector2 v2 = {position.x + 25, position.y + 10};
        Vector2 v3 = {position.x + 10, position.y + 15};
        DrawTriangle(v1, v2, v3, ORANGE);
    }
};

int main() {
    return 0;
}