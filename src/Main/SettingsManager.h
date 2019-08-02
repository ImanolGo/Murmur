/*
 *  h
 *  Murmur
 *
 *  Created by Imanol Gomez on 17/06/15.
 *
 */


#pragma once

#include "Manager.h"


//========================== class SettingsManager ==============================
//============================================================================
/** \class SettingsManager SettingsManager.h
 *	\brief Class managing the whole settings of the application
 *	\details it reads from an xml settings file and provides access to the information
 */



typedef  map<string,string>          ResourcesPathMap;       ///< defines a map of path attached to the resources name
typedef  vector<string>              IpsVector;     ///< Defines a vector of ip addresses



class SettingsManager: public Manager
{
    
    static const string APPLICATION_SETTINGS_FILE_NAME;
    static const string LOCALHOST;
    static const int MAX_NUM_WINDOWS;
    
    public:
    
        //! Destructor
        ~SettingsManager();
    
        //! Constructor
        SettingsManager();

        //! Compares two transition objects
        void setup();

        const ResourcesPathMap& getTextureResourcesPath() const {return m_texturesPath;}

        ofColor getColor(const string& colorName);
    
        const IpsVector& getProjectorsIps() const {return m_projectorsIps;}
        
        string getIpFloor() const {return m_ipFloor;}
        
        string getIpContour() const {return m_ipContour;}

        int getOscPortReceive() const {return m_portOscReceive;}
    
        int getOscPortFloor() const {return m_portOscFloor;}

        int getOscPortContour() const {return m_portOscContour;}

        int getUdpPortReceive() const {return m_portUdpReceive;}

        int getOscPortUnity() const {return m_portOscUnity;}


    private:

        //! Loads the settings file
        bool loadSettingsFile();

        //! Loads all the settings
        void loadAllSettings();

        //! Sets all the network properties
        void setNetworkProperties();

        //! Sets all the window properties
        void setWindowProperties();
    
        //! Loads all the app colors
        void loadColors();

        //! Loads all the textures settings
        void loadTextureSettings();
    
    private:
    
        typedef             map< string, ofColor>    ColorMap;               ///< Defines a map of colors attached to a name


        ofXml		            m_xml;          ///< instance of the xml parser
        ResourcesPathMap        m_texturesPath;         ///< stores the texture paths
        ColorMap                m_colors;               ///< stores all the application's colors
        int                     m_portUdpReceive;       ///< stores the UDP port to receive UDP messages from
        int                     m_portOscReceive;       ///< stores the UDP port to receive OSC messages from
        int                     m_portOscFloor;         ///< stores the UDP port to send OSC messages to Floor computer
        int                     m_portOscContour;       ///< stores the UDP port to send OSC messages to Contour computer
        int                     m_portOscUnity;         ///< stores the UDP port to send OSC messages to Unity 3D
        string                  m_ipFloor;              ///< stores the Ip Address of the floor tracking computer
        string                  m_ipContour;            ///< stores the Ip Address of the contour tracking computer
        IpsVector               m_projectorsIps;        ///< stores the Ip adresses of all the projectors
};



