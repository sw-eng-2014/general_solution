#include "OGRE.h"
 
/*The base application for Ogre
	Made by Dennis Mark
	University of Technology Wroclaw
*/

//-------------------------------------------------------------------------------------
//Constructor init all variables
OGREBase::OGREBase(void)
	 : mRoot(0),
    mPluginsCfg(Ogre::StringUtil::BLANK),
	mOgreCfg(Ogre::StringUtil::BLANK),
	mOgreLog(Ogre::StringUtil::BLANK),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mCloseApplication(false)
{
}
//-------------------------------------------------------------------------------------
//Destructor clear window and mRoot
OGREBase::~OGREBase(void)
{
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	if (mTrayMgr) delete mTrayMgr;
	if (mOverlaySystem) delete mOverlaySystem;
	delete mRoot;
}
 
//-------------------------------------------------------------------------------------
//Go start running the application
bool OGREBase::OGREgo(void)
{;
	return setup();
}

//-------------------------------------------------------------------------------------
//Adjust mouse clipping area when window resizes
void OGREBase::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
 
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
//------------------------------------------------------------------------------------- 
//Unattach OIS before window shutdown
void OGREBase::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the Base window in these demos)
    if(rw == mWindow)
    {
        if(mInputManager)
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );
 
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

//-------------------------------------------------------------------------------------
//Main function for the setup runs all different setup sequences
bool OGREBase::setup(void)
{
	createRoot();
	loadConfig();
	addOverlay();
	if (!generateRenderWindow()) return false;
	createSceneBase();
	createScene();
	createCamera();
	createViewports();
	startOIS();
	initSDKTray();
	finalTouch();

	return true;
}

//-------------------------------------------------------------------------------------
//Create the mRoot variable
void OGREBase::createRoot(void)
{
	mOgreCfg = "ogre.cfg";
	mOgreLog = "Ogre.log";
	#ifdef _DEBUG
		mPluginsCfg = "plugins_d.cfg";
	#else
		mPluginsCfg = "plugins.cfg";
#endif
	// construct Ogre::Root
    mRoot = new Ogre::Root(mPluginsCfg,mOgreCfg,mOgreLog);
}

//-------------------------------------------------------------------------------------
//Load the config file and all the resources
void OGREBase::loadConfig(void)
{
	#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	#else
	mResourcesCfg = "resources.cfg";
	#endif 
	//Load configFile
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);
	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}
//-------------------------------------------------------------------------------------
//Create the overlay for the SDK tray manager
void OGREBase::addOverlay(void)
{
	// Create the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");
	// Initialize the OverlaySystem 
    mOverlaySystem = new Ogre::OverlaySystem();
    mSceneMgr->addRenderQueueListener(mOverlaySystem);
}


//-------------------------------------------------------------------------------------
bool OGREBase::generateRenderWindow(void)
{
		//Render window
		// Show the configuration dialog and initialise the system
		if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
		{
			return false;
		}

		//Create a render window
		mWindow = mRoot->initialise(true, "Base application render window");

		// Set default mipmap level (note: some APIs ignore this)
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
		// initialise all resource groups
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		return true;
}

//-------------------------------------------------------------------------------------
//Create the scene manager and the default scene 
void OGREBase::createSceneBase(void)
{
		//Set settings
		mSceneMgr->setAmbientLight(Ogre::ColourValue(0.8,0.8,0.8));
		mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
  
		//Create floor 
		Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
		Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 1500, 1500, 100, 100, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
		Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
		entGround->setMaterialName("Examples/Rockwall");
		entGround->setCastShadows(false);
		

		
		// Create a light
		Ogre::Light* light = mSceneMgr->createLight("BaseLight");
		light ->setType(Ogre::Light::LT_POINT);
		light->setPosition(100,100,100);

}

//-------------------------------------------------------------------------------------
//Create the scene manager and the default scene 
void OGREBase::createScene(void)
{
	//Empty function for the creation of the base scene
}
//-------------------------------------------------------------------------------------
//Create the camera
void OGREBase::createCamera(void)
{
	/**
		//Old camera
		// Create the camera
		mCamera = mSceneMgr->createCamera("PlayerCam");
 
		// Position it at 80 in Z direction
		mCamera->setPosition(Ogre::Vector3(0,500,500));
		// Look back along -Z
		mCamera->lookAt(Ogre::Vector3(0,0,0));
		mCamera->setNearClipDistance(5);*/

		
		// Create the camera
		mCamera = mSceneMgr->createCamera("PlayerCam");
 
		// Position it at 80 in Z direction
		mCamera->setPosition(Ogre::Vector3(0,0,0));
		// Look back along -Z
		//mCamera->lookAt(Ogre::Vector3(0,0,0));
		mCamera->setNearClipDistance(5);
		// Create the camera's top node (which will only handle position).
		cameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("cameraNode");
		cameraNode->setPosition(0, 700, 700);

		cameraNode->pitch(Ogre::Degree(-45));
		// Create the camera's yaw node as a child of camera's top node.
		cameraInsertion = cameraNode->createChildSceneNode("cameraInsertion");
		cameraInsertion->attachObject(mCamera);
		

}

