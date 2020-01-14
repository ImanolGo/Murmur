/*
 *  WindowSettingsManager.h
 *  Murmur
 *  
 *  Created by Imanol Gomez on 23/07/18.
 *
 */

#pragma once

#include "ofMain.h"

#include "WindowSettingsManager.h"

//========================== class WindowSettingsManager ==============================
//============================================================================
/** \class WindowSettingsManager WindowSettingsManager.h
 *	\brief Class managing the window settings
 *	\details it setups and provides access the window settings
 */


class WindowSettingsManager
{
    
    static const int MAX_NUM_WINDOWS; ///Defines the maximum number of windows available
    
    
public:
    
    typedef     shared_ptr<ofGLFWWindowSettings>  WindowSettingsPtr;     ///< Pointer ofGLFWWindowSettings
    typedef     vector<ofGLFWWindowSettings>    WindowSettingsVector;   ///< Vector of WindowSettingsPtr
    

    //! Destructor
    ~WindowSettingsManager();

    //! Returns the singleton instance.
    static WindowSettingsManager& getInstance();
    
    const WindowSettingsVector&  getWindowsSettings(){return m_windows;}

	const ofGLFWWindowSettings&  getMainWindowSettings() { return m_mainWindow; }
    
    ofGLFWWindowSettings getWindowsSettings(int index);

private:
    

     //! Constructor
     WindowSettingsManager();

	//! Stop the compiler generating methods of copy the object
	 WindowSettingsManager(WindowSettingsManager const& copy);              // Don't Implement

    //! Prevent operator= being generated.
     WindowSettingsManager& operator=(WindowSettingsManager const& copy);     // Don't implement

     //! Setup
     void setup();

    //==========================================================================
    
    //! Read all the window settings
    void readSettings();

	void setupMainWindow();


private:

    
    WindowSettingsVector			m_windows;				///< Vector holding all the window settings
	ofGLFWWindowSettings			m_mainWindow;			///< Main Window Setting

};

//==========================================================================
