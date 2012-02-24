#pragma once

#include <vector>
#include <list>

#include "Particle.h"

class TrailController {
  public:
    TrailController();
    
    void addParticle(const ci::Vec3f &newLocation, const ci::Vec3f &newVelocity);
    void update();
    void draw(const ci::Vec3f &currentLocation, bool mouseActive);
    
    std::vector<Particle>	particles;
};