/*
 *  WindowSettingsManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 23/07/18.
 *
 */

#include "ofAppGLFWWindow.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#ifdef TARGET_LINUX
#include "ofIcon.h"
#include "ofImage.h"
#define GLFW_EXPOSE_NATIVE_X11
#ifndef TARGET_OPENGLES
#define GLFW_EXPOSE_NATIVE_GLX
#else
#define GLFW_EXPOSE_NATIVE_EGL
#endif
#include <X11/extensions/Xrandr.h>
#include <X11/XKBlib.h>
#include "GLFW/glfw3native.h"
#include <X11/Xatom.h>
#include <xcb/xcb.h>
#include <xcb/xcbext.h>
#elif defined(TARGET_OSX)
#include <Cocoa/Cocoa.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#include "GLFW/glfw3native.h"
#elif defined(TARGET_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif


#include "WindowSettingsManager.h"

const int WindowSettingsManager::MAX_NUM_WINDOWS = 3; ///Defines the maximum number of windows available

WindowSettingsManager& WindowSettingsManager::getInstance()
{
	// The only instance
	// Guaranteed to be lazy initialized
	// Guaranteed that it will be destroyed correctly
	static WindowSettingsManager    m_instance;
	return m_instance;

}

WindowSettingsManager::WindowSettingsManager()
{
	this->setup();
}


WindowSettingsManager::~WindowSettingsManager()
{
	ofLogNotice() << "WindowSettingsManager::Destructor";
}


void WindowSettingsManager::setup()
{
	ofLogNotice() << "WindowSettingsManager::initialized";
	this->readSettings();
	this->setupMainWindow();
}

void WindowSettingsManager::readSettings()
{
	glfwInit();
	int numDisplays;
	GLFWmonitor** monitors = glfwGetMonitors(&numDisplays);
	cout << "WindowSettingsManager::readSettings-> Number of screens found: " << numDisplays << endl;


	for (int displayID = 0; displayID < numDisplays; displayID++)
	{
		//WindowSettingsPtr settings = WindowSettingsPtr (new ofGLFWWindowSettings());
		ofGLFWWindowSettings settings;

		int xM; int yM;
		glfwGetMonitorPos(monitors[displayID], &xM, &yM);
		const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[displayID]);

		
		settings.decorated = false;
		settings.windowMode = OF_FULLSCREEN;
		//settings.windowMode = OF_WINDOW;

		if (displayID == 0) {
			settings.windowMode = OF_WINDOW;
			settings.decorated = true;

			
		}
		

		settings.setSize(desktopMode->width, desktopMode->height);
		settings.setPosition(ofVec2f(xM, yM));

		//settings.setGLVersion(3,3);
		settings.setGLVersion(2, 1);

		
		//settings.windowMode = OF_GAME_MODE;
		settings.multiMonitorFullScreen = false;

		m_windows.push_back(settings);

	}

	sort(m_windows.begin(), m_windows.end(),
		[](const ofGLFWWindowSettings & a, const ofGLFWWindowSettings & b) -> bool
	{
		return a.getPosition().x < b.getPosition().x;
	});


	float x = m_windows.back().getPosition().x + m_windows.back().getWidth();
	

	for (int displayID = numDisplays; displayID < MAX_NUM_WINDOWS; displayID++)
	{
		auto windowSettings = m_windows.front();
		windowSettings.setPosition(glm::vec2(x, windowSettings.getPosition().y));
		m_windows.push_back(windowSettings);
		x += windowSettings.getWidth();
	}


	ofLogNotice() << "WindowSettingsManager::readSettings -> Displays detected: " << numDisplays;

	int i = 0;
	for (auto windowSettings : m_windows)
	{
		ofLogNotice() << "WindowSettingsManager::readSettings -> Window " << i << ": x = " << windowSettings.getPosition().x
			<< ", y = " << windowSettings.getPosition().y << ", width = " << windowSettings.getWidth() << ", height = " << windowSettings.getHeight();
		i++;
	}
}


void WindowSettingsManager::setupMainWindow()
{
	m_mainWindow.doubleBuffering = true;
	m_mainWindow.windowMode = OF_FULLSCREEN;
	m_mainWindow.title = "MurmurRenderer";
	//m_mainWindow.setGLVersion(3,3);
	m_mainWindow.setGLVersion(2, 1);
	m_mainWindow.decorated = false;
	m_mainWindow.multiMonitorFullScreen = true;

	float width = 0.0;
	float height = 0.0;

	for (auto windowSettings : m_windows)
	{
		width += windowSettings.getWidth();
		if (height < windowSettings.getHeight()) {
			height = windowSettings.getHeight();
		}
	}

	m_mainWindow.setSize(width, height);

	ofLogNotice() << "WindowSettingsManager::setupMainWindow -> Main Window " << ", width = " << width << ", height = " << height;



	m_workAreaWindow = m_windows[0];
	int y_offset = 100;
	auto pos = m_workAreaWindow.getPosition();
	pos.y -= y_offset;
	pos.x = 0;
	pos.y = 50;
	m_workAreaWindow.setPosition(pos);
	width = m_workAreaWindow.getWidth() - 2;
	height = m_workAreaWindow.getHeight() - y_offset;
	m_workAreaWindow.setSize(width, height);

}

ofGLFWWindowSettings WindowSettingsManager::getWindowsSettings(int index)
{
	ofGLFWWindowSettings settings;
	if (index < 0 || index >= m_windows.size())
	{
		return m_windows[0];
	}

	return m_windows[index];
}