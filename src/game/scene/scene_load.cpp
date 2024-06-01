#include "scene_load.h"

#include <stdio.h>

#include "../mz_director.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneLoad::SceneLoad(MZDirector *director) : MZScene(director, SceneType::ST_LOAD)
{
    currentState = SS_INIT;
    deltaTime = 0.0f;

    if (director->config->currentConfig.video)
        optionButtons[0] = BTN_VID_ON;
    else
        optionButtons[0] = BTN_VID_OFF;

    if (director->config->currentConfig.audio)
        optionButtons[1] = BTN_SND_ON;
    else
        optionButtons[1] = BTN_SND_OFF;

    if (director->config->currentConfig.music)
        optionButtons[2] = BTN_MUS_ON;
    else
        optionButtons[2] = BTN_MUS_OFF;

    optionButtons[3] = BTN_EXIT;
}

SceneLoad::~SceneLoad() {}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneLoad::update()
{
    switch (currentState)
    {
    case SS_INIT:
        ssInit();
        break;
    case SS_LOAD_DATA:
        ssLoadData();
        break;
    case SS_LOAD_DATA_END:
        ssLoadDataEnd();
        break;
    case SS_TRANSITION_INTRO:
        ssTransitionIntro();
        break;
    default:
        break;
    }

    // Delta Time Handle
    deltaTime += GetFrameTime();
}

void SceneLoad::ssInit()
{
    // Draw 3D
    //--------------------------------------------------------------------------------------
    director->composite->beginRT();
    director->composite->beginMode3D();
    director->composite->endMode3D();
    director->composite->endRT();

    // Draw Composite
    //--------------------------------------------------------------------------------------
    director->composite->beginDrawing();
    director->composite->drawComposite();
    if (deltaTime >= 1.0f)
        director->composite->drawLogo(deltaTime - 1.0f);
    director->composite->endDrawing();

    if (deltaTime >= 2.0f)
    {
        deltaTime = 0.0f;
        currentState = SS_LOAD_DATA;
    }
}

void SceneLoad::ssLoadData()
{
    // Load Data
    //--------------------------------------------------------------------------------------
    director->audio->loadResources();
    director->render3D->loadResources();
    director->renderVS->loadResources();
    director->uciEngine->initialize();

    if (!director->skipIntro)
        director->audio->playIntroSound();

    // Draw 3D
    //--------------------------------------------------------------------------------------
    director->composite->beginRT();
    director->composite->beginMode3D();
    director->composite->endMode3D();
    director->composite->endRT();

    // Draw Composite
    //--------------------------------------------------------------------------------------
    director->composite->beginDrawing();
    director->composite->drawComposite();
    director->composite->drawLogo(1.0f);
    director->composite->endDrawing();

    // End
    //--------------------------------------------------------------------------------------
    deltaTime = 0.0f;
    // TODO - REMOVE DEBUG LOAD DIRECTLY MENU
    if (director->skipIntro)
    {
        director->composite->updateBaseCamPosition(menuCamera.position);
        director->composite->updateBaseCamTarget(menuCamera.target);
        director->composite->updateBaseCamFov(menuCamera.fovy);
        director->composite->updateFinalCam();
        director->audio->playIntroMusic();
        director->audio->setIntroMusicVolume(0.6f);
        deltaTime = 0.0f;
        director->newScene = ST_MENU_SELECT;
    }
    currentState = SS_LOAD_DATA_END;
}

void SceneLoad::ssLoadDataEnd()
{
    // Draw 3D
    //--------------------------------------------------------------------------------------
    director->composite->beginRT();
    director->composite->beginMode3D();
    director->composite->endMode3D();
    director->composite->endRT();

    // Draw Composite
    //--------------------------------------------------------------------------------------
    director->composite->beginDrawing();
    director->composite->drawComposite();
    director->composite->drawLogo(1.0f);
    director->composite->endDrawing();

    if (deltaTime >= 3.0f)
    {
        deltaTime = 0.0f;
        currentState = SS_TRANSITION_INTRO;
    }
}

void SceneLoad::ssTransitionIntro()
{
    // Update Camera
    //--------------------------------------------------------------------------------------
    director->composite->updateBaseCamPosition(
        Math::vector3Lerp(initCamera.position, menuCamera.position, Math::easeInOutExpo(deltaTime / 5.0f)));
    director->composite->updateBaseCamTarget(
        Math::vector3Lerp(initCamera.target, menuCamera.target, Math::easeInOutExpo(deltaTime / 5.0f)));
    director->composite->updateBaseCamFov(
        Math::floatLerp(initCamera.fovy, menuCamera.fovy, Math::easeInOutExpo(deltaTime / 5.0f)));
    director->composite->updateFinalCam();

    // Update Audio
    //--------------------------------------------------------------------------------------
    director->audio->setIntroMusicVolume(Math::floatLerp(0.1f, 0.6f, deltaTime / 5.0f));
    director->audio->updateAudio();

    // Draw 2D
    //--------------------------------------------------------------------------------------
    director->renderVS->beginRT();
    for (UIButton opt : optionButtons)
    {
        director->renderVS->drawOption(opt, BS_DISABLED);
    }
    director->renderVS->endRT();

    // Draw 3D
    //--------------------------------------------------------------------------------------
    director->composite->beginRT();
    director->composite->beginMode3D();
    director->render3D->drawArcade();
    director->renderVS->drawArcadeVS();
    director->composite->endMode3D();
    director->composite->drawBlankOverlay(1.0f - deltaTime / 2.0f);
    director->composite->endRT();

    // Draw Composite
    //--------------------------------------------------------------------------------------
    director->composite->beginDrawing();
    director->composite->drawComposite();
    director->composite->drawLogo(1.0f - deltaTime / 2.0f);
    director->composite->endDrawing();

    if (deltaTime >= 5.0f)
    {
        director->composite->updateBaseCamPosition(menuCamera.position);
        director->composite->updateBaseCamTarget(menuCamera.target);
        director->composite->updateBaseCamFov(menuCamera.fovy);
        director->composite->updateFinalCam();
        director->audio->playIntroMusic();
        director->audio->setIntroMusicVolume(0.6f);

        deltaTime = 0.0f;
        director->newScene = ST_MENU_SELECT;
    }
}