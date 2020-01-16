/*
 *  FluidVisual.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 29/05/15.
 *
 */


#pragma once

#include "ofxFlowTools.h"
#include "ofxGui.h"

#define USE_FASTER_INTERNAL_FORMATS			// about 15% faster but gives errors from ofGLUtils
using namespace flowTools;

//========================== class FluidManager ==============================
//============================================================================
/** \class FluidVisual FluidVisual.h
 *	\brief Class managing the fluid visuals
 *	\details It creates and manages all the parameters and fluid-like visualizations 
 */


class FluidVisual
{
    
public:
    
    //! Constructor
    FluidVisual();
    
    //! Destructor
    ~FluidVisual();
    
    //! Setup the Fluid Visual
    void setup(string settingsName, float downSampling = 1.0f);
    
    //! Update the Fluid Visual
    void update();
    
    //! Draw the Fluid Visual
    //void draw();
    
     //! Draw the Fluid Visual within a given area
    void draw(const ofRectangle& area);
    
    //! Draw the Fluid Visual GUI
    void drawGui();
    
    //! Set the source texture use to create the fluid
    void setSource(const ofFbo& source);
    
    //Gui
    void	setupGui();
    
    void    resetDrawForces(bool& _value) { if (_value) {for (int i=0; i<m_numDrawForces; i++) m_flexDrawForces[i].reset();}}
   
    void	setDisplayScalarScale(float& _value) { m_displayScalar.setScale(_value); }
   
    void	setVelocityFieldArrowScale(float& _value) { m_velocityField.setVectorSize(_value); }
   
    void	setTemperatureFieldBarScale(float& _value) { m_temperatureField.setVectorSize(_value); }

    void	setVisualisationLineSmooth(bool& _value) { m_velocityField.setLineSmooth(_value); }
    
    void    addForce(ofVec2f position);
   
    
private:
    
    void drawVisuals();
    
    void setupFluid();
    
    void updateFluid();
    
    void drawFluid();
    
public:
    
    static const int FLUID_WIDTH;
    static const int FLUID_HEIGHT;
    
    ofxPanel            m_gui;
    string              m_guiSettingsName;
    ofParameter<bool>	toggleGuiDraw;
    ofParameterGroup	visualisationParameters;
    
    ofParameterGroup	drawForceParameters;
    ofParameter<bool>	doResetDrawForces;
    ofParameterGroup	leftButtonParameters;
    ofParameterGroup	rightButtonParameters;
    ofParameter<bool>	showScalar;
    ofParameter<bool>	showField;
    ofParameter<float>	displayScalarScale;
    ofParameter<float>	velocityFieldArrowScale;
    ofParameter<float>	temperatureFieldBarScale;
    ofParameter<bool>	visualisationLineSmooth;
    
    
    void reset();
    
private:
    
    
    ftOpticalFlow		m_opticalFlow;
    ftVelocityMask		m_velocityMask;
    ftFluidSimulation	m_fluid;
    
    ftDisplayScalar		m_displayScalar;
    ftVelocityField		m_velocityField;
    ftTemperatureField	m_temperatureField;
    
    float               m_downSampling;
    
    int					m_numDrawForces;
    ftDrawForce*		m_flexDrawForces;
    
    ofFbo               m_source;
    
    ofVec2f				m_lastPosition;
    
    
};



