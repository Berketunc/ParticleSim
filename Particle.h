#ifndef PARTICLE_H
#define PARTICLE_H

#include "raylib.h"
#include "raymath.h"

class Particle {
public:
    Vector2 pos;
    Vector2 pos_old;
    Vector2 acc;
    float radius;
    float mass;
    Color c;

    // Constructor declaration
    Particle(int screenW, int screenH, float gravity);

    // Function declarations / members
    void update_pos(float dt);
    void handle_wall_collision(int screenW, int screenH);
    void draw() const;
};

#endif