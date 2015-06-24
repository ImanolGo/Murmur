#include "ofMain.h"
#include "ofxMultiGLFWWindow.h"
#include "ofGLProgrammableRenderer.cpp"
#include "MurmurRendererApp.h"


//========================================================================
int main( ){

    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    
    ofxMultiGLFWWindow glfw;
    glfw.setOpenGLVersion(3,2); // must be set
    
    glfw.windowCount = 2;
    ofSetupOpenGL(&glfw,1024,768,OF_WINDOW);
    
    ofRunApp(new MurmurRendererApp());

}
