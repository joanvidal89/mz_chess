#include "scene_exit.h"
#include "../mz_director.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneExit::SceneExit(MZDirector *director) : MZScene(director, SceneType::ST_EXIT)
{
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

SceneExit::~SceneExit() {}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneExit::update()
{
    // Update Camera
    //--------------------------------------------------------------------------------------
    director->composite->updateBaseCamPosition(Math::vector3Lerp(menuCamera.position, initCamera.position, Math::easeInOutExpo(deltaTime / 4.0f)));
    director->composite->updateBaseCamTarget(Math::vector3Lerp(menuCamera.target, initCamera.target, Math::easeInOutExpo(deltaTime / 4.0f)));
    director->composite->updateBaseCamFov(Math::floatLerp(menuCamera.fovy, initCamera.fovy, Math::easeInOutExpo(deltaTime / 4.0f)));
    director->composite->updateFinalCam();

    // Update Audio
    //--------------------------------------------------------------------------------------
    director->audio->updateAudio();

    // Draw VS
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
    director->composite->drawBlackOverlay(deltaTime / 3.0f);
    director->composite->endRT();

    // Draw Composite
    //--------------------------------------------------------------------------------------
    director->composite->beginDrawing();
    director->composite->drawComposite();
    director->composite->endDrawing();

    if (deltaTime >= 3.0f)
    {
        director->exitGame = true;
    }

    // Delta Time Handle
    deltaTime += GetFrameTime();
}