
#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const int ScreenWidth = 800;
const int ScreenHeight = 450;
const int Gravity = 600;
const int JumpStrength = -350;
const int PipeSpeed = 200;
const int PipeSpawnX = 300; // Distance between pipes
const int PipeWidth = 80;
const int PIPE_GAP = 170; // Vertical gap size for bird to fly through

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
    float GetRadius() { return radius; }
    void SetPosition(Vector2 pos) { position = pos; }
    void SetVelocity(float vel) { velocity = vel; }
    void Jump() { velocity = JumpStrength; }
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
    float gapY;
    bool passed;

public:
    Pipe(float xPos, float gap) : x(xPos), gapY(gap), passed(false) {}
    float GetX() { return x; }
    float GetGapY() { return gapY; }
    bool IsPassed() { return passed; }
    void SetPassed(bool p) { passed = p; }
    void Update(float dt) { x -= PipeSpeed * dt; }

    void Draw() const
    {
        Rectangle topRec = {x, 0, (float)PipeWidth, gapY - (PIPE_GAP / 2)};
        DrawRectangleRec(topRec, GREEN);
        DrawRectangleLinesEx(topRec, 3, DARKGREEN);
        Rectangle bottomRec = {x, gapY + (PIPE_GAP / 2), (float)PipeWidth, (float)ScreenHeight - (gapY + (PIPE_GAP / 2))};
        DrawRectangleRec(bottomRec, GREEN);
        DrawRectangleLinesEx(bottomRec, 3, DARKGREEN);
    }
};

enum GameScreen
{
    LOGO = 0,
    TITLE,
    GAMEPLAY,
    ENDING
};

int main()
{
    InitWindow(ScreenWidth, ScreenHeight, "Floppy");
    SetTargetFPS(60);
    srand(time(NULL));

    GameScreen currentScreen = TITLE;
    Bird bird(20, GOLD);
    vector<Pipe> pipes;
    int score = 0;
    int highScore = 0;

    auto ResetGame = [&]()
    {
        bird.SetPosition({100, ScreenHeight / 2.0f});
        bird.SetVelocity(0);
        pipes.clear();
        score = 0;
        float randomGap = rand() % (ScreenHeight - 200) + 100;
        pipes.push_back(Pipe(ScreenWidth + 100, randomGap));
    };

    ResetGame();

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        switch (currentScreen)
        {
        case TITLE:
            if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                ResetGame();
                currentScreen = GAMEPLAY;
            }
            break;
        case GAMEPLAY:
            if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                bird.Jump();
            bird.Update(dt);

            if (pipes.empty() || (ScreenWidth - pipes.back().GetX()) >= PipeSpawnX)
            {
                float randomGap = rand() % (ScreenHeight - 250) + 125;
                pipes.push_back(Pipe(ScreenWidth, randomGap));
            }

            for (size_t i = 0; i < pipes.size(); i++)
            {
                pipes[i].Update(dt);
                Rectangle topPipeRect = {pipes[i].GetX(), 0, (float)PipeWidth, pipes[i].GetGapY() - (PIPE_GAP / 2.0f)};
                Rectangle bottomPipeRect = {pipes[i].GetX(), pipes[i].GetGapY() + (PIPE_GAP / 2.0f), (float)PipeWidth, (float)ScreenHeight};
                if (CheckCollisionCircleRec(bird.GetPosition(), bird.GetRadius() - 2, topPipeRect) ||
                    CheckCollisionCircleRec(bird.GetPosition(), bird.GetRadius() - 2, bottomPipeRect))
                {
                    currentScreen = ENDING;
                }
                if (!pipes[i].IsPassed() && pipes[i].GetX() + PipeWidth < bird.GetPosition().x)
                {
                    score++;
                    pipes[i].SetPassed(true);
                }
            }

            if (!pipes.empty() && pipes[0].GetX() < -PipeWidth)
                pipes.erase(pipes.begin());
            if (bird.GetPosition().y >= ScreenHeight + bird.GetRadius() || bird.GetPosition().y <= -bird.GetRadius())
            {
                currentScreen = ENDING;
            }
            break;
        case ENDING:
            if (score > highScore)
                highScore = score;
            if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                ResetGame();
                currentScreen = GAMEPLAY;
            }
            break;
        default:
            break;
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawRectangle(0, ScreenHeight - 20, ScreenWidth, 20, DARKBROWN);
        DrawRectangle(0, ScreenHeight - 35, ScreenWidth, 15, LIGHTGRAY);
        for (auto &pipe : pipes)
            pipe.Draw();
        bird.Draw();

        switch (currentScreen)
        {
        case TITLE:
        {
            DrawText("FLOPPY", ScreenWidth / 2 - MeasureText("FLOPPY", 40) / 2, ScreenHeight / 3, 40, WHITE);
            DrawText("Press SPACE to Jump", ScreenWidth / 2 - MeasureText("Press SPACE to Jump", 20) / 2, ScreenHeight / 2, 20, DARKGRAY);
            break;
        }
        case GAMEPLAY:
        {
            string scoreText = to_string(score);
            DrawText(scoreText.c_str(), ScreenWidth / 2 - MeasureText(scoreText.c_str(), 40) / 2, 50, 40, WHITE);
            DrawText(scoreText.c_str(), ScreenWidth / 2 - MeasureText(scoreText.c_str(), 40) / 2 + 2, 52, 40, BLACK);
        }
        break;
        case ENDING:
        {
            DrawText("GAME OVER", ScreenWidth / 2 - MeasureText("GAME OVER", 40) / 2, ScreenHeight / 3, 40, MAROON);
            string finalScore = "Score: " + to_string(score);
            string bestScore = "Best: " + to_string(highScore);
            DrawText(finalScore.c_str(), ScreenWidth / 2 - MeasureText(finalScore.c_str(), 20) / 2, ScreenHeight / 2, 20, DARKGRAY);
            DrawText(bestScore.c_str(), ScreenWidth / 2 - MeasureText(bestScore.c_str(), 20) / 2, ScreenHeight / 2 + 30, 20, DARKGRAY);
            DrawText("Press SPACE to Retry", ScreenWidth / 2 - MeasureText("Press SPACE to Retry", 20) / 2, ScreenHeight / 2 + 70, 20, WHITE);
            break;
        }
        default:
            break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
