/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------
*/
#include <RTShaderSystem/OgreRTShaderSystem.h>
#include "TutorialApplication.h"

using namespace Ogre;
using namespace OgreBites;

const char* const RGN_DEFAULT = "General";
const char* const RGN_INTERNAL = "OgreInternal";
const char* const RGN_AUTODETECT = "OgreAutodetect";

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

	//! [cameraposition]
	mCamera->setPosition(Ogre::Vector3(200, 300, 400));
	mCamera->lookAt(Vector3(0, 0, 0));
    //! [cameraposition]

    //! [lightingsset]
    mSceneMgr->setAmbientLight(ColourValue(0, 0, 0));
    mSceneMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);
    //! [lightingsset]

    //! [ninja]
    Entity* ninjaEntity = mSceneMgr->createEntity("ninja.mesh");
    ninjaEntity->setCastShadows(true);
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ninjaEntity);
    //! [ninja]

    //! [plane]
    Plane plane(Vector3::UNIT_Y, 0);
    //! [plane]
    //! [planedefine]
    MeshManager::getSingleton().createPlane(
            "ground", RGN_DEFAULT,
            plane,
            1500, 1500, 20, 20,
            true,
            1, 5, 5,
            Vector3::UNIT_Z);
    //! [planedefine]
    //! [planecreate]
    Entity* groundEntity = mSceneMgr->createEntity("ground");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
    //! [planecreate]
    //! [planenoshadow]
    groundEntity->setCastShadows(false);
    //! [planenoshadow]
    //! [planesetmat]
    groundEntity->setMaterialName("Examples/Rockwall");
    //! [planesetmat]

    //! [spotlight]
    Light* spotLight = mSceneMgr->createLight("SpotLight");
    //! [spotlight]
    //! [spotlightcolor]
    spotLight->setDiffuseColour(0, 0, 1.0);
    spotLight->setSpecularColour(0, 0, 1.0);
    //! [spotlightcolor]
    //! [spotlighttype]
    spotLight->setType(Light::LT_SPOTLIGHT);
    //! [spotlighttype]
    //! [spotlightposrot]
    spotLight->setDirection(-1, -1, 0);
    spotLight->setPosition(Vector3(200, 200, 0));
    //! [spotlightposrot]
    //! [spotlightrange]
    spotLight->setSpotlightRange(Degree(35), Degree(50));
    //! [spotlightrange]

    //! [directlight]
    Light* directionalLight = mSceneMgr->createLight("DirectionalLight");
    directionalLight->setType(Light::LT_DIRECTIONAL);
    //! [directlight]
    //! [directlightcolor]
    directionalLight->setDiffuseColour(ColourValue(0.4, 0, 0));
    directionalLight->setSpecularColour(ColourValue(0.4, 0, 0));
    //! [directlightcolor]
    //! [directlightdir]
    directionalLight->setDirection(Vector3(0, -1, 1));
    //! [directlightdir]

    //! [pointlight]
    Light* pointLight = mSceneMgr->createLight("PointLight");
    pointLight->setType(Light::LT_POINT);
    //! [pointlight]
    //! [pointlightcolor]
    pointLight->setDiffuseColour(0.3, 0.3, 0.3);
    pointLight->setSpecularColour(0.3, 0.3, 0.3);
    //! [pointlightcolor]
    //! [pointlightpos]
    SceneNode* pointLightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    pointLightNode->attachObject(pointLight);
    pointLightNode->setPosition(Vector3(0, 150, 250));
    //! [pointlightpos]
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
