/*
 *  ofxFluid.cpp
 *
 *  Created by Patricio González Vivo on 9/29/11.
 *  Copyright 2011 http://PatricioGonzalezVivo.com All rights reserved.
 *
 *  Created ussing:
 *
 *    - Mark Harris article from GPU Gems 1
 *      http://http.developer.nvidia.com/GPUGems/gpugems_ch38.html
 *
 *    - Phil Rideout
 *      http://prideout.net/blog/?p=58
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 *  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  ************************************************************************************ */

#include "ofxFluid.h"

ofxFluid::ofxFluid(){
    passes = 1;
    internalFormat = GL_RGB;
    
    // ADVECT
    fragmentShader = STRINGIFY(
                               uniform sampler2DRect tex0;         // Real obstacles
                               uniform sampler2DRect backbuffer;
                               uniform sampler2DRect VelocityTexture;
                               
                               uniform float TimeStep;
                               uniform float Dissipation;
                               
                               void main(){
                                   vec2 st = gl_TexCoord[0].st;
                                   
                                   float solid = texture2DRect(tex0, st).r;
                                   
                                   if (solid > 0.1) {
                                       gl_FragColor = vec4(0.0,0.0,0.0,0.0);
                                       return;
                                   }
                                   
                                   vec2 u = texture2DRect(VelocityTexture, st).rg;
                                   vec2 coord =  st - TimeStep * u;
                                   
                                   gl_FragColor = Dissipation * texture2DRect(backbuffer, coord);
                               }
                               );
    
    
    // JACOBI
    string fragmentJacobiShader = STRINGIFY(
                                            uniform sampler2DRect Pressure;
                                            uniform sampler2DRect Divergence;
                                            uniform sampler2DRect tex0;
                                            
                                            uniform float Alpha;
                                            uniform float InverseBeta;
                                            
                                            void main() {
                                                vec2 st = gl_TexCoord[0].st;
                                                
                                                vec4 pN = texture2DRect(Pressure, st + vec2(0.0, 1.0));
                                                vec4 pS = texture2DRect(Pressure, st + vec2(0.0, -1.0));
                                                vec4 pE = texture2DRect(Pressure, st + vec2(1.0, 0.0));
                                                vec4 pW = texture2DRect(Pressure, st + vec2(-1.0, 0.0));
                                                vec4 pC = texture2DRect(Pressure, st);
                                                
                                                vec3 oN = texture2DRect(tex0, st + vec2(0.0, 1.0)).rgb;
                                                vec3 oS = texture2DRect(tex0, st + vec2(0.0, -1.0)).rgb;
                                                vec3 oE = texture2DRect(tex0, st + vec2(1.0, 0.0)).rgb;
                                                vec3 oW = texture2DRect(tex0, st + vec2(-1.0, 0.0)).rgb;
                                                
                                                if (oN.x > 0.1) pN = pC;
                                                if (oS.x > 0.1) pS = pC;
                                                if (oE.x > 0.1) pE = pC;
                                                if (oW.x > 0.1) pW = pC;
                                                
                                                vec4 bC = texture2DRect(Divergence, st );
                                                gl_FragColor = (pW + pE + pS + pN + Alpha * bC) * InverseBeta;
                                            }
                                            );
    
    jacobiShader.unload();
    jacobiShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentJacobiShader);
    jacobiShader.linkProgram();
    
    
    //SUBSTRACT GRADIENT
    string fragmentSubtractGradientShader = STRINGIFY(
                                                      uniform sampler2DRect Velocity;
                                                      uniform sampler2DRect Pressure;
                                                      uniform sampler2DRect tex0;
                                                      
                                                      uniform float GradientScale;
                                                      
                                                      void main(){
                                                          vec2 st = gl_TexCoord[0].st;
                                                          
                                                          vec3 oC = texture2DRect(tex0, st ).rgb;
                                                          if (oC.x > 0.1) {
                                                              gl_FragColor.gb = oC.yz;
                                                              return;
                                                          }
                                                          
                                                          float pN = texture2DRect(Pressure, st + vec2(0.0, 1.0)).r;
                                                          float pS = texture2DRect(Pressure, st + vec2(0.0, -1.0)).r;
                                                          float pE = texture2DRect(Pressure, st + vec2(1.0, 0.0)).r;
                                                          float pW = texture2DRect(Pressure, st + vec2(-1.0, 0.0)).r;
                                                          float pC = texture2DRect(Pressure, st).r;
                                                          
                                                          vec3 oN = texture2DRect(tex0, st + vec2(0.0, 1.0)).rgb;
                                                          vec3 oS = texture2DRect(tex0, st + vec2(0.0, -1.0)).rgb;
                                                          vec3 oE = texture2DRect(tex0, st + vec2(1.0, 0.0)).rgb;
                                                          vec3 oW = texture2DRect(tex0, st + vec2(-1.0, 0.0)).rgb;
                                                          
                                                          vec2 obstV = vec2(0.0,0.0);
                                                          vec2 vMask = vec2(1.0,1.0);
                                                          
                                                          if (oN.x > 0.1) { pN = pC; obstV.y = oN.z; vMask.y = 0.0; }\
                                                          if (oS.x > 0.1) { pS = pC; obstV.y = oS.z; vMask.y = 0.0; }\
                                                          if (oE.x > 0.1) { pE = pC; obstV.x = oE.y; vMask.x = 0.0; }\
                                                          if (oW.x > 0.1) { pW = pC; obstV.x = oW.y; vMask.x = 0.0; }\
                                                          
                                                          vec2 oldV = texture2DRect(Velocity, st).rg;
                                                          vec2 grad = vec2(pE - pW, pN - pS) * GradientScale;
                                                          vec2 newV = oldV - grad;
                                                          
                                                          gl_FragColor.rg = (vMask * newV) + obstV;
                                                          
                                                      }
                                                      );
    subtractGradientShader.unload();
    subtractGradientShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentSubtractGradientShader);
    subtractGradientShader.linkProgram();
    
    
    // COMPUTE DIVERGENCE
    string fragmentComputeDivergenceShader = STRINGIFY(
                                                       uniform sampler2DRect Velocity;
                                                       uniform sampler2DRect tex0;
                                                       
                                                       uniform float HalfInverseCellSize;
                                                       
                                                       void main(){
                                                           vec2 st = gl_TexCoord[0].st;
                                                           
                                                           vec2 vN = texture2DRect(Velocity, st + vec2(0.0,1.0)).rg;
                                                           vec2 vS = texture2DRect(Velocity, st + vec2(0.0,-1.0)).rg;
                                                           vec2 vE = texture2DRect(Velocity, st + vec2(1.0,0.0)).rg;
                                                           vec2 vW = texture2DRect(Velocity, st + vec2(-1.0,0.0)).rg;
                                                           
                                                           vec3 oN = texture2DRect(tex0, st + vec2(0.0,1.0)).rgb;
                                                           vec3 oS = texture2DRect(tex0, st + vec2(0.0,-1.0)).rgb;
                                                           vec3 oE = texture2DRect(tex0, st + vec2(1.0,0.0)).rgb;
                                                           vec3 oW = texture2DRect(tex0, st + vec2(-1.0,0.0)).rgb;
                                                           
                                                           if (oN.x > 0.1) vN = oN.yz;
                                                           if (oS.x > 0.1) vS = oS.yz;
                                                           if (oE.x > 0.1) vE = oE.yz;
                                                           if (oW.x > 0.1) vW = oW.yz;
                                                           
                                                           gl_FragColor.r = HalfInverseCellSize * (vE.x - vW.x + vN.y - vS.y);
                                                       }
                                                       );
    computeDivergenceShader.unload();
    computeDivergenceShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentComputeDivergenceShader);
    computeDivergenceShader.linkProgram();
    
    
    // APPLY IMPULSE
    string fragmentApplyImpulseShader = STRINGIFY(
                                                  uniform vec2    Point;
                                                  uniform float   Radius;
                                                  uniform vec3    Value;
                                                  
                                                  void main(){
                                                      float d = distance(Point, gl_TexCoord[0].st);
                                                      if (d < Radius) {
                                                          float a = (Radius - d) * 0.5;
                                                          a = min(a, 1.0);
                                                          gl_FragColor = vec4(Value, a);
                                                      } else {
                                                          gl_FragColor = vec4(0);
                                                      }
                                                  } 
                                                  );
    applyImpulseShader.unload();
    applyImpulseShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentApplyImpulseShader);
    applyImpulseShader.linkProgram();
    
    
    // APPLY EXT. VELOCITY
    string fragmentApplyExtVelocity = STRINGIFY(
                                                uniform sampler2DRect    buffer;
                                                uniform sampler2DRect    ExternalVelocity;
                                                uniform float            Strength;
                                                
                                                float kernel[9];
                                                vec2 offset[9];
                                                float fade = 0.0003;
                                                  
                                                  void main(){
                                                      
                                                      //Blur input channels
                                                      vec2  st = gl_TexCoord[0].st;
                                                      vec4 sum = vec4(0.0);
                                                      
                                                      offset[0] = vec2(-1.0, -1.0);
                                                      offset[1] = vec2(0.0, -1.0);
                                                      offset[2] = vec2(1.0, -1.0);
                                                      
                                                      offset[3] = vec2(-1.0, 0.0);
                                                      offset[4] = vec2(0.0, 0.0);
                                                      offset[5] = vec2(1.0, 0.0);
                                                      
                                                      offset[6] = vec2(-1.0, 1.0);
                                                      offset[7] = vec2(0.0, 1.0);
                                                      offset[8] = vec2(1.0, 1.0);
                                                      
                                                      kernel[0] = 1.0/16.0;   kernel[1] = 2.0/16.0;   kernel[2] = 1.0/16.0;
                                                      kernel[3] = 2.0/16.0;   kernel[4] = 4.0/16.0;   kernel[5] = 2.0/16.0;
                                                      kernel[6] = 1.0/16.0;   kernel[7] = 2.0/16.0;   kernel[8] = 1.0/16.0;
                                                      
                                                      int i = 0;
                                                      for (i = 0; i < 9; i++){
                                                          vec4 tmp = texture2DRect(ExternalVelocity, st + offset[i]) * Strength;
                                                          sum += tmp * kernel[i];
                                                      }
                                                      
                                                      vec4 previousValue = texture2DRect(ExternalVelocity, st);
                                                      vec4 blurColour = (1.0 - fade) * previousValue +  fade * vec4(sum.rgb, previousValue.a);
                
                                                      gl_FragColor = blurColour + texture2DRect(buffer, gl_TexCoord[0].st);
                                                  }
                                                  );
    applyExternalVelocityShader.unload();
    applyExternalVelocityShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentApplyExtVelocity);
    applyExternalVelocityShader.linkProgram();
    
    
    // APPLY EXT. DEPTH
    string fragmentApplyDepth = STRINGIFY(
                                                uniform sampler2DRect    buffer;
                                                uniform sampler2DRect    ExternalDepth;
                                                uniform float            multiplier;
                                                
                                                void main(){
                                                    vec2 st = gl_TexCoord[0].st;
                                                    
                                                    vec4 depthColour = vec4(texture2DRect(ExternalDepth, st).b * multiplier, texture2DRect(ExternalDepth, st).b * multiplier, texture2DRect(ExternalDepth, st).b * multiplier, 0.1);
                                                    
                                                    gl_FragColor = depthColour;
                                                }
                                                );
    applyExternalDyeShader.unload();
    applyExternalDyeShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentApplyDepth);
    applyExternalDyeShader.linkProgram();

    
    
    
    
    

    
    //APPLY BUOYANCY
    string fragmentApplyBuoyancyShader = STRINGIFY(
                                                   uniform sampler2DRect Velocity;
                                                   uniform sampler2DRect Temperature;
                                                   uniform sampler2DRect Density;
                                                   
                                                   uniform float AmbientTemperature;
                                                   uniform float TimeStep;
                                                   uniform float Sigma;
                                                   uniform float Kappa;
                                                   
                                                   uniform vec2  Gravity;
                                                   
                                                   void main(){
                                                       vec2 st = gl_TexCoord[0].st;
                                                       
                                                       float T = texture2DRect(Temperature, st).r;
                                                       vec2 V = texture2DRect(Velocity, st).rg;
                                                       
                                                       gl_FragColor.rg = V;
                                                       
                                                       if (T > AmbientTemperature) {
                                                           float D = texture2DRect(Density, st).r;
                                                           gl_FragColor.rg += (TimeStep * (T - AmbientTemperature) * Sigma - D * Kappa ) * Gravity;
                                                       }
                                                   }
                                                   );
    applyBuoyancyShader.unload();
    applyBuoyancyShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentApplyBuoyancyShader);
    applyBuoyancyShader.linkProgram();
    
    cellSize            = 1.25f;
    gradientScale       = 1.00f / cellSize;
    ambientTemperature  = 0.0f;
    numJacobiIterations = 40;
    timeStep            = 0.125f;
    smokeBuoyancy       = 1.0f;
    smokeWeight         = 0.05f;
    
    bDrawVelocity = false;
    bDrawPressure = true;
    bDrawTemperature = false;
    
    //gForce.set(0,-0.98);
}

