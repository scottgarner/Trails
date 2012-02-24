#pragma once

#include <vector>

#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/Text.h"

class Particle {
    public:
    Particle();
    Particle(const ci::Vec3f, const ci::Vec3f);

    void update();
    void draw();
    
    ci::Vec3f location;
    ci::Vec3f velocity;
    ci::ColorA color;
    ci::gl::Texture label;

    int age;
};