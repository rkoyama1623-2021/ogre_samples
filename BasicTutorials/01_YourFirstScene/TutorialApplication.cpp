/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------
*/
#include <RTShaderSystem/OgreRTShaderSystem.h>
#include "TutorialApplication.h"

using namespace Ogre;
using namespace OgreBites;


//-------------------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
	// register our scene with the RTSS
	if (RTShader::ShaderGenerator::initialize())
	{
		RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
		shadergen->addSceneManager(mSceneMgr);
	}

    // -- tutorial section start --
    //! [turnlights]
    mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
    //! [turnlights]

    //! [newlight]
    Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20,80,50);
    //! [newlight]

	// Add entity1
    Entity* ogreEntity = mSceneMgr->createEntity("ogrehead.mesh");
    SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    ogreNode->attachObject(ogreEntity);

	// Add entity2
    Entity* ogreEntity2 = mSceneMgr->createEntity("ogrehead.mesh");
    SceneNode* ogreNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(84, 48, 0));
    ogreNode2->attachObject(ogreEntity2);

    // Add entity3
    Entity* ogreEntity3 = mSceneMgr->createEntity("ogrehead.mesh");
    SceneNode* ogreNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    ogreNode3->setPosition(0, 104, 0);
    ogreNode3->setScale(2, 1.2, 1);
    ogreNode3->attachObject(ogreEntity3);

    Entity* ogreEntity4 = mSceneMgr->createEntity("ogrehead.mesh");
    SceneNode* ogreNode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    ogreNode4->setPosition(-84, 48, 0);
    ogreNode4->roll(Degree(-90));
    ogreNode4->attachObject(ogreEntity4);

    //! [cameramove]
    mCamera->setPosition(0, 47, 222);
    //! [cameramove]

	// -- tutorial section end --
}

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include "macUtils.h"
#endif

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
		int main(int argc, char *argv[])
#endif
		{
			// Create application object
			TutorialApplication app;

			try {
				app.go();
			} catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
				MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
				std::cerr << "An exception has occurred: " <<
					e.getFullDescription().c_str() << std::endl;
#endif
			}

			return 0;
		}

#ifdef __cplusplus
	}
#endif
