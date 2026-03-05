#include "Particle.h"
#include <vector>

const int SUBSTEPS{8};
const int screenWidth = 1280;
const int screenHeight = 800;
const float GRAVITY = 490.f;
const int NUM_PARTICLES = 5;

void handle_particle_collision(Particle& p1, Particle& p2) {
    Vector2 axis = Vector2Subtract(p1.pos, p2.pos);
    float dist = Vector2Length(axis);
    float min_dist = p1.radius + p2.radius;

    if (dist < min_dist && dist > 0) {
        Vector2 normal = Vector2Scale(axis, 1.0f / dist);
        float overlap = min_dist - dist;
        
        float m1 = p1.mass, m2 = p2.mass;
        p1.pos = Vector2Add(p1.pos, Vector2Scale(normal, overlap * (m2 / (m1 + m2))));
        p2.pos = Vector2Subtract(p2.pos, Vector2Scale(normal, overlap * (m1 / (m1 + m2))));

        // Momentum / Bounce
        Vector2 v1 = Vector2Subtract(p1.pos, p1.pos_old);
        Vector2 v2 = Vector2Subtract(p2.pos, p2.pos_old);
        float rel_vel = Vector2DotProduct(Vector2Subtract(v1, v2), normal);

        if (rel_vel < 0) {
            float j = -(1.8f) * rel_vel / (1.0f / m1 + 1.0f / m2);
            p1.pos_old = Vector2Subtract(p1.pos_old, Vector2Scale(normal, j / m1));
            p2.pos_old = Vector2Add(p2.pos_old, Vector2Scale(normal, j / m2));
        }
    }
}

bool is_colliding(const Particle& p_a, const Particle& p_b) {
    return (Vector2Distance(p_a.pos, p_b.pos) <= (p_a.radius + p_b.radius));
}

int main() {
    InitWindow(screenWidth, screenHeight, "ParticleSim OOP");
    SetTargetFPS(120);

    std::vector<Particle> particles;
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

            for (int i = 0; i < (int)particles.size() - 1; i++) {
                for (int j = i + 1; j < (int)particles.size(); j++) {
                    if (is_colliding(particles[i], particles[j])) {
                        handle_particle_collision(particles[i], particles[j]);
                        particles[i].c = ColorFromHSV((float)GetRandomValue(0, 360), 0.8f, 0.9f);
                        particles[j].c = ColorFromHSV((float)GetRandomValue(0, 360), 0.8f, 0.9f);
                    }
                }
            }
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