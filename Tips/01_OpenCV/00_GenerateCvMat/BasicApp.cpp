#include "BasicApp.h"

// https://forums.ogre3d.org/viewtopic.php?t=81095
// int GetFormat(const Ogre::PixelFormat& pf)
// {
//     Ogre::PixelComponentType pct = Ogre::PixelUtil::getComponentType(pf);
//
//     /*
//     8-bit unsigned integer (uchar)
//     8-bit signed integer (schar)
//     16-bit unsigned integer (ushort)
//     16-bit signed integer (short)
//     32-bit signed integer (int)
//     32-bit floating-point number (float)
//     64-bit floating-point number (double)
//     enum { CV_8U=0, CV_8S=1, CV_16U=2, CV_16S=3, CV_32S=4, CV_32F=5, CV_64F=6 };
//     */
//
//     int cv_pct;
//     if (pct == Ogre::PCT_BYTE)
//     {
//         // Byte per component (8 bit fixed 0.0..1.0)
//         cv_pct = CV_8U;
//     }
//     else if (pct == Ogre::PCT_SHORT)
//     {
//         // Short per component (16 bit fixed 0.0..1.0))
//         cv_pct = CV_16U;
//     }
//     else if (pct ==  Ogre::PCT_FLOAT16)
//     {
//         // 16 bit float per component
//         cv_pct = CV_32F;
//     }
//     else if (pct ==  Ogre::PCT_FLOAT32)
//     {
//         // 32 bit float per component
//         cv_pct = CV_32F;
//     }
//     else if (pct ==  Ogre::PCT_SINT)
//     {
//         // Signed integer per component
//         cv_pct = CV_16S;
//     }
//     else
//     {
//         cv_pct = CV_8U;
//     }
//
//
//     return CV_MAKETYPE(cv_pct, Ogre::PixelUtil::getComponentCount(pf));
// }


BasicApp::BasicApp()
  : mShutdown(false),
    mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mCameraMan(0),
    // mRenderer(0),
    mMouse(0),
    mKeyboard(0),
    mInputMgr(0)
{
}

BasicApp::~BasicApp()
{
    if (mCameraMan) delete mCameraMan;

    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);

    delete mRoot;
}

void BasicApp::go()
{
    #ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
    #else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
    #endif

    if (!setup())
    return;

    mRoot->startRendering();

    destroyScene();
}

cv::Mat* BasicApp::toCvMat(Ogre::TexturePtr texture)
{
    Ogre::RenderTexture* renderTexture = texture->getBuffer()->getRenderTarget();
    Ogre::PixelFormat src_type = texture->getFormat();
    int width = texture->getWidth();
    int height = texture->getHeight();
    // https://stackoverflow.com/questions/8182251/obtain-wrong-rgb-during-creating-dynamic-texture-in-ogre
    // The texture you have created was in the format of Ogre::PF_R8G8B8,
    // but the actual hardwarebuffer Ogre is using is still Ogre::PF_R8G8B8A8
    cv::Mat* mat = new cv::Mat(height, width, CV_8UC4);
    // cv::Mat* mat = new cv::Mat(height, width, GetFormat(src_type));
    Ogre::PixelBox pb(width, height, 1, src_type, mat->ptr());
    renderTexture->copyContentsToMemory(pb, Ogre::RenderTexture::FrameBuffer::FB_AUTO);
    return mat;
};

bool BasicApp::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) mShutdown = true;

    if (mShutdown) return false;

    if (mWindow->isClosed()) return false;

    mKeyboard->capture();
    mMouse->capture();

    mPlaneNode->yaw(Ogre::Radian(0.5 * fe.timeSinceLastFrame));

    mCameraMan->frameRenderingQueued(fe);

    // CEGUI::System::getSingleton().injectTimePulse(fe.timeSinceLastFrame);

    return true;
}

bool BasicApp::keyPressed(const OIS::KeyEvent& ke)
{
    // CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    // context.injectKeyDown((CEGUI::Key::Scan)ke.key);
    // context.injectChar((CEGUI::Key::Scan)ke.text);

    mCameraMan->injectKeyDown(ke);

    return true;
}

bool BasicApp::keyReleased(const OIS::KeyEvent& ke)
{
    // CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    // context.injectKeyUp((CEGUI::Key::Scan)ke.key);

    mCameraMan->injectKeyUp(ke);

    return true;
}

bool BasicApp::mouseMoved(const OIS::MouseEvent& me)
{
    // CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    // context.injectMouseMove(me.state.X.rel, me.state.Y.rel);

    mCameraMan->injectMouseMove(me);

    return true;
}

// // Helper function for mouse events
// CEGUI::MouseButton convertButton(OIS::MouseButtonID id)
// {
//     switch (id)
//     {
//         case OIS::MB_Left:
//         return CEGUI::LeftButton;
//         case OIS::MB_Right:
//         return CEGUI::RightButton;
//         case OIS::MB_Middle:
//         return CEGUI::MiddleButton;
//         default:
//         return CEGUI::LeftButton;
//     }
// }

bool BasicApp::mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
    // CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    // context.injectMouseButtonDown(convertButton(id));

    mCameraMan->injectMouseDown(me, id);

    return true;
}

bool BasicApp::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
    // CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    // context.injectMouseButtonUp(convertButton(id));

    mCameraMan->injectMouseUp(me, id);

    return true;
}

void BasicApp::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState& ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void BasicApp::windowClosed(Ogre::RenderWindow* rw)
{
    if (rw == mWindow)
    {
        if (mInputMgr)
        {
            mInputMgr->destroyInputObject(mMouse);
            mInputMgr->destroyInputObject(mKeyboard);

            OIS::InputManager::destroyInputSystem(mInputMgr);
            mInputMgr = 0;
        }
    }
}

