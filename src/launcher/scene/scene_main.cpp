#include "scene_main.h"
#include "../mz_launcher.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneMain::SceneMain(MZLauncher *launcher) : MZScene(launcher, SceneType::ST_MAIN)
{
    currentState = SS_MENU_IN;
    nextScene = ST_MAIN;
    deltaTime = 0.0f;
}

SceneMain::~SceneMain()
{
}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneMain::update()
{
    launcher->input->handleMouseInput();

    switch (currentState)
    {
    case SS_MENU_IN:
        performMenuIn();
        break;
    case SS_MENU:
        performMenu();
        break;
    case SS_MENU_OUT:
        performMenuOut();
    default:
        break;
    }

    deltaTime += GetFrameTime();
}

void SceneMain::performMenuIn()
{
    launcher->render2D->beginPanelTexture(deltaTime * 4);
    for (UIElement btn : sceneButtons)
    {
        launcher->render2D->drawButton(btn, BS_DISABLED);
    }
    launcher->render2D->endPanelTexture(deltaTime * 4);
    launcher->render2D->beginRenderTexture();
    launcher->render2D->endRenderTexture();
    launcher->render2D->drawLauncher(1.0f);
    if (deltaTime >= 0.25f)
    {
        deltaTime = 0.0f;
        currentState = SS_MENU;
    }
}

void SceneMain::performMenu()
{
    launcher->render2D->beginPanelTexture(1.0f);
    currentHoverBtnId = -1;
    for (UIElement btn : sceneButtons)
    {
        if (launcher->input->checkMouseOver(btn.rect))
        {
            currentHoverBtnId = btn.id;
            if (IsMouseButtonPressed(0))
            {
                performButtonAction(btn.id);
                launcher->render2D->drawButton(btn, BS_CLICK);
            }
            else
            {
                if (lastHoverBtnId != currentHoverBtnId)
                    launcher->audio->playHoverSound();

                launcher->render2D->drawButton(btn, BS_HOVER);
            }
        }
        else
        {
            launcher->render2D->drawButton(btn, BS_ENABLED);
        }
    }
    lastHoverBtnId = currentHoverBtnId;

    launcher->render2D->endPanelTexture(1.0f);
    launcher->render2D->beginRenderTexture();
    launcher->render2D->endRenderTexture();
    launcher->render2D->drawLauncher(1.0f);
}

void SceneMain::performMenuOut()
{
    launcher->render2D->beginPanelTexture(1.0f - (deltaTime * 4));
    for (UIElement btn : sceneButtons)
    {
        launcher->render2D->drawButton(btn, BS_DISABLED);
    }
    launcher->render2D->endPanelTexture(1.0f - (deltaTime * 4));
    launcher->render2D->beginRenderTexture();
    launcher->render2D->endRenderTexture();
    launcher->render2D->drawLauncher(1.0f);
    if (deltaTime >= 0.25f)
    {
        deltaTime = 0.0f;
        launcher->newScene = nextScene;
    }
}

void SceneMain::performButtonAction(int id)
{
    switch (id)
    {
    case 0:
        launcher->audio->playAcceptSound();
        currentState = SS_MENU_OUT;
        nextScene = ST_LOGIN;
        deltaTime = 0.0f;
        break;
    case 1:
        launcher->audio->playAcceptSound();
        currentState = SS_MENU_OUT;
        nextScene = ST_REGISTER;
        deltaTime = 0.0f;
        break;
        break;
    case 2:
        launcher->audio->playCancelSound();
        currentState = SS_MENU_OUT;
        nextScene = ST_OUTRO;
        deltaTime = 0.0f;
        break;
    default:
        break;
    }
}
