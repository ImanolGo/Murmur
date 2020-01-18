#include "ofMain.h"
#include "WindowSettingsManager.h"
#include "MurmurRendererApp.h"


//========================================================================
int main( ){
    
	auto settingsGL = WindowSettingsManager::getInstance().getMainWindowSettings();

	ofCreateWindow(settingsGL);
	ofRunApp(new MurmurRendererApp());

}


#ifndef TARGET_WIN32

int main() {

	auto settingsGL = WindowSettingsManager::getInstance().getMainWindowSettings();

	ofCreateWindow(settingsGL);
	ofRunApp(new MurmurRendererApp());
}

#else

#include "../../resource.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	auto settingsGL = WindowSettingsManager::getInstance().getMainWindowSettings();

	ofCreateWindow(settingsGL);


	HWND hwnd = ofGetWin32Window();
	HICON hMyIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hMyIcon);

	ofRunApp(new MurmurRendererApp());

}

#endif