void ofxFluid::allocate(int _width, int _height, float _scale){
    width = _width;
    height = _height;
    scale = _scale;
    
    gridWidth = width * scale;
    gridHeight = height * scale;
    
    pingPong.allocate(gridWidth,gridHeight,GL_RGB32F,0.999f); // Density buffer
    velocityBuffer.allocate(gridWidth,gridHeight,GL_RGB32F,0.9f);
    externalVelocityBuffer.allocate(gridWidth,gridHeight,GL_RGB32F,0.9f);
    temperatureBuffer.allocate(gridWidth,gridHeight,GL_RGB32F,0.99f);
    pressureBuffer.allocate(gridWidth,gridHeight,GL_RGB32F,0.9f);
    
    initFbo(obstaclesFbo, gridWidth, gridHeight, GL_RGB);
    initFbo(divergenceFbo, gridWidth, gridHeight, GL_RGB16F);
    
    compileCode();
    
    temperatureBuffer.src->begin();
    ofClear( ambientTemperature );
    temperatureBuffer.src->end();
}

void ofxFluid::addTemporalForce(ofPoint _pos, ofPoint _vel, ofFloatColor _col, float _rad, float _temp, float _den){
    punctualForce f;
    
    f.pos = _pos * scale;
    f.vel = _vel;
    f.color.set(_col.r,_col.g,_col.b);
    f.rad = _rad;
    f.temp = _temp;
    f.den = _den;
    
    temporalForces.push_back(f);
}

