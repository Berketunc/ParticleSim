#include "Particle.h"
#include "collision.h"

const int SUBSTEPS{8};
const int screenWidth = 1280;
const int screenHeight = 800;
const float GRAVITY = 490.f;
const int NUM_PARTICLES = 5;

int main() {
    InitWindow(screenWidth, screenHeight, "Full OOP Particle Simulation");
    SetTargetFPS(120);

    std::vector<Particle> particles;
    collision col;
    particles.reserve(NUM_PARTICLES);
    for (int i = 0; i < NUM_PARTICLES; i++) {
        particles.emplace_back(screenWidth, screenHeight, GRAVITY);
    }

    while (!WindowShouldClose()) {
        float dt = (1.0f / 60.0f) / SUBSTEPS;

        for (int i = 0; i < SUBSTEPS; i++) {
            for (auto& p : particles) {
                p.update_pos(dt);
                p.handle_wall_collision(screenWidth, screenHeight);
            }

            col.update(dt, SUBSTEPS, particles, screenWidth, screenHeight);
        }

        BeginDrawing();
            ClearBackground(BLACK);
            for (const auto& p : particles) p.draw();
            DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}