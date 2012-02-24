#include "Particle.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"

using namespace ci;
using namespace ci::app;

Particle::Particle()
{
    
}

Particle::Particle( Vec3f newLocation, Vec3f newVelocity)
{
	location = newLocation;
    age = 0;
    
    if (getElapsedFrames()/10 % 2 == 0) 
        velocity = Vec3f(Rand::randFloat( -.25f, .25f ), Rand::randFloat( 0.0f, .25f ),Rand::randFloat( -.25f, .25f ));
    else
        velocity = Vec3f(Rand::randFloat( -.25f, .25f ), -Rand::randFloat( 0.0f, .25f ),Rand::randFloat( -.25f, .25f ));
    
    newVelocity /= 10;
    velocity += newVelocity;
    
    color = ColorA(.5 + Rand::randFloat(0.0f,.5f), .5 + Rand::randFloat(0.0f,.5f), .5 +sin(getElapsedFrames()/50) + 1 /4, 1);
    
}

void Particle::update()
{
    color.a = .5 * (1 - age++/500.0);
    location += velocity * .9;    
}

void Particle::draw()
{
//    char* locationString = new char[10];
//    sprintf(locationString, "%i", (int)location.x);
    
    //TextLayout layout;
    
    //layout.setFont( Font( "HelveticaNeue-Bold", 12.0f ) );
    //layout.setColor( Color( 1, 0, 0 ) );
    //layout.addCenteredLine( locationString );
    
    //label = gl::Texture( layout.render( true ) );
    
    //gl::draw( label, Vec2f( 10, 10 ) );
	//gl::drawSphere( location, 1, 4 );
//    gl::drawStrokedCircle(Vec2f(location.x, location.y),2);
    
//    gl::drawString (locationString, Vec2f(location.x, location.y)); 
    
    
}