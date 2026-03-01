#include <vector>
#include "raylib.h"
#include "raymath.h"

const int SUBSTEPS{8};
const int screenWidth = 1280;
const int screenHeight = 800;
const float GRAVITY = 490.f; // 9.8 ms^2 x 100pixel/m = 980 pixels/s^2
const int NUM_PARTICLES = 5;

struct Particle{
    Vector2 pos;
    Vector2 pos_old;
    Vector2 acc;
    float radius, mass{radius};
    Color c;

    // Constructor
    Particle() {
        pos = { (float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight) };
        pos_old = Vector2Subtract(pos, { (float)GetRandomValue(-2, 2), (float)GetRandomValue(-2, 2) });
        acc = { 0, GRAVITY };
        radius = 20.0f;
        c = ColorFromHSV((float)GetRandomValue(0, 360), 0.8f, 0.9f);
    }
};

// Declare the particles
std::vector<Particle> particles;

void update_pos(Particle &p, float dt) {
    // Friction
    float friction = 0.99f;

    // Calculate velocity (current position - old position)
    Vector2 velocity = Vector2Subtract(p.pos, p.pos_old);
    
    // Update old position
    p.pos_old = p.pos;
    
    // pos + velocity + (acc * dt)
    Vector2 acceleration = Vector2Scale(p.acc, dt * dt);
    p.pos = Vector2Add(Vector2Add(p.pos, velocity), acceleration);
}

void handle_wall_collision(Particle &p) {
    float bounce = 1.0f;

     // Check for wall collisions
    if (p.pos.x + p.radius >= screenWidth) {
            float velocity_x = p.pos.x - p.pos_old.x;
            p.pos.x = screenWidth - p.radius;
            p.pos_old.x = p.pos.x + (velocity_x * bounce); 
        }
        else if (p.pos.x - p.radius <= 0) {
            float velocity_x = p.pos.x - p.pos_old.x;
            p.pos.x = p.radius;
            p.pos_old.x = p.pos.x + (velocity_x * bounce);
        }

        if (p.pos.y + p.radius >= screenHeight) {
            float bounce = 0.7f;    
            float wall_friction = 0.95f; 

            float vx = p.pos.x - p.pos_old.x;
            float vy = p.pos.y - p.pos_old.y;
            p.pos.y = screenHeight - p.radius;
            p.pos_old.y = p.pos.y + (vy * bounce);
            p.pos_old.x = p.pos.x - (vx * wall_friction);
        }
        else if (p.pos.y - p.radius <= 0) {
            float velocity_y = p.pos.y - p.pos_old.y;
            p.pos.y = p.radius;
            p.pos_old.y = p.pos.y + (velocity_y * bounce);
        }
}

void handle_particle_collision(Particle& p1, Particle& p2){
    Vector2 collision_axis = Vector2Subtract(p1.pos, p2.pos);
    float distance = Vector2Length(collision_axis);
    float min_dist = p1.radius + p2.radius;

    if (distance < min_dist && distance > 0) {
        Vector2 normal = Vector2Scale(collision_axis, 1.0f / distance);
        float overlap = min_dist - distance;
        
        // Static Resolution prevent overlapping
        float m1 = p1.radius; // use radius as a proxy for mass
        float m2 = p2.radius;
        float mass_ratio1 = m2 / (m1 + m2);
        float mass_ratio2 = m1 / (m1 + m2);

        p1.pos = Vector2Add(p1.pos, Vector2Scale(normal, overlap * mass_ratio1));
        p2.pos = Vector2Subtract(p2.pos, Vector2Scale(normal, overlap * mass_ratio2));

        // Dynamic Resolution, bounce
        Vector2 v1 = Vector2Subtract(p1.pos, p1.pos_old);
        Vector2 v2 = Vector2Subtract(p2.pos, p2.pos_old);
        
        // Relative velocity
        Vector2 relative_vel = Vector2Subtract(v1, v2);
        float velocity_along_normal = Vector2DotProduct(relative_vel, normal);

        // Only resolve if they are moving TOWARDS each other
        if (velocity_along_normal < 0) {
            float restitution = 0.8f; // 1.0 = perfect bounce, 0.0 = clay
            float j = -(1.0f + restitution) * velocity_along_normal;
            j /= (1.0f / m1 + 1.0f / m2);

            Vector2 impulse = Vector2Scale(normal, j);
            
            p1.pos_old = Vector2Subtract(p1.pos_old, Vector2Scale(impulse, 1.0f / m1));
            p2.pos_old = Vector2Add(p2.pos_old, Vector2Scale(impulse, 1.0f / m2));
        }
    }
}

bool is_colliding(Particle& p_a, Particle& p_b){
    return (Vector2Distance(p_a.pos, p_b.pos) <= (p_a.radius + p_b.radius));
}

void update(float dt) {
    for (Particle &p : particles) {
        update_pos(p, dt);
        handle_wall_collision(p);
}

    for(int i{0}; i < NUM_PARTICLES - 1; i++) {
        for (int j{i + 1}; j < NUM_PARTICLES; j++) {
            if (is_colliding(particles[i], particles[j])) {
                handle_particle_collision(particles[i], particles[j]);
                Vector3 color_a = ColorToHSV(particles[i].c);
                Vector3 color_b = ColorToHSV(particles[j].c);

                particles[i].c = ColorFromHSV((float)GetRandomValue(0, 360), 0.8f, 0.9f);
                particles[j].c = ColorFromHSV((float)GetRandomValue(0, 360), 0.8f, 0.9f);
            }
        }
    }
}

void draw() {
    // Draw loop
    ClearBackground(BLACK);
    DrawFPS(0, 0);

    for (const Particle &p : particles) {
        DrawCircleV(p.pos, p.radius, p.c);
    }
}

int main() {
    SetTargetFPS(120);
    InitWindow(screenWidth, screenHeight, "ParticleSim Trial");

    // Execute particles
    particles.reserve(NUM_PARTICLES);
    for (int i{0}; i < NUM_PARTICLES; i++) { 
        particles.push_back(Particle()); // Call the constructor
    }

while (!WindowShouldClose()) {
    float frame_time = GetFrameTime();
    float dt = (1.0f / 60.0f) / SUBSTEPS;

    for(int i{0}; i < SUBSTEPS; i++) {
        // Update frame
        update(dt);
    }
    

    //Draw loop
    BeginDrawing();
        draw();
    EndDrawing();
}

CloseWindow();
return 0;
}