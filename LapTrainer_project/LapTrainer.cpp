#include "LapTrainer.h"
//-------------------------------------------------------------------------------------
LapTrainer::LapTrainer(void)
{ 
	srand (time(NULL));   // 
	//lab constructor
	num_res=simball.DevIdentificTech(7330,1);
	num_res=simball.ConnectToSimBall(0);
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
		//Create penguin
		Ogre::Entity* entPenguin = mSceneMgr->createEntity("Penguin", "instrument_stick.mesh");
		entPenguin -> setCastShadows(true);
		Ogre::SceneNode* nodPenguin = mSceneMgr->getRootSceneNode()->createChildSceneNode("PenguinNode", Ogre::Vector3( 0, 20, 0 ));
		nodPenguin->scale( .1, .1, .1); 
		//nodPenguin->attachObject(entPenguin);
		Ogre::SceneNode* child = nodPenguin->createChildSceneNode("MoveNode");
		child->attachObject(entPenguin);
		child->translate(-150,0,0);

		//Create penguin
		Ogre::Entity* entPenguin2 = mSceneMgr->createEntity("Penguin2", "instrument_stick.mesh");
		entPenguin2 -> setCastShadows(true);
		Ogre::SceneNode* nodPenguin2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("PenguinNode2", Ogre::Vector3( -10, 5, 0 ));
		nodPenguin2->scale( .1, .1, .1); 
		//nodPenguin->attachObject(entPenguin);
		Ogre::SceneNode* child2 = nodPenguin2->createChildSceneNode("MoveNode2");
		child2->attachObject(entPenguin2);
		child2->translate(-150,0,0);
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


	//Get Simballs values
	
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
		mSceneMgr->getSceneNode("PenguinNode")->yaw(Ogre::Degree(YawCurrentL - mYawL));
		mYawL = YawCurrentL;
	}

	//Pitch
	static int mPitchL = 0;
	int PitchCurrentL = simball_Left.APitch;
	//Calculate pitch difference
	if (PitchCurrentL != mPitchL){
		mSceneMgr->getSceneNode("PenguinNode")->roll(Ogre::Degree(PitchCurrentL - mPitchL));
		mPitchL = PitchCurrentL;
	}

	//Roll
	static int mRotL = 0;
	int RotCurrentL = simball_Left.ARotation;
	//Calculate rotation difference
	if (RotCurrentL != mRotL){
		mSceneMgr->getSceneNode("PenguinNode")->pitch(Ogre::Degree(RotCurrentL - mRotL));
		mRotL = RotCurrentL;
	}

	//Insertion
	static int mInsL = 0;
	Ogre::Vector3 transVectorSbL = Ogre::Vector3::ZERO;
	int InsCurrentL = simball_Left.AInsertion;
	//Calculate rotation difference
	if (InsCurrentL != mInsL){
		transVectorSbL.x = (InsCurrentL - mInsL);
		mSceneMgr->getSceneNode("MoveNode")->translate(transVectorSbL, Ogre::Node::TS_LOCAL);
		mInsL = InsCurrentL;
	}
 
	//-------------------Right Stick-----------------------------------------------------------------
	//Yaw
	static int mYawR = 0;
	int YawCurrentR = simball_Right.AYaw;
	//Calculate yaw difference
	if (YawCurrentR != mYawR){
		mSceneMgr->getSceneNode("PenguinNode2")->yaw(Ogre::Degree(YawCurrentR - mYawR));
		mYawR = YawCurrentR;
	}

	//Pitch
	static int mPitchR = 0;
	int PitchCurrentR = simball_Right.APitch;
	//Calculate pitch difference
	if (PitchCurrentR != mPitchR){
		mSceneMgr->getSceneNode("PenguinNode2")->roll(Ogre::Degree(PitchCurrentR - mPitchR));
		mPitchR = PitchCurrentR;
	}

	//Roll
	static int mRotR = 0;
	int RotCurrentR = simball_Right.ARotation;
	//Calculate rotation difference
	if (RotCurrentR != mRotR){
		mSceneMgr->getSceneNode("PenguinNode2")->pitch(Ogre::Degree(RotCurrentR - mRotR));
		mRotR = RotCurrentR;
	}

	//Insertion
	static int mInsR = 0;
	Ogre::Vector3 transVectorSbR = Ogre::Vector3::ZERO;
	int InsCurrentR = simball_Right.AInsertion;
	//Calculate rotation difference
	if (InsCurrentR != mInsR){
		transVectorSbR.x = (InsCurrentR - mInsR);
		mSceneMgr->getSceneNode("MoveNode2")->translate(transVectorSbR, Ogre::Node::TS_LOCAL);
		mInsR = InsCurrentR;
	}
	
	
	
	//Keyboard movements 
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
	Ogre::Vector3 transVector2 = Ogre::Vector3::ZERO;
		if (mKeyboard->isKeyDown(OIS::KC_W)) // Forward
		{
			transVector.z -= mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_S)) // Backward
		{
			if(mKeyboard->isKeyDown( OIS::KC_LSHIFT ))
			{
				// Yaw left
				mSceneMgr->getSceneNode("PenguinNode")->pitch(Ogre::Degree(mRotate * 5));
			} else {
			transVector.z += mMove;
			}
		}
		if (mKeyboard->isKeyDown(OIS::KC_A)) // Left - yaw or strafe
		{
			if(mKeyboard->isKeyDown( OIS::KC_LSHIFT ))
			{
				// Yaw left
				mSceneMgr->getSceneNode("PenguinNode")->roll(Ogre::Degree(mRotate * 5));
			} else {
				transVector.x -= mMove; // Strafe left
			}
		}
		if (mKeyboard->isKeyDown(OIS::KC_D)) // Right - yaw or strafe
		{
			if(mKeyboard->isKeyDown( OIS::KC_LSHIFT ))
			{
				// Yaw right
				mSceneMgr->getSceneNode("PenguinNode")->yaw(Ogre::Degree(-mRotate * 5));
			} else {
				transVector.x += mMove; // Strafe right
			}
		}
		if (mKeyboard->isKeyDown(OIS::KC_Z)) // Up
		{
			transVector.y += mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_X)) // Down
		{
			transVector.y -= mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_1)) // Change speed
		{
			mMove = 125;
			mRotate = 0.07;
		}
		if (mKeyboard->isKeyDown(OIS::KC_2)) // Change speed
		{
			mMove = 250;
			mRotate = 0.13;
		}
		if (mKeyboard->isKeyDown(OIS::KC_2)) // Change speed
		{
			mMove = 250;
			mRotate = 0.13;
		}
		if (mKeyboard->isKeyDown(OIS::KC_L)) // Forward
		{
			transVector2.x -= mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_K)) // Backward
		{
			transVector2.x += mMove;
		}
		// Translate the results to the scenenode

		//Move node on x-axis for the insertion 
		mSceneMgr->getSceneNode("MoveNode")->translate(transVector2 * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		mSceneMgr->getSceneNode("PenguinNode")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

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