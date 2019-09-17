/*
 *  FluidVisual.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 05/05/15.
 *
 */


#include "ofMain.h"

#include "AppManager.h"

#include "FluidVisual.h"


const int FluidVisual::FLUID_WIDTH = 1280;
const int FluidVisual::FLUID_HEIGHT = 720;

FluidVisual::FluidVisual(): m_downSampling(1.0f)
{
    //Intentionaly left empty
}


FluidVisual::~FluidVisual()
{
   m_gui.saveToFile(m_guiSettingsName);
}


void FluidVisual::setup(string settingsName, float downSampling)
{
    m_guiSettingsName = settingsName;
    m_downSampling = downSampling;
    
    this->setupFluid();
    this->setupGui();
}


void FluidVisual::setupFluid()
{
    
    float drawWidth = FLUID_WIDTH*m_downSampling;
    float drawHeight = FLUID_HEIGHT*m_downSampling;
    
    // process all but the density on 16th resolution
    int flowWidth = drawWidth/4;
    int flowHeight = drawHeight/4;
    
    // Flow & Mask
    m_opticalFlow.setup(flowWidth, flowHeight);
    m_velocityMask.setup(drawWidth, drawHeight);
    
    // m_fluid
    #ifdef USE_FASTER_INTERNAL_FORMATS
        m_fluid.setup(flowWidth, flowHeight, drawWidth, drawHeight, true);
    #else
        m_fluid.setup(flowWidth, flowHeight, drawWidth, drawHeight, false);
    #endif
    
    
    // Visualisation
    m_displayScalar.allocate(flowWidth, flowHeight);
    m_velocityField.allocate(flowWidth / 4, flowHeight / 4);
    m_temperatureField.allocate(flowWidth / 4, flowHeight / 4);
    
    // Draw Forces
    m_numDrawForces = 3;
    m_flexDrawForces = new ftDrawForce[m_numDrawForces];
    m_flexDrawForces[0].setup(drawWidth, drawHeight, FT_DENSITY, true);
    m_flexDrawForces[0].setName("draw full res");
    m_flexDrawForces[1].setup(flowWidth, flowHeight, FT_VELOCITY, true);
    m_flexDrawForces[1].setName("draw flow res 1");
    m_flexDrawForces[2].setup(flowWidth, flowHeight, FT_TEMPERATURE, true);
    m_flexDrawForces[2].setName("draw flow res 2");
    
}

void FluidVisual::setupGui()
{
    m_gui.setup("FluidGUI", m_guiSettingsName);
    m_gui.setPosition(300,20);
    m_gui.setDefaultBackgroundColor(ofColor(0, 0, 0, 127));
    m_gui.setDefaultFillColor(ofColor(160, 160, 160, 160));
  
    int guiColorSwitch = 0;
    ofColor guiHeaderColor[2];
    guiHeaderColor[0].set(160, 160, 80, 200);
    guiHeaderColor[1].set(80, 160, 160, 200);
    ofColor guiFillColor[2];
    guiFillColor[0].set(160, 160, 80, 200);
    guiFillColor[1].set(80, 160, 160, 200);
    
    m_gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    m_gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    m_gui.add(m_opticalFlow.parameters);
    
    m_gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    m_gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    m_gui.add(m_velocityMask.parameters);
    
    m_gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    m_gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    m_gui.add(m_fluid.parameters);
    
    
    visualisationParameters.setName("visualisation");
    visualisationParameters.add(showScalar.set("show scalar", true));
    visualisationParameters.add(showField.set("show field", true));
    visualisationParameters.add(displayScalarScale.set("display scalar scale", 0.25, 0.05, 0.5));
    displayScalarScale.addListener(this, &FluidVisual::setDisplayScalarScale);
    visualisationParameters.add(velocityFieldArrowScale.set("arrow scale", 0.6, 0.2, 1));
    velocityFieldArrowScale.addListener(this, &FluidVisual::setVelocityFieldArrowScale);
    visualisationParameters.add(temperatureFieldBarScale.set("temperature scale", 0.25, 0.05, 0.5));
    temperatureFieldBarScale.addListener(this, &FluidVisual::setTemperatureFieldBarScale);
    visualisationParameters.add(visualisationLineSmooth.set("line smooth", false));
    visualisationLineSmooth.addListener(this, &FluidVisual::setVisualisationLineSmooth);
    
    m_gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    m_gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    m_gui.add(visualisationParameters);
    
    leftButtonParameters.setName("mouse left button");
    for (int i=0; i<3; i++) {
        leftButtonParameters.add(m_flexDrawForces[i].parameters);
    }
    m_gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    m_gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    m_gui.add(leftButtonParameters);
    
    m_gui.loadFromFile(m_guiSettingsName);
    m_gui.minimizeAll();

}


void FluidVisual::update()
{
    this->updateFluid();
}

void FluidVisual::updateFluid()
{
    double dt = ofGetLastFrameTime();
    
    m_fluid.addVelocity(m_opticalFlow.getOpticalFlowDecay());
    m_fluid.addDensity(m_velocityMask.getColorMask());
    m_fluid.addTemperature(m_velocityMask.getLuminanceMask());
    
    for (int i=0; i<m_numDrawForces; i++) {
        m_flexDrawForces[i].update();
        if (m_flexDrawForces[i].didChange()) {
            // if a force is constant multiply by deltaTime
            float strength = m_flexDrawForces[i].getStrength();
            if (!m_flexDrawForces[i].getIsTemporary())
                strength *=dt;
            switch (m_flexDrawForces[i].getType()) {
                case FT_DENSITY:
                    m_fluid.addDensity(m_flexDrawForces[i].getTextureReference(), strength);
                    break;
                case FT_VELOCITY:
                    m_fluid.addVelocity(m_flexDrawForces[i].getTextureReference(), strength);
                    break;
                case FT_TEMPERATURE:
                    m_fluid.addTemperature(m_flexDrawForces[i].getTextureReference(), strength);
                    break;
                case FT_PRESSURE:
                    m_fluid.addPressure(m_flexDrawForces[i].getTextureReference(), strength);
                    break;
                case FT_OBSTACLE:
                    m_fluid.addTempObstacle(m_flexDrawForces[i].getTextureReference());
                default:
                    break;
            }
        }
    }
    
    m_fluid.update();
}

void FluidVisual::draw(const ofRectangle& area)
{
    m_fluid.draw(area.x, area.y, area.width, area.height);
}

/*void FluidVisual::draw()
{
    m_fluid.draw(0, 0, ofGetWidth(), ofGetHeight());
}*/

void FluidVisual::drawGui()
{
    m_gui.draw();
}

void FluidVisual::addForce(ofVec2f position) {
    
    ofVec2f velocity = position - m_lastPosition;
    
    for (int i=0; i<m_numDrawForces; i++) {
        if (m_flexDrawForces[i].getType() == FT_VELOCITY)
            m_flexDrawForces[i].setForce(velocity);
        m_flexDrawForces[i].applyForce(position);
    }
    
    m_lastPosition.set(position.x, position.y);
}


void FluidVisual::setSource(const ofFbo& source)
{
    double dt = ofGetLastFrameTime();
    
    m_source =  source;
    
    m_opticalFlow.setSource(m_source.getTextureReference());
    m_opticalFlow.update(dt);
    
    m_velocityMask.setDensity(m_source.getTextureReference());
    m_velocityMask.setVelocity(m_opticalFlow.getOpticalFlow());
    m_velocityMask.update();
}



void FluidVisual::reset()
{

    for (int i=0; i<m_numDrawForces; i++) {
        m_flexDrawForces[i].reset();
    }
    
    m_fluid.reset();

}

