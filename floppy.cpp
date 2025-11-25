
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
public:
    Vector2 position; float velocity; float radius;
    Bird(float r = 20, Color c = GOLD) { radius = r; velocity = 0; position = {100, ScreenHeight / 2.0f}; }
    void Update(float dt) { velocity += Gravity * dt; position.y += velocity * dt; }
    void Jump() { velocity = JumpStrength; }
    void Draw() const {
        DrawCircleV(position, radius, GOLD);
        DrawCircle(position.x + 10, position.y - 5, 5, WHITE);
        DrawCircle(position.x + 12, position.y - 5, 2, BLACK);
        DrawTriangle({position.x + 10, position.y + 5}, {position.x + 25, position.y + 10}, {position.x + 10, position.y + 15}, ORANGE);
    }
};

class Pipe
{
public:
    float x, gapY; bool passed;
    Pipe(float xPos, float gap) : x(xPos), gapY(gap), passed(false) {}
    void Update(float dt) { x -= PipeSpeed * dt; }
    void Draw() const {
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
    InitWindow(ScreenWidth, ScreenHeight, "Commit 3: Pipe Spawning");
    SetTargetFPS(60);
    srand(time(NULL));

    Bird bird;
    vector<Pipe> pipes;
    pipes.push_back(Pipe(ScreenWidth, rand() % (ScreenHeight - 250) + 125));

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) bird.Jump();
        bird.Update(dt);

        // Pipe Spawning
        if (pipes.empty() || (ScreenWidth - pipes.back().x) >= PipeSpawnX)
        {
            pipes.push_back(Pipe(ScreenWidth, rand() % (ScreenHeight - 250) + 125));
        }

        // Update Pipes
        for (auto &pipe : pipes)
        {
            pipe.Update(dt);
        }

        // Remove off-screen pipes
        if (!pipes.empty() && pipes.front().x < -PipeWidth)
        {
            pipes.erase(pipes.begin());
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);
        for (auto &pipe : pipes) pipe.Draw();
        bird.Draw();
        DrawRectangle(0, ScreenHeight - 20, ScreenWidth, 20, DARKBROWN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

