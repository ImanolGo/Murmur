#include "ofMain.h"
#include "WindowSettingsManager.h"
#include "MurmurRendererApp.h"


//========================================================================
int main( ){
    
	auto settingsGL = WindowSettingsManager::getInstance().getMainWindowSettings();
	//WindowSettingsManager::WindowSettingsVector windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();

	ofCreateWindow(settingsGL);

	//ofSetupOpenGL(1024, 768, OF_WINDOW);
	ofRunApp(new MurmurRendererApp());

 //   WindowSettingsManager::WindowSettingsVector windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();
 //   
 //   
 //   shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(windowSettings[0]);
	//shared_ptr<MurmurRendererApp> mainApp(new MurmurRendererApp);

	//windowSettings[1].shareContextWith = mainWindow;
	//shared_ptr<ofAppBaseWindow> secondWindow = ofCreateWindow(windowSettings[1]);

	//ofAddListener(secondWindow->events().draw, mainApp.get(), &MurmurRendererApp::drawScreen2);
 //   
	///*
 // 
 //   
 //   windowSettings[2].shareContextWith = mainWindow;
 //   shared_ptr<ofAppBaseWindow> thirdWindow = ofCreateWindow(windowSettings[2]);
 //   
 //   //secondWindow->setVerticalSync(false);
 //   
 //   
 //  // ofAddListener(secondWindow->events().keyPressed,mainApp.get(),&MurmurRendererApp::keyPressed2);
 //   
 //   ofAddListener(thirdWindow->events().draw,mainApp.get(),&MurmurRendererApp::drawScreen3);
 //   //ofAddListener(secondWindow->events().keyPressed,mainApp.get(),&MurmurRendererApp::keyPressed3);
 //   */
 //   ofRunApp(mainWindow, mainApp);
 //   //mainWindow->setWindowShape(windowSettings[0].getWidth(), windowSettings[0].getHeight());
 //   ofRunMainLoop();
    
}
