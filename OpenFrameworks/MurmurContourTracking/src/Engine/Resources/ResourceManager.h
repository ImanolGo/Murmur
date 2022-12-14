/*
 *  ResourceManager.h

 *
 *  Created by Imanol Gomez on 16/02/15.
 *
 */

#pragma once

#include "ofMain.h"
#include "Manager.h"


//==============================================================================
/** \class ResourceManager ResourceManager.h
*	\brief Manages ofTexture.
*	\details Manages loading and unloading of all resources.
*/
//==============================================================================

class ResourceManager: public Manager
{
    static const int DEFAULT_IMAGE_SIZE;    ///! Defines the default image size to be allocated

	public:

		//! Constructor.
        ResourceManager();
		//! Destructor.
        ~ResourceManager();

		//! Initializes the resource manager.
		void setup();

        //! Searches for the specified texture name and returns the first found.
        ofPtr<ofTexture> getTexture(const string& name) const;
    
        //! Add new texture externally
        bool addTexture(string name, string path);
    
        //! Load a certain texture
        void loadTexture(const string& name, const string& path);

	private:

		//! Creates the default resource
		void createDefaultResource();

		//! Tells if the textures is already in the container
		bool containsTexture(const string& name) const;

		//! Loads all the resources.
		void loadResources();

		//! Loads all the textures.
		void loadTextures();
    
    
	private:

		//======================= Typedefs =====================================

		typedef std::map< string, ofPtr<ofTexture> >	TextureMap;			///< holds a list of textures sorted by name

		//======================================================================

		TextureMap		    m_textures;        ///< map of textures handles attached to a name
		ofPtr<ofTexture>    m_defaultTexture;  ///< stores a default texture in case another resource cannot be loaded

};



