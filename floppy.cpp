#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int ScreenWidth = 800;
const int ScreenHeight = 450;
const int Gravity = 600;
const int JumpStrength = -350;
const int PipeSpeed = 200;
const int PipeSpawnX = 300;
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

    Vector2 GetPosition() { return position; }
    void Update(float dt) { velocity += Gravity * dt; position.y += velocity * dt; }
    void Jump() { velocity = JumpStrength; }

    void Draw() const
    {
        DrawCircleV(position, radius, color);
        DrawCircle(position.x + 10, position.y - 5, 5, WHITE);
        DrawCircle(position.x + 12, position.y - 5, 2, BLACK);
        Vector2 v1 = {position.x + 10, position.y + 5};
        Vector2 v2 = {position.x + 25, position.y + 10};
        Vector2 v3 = {position.x + 10, position.y + 15};
        DrawTriangle(v1, v2, v3, ORANGE);
    }
};

class Pipe
{
public:
    float x, gapY; bool passed;
    Pipe(float xPos, float gap) : x(xPos), gapY(gap), passed(false) {}
    void Update(float dt) { x -= PipeSpeed * dt; }
    void Draw() const
    {
        Rectangle topRec = {x, 0, (float)PipeWidth, gapY - (PipeGap / 2)};
        DrawRectangleRec(topRec, GREEN);
        DrawRectangleLinesEx(topRec, 3, DARKGREEN);
        Rectangle bottomRec = {x, gapY + (PipeGap / 2), (float)PipeWidth, (float)ScreenHeight - (gapY + (PipeGap / 2))};
        DrawRectangleRec(bottomRec, GREEN);
        DrawRectangleLinesEx(bottomRec, 3, DARKGREEN);
    }
};

int main()
{
    InitWindow(ScreenWidth, ScreenHeight, "Commit 2: Bird Physics");
    SetTargetFPS(60);
    srand(time(NULL));

    Bird bird(20, GOLD);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // Handle player input
        if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            bird.Jump();
        }
        // Update bird physics
        bird.Update(dt);


        // Drawing
        BeginDrawing();
        ClearBackground(SKYBLUE);
        bird.Draw();
        DrawRectangle(0, ScreenHeight - 20, ScreenWidth, 20, DARKBROWN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
