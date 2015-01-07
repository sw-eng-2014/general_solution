#include "LapTrainer.h"
//-------------------------------------------------------------------------------------
LapTrainer::LapTrainer(void)
{ 
	srand (time(NULL));   
	//lab constructor
	num_res=simball.DevIdentificTech(7330,1);
	num_res=simball.ConnectToSimBall(0);
	if (num_res > 0)
		mNoSimballConnected = false;
	else 
		mNoSimballConnected = true;
}
//-------------------------------------------------------------------------------------
LapTrainer::~LapTrainer(void)
{
	num_res= simball.DisconnectFromSimBall(0);
	hid_free_enumeration(simball.device); //we free enumeration
	Sleep (50);
	hid_exit();
}
//--------------------------------------------------------------------------------------
void LapTrainer::createScene(void)
{
		//Create right stick
		Ogre::Entity* StickRight = mSceneMgr->createEntity("StickRight", "instrument_stick.mesh");
		StickRight -> setCastShadows(true);
		Ogre::SceneNode* RightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("RightNode", Ogre::Vector3(300, 100, 100));
		RightNode->scale( 1, 1, 1); 
		Ogre::SceneNode* child = RightNode->createChildSceneNode("MoveNodeRight");
		child->attachObject(StickRight);
		child->translate(150,0,0);

		Ogre::Entity* entPenguin2 = mSceneMgr->createEntity("StickLeft", "instrument_stick.mesh");
		entPenguin2 -> setCastShadows(true);
		Ogre::SceneNode* nodPenguin2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("LeftNode", Ogre::Vector3(-300, 100, 100));
		nodPenguin2->scale( 1, 1, 1); 
		Ogre::SceneNode* child2 = nodPenguin2->createChildSceneNode("MoveNodeLeft");
		child2->attachObject(entPenguin2);
		child2->translate(150,0,0);

}
//-------------------------------------------------------------------------------------
//Creation of frame listner
 void LapTrainer::createFrameListener(void){
	mMove = 250;
 }
 //-------------------------------------------------------------------------------------
//Key pressed event
bool LapTrainer::keyPressed(const OIS::KeyEvent &arg){
	bool ret = OGREBase::keyPressed(arg);

    return ret;
}

//-------------------------------------------------------------------------------------
//Key released event
bool LapTrainer::keyReleased(const OIS::KeyEvent &arg){
	bool ret = OGREBase::keyReleased(arg);
	
	return ret;
}

