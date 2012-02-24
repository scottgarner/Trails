#include <vector>

#include "cinder/app/AppCocoaTouch.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "cinder/Camera.h"
#include "cinder/Rand.h"
#include "cinder/ImageIo.h"

#include "TrailController.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TrailsTouchApp : public AppCocoaTouch {
  public:
	virtual void	setup();
    virtual void	accelerated( AccelEvent event );    
	virtual void	update();
	virtual void    draw();
    
	CameraPersp		mCam;    
    Matrix44f		mModelView;
    Matrix44f       mProjection;
    Area            mViewPort;       
    
    virtual void    touchesBegan( TouchEvent event );
    virtual void    touchesMoved( TouchEvent event );
    virtual void    touchesEnded( TouchEvent event );    
    
    Vec3f           touchLocation;
    Vec3f           touchVelocity; 
    
    TrailController trailController;    
    bool            touchActive;    
    
	gl::Texture     splashTexture;  
};

void TrailsTouchApp::setup()
{
    touchActive = false;
    
	enableAccelerometer();
    
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	mCam.lookAt( Vec3f( 0, 0, 5), Vec3f::zero() );	    
    
    gl::disableDepthRead();
    gl::enableDepthWrite();
    gl::enableAlphaBlending();  
    
	splashTexture = gl::Texture( loadImage( loadResource( RES_SPLASH ) ) );       
    
	touchLocation = Vec3f::zero();
	touchVelocity = Vec3f::zero(); 
}

void TrailsTouchApp::accelerated( AccelEvent event )
{    
    
	if( event.isShake() ) {
        trailController.particles.clear();
        console() << "shake" << std::endl;
    }
}      

void TrailsTouchApp::touchesBegan( TouchEvent event )
{
	for( vector<TouchEvent::Touch>::const_iterator curTouch = event.getTouches().begin(); curTouch != event.getTouches().end(); ++curTouch ) {
        touchActive = true;
        
        touchVelocity = Vec3f::zero();
        touchLocation = Vec3f(curTouch->getPos(),0);
        
        trailController.addParticle(touchLocation, touchVelocity);
	}
}

void TrailsTouchApp::touchesMoved( TouchEvent event )
{
	for( vector<TouchEvent::Touch>::const_iterator curTouch = event.getTouches().begin(); curTouch != event.getTouches().end(); ++curTouch ) {
        
        touchVelocity = ( Vec3f(curTouch->getPos(),0) - touchLocation );
        touchLocation = Vec3f(curTouch->getPos(),0);
    }
}

void TrailsTouchApp::touchesEnded( TouchEvent event )
{
	for( vector<TouchEvent::Touch>::const_iterator curTouch = event.getTouches().begin(); curTouch != event.getTouches().end(); ++curTouch ) {
        //touchVelocity = ( Vec3f(curTouch->getPos(),0) - touchLocation );
        touchLocation = Vec3f(curTouch->getPos(),0);        
        
        trailController.addParticle(touchLocation, touchVelocity);
        
		touchActive = false;
	}
}

void TrailsTouchApp::update()
{
    if (touchActive && app::getElapsedFrames() % 10 == 0)
        //if (touchActive)
        trailController.addParticle(touchLocation, touchVelocity / 1);
    trailController.update(); 
}

void TrailsTouchApp::draw()
{
    gl::setMatricesWindowPersp(getWindowSize());    
	gl::clear(Color(0.2f, 0.2f, 0.2f)); 
    trailController.draw(touchLocation, touchActive);    
    
    // Show Splash
    
    if (getElapsedFrames() < 320) {
        float splashAlpha;
        
        if (getElapsedFrames() > 240)
            splashAlpha = 1.0 - ((getElapsedFrames() - 240.0) / 60.0);
        else
            splashAlpha = 1.0f;
        
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        
        glColor4f( 1.0f, 1.0f, 1.0f, splashAlpha );    
        
        gl::draw(splashTexture,
                 Vec2f((getWindowWidth() / 2) - (splashTexture.getWidth() / 2),
                       (getWindowHeight() / 2) - (splashTexture.getHeight() / 2)));
    }
}    

CINDER_APP_COCOA_TOUCH( TrailsTouchApp, RendererGl )
