#include "scene_req_signup.h"
#include "../mz_launcher.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneReqSignup::SceneReqSignup(MZLauncher *launcher) : MZScene(launcher, SceneType::ST_REQUEST_SIGNUP)
{
    currentState = SS_START_REQUEST;
    nextScene = ST_MAIN;
    deltaTime = 0.0f;
}

SceneReqSignup::~SceneReqSignup()
{
}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneReqSignup::update()
{
    launcher->input->handleMouseInput();

    switch (currentState)
    {
    case SS_START_REQUEST:
        launcher->comm->startSignupRequest();
        currentState = SS_AWAIT_REQUEST;
        performMenuStart();
        break;
    case SS_AWAIT_REQUEST:
        performMenuAwait();
        break;
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

void SceneReqSignup::performMenuStart()
{
    launcher->render2D->beginPanelTexture(0.0f);
    launcher->render2D->endPanelTexture(0.0f);
    launcher->render2D->beginRenderTexture();
    launcher->render2D->endRenderTexture();
    launcher->render2D->drawLauncher(1.0f);
}

void SceneReqSignup::performMenuAwait()
{
    launcher->render2D->beginPanelTexture(0.0f);
    launcher->render2D->endPanelTexture(0.0f);
    launcher->render2D->beginRenderTexture();
    launcher->render2D->drawLoading(deltaTime);
    launcher->render2D->endRenderTexture();
    launcher->render2D->drawLauncher(1.0f);

    if (launcher->comm->isReadySignupRequest())
    {
        deltaTime = 0.0f;
        responseText = launcher->comm->getSignupResponse();
        currentState = SS_MENU_IN;
        launcher->audio->playErrorSound();
    }
    deltaTime += GetFrameTime();
}

void SceneReqSignup::performMenuIn()
{
    launcher->render2D->beginPanelTexture(deltaTime * 4);
    launcher->render2D->drawButton(btn_back, BS_DISABLED);
    launcher->render2D->drawTitledFrame(frame_signup);
    launcher->render2D->drawTextRect(responseText.c_str(), Rectangle{frame_signup.rect.x + 8, frame_signup.rect.y + 16, frame_signup.rect.width - 16, frame_signup.rect.height - 32}, WHITE);
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

void SceneReqSignup::performMenu()
{
    launcher->render2D->beginPanelTexture(1.0f);

    currentHoverBtnId = -1;

    // HANDLE BUTTONS
    if (launcher->input->checkMouseOver(btn_back.rect))
    {
        currentHoverBtnId = 1;

        if (launcher->input->getMouseButtonDown() == 1)
        {
            launcher->audio->playAcceptSound();
            currentState = SS_MENU_OUT;
            deltaTime = 0.0f;
            launcher->render2D->drawButton(btn_back, BS_CLICK);
        }
        else
        {
            launcher->render2D->drawButton(btn_back, BS_HOVER);
            if (lastHoverBtnId != currentHoverBtnId)
                launcher->audio->playHoverSound();
        }
    }
    else
        launcher->render2D->drawButton(btn_back, BS_ENABLED);

    lastHoverBtnId = currentHoverBtnId;

    launcher->render2D->drawTitledFrame(frame_signup);
    launcher->render2D->drawTextRect(responseText.c_str(), Rectangle{frame_signup.rect.x + 8, frame_signup.rect.y + 16, frame_signup.rect.width - 16, frame_signup.rect.height - 32}, WHITE);
    launcher->render2D->endPanelTexture(1.0f);
    launcher->render2D->beginRenderTexture();
    launcher->render2D->endRenderTexture();
    launcher->render2D->drawLauncher(1.0f);
}

void SceneReqSignup::performMenuOut()
{
    launcher->render2D->beginPanelTexture(1.0f - (deltaTime * 4));
    launcher->render2D->drawButton(btn_back, BS_DISABLED);
    launcher->render2D->drawTitledFrame(frame_signup);
    launcher->render2D->drawTextRect(responseText.c_str(), Rectangle{frame_signup.rect.x + 8, frame_signup.rect.y + 16, frame_signup.rect.width - 16, frame_signup.rect.height - 32}, WHITE);
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