void ofxFluid::addConstantForce(ofPoint _pos, ofPoint _vel, ofFloatColor _col, float _rad, float _temp, float _den){
    punctualForce f;
    
    f.pos = _pos * scale;
    f.vel = _vel;
    f.color.set(_col.r,_col.g,_col.b);
    f.rad = _rad;
    f.temp = _temp;
    f.den = _den;
    
    constantForces.push_back(f);
}

void ofxFluid::update(){
    //Obstacles need it´s the half othe the side
    ofPushStyle();
    obstaclesFbo.begin();
    ofSetColor(255, 255);
    textures[0].draw(0,0,gridWidth,gridHeight);
    obstaclesFbo.end();
    ofPopStyle();
    
    advect(velocityBuffer); 
    velocityBuffer.swap();
    
    advect(temperatureBuffer); 
    temperatureBuffer.swap();
    
    advect(pingPong); 
    pingPong.swap();
    
    applyBuoyancy();
    velocityBuffer.swap();
    
    
    //Apply external velocity from kinect
    //-----------------------------------
    //  applyExternalVelocity(temperatureBuffer, externalVelocityBuffer.src->getTextureReference());
    //  temperatureBuffer.swap();
    //
    //    applyExternalVelocity(pingPong, externalVelocityBuffer.src->getTextureReference());
    //    pingPong.swap();
    //
    applyExternalVelocity(velocityBuffer, externalVelocityBuffer.src->getTextureReference());
    velocityBuffer.swap();
    
    applyExternalDye(temperatureBuffer, externalVelocityBuffer.src->getTextureReference(), 20.0f);
    applyExternalDye(pingPong, externalVelocityBuffer.src->getTextureReference(), 1.0f);
    //-----------------------------------
    
    
    if ( temporalForces.size() != 0){
        for(int i = 0; i < temporalForces.size(); i++){
            applyImpulse(temperatureBuffer, temporalForces[i].pos, ofVec3f(temporalForces[i].temp,temporalForces[i].temp,temporalForces[i].temp), temporalForces[i].rad);
            if (temporalForces[i].color.length() != 0)
                applyImpulse(pingPong, temporalForces[i].pos, temporalForces[i].color * temporalForces[i].den, temporalForces[i].rad);
            if (temporalForces[i].vel.length() != 0)
                applyImpulse(velocityBuffer , temporalForces[i].pos, temporalForces[i].vel, temporalForces[i].rad);
        }
        temporalForces.clear();
    }
    
    if ( constantForces.size() != 0)
        for(int i = 0; i < constantForces.size(); i++){
            applyImpulse(temperatureBuffer, constantForces[i].pos, ofVec3f(constantForces[i].temp,constantForces[i].temp,constantForces[i].temp), constantForces[i].rad);
            if (constantForces[i].color.length() != 0)
                applyImpulse(pingPong, constantForces[i].pos, constantForces[i].color * constantForces[i].den, constantForces[i].rad);
            if (constantForces[i].vel.length() != 0)
                applyImpulse(velocityBuffer , constantForces[i].pos, constantForces[i].vel, constantForces[i].rad);
        }
    
    

    
    
    computeDivergence();
    pressureBuffer.src->begin();
    ofClear(0);
    pressureBuffer.src->end();
    
    for (int i = 0; i < numJacobiIterations; i++) {
        jacobi();
        pressureBuffer.swap();
    }
    
    subtractGradient();
    velocityBuffer.swap();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofDisableBlendMode();
}

