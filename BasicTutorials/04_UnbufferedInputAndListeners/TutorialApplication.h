/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------
*/
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"

class TutorialApplication : public BaseApplication
{
public:
  TutorialApplication();
  virtual ~TutorialApplication();

protected:
  virtual void createScene();
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

private:
  bool processUnbufferedInput(const Ogre::FrameEvent& fe);

};
#endif // #ifndef __TutorialApplication_h_