//-------------------------------------------------------------------------------------
// Create the viewports
void OGREBase::createViewports(void)
{
		// Create one viewport, entire window
		Ogre::Viewport* vp = mWindow->addViewport(mCamera);	
		vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
 
		// Alter the camera aspect ratio to match the viewport
		mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

//-------------------------------------------------------------------------------------
// Start the OIS for the keyboard input
void OGREBase::startOIS(void)
{
		Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
		OIS::ParamList pl;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;
 
		mWindow->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
		mInputManager = OIS::InputManager::createInputSystem( pl );
		
		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
		mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
		mMouse->setEventCallback(this);
		mKeyboard->setEventCallback(this);
		//
		//Set initial mouse clipping size
		windowResized(mWindow);
 
		//Register as a Window listener
		Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

}

//-------------------------------------------------------------------------------------
// Create the SDKtray manager
void OGREBase::initSDKTray(void)
{
	mInputContext.mKeyboard = mKeyboard;
    mInputContext.mMouse = mMouse;
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
    mTrayMgr->hideCursor();

    // Create a params panel for displaying sample details
    Ogre::StringVector items;
	items.push_back("Item");
    items.push_back("X");
    items.push_back("Y");
    items.push_back("Z");
    items.push_back("");
    items.push_back("Pitch symball");
	items.push_back("Pitch node");
	items.push_back("");
    items.push_back("Yaw symball");
	items.push_back("Yaw node");
	items.push_back("");
    items.push_back("Roll symball");
	items.push_back("Roll node");//12
    items.push_back("");
	items.push_back("Insertion symball");
    items.push_back("Insertion node");

	//Create details panel
    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
	mSelectedElement = 1;
    mDetailsPanel->hide();

}

//-------------------------------------------------------------------------------------
//Start the rendering handle the final items
void OGREBase::finalTouch(void)
{
		mRoot->addFrameListener(this);
		mRoot->startRendering();
}

//-------------------------------------------------------------------------------------
//Frame rendering, this function occures before the frame rendering is done
bool OGREBase::frameRenderingQueued(const Ogre::FrameEvent& evt)
	{
		if(mWindow->isClosed())
			return false;
		if(mCloseApplication)
			return false;

		//Need to capture/update each device
		mKeyboard->capture();
		mMouse->capture();


		return true;
	}

//-------------------------------------------------------------------------------------
//Key pressed event
bool OGREBase::keyPressed(const OIS::KeyEvent &arg){
	
	if (arg.key == OIS::KC_ESCAPE)  //Exit program
    {
        mCloseApplication =true;
    }
	else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
	else if (arg.key == OIS::KC_4)   // toggle visibility of even rarer debugging details
    {
		mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	}
	else if (arg.key == OIS::KC_5)   // toggle visibility of even rarer debugging details
    {
		mTrayMgr->hideFrameStats();
	}
	else if (arg.key == OIS::KC_6)   // toggle visibility of even rarer debugging details
    {
		mSelectedElement = 1;
		if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
			{
			mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
			mDetailsPanel->show();
		}
	}
	else if (arg.key == OIS::KC_7)   // toggle visibility of even rarer debugging details
    {
		mSelectedElement = 2;
		if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
			{
			mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
			mDetailsPanel->show();
		}
	}
	else if (arg.key == OIS::KC_8)   // toggle visibility of even rarer debugging details
    {
		mSelectedElement = 3;
		if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
			{
			mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
			mDetailsPanel->show();
		}
	}
	else if (arg.key == OIS::KC_9)   // toggle visibility of even rarer debugging details
    {
		if (mDetailsPanel->getTrayLocation() != OgreBites::TL_NONE)
			{
			mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
		}
	}
	return true;}

//-------------------------------------------------------------------------------------
//Key released event
bool OGREBase::keyReleased(const OIS::KeyEvent &arg){return true;}

//-------------------------------------------------------------------------------------
//Mouse moved event
bool OGREBase::mouseMoved(const OIS::MouseEvent &arg){return true;}

//-------------------------------------------------------------------------------------
// Mouse pressed event
bool OGREBase::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){return true;}

//-------------------------------------------------------------------------------------
// Mouse released event
bool OGREBase::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){return true;}
