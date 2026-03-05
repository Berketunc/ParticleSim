#ifndef COLLISION_H
#define COLLISION_H

#include <vector>
#include "Particle.h"

class collision {
public:
    // Methods to update the simulation
    void update(float dt, int substeps, std::vector<Particle>& particles, int screenW, int screenH);
    
private:
    // Physics logic
    bool is_colliding(const Particle& p1, const Particle& p2);
    void handle_particle_collision(Particle& p1, Particle& p2);
};

#endif