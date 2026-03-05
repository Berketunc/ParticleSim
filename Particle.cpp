#include "Particle.h"

Particle::Particle(int screenW, int screenH, float gravity) {
    pos = { (float)GetRandomValue(0, screenW), (float)GetRandomValue(0, screenH) };
    pos_old = Vector2Subtract(pos, { (float)GetRandomValue(-3, 3), (float)GetRandomValue(-3, 3) });
    acc = { 0, gravity };
    radius = 20.0f;
    mass = radius;
    c = ColorFromHSV((float)GetRandomValue(0, 360), 0.8f, 0.9f);
}

void Particle::update_pos(float dt) {
    Vector2 velocity = Vector2Subtract(pos, pos_old);
    pos_old = pos;
    Vector2 acceleration = Vector2Scale(acc, dt * dt);
    pos = Vector2Add(Vector2Add(pos, velocity), acceleration);
}

void Particle::handle_wall_collision(int screenW, int screenH) {
    float bounce = 1.0f;
    if (pos.x + radius >= screenW || pos.x - radius <= 0) {
        float vx = pos.x - pos_old.x;
        pos.x = (pos.x + radius >= screenW) ? screenW - radius : radius;
        pos_old.x = pos.x + (vx * bounce);
    }

    if (pos.y + radius >= screenH) {
        float ground_bounce = 0.7f;
        float wall_friction = 0.95f;
        float vx = pos.x - pos_old.x;
        float vy = pos.y - pos_old.y;
        pos.y = screenH - radius;
        pos_old.y = pos.y + (vy * ground_bounce);
        pos_old.x = pos.x - (vx * wall_friction);
    } else if (pos.y - radius <= 0) {
        float vy = pos.y - pos_old.y;
        pos.y = radius;
        pos_old.y = pos.y + (vy * bounce);
    }
}

void Particle::draw() const {
    DrawCircleV(pos, radius, c);
}