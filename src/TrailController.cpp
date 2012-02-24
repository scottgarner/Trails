#include "cinder/Rand.h"
#include "cinder/app/AppBasic.h"

#include "TrailController.h"

using namespace ci;

using std::list;
using std::vector;

TrailController::TrailController()
{
    
}

void TrailController::addParticle(const Vec3f &newLocation, const Vec3f &newVelocity)
{
    particles.push_back( Particle( newLocation, newVelocity));
}



void TrailController::update()
{
    // Particles
    
	for( vector<Particle>::iterator curParticle = particles.begin(); curParticle != particles.end(); ){
		if( curParticle->age >= 500.0 ){
			curParticle = particles.erase( curParticle );
		} else {
			curParticle->update();
			curParticle++;
		}
	}     
    
}

void TrailController::draw(const ci::Vec3f &currentLocation, bool mouseActive)
{
    std::vector<Vec3f> points;
    std::vector<ColorA> colors;
    
    for( vector<Particle>::iterator curParticle = particles.begin(); curParticle != particles.end(); curParticle++ ) {
		curParticle->draw();
        
        points.push_back(curParticle->location );
        colors.push_back(curParticle->color);
	}
    
    if (mouseActive) {
        points.push_back(currentLocation);
        colors.push_back(ColorA(1,1,.75,.5));
    }
    
    glEnableClientState( GL_VERTEX_ARRAY );     
    glVertexPointer( 3, GL_FLOAT, 0, &points[0].x );  
    
    glDrawArrays( GL_POINTS, 0, points.size() );
    
    glEnableClientState( GL_COLOR_ARRAY );    
    glColorPointer( 4, GL_FLOAT, 0, &colors[0].r );      

    glDrawArrays( GL_LINE_STRIP, 0, points.size() );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, points.size() );    

    glDisableClientState( GL_COLOR_ARRAY );   
    glDisableClientState( GL_VERTEX_ARRAY );  
}








