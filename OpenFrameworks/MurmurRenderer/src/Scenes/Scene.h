/*
 *  Scene.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 25/06/15.
 *
 */

#pragma once

struct SceneIndex{
    explicit SceneIndex(int i)
    :val(i){}
    
    int val;
    
    //implicit conversion
    operator int() const { return val; }
};

struct WindowIndex{
    explicit WindowIndex(int i)
    :val(i){}
    
    int val;
    
    //implicit conversion
    operator int() const { return val; }
};

#include "ofxScene.h"

class Scene 
{
    
public:
    
    Scene(): index(0), window_index(0){}
    
public:

    ofPtr<ofxScene>     scene;
    string              name;
    SceneIndex          index;
    WindowIndex         window_index;

};

