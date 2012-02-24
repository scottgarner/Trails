#include <vector>

#include "cinder/app/AppBasic.h"
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

class TrailsApp : public AppBasic {
  public:
	virtual void    setup();
    virtual void    prepareSettings( Settings *settings );
	virtual void    update();
	virtual void    draw();
    
	CameraPersp		mCam;    
    Matrix44f		mModelView;
    Matrix44f       mProjection;
    Area            mViewPort;       

    virtual void    keyDown( KeyEvent event );
    virtual void    mouseDrag( MouseEvent event );
    virtual void    mouseMove( MouseEvent event );
	virtual void    mouseDown( MouseEvent event );
    virtual void    mouseUp( MouseEvent event );
    
    Vec3f           mouseLocation;
    Vec3f           mouseVelocity; 
    
    TrailController trailController;    
    bool            mouseActive;    
    
	gl::Texture     splashTexture;    
    
    int             frameCounter;
};

void TrailsApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 1280, 720 );
	settings->setFrameRate( 60.0f );
}

void TrailsApp::setup()
{
    mouseActive = false;
    
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	mCam.lookAt( Vec3f( 0, 0, 5), Vec3f::zero() );	    
    
    gl::disableDepthRead();
    gl::enableDepthWrite();
    gl::enableAlphaBlending();  
    
	splashTexture = gl::Texture( loadImage( loadResource( RES_SPLASH ) ) );    
    
	mouseLocation = Vec3f::zero();
	mouseVelocity = Vec3f::zero(); 
    
    frameCounter = 0;
}

void TrailsApp::keyDown( KeyEvent event )
{
	if( event.getCode() == KeyEvent::KEY_f ) {
		setFullScreen( ! isFullScreen() );
	}
    else if( event.getCode() == KeyEvent::KEY_s ) {
        writeImage( getHomeDirectory() + "trails_" + toString( frameCounter ) + ".png", copyWindowSurface() );
        frameCounter++;
	}
	else if( event.getCode() == KeyEvent::KEY_ESCAPE ) {
		setFullScreen( false );
	}
    else if ( event.getCode() == KeyEvent::KEY_SPACE ) {
        trailController.particles.clear();
    }
}

void TrailsApp::mouseDown( MouseEvent event )
{
    mouseActive = true;
    
    mouseVelocity = Vec3f::zero();
    mouseLocation = Vec3f(event.getPos(),0);
    
    trailController.addParticle(mouseLocation, mouseVelocity);    
}

void TrailsApp::mouseDrag( MouseEvent event )
{
	//mouseMove( event );
    mouseVelocity = ( Vec3f(event.getPos(),0) - mouseLocation );
    mouseLocation = Vec3f(event.getPos(),0);    
}

void TrailsApp::mouseMove( MouseEvent event )
{
    //mouseVelocity = ( Vec3f(event.getPos(),0) - mouseLocation );
    //mouseLocation = Vec3f(event.getPos(),0);
}

void TrailsApp::mouseUp( MouseEvent event )
{

    //mouseVelocity = ( Vec3f(event.getPos(),0) - mouseLocation );
    mouseLocation = Vec3f(event.getPos(),0);       
        
    trailController.addParticle(mouseLocation, mouseVelocity);
        
    mouseActive = false;
}

void TrailsApp::update()
{
    if (mouseActive && app::getElapsedFrames() % 10 == 0) {
        trailController.addParticle(mouseLocation, mouseVelocity);
        //console() << mouseVelocity << std::endl;
    }
    trailController.update(); 
}

void TrailsApp::draw()
{
    
    gl::setMatricesWindowPersp(getWindowSize());    
	gl::clear(Color(0.2f, 0.2f, 0.2f)); 
    trailController.draw(mouseLocation, mouseActive);    
    
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



CINDER_APP_BASIC( TrailsApp, RendererGl )
