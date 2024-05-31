#include "scene_req_login.h"
#include "../mz_launcher.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneReqLogin::SceneReqLogin(MZLauncher *launcher) : MZScene(launcher, SceneType::ST_REQUEST_LOGIN)
{
    currentState = SS_START_REQUEST;
    nextScene = ST_MAIN;
    deltaTime = 0.0f;
}

SceneReqLogin::~SceneReqLogin() {}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneReqLogin::update()
{
    launcher->input->handleMouseInput();

    switch (currentState)
    {
    case SS_START_REQUEST:
        launcher->comm->startLoginRequest();
        currentState = SS_AWAIT_REQUEST;
        performMenuAwait();
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

void SceneReqLogin::performMenuAwait()
{
    launcher->render2D->beginPanelTexture(0.0f);
    launcher->render2D->endPanelTexture(0.0f);
    launcher->render2D->beginRenderTexture();
    launcher->render2D->drawLoading(deltaTime);
    launcher->render2D->endRenderTexture();
    launcher->render2D->drawLauncher(1.0f);

    if (launcher->comm->isReadyLoginRequest())
    {
        deltaTime = 0.0f;
        responseText = launcher->comm->getLoginResponse();

        size_t pos1 = responseText.find(':');
        if (pos1 == std::string::npos)
        {
            currentState = SS_MENU_IN;
        }
        else
        {
            launcher->audio->playErrorSound();
            std::string id = responseText.substr(pos1 + 1);
            launcher->accountId = std::stoi(id);
            launcher->loginSuccessful = true;
            launcher->newScene = ST_OUTRO;
        }
    }

    deltaTime += GetFrameTime();
}

void SceneReqLogin::performMenuIn()
{
    launcher->render2D->beginPanelTexture(deltaTime * 4);
    launcher->render2D->drawButton(btn_back, BS_DISABLED);
    launcher->render2D->drawTitledFrame(frame_login);
    launcher->render2D->drawTextRect(responseText.c_str(), Rectangle{frame_login.rect.x + 8, frame_login.rect.y + 16, frame_login.rect.width - 16, frame_login.rect.height - 32}, WHITE);
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

void SceneReqLogin::performMenu()
{
    launcher->render2D->beginPanelTexture(1.0f);

    currentHoverBtnId = -1;

    // HANDLE BUTTONS
    if (launcher->input->checkMouseOver(btn_back.rect))
    {
        currentHoverBtnId = 1;

        if (IsMouseButtonPressed(0))
        {
            launcher->audio->playAcceptSound();
            currentState = SS_MENU_OUT;
            nextScene = ST_LOGIN;
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

    launcher->render2D->drawTitledFrame(frame_login);
    launcher->render2D->drawTextRect(responseText.c_str(), Rectangle{frame_login.rect.x + 8, frame_login.rect.y + 16, frame_login.rect.width - 16, frame_login.rect.height - 32}, WHITE);
    launcher->render2D->endPanelTexture(1.0f);
    launcher->render2D->beginRenderTexture();
    launcher->render2D->endRenderTexture();
    launcher->render2D->drawLauncher(1.0f);
}

void SceneReqLogin::performMenuOut()
{
    launcher->render2D->beginPanelTexture(1.0f - (deltaTime * 4));
    launcher->render2D->drawButton(btn_back, BS_DISABLED);
    launcher->render2D->drawTitledFrame(frame_login);
    launcher->render2D->drawTextRect(responseText.c_str(), Rectangle{frame_login.rect.x + 8, frame_login.rect.y + 16, frame_login.rect.width - 16, frame_login.rect.height - 32}, WHITE);
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
