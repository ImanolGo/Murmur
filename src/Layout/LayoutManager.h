/*
 *  LayoutManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 17/06/15.
 *
 */

#pragma once

#include "Manager.h"
#include "TextVisual.h"
#include "ImageVisual.h"
#include "RectangleVisual.h"

//========================== class LayoutManager ==============================
//============================================================================
/** \class LayoutManager LayoutManager.h
 *    \brief Class managing the layout of the application
 *    \details It creates an places al the text and elements regarding the layout
 */

class LayoutManager: public Manager
{
public:
    
    static const int MARGIN;
    static const int FRAME_MARGIN;
    static const string LAYOUT_FONT;
    static const string LAYOUT_FONT_LIGHT;
    
    //! Constructor
    LayoutManager();
    
    //! Destructor
    ~LayoutManager();
    
    //! Set-up the layout
    void setup();
    
    //! Update Layer
    void update();
    
    //! Draw Layer
    void draw();
    
    void windowResized(int w, int h);
    
    int getCropLeft() const {return m_cropLeft;}
    
    int getCropRight() const {return m_cropRight;}
    
    int getCropTop() const {return m_cropTop;}
    
    int getCropBottom() const {return m_cropBottom;}
    
    void setIsMasked(bool value){m_isMasked = value;}
    
    void setCircleOffset(const ofPoint& pos){m_circleOffset = pos;}
    
    const ofPoint& getCircleOffset(){return m_circleOffset;}
    
    bool isMasked() const {return m_isMasked;}
    
    void onCropLeft( int & pixels);
    
    void onCropRight( int & pixels);
    
    void onCropTop( int & pixels);
    
    void onCropBottom( int & pixels);
    
    ofPtr<ofRectangle> getWindowRect(string& name) {return m_windowRects[name];}
    
    void begin(string& name);
    
    void end(string& name);
    
    void drawFront();
    
    void drawTop();
    
private:
    
    
    //! Create the text visuals
    void createTextVisuals();
    
    //! Create the image visuals
    void createImageVisuals();
    
    //! Create the background Image
    void createBackground();
    
    //! Add all visuals as overlays
    void addVisuals();
    
    void setupFbos();
    
    void resetWindowRects();
        
    void resetWindowFrames();
    
    void resetWindowTitles();
    
    void setupWindowFrames();
    
    void updateFbos();
    
    void updateFrontFbo();
    
    void updateTopFbo();
    
    void drawFbos();
    
    void drawRectangles();
    
    void drawText();
    
    
private:
    
    
    typedef  map<string, ofPtr<TextVisual> >      TextMap;            ///< defines a map of Text attached to an identifier
    typedef  map<string, ofPtr<ImageVisual>  >    ImageMap;           ///< defines a map of ImageVisual Map attached to an identifier
    
    typedef  map<string, ofPtr<ofFbo>  >    FboMap;           ///< defines a map of Fbos attached to an identifier
    typedef  map<string, ofPtr<ofRectangle>  >    FboRectangles;           ///< defines a map of ofRectangle attached to an identifier
    typedef  map<string, ofPtr<RectangleVisual>  >    FboFrames;           ///< defines a map of RectangleVisual attached to an identifier
    
    TextMap             m_textVisuals;             ///< map storing the text visuals attached to a name
    ImageMap            m_imageVisuals;            ///< map storing the image visuals attached to a name
    
    ofRectangle         m_layoutRect;
    
    FboRectangles       m_windowRects;
    FboFrames           m_windowFrames;
    FboMap              m_fbos;
    
    int                 m_cropLeft, m_cropRight, m_cropTop, m_cropBottom;
    ofPoint             m_circleOffset;
    
    bool                m_isMasked;
};

//==========================================================================