bool BasicApp::setup()
{
    mRoot = new Ogre::Root(mPluginsCfg);

    setupResources();

    if (!configure())
    return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    createResourceListener();
    loadResources();

    // setupCEGUI();

    createScene();

    createFrameListener();

    return true;
}

bool BasicApp::configure()
{
    if (!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
    {
        return false;
    }

    mWindow = mRoot->initialise(true, "ITutorial");

    return true;
}

void BasicApp::chooseSceneManager()
{
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
}

void BasicApp::createCamera()
{
    mCamera = mSceneMgr->createCamera("PlayerCam");

    mCamera->setPosition(Ogre::Vector3(0, 0, 80));
    mCamera->lookAt(Ogre::Vector3(0, 0, -300));
    mCamera->setNearClipDistance(5);

    mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

void BasicApp::createScene()
{
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20, 80, 50);

    mCamera->setPosition(60, 200, 70);
    mCamera->lookAt(0,0,0);

    Ogre::MaterialPtr mat =
        Ogre::MaterialManager::getSingleton().create(
        "PlaneMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    Ogre::TextureUnitState* tuisTexture =
        mat->getTechnique(0)->getPass(0)->createTextureUnitState("grass_1024.jpg");

    mPlane = new Ogre::MovablePlane("Plane");
    mPlane->d = 0;
    mPlane->normal = Ogre::Vector3::UNIT_Y;

    Ogre::MeshManager::getSingleton().createPlane(
        "PlaneMesh",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        *mPlane,
        120, 120, 1, 1,
        true,
        1, 1, 1,
        Ogre::Vector3::UNIT_Z);
    mPlaneEntity = mSceneMgr->createEntity("PlaneMesh");
    mPlaneEntity->setMaterialName("PlaneMat");

    mPlaneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mPlaneNode->attachObject(mPlaneEntity);

    // Texture
    Ogre::TexturePtr rttTexture =
        Ogre::TextureManager::getSingleton().createManual(
            "RttTex",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D,
            mWindow->getWidth(), mWindow->getHeight(),
            0,
            Ogre::PF_R8G8B8A8, // Hardwarebuffer always use alpha, so use A8.
            Ogre::TU_RENDERTARGET);

    Ogre::RenderTexture* renderTexture = rttTexture->getBuffer()->getRenderTarget();
    renderTexture->addViewport(mCamera);
    renderTexture->getViewport(0)->setClearEveryFrame(true);
    renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
    renderTexture->getViewport(0)->setOverlaysEnabled(false);

    renderTexture->update();
    // renderTexture->writeContentsToFile("start.png");
    cv::Mat* cvmat = toCvMat(rttTexture);
    cv::imwrite("start_cv.png", *cvmat);

    // Mini screen to show texture
    mMiniscreen = new Ogre::Rectangle2D(true);
    mMiniscreen->setCorners(.5, 1.0, 1.0, .5);
    mMiniscreen->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
    Ogre::SceneNode* miniscreenNode =
        mSceneMgr->getRootSceneNode()->createChildSceneNode();
    miniscreenNode->attachObject(mMiniscreen);

    // Material
    Ogre::MaterialPtr renderMat =
    Ogre::MaterialManager::getSingleton().create(
        "RttMat",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    renderMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    renderMat->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex");

    // Associate Material(Texture) to the Mini screen
    mMiniscreen->setMaterial("RttMat");

    renderTexture->addListener(this);

    // CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    // CEGUI::Window* rootWin = wmgr.loadLayoutFromFile("test.layout");

    // CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(rootWin);

}

void BasicApp::destroyScene()
{
}

void BasicApp::createFrameListener()
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputMgr = OIS::InputManager::createInputSystem(pl);

    mKeyboard = static_cast<OIS::Keyboard*>(
        mInputMgr->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(
        mInputMgr->createInputObject(OIS::OISMouse, true));

    mKeyboard->setEventCallback(this);
    mMouse->setEventCallback(this);

    windowResized(mWindow);

    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mRoot->addFrameListener(this);

    Ogre::LogManager::getSingletonPtr()->logMessage("Finished");
}

void BasicApp::createViewports()
{
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) /
        Ogre::Real(vp->getActualHeight()));
}

void BasicApp::setupResources()
{
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();

    while (secIt.hasMoreElements())
    {
        secName = secIt.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator setIt;

        for (setIt = settings->begin(); setIt != settings->end(); ++setIt)
        {
            typeName = setIt->first;
            archName = setIt->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
            }
        }
    }

void BasicApp::createResourceListener()
{
}

void BasicApp::loadResources()
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

// bool BasicApp::setupCEGUI()
// {
//     Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing CEGUI ***");
//
//     mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
//
//     CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
//     CEGUI::Font::setDefaultResourceGroup("Fonts");
//     CEGUI::Scheme::setDefaultResourceGroup("Schemes");
//     CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
//     CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
//
//     CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
//     CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
//
//     CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
//
//     context.setDefaultFont("DejaVuSans-10");
//     context.getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
//
//     Ogre::LogManager::getSingletonPtr()->logMessage("Finished");
//
//     return true;
// }

void BasicApp::preRenderTargetUpdate(const Ogre::RenderTargetEvent& rte)
{
    mMiniscreen->setVisible(false);
}

void BasicApp::postRenderTargetUpdate(const Ogre::RenderTargetEvent& rte)
{
    mMiniscreen->setVisible(true);
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    #define WIN32_LEAN_AND_MEAN
    #include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
{
    BasicApp app;

    try
    {
        app.go();
    }
    catch(Ogre::Exception& e)
    {
        #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox(
            NULL,
            e.getFullDescription().c_str(),
            "An exception has occured!",
            MB_OK | MB_ICONERROR | MB_TASKMODAL);
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