void ofxFluid::draw(int x, int y, float _width, float _height){
    if (_width == -1) _width = width;
    if (_height == -1) _height = height;
    
    ofPushStyle();
    glEnable(GL_BLEND);
    ofSetColor(255);
    
    if(bDrawVelocity)
        velocityBuffer.src->draw(x,y,_width,_height);
    if(bDrawPressure)
        pingPong.src->draw(x,y,_width,_height);
    if(bDrawTemperature)
        temperatureBuffer.src->draw(x,y,_width,_height);


    
    //textures[0].draw(x,y,_width,_height);
    glDisable(GL_BLEND);
    ofPopStyle();
}

void ofxFluid::setTextureToBuffer(ofTexture & _tex, ofxSwapBuffer & _buffer){
    ofPushMatrix();
    ofScale(scale, scale);
    for(int i = 0; i < 2; i++){
        _buffer[i].begin();
        ofSetColor(255);
        _tex.draw(gridWidth*0.5-_tex.getWidth()*0.5 * scale,
                  gridHeight*0.5-_tex.getHeight()*0.5 * scale, 
                  _tex.getWidth()*scale,
                  _tex.getHeight()*scale);
        _buffer[i].end();
    }
    ofPopMatrix();
    
    update();
}

void ofxFluid::advect(ofxSwapBuffer& _buffer){
    _buffer.dst->begin();
    shader.begin();
    shader.setUniform1f("TimeStep", timeStep);
    shader.setUniform1f("Dissipation", _buffer.diss);
    shader.setUniformTexture("VelocityTexture", velocityBuffer.src->getTextureReference(), 0);
    shader.setUniformTexture("backbuffer", _buffer.src->getTextureReference(), 1);
    shader.setUniformTexture("tex0", obstaclesFbo.getTextureReference(), 2);
    
    renderFrame(gridWidth,gridHeight);
    
    shader.end();
    _buffer.dst->end();
}


