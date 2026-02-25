#include "raylib.h"
#include <vector>

const int screenWidth = 1280;
const int screenHeight = 800;
const float GRAVITY = 0.08f;

struct Particle{
    float x, y, radius, vx, vy;
    Color c;
    float ax, ay;

};

// Create the particles
std::vector<Particle> particles;

void update() {
    for (Particle &p : particles) {
        // Update x and y position based on velocity
        p.x += p.vx;
        p.y += p.vy;

        // Update x and y velocity based on acceleration
        p.vx += p.ax;
        p.vy += p.ay;

    if (p.x + p.radius >= screenWidth) {
            p.x = screenWidth - p.radius; 
            p.vx *= -0.8f; 
        }
        else if (p.x - p.radius <= 0) {
            p.x = p.radius; 
            p.vx *= -0.8f; 
        }

        if (p.y + p.radius >= screenHeight) {
            p.y = screenHeight - p.radius;
            p.vy *= -0.8f;
        }
        else if (p.y - p.radius <= 0) {
            p.y = p.radius;
            p.vy *= -0.8f;
        }
    }
}

void draw() {
    // Draw loop
    ClearBackground(BLACK);
    DrawFPS(0, 0);

    for (const Particle &p : particles) {
        DrawCircle(p.x, p.y, p.radius, p.c);
    }
}

int main() {
    SetTargetFPS(120);
    InitWindow(screenWidth, screenHeight, "ParticleSim Trial");

    for (int i{0}; i < 10; i++) { 
        particles.push_back(Particle{
            (float)GetRandomValue(0, screenWidth), 
            (float)GetRandomValue(0, screenHeight), 
            10, 
            (float)GetRandomValue(-5, 5), 
            (float)GetRandomValue(-5, 5), 
            ColorFromHSV((float)GetRandomValue(0, 360), 0.8f, 0.9f),
            0, GRAVITY  
        });
    }

while (!WindowShouldClose()) {
    // Update frame
    update();

    //Draw loop
    BeginDrawing();
        draw();
    EndDrawing();
}

CloseWindow();
return 0;
}