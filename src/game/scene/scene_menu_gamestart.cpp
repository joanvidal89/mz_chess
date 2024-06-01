#include "scene_menu_select.h"

#include "../mz_director.h"
#include "scene_menu_gamestart.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneMenuGameStart::SceneMenuGameStart(MZDirector *director) : MZScene(director, SceneType::ST_MENU_GAME_START)
{
    deltaTime = 0.0f;
    currentState = SS_INIT;
    futureScene = ST_GAME;

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

SceneMenuGameStart::~SceneMenuGameStart() {}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneMenuGameStart::update()
{
    // Update Audio
    //--------------------------------------------------------------------------------------
    director->audio->updateAudio();

    // Update Camera
    //--------------------------------------------------------------------------------------
    director->composite->updateFinalCamDampen();

    // Scene Flow
    //--------------------------------------------------------------------------------------
    switch (currentState)
    {
    case SS_INIT:
        director->uciEngine->startSetLevel();
        currentState = SS_AWAIT_GAME_LOAD;
        ssAwaitGameLoad();
        break;
    case SS_AWAIT_GAME_LOAD:
        ssAwaitGameLoad();
        break;
    default:
        break;
    }

    deltaTime += GetFrameTime();
}

void SceneMenuGameStart::ssAwaitGameLoad()
{
    // Draw VS
    //--------------------------------------------------------------------------------------
    director->renderVS->beginRT();
    director->renderVS->drawUIPanel();
    for (UIButton opt : optionButtons)
    {
        director->renderVS->drawOption(opt, BS_DISABLED);
    }
    director->renderVS->drawLoading(deltaTime);
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

    deltaTime += GetFrameTime();

    if (director->uciEngine->isReadySetLevel())
    {
        if (director->uciEngine->getSetLevelResponse() == "ERROR")
        {
             director->newScene = ST_EXIT;
        }
        director->newScene = ST_GAME;
    }
}