void ofxFluid::jacobi(){
    pressureBuffer.dst->begin();
    jacobiShader.begin();
    jacobiShader.setUniform1f("Alpha", -cellSize * cellSize);
    jacobiShader.setUniform1f("InverseBeta", 0.25f);
    jacobiShader.setUniformTexture("Pressure", pressureBuffer.src->getTextureReference(), 0);
    jacobiShader.setUniformTexture("Divergence", divergenceFbo.getTextureReference(), 1);
    jacobiShader.setUniformTexture("tex0", obstaclesFbo.getTextureReference(), 2);
    
    renderFrame(gridWidth,gridHeight);
    
    jacobiShader.end();
    pressureBuffer.dst->end();
}

void ofxFluid::subtractGradient(){
    velocityBuffer.dst->begin();
    subtractGradientShader.begin();
    subtractGradientShader.setUniform1f("GradientScale", gradientScale);
    
    subtractGradientShader.setUniformTexture("Velocity", velocityBuffer.src->getTextureReference(), 0);
    subtractGradientShader.setUniformTexture("Pressure", pressureBuffer.src->getTextureReference(), 1);
    subtractGradientShader.setUniformTexture("tex0", obstaclesFbo.getTextureReference(), 2);
    
    renderFrame(gridWidth,gridHeight);
    
    subtractGradientShader.end();
    velocityBuffer.dst->end();
    
    ofDisableBlendMode();
}

