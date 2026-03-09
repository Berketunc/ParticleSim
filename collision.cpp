#include "collision.h"

bool collision::is_colliding(const Particle& p1, const Particle& p2) {
    return Vector2Distance(p1.pos, p2.pos) <= (p1.radius + p2.radius);
}

void collision::handle_particle_collision(Particle& p1, Particle& p2) {
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

void collision::update(float dt, int substeps, std::vector<Particle>& particles, int screenW, int screenH) {
    // Handle collisions 
    for (size_t i = 0; i < particles.size(); i++) {
        for (size_t j = i + 1; j < particles.size(); j++) {
            if (is_colliding(particles[i], particles[j])) {
                handle_particle_collision(particles[i], particles[j]);

                collision_count++; 
                
                // Collision colors
                particles[i].c = ColorFromHSV((float)GetRandomValue(0, 360), 0.8f, 0.9f);
                particles[j].c = ColorFromHSV((float)GetRandomValue(0, 360), 0.8f, 0.9f);
            }
        }
    }
}