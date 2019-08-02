/*
 *  SceneManager.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 25/06/15.
 *
 */

#pragma once

#include "Manager.h"
#include "Scene.h"
#include "SettingsManager.h"
#include "TextVisual.h"
#include "RectangleVisual.h"
#include "ofxSceneManager.h"

//========================== class SceneManager ==============================
//============================================================================
/** \class SceneManager SceneManager.h
 *	\brief Class managing the scenes of the application
 *	\details It creates and manages all the scenes 
 */

enum wIndex { FRONT = 1, TOP = 2};

class SceneManager: public Manager
{
public:

    //! Constructor
    SceneManager();

    //! Destructor
    ~SceneManager();

    //! Set-up the scenes
    void setup();
    
    //! Update the scenes
    void update();
    
    //! Get the windows settings of a specific scene
    ofGLFWWindowSettings getWindowSettings(ofxScene* scene);
    
    //! Draw a specific Scene Manager
    void draw(WindowIndex w);
    
    //! Changes the scene according to a name
    bool changeScene(string sceneName);
    
    //! Changes the scene according to a index
    void changeSceneIndex(int& sceneIndex);
    
    //! Returns the number of total secenes
    int getNumberScenes();
    
    //! Returns the name of a scene given the index
    string getSceneName(int sceneIndex);
    
    //! Runs the scene manager attached to a window
    bool run(WindowIndex w);
    
    //! Transition Time controlled by GUI
    void onTransitionTimeChange(float & value);
    
    //! Opacity value controlled by GUI
    void onOpacityChange(float & value);
    
    //! Opacity of a specific scene manager value controlled by GUI
    void onOpacityChange(float & value, WindowIndex w);
    
    //! Opacity of a FRONT scene manager value controlled by GUI
    void onFrontOpacityChange(float & value);
    
    //! Opacity of a TOP scene manager value controlled by GUI
    void onTopOpacityChange(float & value);


private:

    //! Create the scene managers
    void createSceneManagers();
    
    //! Create the scenes
    void createScenes();
    
    //! Create a scene
    void createScene(string name, SceneIndex i, ofPtr<ofxScene> ofx_scene, WindowIndex w);
    
    //! Add scene to a specific scene manager
    bool addSceneToSceneManager(ofPtr<Scene> scene, WindowIndex w);
    
    //! Create the front scenes
    void createFrontScenes();
    
    //! Create the top scenes
    void createTopScenes();
    
    //! setups the text layout
    void setupText();
    
    //! updates the text layout
    void updateText(const string& text);

private:

    typedef  map<WindowIndex, ofPtr < ofxSceneManager> >    SceneManagerMap;       ///< defines a map of scene manager attached to a window index
    typedef  vector< ofPtr <Scene > >               SceneVector;           ///< defines a vector of Scenes
    
    
    SceneManagerMap     m_sceneManagers;
    SceneVector         m_scenes;
    
    ofPtr<TextVisual>        m_sceneText;
    ofPtr<RectangleVisual>   m_sceneRect;
};

//==========================================================================


