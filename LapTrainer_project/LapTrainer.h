#include "OGRE.h"
#include "Simball.h"
#include <cassert>
#include <cmath>

class LapTrainer : public OGREBase
{
public:
    LapTrainer(void);
    virtual ~LapTrainer(void);
	SimBall simball, simball_Left, simball_Right,simball_camera;
	int num_res;
private:	

protected:
	virtual void createScene(void);
	virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual void createFrameListener(void);
	Ogre::Real mMove;            // The movement constant
};