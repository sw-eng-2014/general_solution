#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreException.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>

#include <OgreMeshManager.h>

#  include <SdkTrays.h>


class OGREBase : public Ogre::WindowEventListener, public Ogre::FrameListener,  public OIS::KeyListener, public OIS::MouseListener, public OgreBites::SdkTrayListener
{
public:
    OGREBase(void);
    virtual ~OGREBase(void);
    bool OGREgo(void);

	// Private functions 
private:
	virtual bool setup(void);
	virtual void createRoot(void);
	virtual void loadConfig(void);
	virtual void addOverlay(void);
	virtual bool generateRenderWindow(void);
	virtual void createSceneBase(void);
	virtual void createScene(void);
	virtual void createCamera(void);
	virtual void createViewports(void);
	virtual void startOIS(void);
	virtual void initSDKTray(void);
	virtual void finalTouch(void);

protected: 
	// Ogre::WindowEventListener
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	// Mouse events
	virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	//Ogre
	Ogre::Root* mRoot;
    Ogre::String mPluginsCfg;
	Ogre::String mOgreCfg;
	Ogre::String mOgreLog;
	Ogre::String mResourcesCfg;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::SceneNode *cameraNode;
	Ogre::SceneNode *cameraInsertion;
	Ogre::Camera* mCamera;
	bool mCloseApplication; 

	// OgreBites
    OgreBites::InputContext     mInputContext;
    OgreBites::SdkTrayManager*	mTrayMgr;
	Ogre::OverlaySystem*        mOverlaySystem;
    OgreBites::ParamsPanel*     mDetailsPanel;  
	OgreBites::ParamsPanel*     mDetailsPanel2;   	
	int							mSelectedElement;
	//OIS
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
};