void ofxFluid::computeDivergence(){
    divergenceFbo.begin();
    computeDivergenceShader.begin();
    computeDivergenceShader.setUniform1f("HalfInverseCellSize", 0.5f / cellSize);
    computeDivergenceShader.setUniformTexture("Velocity", velocityBuffer.src->getTextureReference(), 0);
    computeDivergenceShader.setUniformTexture("tex0", obstaclesFbo.getTextureReference(), 1);
    
    renderFrame(gridWidth,gridHeight);
    
    computeDivergenceShader.end();
    divergenceFbo.end();
}

void ofxFluid::applyImpulse(ofxSwapBuffer& _buffer, ofPoint _force, ofPoint _value, float _radio){
    glEnable(GL_BLEND);
    _buffer.src->begin();
    applyImpulseShader.begin();
    
    applyImpulseShader.setUniform2f("Point", (float)_force.x, (float)_force.y);
    applyImpulseShader.setUniform1f("Radius", (float) _radio );
    applyImpulseShader.setUniform3f("Value", (float)_value.x, (float)_value.y, (float)_value.z);
    
    renderFrame(gridWidth,gridHeight);
    
    applyImpulseShader.end();
    _buffer.src->end();
    glDisable(GL_BLEND);
}

void ofxFluid::applyExternalVelocity(ofxSwapBuffer& _buffer, ofTexture velocityTex){
    glEnable(GL_BLEND);
    _buffer.dst->begin();
    applyExternalVelocityShader.begin();
    applyExternalVelocityShader.setUniformTexture("buffer", _buffer.src->getTextureReference(), 0);
    applyExternalVelocityShader.setUniformTexture("ExternalVelocity", externalVelocityBuffer.src->getTextureReference(), 0);
    applyExternalVelocityShader.setUniform1f("Strength", 1.0f );

    renderFrame(gridWidth,gridHeight);
    applyExternalVelocityShader.end();
    _buffer.dst->end();
    glDisable(GL_BLEND);

}


void ofxFluid::applyExternalDye(ofxSwapBuffer& _buffer, ofTexture depthTex, float multiplier){
    glEnable(GL_BLEND);
    _buffer.src->begin();
    applyExternalDyeShader.begin();
    applyExternalDyeShader.setUniformTexture("buffer", _buffer.src->getTextureReference(), 0);
    applyExternalDyeShader.setUniformTexture("ExternalDepth", depthTex, 0);
    applyExternalDyeShader.setUniform1f("multiplier", multiplier);
    
    renderFrame(gridWidth,gridHeight);
    applyExternalDyeShader.end();
    _buffer.src->end();
    glDisable(GL_BLEND);
    
}

void ofxFluid::applyBuoyancy(){
    velocityBuffer.dst->begin();
    applyBuoyancyShader.begin();
    applyBuoyancyShader.setUniform1f("AmbientTemperature", ambientTemperature );
    applyBuoyancyShader.setUniform1f("TimeStep", timeStep );
    applyBuoyancyShader.setUniform1f("Sigma", smokeBuoyancy );
    applyBuoyancyShader.setUniform1f("Kappa", smokeWeight );
    
    applyBuoyancyShader.setUniform2f("Gravity", (float)gForce.x, (float)gForce.y );
    
    applyBuoyancyShader.setUniformTexture("Velocity", velocityBuffer.src->getTextureReference(), 0);
    applyBuoyancyShader.setUniformTexture("Temperature", temperatureBuffer.src->getTextureReference(), 1);
    applyBuoyancyShader.setUniformTexture("Density", pingPong.src->getTextureReference(), 2);
    
    renderFrame(gridWidth,gridHeight);
    
    applyBuoyancyShader.end();
    velocityBuffer.dst->end();
}