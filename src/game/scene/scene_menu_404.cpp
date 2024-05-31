#include "scene_menu_404.h"

#include "../mz_director.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneMenu404::SceneMenu404(MZDirector *director) : MZScene(director, SceneType::ST_MENU_404)
{
    deltaTime = 0.0f;
    currentState = SS_MENU_IN;
    futureScene = ST_MENU_SELECT;
    lastHoverBtnId = -1;
    currentHoverBtnId = -1;

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

SceneMenu404::~SceneMenu404() {}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneMenu404::update()
{
    // Update Audio
    //--------------------------------------------------------------------------------------
    director->audio->updateAudio();

    // Scene Flow
    //--------------------------------------------------------------------------------------
    switch (currentState)
    {
    case SS_MENU_IN:
        ssMenuIn();
        break;
    case SS_MENU:
        ssMenu();
        break;
    case SS_MENU_OUT:
        ssMenuOut();
        break;
    default:
        break;
    }

    deltaTime += GetFrameTime();
}

void SceneMenu404::ssMenuIn()
{
    // Update Camera
    //--------------------------------------------------------------------------------------
    director->composite->updateFinalCamDampen();

    director->renderVS->beginPanelTexture(int(deltaTime * 20));
    director->renderVS->drawWarningSign();
    director->renderVS->endPanelTexture();

    // Draw VS
    //--------------------------------------------------------------------------------------
    director->renderVS->beginRT();
    director->renderVS->drawUIPanel();
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
    director->composite->drawCursor();
    director->composite->endRT();

    // Draw Composite
    //--------------------------------------------------------------------------------------
    director->composite->beginDrawing();
    director->composite->drawComposite();
    director->composite->endDrawing();

    if (deltaTime >= 0.5f)
    {
        currentState = SS_MENU;
        deltaTime = 0.0f;
    }
}

void SceneMenu404::ssMenu()
{
    // Update Camera
    //--------------------------------------------------------------------------------------
    director->composite->updateFinalCamDampen();

    // Panel Update
    director->renderVS->beginPanelTexture(10);
    director->renderVS->drawWarningSign();
    director->renderVS->endPanelTexture();
    lastHoverPlayId = currentHoverPlayId;

    // Draw VS
    //--------------------------------------------------------------------------------------
    director->renderVS->beginRT();
    director->renderVS->drawUIPanel();
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
    director->composite->drawCursor();
    director->composite->endRT();

    // Draw Composite
    //--------------------------------------------------------------------------------------
    director->composite->beginDrawing();
    director->composite->drawComposite();
    director->composite->endDrawing();

    if (deltaTime >= 1.5f)
    {
        currentState = SS_MENU_OUT;
        deltaTime = 0.0f;
    }
}

void SceneMenu404::ssMenuOut()
{
    // Update Camera
    //--------------------------------------------------------------------------------------
    director->composite->updateBaseCamPosition(
        Math::vector3Lerp(menuCamera.position, initCamera.position, Math::easeInOutExpo(deltaTime / 4.0f)));
    director->composite->updateBaseCamTarget(
        Math::vector3Lerp(menuCamera.target, initCamera.target, Math::easeInOutExpo(deltaTime / 4.0f)));
    director->composite->updateBaseCamFov(
        Math::floatLerp(menuCamera.fovy, initCamera.fovy, Math::easeInOutExpo(deltaTime / 4.0f)));
    director->composite->updateFinalCam();

    // Panel Update
    director->renderVS->beginPanelTexture(10);
    director->renderVS->drawWarningSign();
    director->renderVS->endPanelTexture();
    lastHoverPlayId = currentHoverPlayId;

    // Draw VS
    //--------------------------------------------------------------------------------------
    director->renderVS->beginRT();
    director->renderVS->drawUIPanel();
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
}