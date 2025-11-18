#include "raylib.h"
#include <vector>

using namespace std;

const int ScreenWidth = 800;
const int ScreenHeight = 450;
const int Gravity = 600;
const int JumpStrength = -350;
const int PipeSpeed = 200;
const int PipeSpawnX = 300; // Distance between pipes
const int PipeWidth = 80;
const int PipeGap = 170;

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
        position = {100, ScreenHeight / 2.0f};
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
        velocity = JumpStrength;
    }

    void Update(float dt)
    {
        velocity += Gravity * dt;
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

class Pipe
{
private:
    float x;
    float gapY; // The center Y position of the gap
    bool passed;

public:
    Pipe(float xPos = ScreenWidth, float gap = ScreenHeight / 2.0f)
    {
        x = xPos;
        gapY = gap;
        passed = false;
    }

    float GetX()
    {
        return x;
    }

    float GetGapY()
    {
        return gapY;
    }

    bool IsPassed()
    {
        return passed;
    }

    void SetX(float xPos)
    {
        x = xPos;
    }
    void SetGapY(float gap)
    {
        gapY = gap;
    }
    void SetPassed(bool p)
    {
        passed = p;
    }

    void Update(float dt)
    {
        x -= PipeSpeed * dt;
    }

    void Draw() const
    {
        // Top Pipe
        Rectangle topRec = {x, 0, (float)PipeWidth, gapY - (PipeGap / 2)};
        DrawRectangleRec(topRec, GREEN);
        DrawRectangleLinesEx(topRec, 3, DARKGREEN); // Outline

        // Bottom Pipe
        Rectangle bottomRec = {x, gapY + (PipeGap / 2), (float)PipeWidth, (float)ScreenHeight - (gapY + (PipeGap / 2))};
        DrawRectangleRec(bottomRec, GREEN);
        DrawRectangleLinesEx(bottomRec, 3, DARKGREEN); // Outline
    }
};

int main()
{
    InitWindow(ScreenWidth, ScreenHeight, "Raylib - Flappy Bird Clone");
    SetTargetFPS(60);
    srand(time(NULL));

    Bird bird(20, GOLD);
    return 0;
}