bool LapTrainer::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	//Static variables 
	static bool mMouseDown = false;     // If a mouse button is depressed
	static Ogre::Real mRotate = 0.13;   // The rotate constant
	static Ogre::Real mMove = 250;      // The movement constant
	bool currMouse = mMouse->getMouseState().buttonDown(OIS::MB_Left);
	bool ret = OGREBase::frameRenderingQueued(evt);

	if (!mNoSimballConnected) //Check if connected otherwise ignore
	{
		//--------------------------------Get Simballs values--------------------------------------------
		//Load data symbols 
		simball_Left.ReadSimBallState(simball.handle[1]);
		simball_Right.ReadSimBallState(simball.handle[2]);
		//simball_camera.ReadSimBallState(simball.handle[3]);

		//-------------------Left Stick-----------------------------------------------------------------
		//Yaw
		static int mYawL = 0;
		int YawCurrentL = simball_Left.AYaw;
		//Calculate yaw difference
		if (YawCurrentL != mYawL){
			mSceneMgr->getSceneNode("RightNode")->yaw(Ogre::Degree(YawCurrentL - mYawL));
			mYawL = YawCurrentL;
		}

		//Pitch
		static int mPitchL = 0;
		int PitchCurrentL = simball_Left.APitch;
		//Calculate pitch difference
		if (PitchCurrentL != mPitchL){
			mSceneMgr->getSceneNode("RightNode")->roll(Ogre::Degree(PitchCurrentL - mPitchL));
			mPitchL = PitchCurrentL;
		}

		//Roll
		static int mRotL = 0;
		int RotCurrentL = simball_Left.ARotation;
		//Calculate rotation difference
		if (RotCurrentL != mRotL){
			mSceneMgr->getSceneNode("RightNode")->pitch(Ogre::Degree(RotCurrentL - mRotL));
			mRotL = RotCurrentL;
		}

		//Insertion
		static int mInsL = 0;
		Ogre::Vector3 transVectorSbL = Ogre::Vector3::ZERO;
		int InsCurrentL = simball_Left.AInsertion;
		//Calculate rotation difference
		if (InsCurrentL != mInsL){
			transVectorSbL.x = (InsCurrentL - mInsL);
			mSceneMgr->getSceneNode("MoveNodeRight")->translate(transVectorSbL, Ogre::Node::TS_LOCAL);
			mInsL = InsCurrentL;
		}
 
		//-------------------Right Stick-----------------------------------------------------------------
		//Yaw
		static int mYawR = 0;
		int YawCurrentR = simball_Right.AYaw;
		//Calculate yaw difference
		if (YawCurrentR != mYawR){
			mSceneMgr->getSceneNode("LeftNode")->yaw(Ogre::Degree(YawCurrentR - mYawR));
			mYawR = YawCurrentR;
		}

		//Pitch
		static int mPitchR = 0;
		int PitchCurrentR = simball_Right.APitch;
		//Calculate pitch difference
		if (PitchCurrentR != mPitchR){
			mSceneMgr->getSceneNode("LeftNode")->roll(Ogre::Degree(PitchCurrentR - mPitchR));
			mPitchR = PitchCurrentR;
		}

		//Roll
		static int mRotR = 0;
		int RotCurrentR = simball_Right.ARotation;
		//Calculate rotation difference
		if (RotCurrentR != mRotR){
			mSceneMgr->getSceneNode("LeftNode")->pitch(Ogre::Degree(RotCurrentR - mRotR));
			mRotR = RotCurrentR;
		}

		//Insertion
		static int mInsR = 0;
		Ogre::Vector3 transVectorSbR = Ogre::Vector3::ZERO;
		int InsCurrentR = simball_Right.AInsertion;
		//Calculate rotation difference
		if (InsCurrentR != mInsR){
			transVectorSbR.x = (InsCurrentR - mInsR);
			mSceneMgr->getSceneNode("MoveNodeLeft")->translate(transVectorSbR, Ogre::Node::TS_LOCAL);
			mInsR = InsCurrentR;
		}
	
	//-------------------Camera Stick-----------------------------------------------------------------

	}
	//-------------------Keyboard movements left node-----------------------------------------------------------------
	//More information of the shorcuts of OIS: https://code.google.com/p/ezonas/source/browse/Ogrish/OIS-keycodes.txt?spec=svn92d08c346e48025efafc980557ee738ab0bd6ecd&r=92d08c346e48025efafc980557ee738ab0bd6ecd
	Ogre::Vector3 transVectorLeft = Ogre::Vector3::ZERO;
	Ogre::Vector3 transVectorInsertionLeft = Ogre::Vector3::ZERO;
		if (mKeyboard->isKeyDown(OIS::KC_W)) // Forward
		{
			transVectorLeft.z -= mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_S)) // Backward
		{
			if(mKeyboard->isKeyDown( OIS::KC_LSHIFT ))
			{
				// Pitch left
				mSceneMgr->getSceneNode("LeftNode")->pitch(Ogre::Degree(mRotate * 5));
			} 
			else if(mKeyboard->isKeyDown( OIS::KC_LMENU)) //Left alt
			{
				// Pitch right
				mSceneMgr->getSceneNode("LeftNode")->pitch(Ogre::Degree(mRotate * -5));
			}
			else {
			transVectorLeft.z += mMove;
			}
		}
		if (mKeyboard->isKeyDown(OIS::KC_A)) // Left - or roll
		{
			if(mKeyboard->isKeyDown( OIS::KC_LSHIFT ))
			{
				// Roll left
				mSceneMgr->getSceneNode("LeftNode")->roll(Ogre::Degree(mRotate * 5));
			} 
			else if(mKeyboard->isKeyDown( OIS::KC_LMENU)) //Left alt
			{
				// Roll right
				mSceneMgr->getSceneNode("LeftNode")->roll(Ogre::Degree(mRotate * -5));
			}
			else {
				transVectorLeft.x -= mMove; // Strafe left
			}
		}
		if (mKeyboard->isKeyDown(OIS::KC_D)) // Right - yaw or strafe
		{
			if(mKeyboard->isKeyDown( OIS::KC_LSHIFT ))
			{
				// Yaw left
				mSceneMgr->getSceneNode("LeftNode")->yaw(Ogre::Degree(mRotate * 5));
			} 
			else if(mKeyboard->isKeyDown( OIS::KC_LMENU)) //Left alt
			{
				// Yaw right
				mSceneMgr->getSceneNode("LeftNode")->yaw(Ogre::Degree(mRotate * -5));
			}
			else {
				transVectorLeft.x += mMove; // Strafe right
			}
		}
		if (mKeyboard->isKeyDown(OIS::KC_Z)) // Up
		{
			transVectorLeft.y += mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_X)) // Down
		{
			transVectorLeft.y -= mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_Q)) // Change x point
		{
			transVectorInsertionLeft.x -= mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_E)) // change x point
		{
			transVectorInsertionLeft.x += mMove;
		}

		// Translate the results to the scenenode
		//Move node on x-axis for the insertion 
		mSceneMgr->getSceneNode("MoveNodeLeft")->translate(transVectorInsertionLeft * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		mSceneMgr->getSceneNode("LeftNode")->translate(transVectorLeft * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

	//-------------------Keyboard movements right node-----------------------------------------------------------------
	//Keyboard movements 
	Ogre::Vector3 transVectorRight = Ogre::Vector3::ZERO;
	Ogre::Vector3 transVectorInsertionRight = Ogre::Vector3::ZERO;
		if (mKeyboard->isKeyDown(OIS::KC_UP)) // Forward
		{
			transVectorRight.z -= mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_DOWN)) // Backward
		{
			if(mKeyboard->isKeyDown( OIS::KC_LSHIFT ))
			{
				// Pitch left
				mSceneMgr->getSceneNode("RightNode")->pitch(Ogre::Degree(mRotate * 5));
			} 
			else if(mKeyboard->isKeyDown( OIS::KC_LMENU)) //Left alt
			{
				// Pitch right
				mSceneMgr->getSceneNode("RightNode")->pitch(Ogre::Degree(mRotate * -5));
			}
			else {
			transVectorRight.z += mMove;
			}
		}
		if (mKeyboard->isKeyDown(OIS::KC_LEFT)) // Left - or roll
		{
			if(mKeyboard->isKeyDown( OIS::KC_LSHIFT ))
			{
				// Roll left
				mSceneMgr->getSceneNode("RightNode")->roll(Ogre::Degree(mRotate * 5));
			} 
			else if(mKeyboard->isKeyDown( OIS::KC_LMENU)) //Left alt
			{
				// Roll right
				mSceneMgr->getSceneNode("RightNode")->roll(Ogre::Degree(mRotate * -5));
			}
			else {
				transVectorRight.x -= mMove; // Strafe left
			}
		}
		if (mKeyboard->isKeyDown(OIS::KC_RIGHT)) // Right - yaw or strafe
		{
			if(mKeyboard->isKeyDown( OIS::KC_LSHIFT ))
			{
				// Yaw left
				mSceneMgr->getSceneNode("RightNode")->yaw(Ogre::Degree(mRotate * 5));
			} 
			else if(mKeyboard->isKeyDown( OIS::KC_LMENU)) //Left alt
			{
				// Yaw right
				mSceneMgr->getSceneNode("RightNode")->yaw(Ogre::Degree(mRotate * -5));
			}
			else {
				transVectorRight.x += mMove; // Strafe right
			}
		}
		if (mKeyboard->isKeyDown(OIS::KC_N)) // Up
		{
			transVectorRight.y += mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_M)) // Down
		{
			transVectorRight.y -= mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_K)) // Change x point
		{
			transVectorInsertionRight.x -= mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_L)) // change x point
		{
			transVectorInsertionRight.x += mMove;
		}
		// Translate the results to the scenenode
		//Move node on x-axis for the insertion 
		mSceneMgr->getSceneNode("MoveNodeRight")->translate(transVectorInsertionRight * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		mSceneMgr->getSceneNode("RightNode")->translate(transVectorRight * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

		//-------------------Change sensibility -----------------------------------------------------------------
		//Change sensibility of the keyboard
		if (mKeyboard->isKeyDown(OIS::KC_1)) // Change speed
		{
			mMove = 50;
			mRotate = 0.07;
		}
		if (mKeyboard->isKeyDown(OIS::KC_2)) // Change speed
		{
			mMove = 100;
			mRotate = 0.10;
		}
		if (mKeyboard->isKeyDown(OIS::KC_3)) // Change speed
		{
			mMove = 250;
			mRotate = 0.13;
		}

		
		//-------------------Move camera -----------------------------------------------------------------
		Ogre::Vector3 transVectorInsertion = Ogre::Vector3::ZERO;
		if (mKeyboard->isKeyDown(OIS::KC_Y)) // Pull back
		{
			transVectorInsertion.z -= mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_U)) // Insert
		{
			transVectorInsertion.z += mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_I)) // Roll
		{
			if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			{
				mSceneMgr->getSceneNode("cameraNode")->roll(Ogre::Degree(mRotate * -5));
			}
			else
			{
				mSceneMgr->getSceneNode("cameraNode")->roll(Ogre::Degree(mRotate * 5));
			}

		}
		if (mKeyboard->isKeyDown(OIS::KC_O)) // Pitch
		{
			if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			{
				mSceneMgr->getSceneNode("cameraNode")->pitch(Ogre::Degree(mRotate * -5));
			}
			else
			{
				mSceneMgr->getSceneNode("cameraNode")->pitch(Ogre::Degree(mRotate * 5));
			}
		}
		if (mKeyboard->isKeyDown(OIS::KC_P)) // Yaw
		{
			if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			{
				mSceneMgr->getSceneNode("cameraNode")->yaw(Ogre::Degree(mRotate * -5));
			}
			else
			{
				mSceneMgr->getSceneNode("cameraNode")->yaw(Ogre::Degree(mRotate * 5));
			}
		}
		mSceneMgr->getSceneNode("cameraInsertion")->translate(transVectorInsertion * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);




	return ret;
}

//FROM HERE WE START LapTrainer PROGRAM
//Ogre startup sequence 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
 
#ifdef __cplusplus
extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int LapTrainer(int argc, char *argv[])
#endif
    {
        // Create application object
        LapTrainer app;
 
        try {
            app.OGREgo();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }
 
        return 0;
    }
 
#ifdef __cplusplus
}
#endif