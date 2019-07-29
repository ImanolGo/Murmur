#include "ofMain.h"
#include "WindowSettingsManager.h"
#include "MurmurRendererApp.h"


//========================================================================
int main( ){
    
    
    WindowSettingsManager::WindowSettingsVector windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();
    
    
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(windowSettings[0]);
    
    windowSettings[1].shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> secondWindow = ofCreateWindow(windowSettings[1]);
    
    windowSettings[2].shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> thirdWindow = ofCreateWindow(windowSettings[2]);
    
    //secondWindow->setVerticalSync(false);
    
    shared_ptr<MurmurRendererApp> mainApp(new MurmurRendererApp);
    ofAddListener(secondWindow->events().draw,mainApp.get(),&MurmurRendererApp::drawScreen2);
   // ofAddListener(secondWindow->events().keyPressed,mainApp.get(),&MurmurRendererApp::keyPressed2);
    
    ofAddListener(secondWindow->events().draw,mainApp.get(),&MurmurRendererApp::drawScreen3);
    //ofAddListener(secondWindow->events().keyPressed,mainApp.get(),&MurmurRendererApp::keyPressed3);
    
    ofRunApp(mainWindow, mainApp);
    mainWindow->setWindowShape(windowSettings[0].getWidth(), windowSettings[0].getHeight());
    ofRunMainLoop();
    
}
