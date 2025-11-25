
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
const int PipeSpawnX = 300;
const int PipeWidth = 80;
const int PipeGap = 170;

class Bird {
public:
    Vector2 position; float velocity; float radius;
    Bird(float r = 20) : radius(r), velocity(0), position({100, ScreenHeight / 2.0f}) {}
    void Update(float dt) { velocity += Gravity * dt; position.y += velocity * dt; }
    void Jump() { velocity = JumpStrength; }
    void Draw() const {
        DrawCircleV(position, radius, GOLD);
        DrawCircle(position.x + 10, position.y - 5, 5, WHITE);
        DrawCircle(position.x + 12, position.y - 5, 2, BLACK);
        DrawTriangle({position.x + 10, position.y + 5}, {position.x + 25, position.y + 10}, {position.x + 10, position.y + 15}, ORANGE);
    }
};

class Pipe {
public:
    float x, gapY; bool passed;
    Pipe(float xPos, float gap) : x(xPos), gapY(gap), passed(false) {}
    void Update(float dt) { x -= PipeSpeed * dt; }
    void Draw() const {
        DrawRectangleRec({x, 0, (float)PipeWidth, gapY - (PipeGap / 2)}, GREEN);
        DrawRectangleLinesEx({x, 0, (float)PipeWidth, gapY - (PipeGap / 2)}, 3, DARKGREEN);
        DrawRectangleRec({x, gapY + (PipeGap / 2), (float)PipeWidth, ScreenHeight - (gapY + PipeGap / 2)}, GREEN);
        DrawRectangleLinesEx({x, gapY + (PipeGap / 2), (float)PipeWidth, ScreenHeight - (gapY + PipeGap / 2)}, 3, DARKGREEN);
    }
};

void ResetGame(Bird &bird, vector<Pipe> &pipes, int &score, bool &gameOver) {
    bird.position = {100, ScreenHeight / 2.0f};
    bird.velocity = 0;
    pipes.clear();
    score = 0;
    gameOver = false;
    pipes.push_back(Pipe(ScreenWidth, rand() % (ScreenHeight - 250) + 125));
}

int main() {
    InitWindow(ScreenWidth, ScreenHeight, “Floppy”);
    SetTargetFPS(60);
    srand(time(NULL));

    Bird bird;
    vector<Pipe> pipes;
    int score = 0;
    bool gameOver = false;

    ResetGame(bird, pipes, score, gameOver);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (!gameOver) {
            if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) bird.Jump();
            bird.Update(dt);

            if (pipes.empty() || (ScreenWidth - pipes.back().x) >= PipeSpawnX) {
                pipes.push_back(Pipe(ScreenWidth, rand() % (ScreenHeight - 250) + 125));
            }

            for (auto &pipe : pipes) {
                pipe.Update(dt);
                if (!pipe.passed && pipe.x + PipeWidth < bird.position.x) {
                    score++;
                    pipe.passed = true;
                }
                Rectangle top = {pipe.x, 0, (float)PipeWidth, pipe.gapY - (PipeGap / 2.0f)};
                Rectangle bot = {pipe.x, pipe.gapY + (PipeGap / 2.0f), (float)PipeWidth, (float)ScreenHeight};
                if (CheckCollisionCircleRec(bird.position, bird.radius - 2, top) || CheckCollisionCircleRec(bird.position, bird.radius - 2, bot)) {
                    gameOver = true;
                }
            }

            if (!pipes.empty() && pipes.front().x < -PipeWidth) pipes.erase(pipes.begin());
            if (bird.position.y >= ScreenHeight - bird.radius || bird.position.y <= bird.radius) gameOver = true;
        } else {
            if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                ResetGame(bird, pipes, score, gameOver);
            }
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);
        for (auto &pipe : pipes) pipe.Draw();
        bird.Draw();
        DrawRectangle(0, ScreenHeight - 20, ScreenWidth, 20, DARKBROWN);
        
        string scoreText = "Score: " + to_string(score);
        DrawText(scoreText.c_str(), 10, 10, 20, WHITE);

        if (gameOver) DrawText("GAME OVER", ScreenWidth / 2 - MeasureText("GAME OVER", 40) / 2, ScreenHeight / 2 - 20, 40, MAROON);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
