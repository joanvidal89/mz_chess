#include "scene_menu_postgame.h"

#include "../mz_director.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneMenuPostgame::SceneMenuPostgame(MZDirector *director) : MZScene(director, SceneType::ST_MENU_POSTGAME)
{
    deltaTime = 0.0f;
    currentState = SS_REQUEST_HTTP;
    futureScene = ST_MENU_SELECT;
    lastHoverBtnId = -1;
    currentHoverBtnId = -1;
    MENU_TITLE = OPP_NAMES[director->opponentId];
    opponentFrame = OPP_FRAMES[director->opponentId];

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

SceneMenuPostgame::~SceneMenuPostgame() {}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneMenuPostgame::update()
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
    case SS_REQUEST_HTTP:
        director->comm->levelId = std::to_string(director->opponentId);
        director->comm->score = std::to_string(director->score);
        director->comm->startRequestPOST();
        currentState = SS_AWAIT_HTTP;
        ssMenuAwait();
        break;
    case SS_AWAIT_HTTP:
        ssMenuAwait();
        break;
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

void SceneMenuPostgame::ssMenuAwait()
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

    // Handle Request
    //--------------------------------------------------------------------------------------
    if (director->comm->isReadyPOST())
    {
        deltaTime = 0.0f;
        std::string response = director->comm->getAnswerPOST();

        if (response.find("ERROR") != std::string::npos)
        {
            director->newScene = ST_MENU_404;
        }
        else
        {
            response.erase(response.size() - 6, 6);
            highscoreText = formatResponse(response);
            currentState = SS_MENU_IN;
        }
    }

    deltaTime += GetFrameTime();
}

void SceneMenuPostgame::ssMenuIn()
{
    director->renderVS->beginPanelTexture(int(deltaTime * 20));
    director->renderVS->drawOpponent(opponentFrame, BS_ENABLED);
    director->renderVS->drawMonoText(HIGHS_LOCATION, highscoreText.c_str(), 12, 1.0f);
    director->renderVS->drawHeaderText(TITLE_LOCATION, MENU_TITLE, 28);
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

void SceneMenuPostgame::ssMenu()
{
    // Panel Update
    currentHoverPlayId = -1;
    director->input->raycastTopScreen(director->composite->getFinalCamera());

    director->renderVS->beginPanelTexture(10);
    for (UIButton play : PLAY_BTNS)
    {
        if (director->input->isVirtualCursorOnRect(play.collision))
        {
            currentHoverPlayId = play.id;
            if (IsMouseButtonPressed(0))
            {
                performPlayButtonAction(play);
                director->renderVS->drawButtonWithText(play, BS_CLICK);
            }
            else
            {
                if (lastHoverBtnId != currentHoverBtnId)
                    director->audio->playHoverSound();

                director->renderVS->drawButtonWithText(play, BS_HOVER);
            }
        }
        else
        {
            director->renderVS->drawButtonWithText(play, BS_ENABLED);
        }
    }
    director->renderVS->drawOpponent(opponentFrame, BS_ENABLED);
    director->renderVS->drawMonoText(HIGHS_LOCATION, highscoreText.c_str(), 12, 1.0f);
    director->renderVS->drawHeaderText(TITLE_LOCATION, MENU_TITLE, 28);
    director->renderVS->endPanelTexture();
    lastHoverPlayId = currentHoverPlayId;

    // Draw VS
    //--------------------------------------------------------------------------------------
    director->renderVS->beginRT();
    director->renderVS->drawUIPanel();
    currentHoverBtnId = -1;
    for (UIButton opt : optionButtons)
    {
        if (director->input->isVirtualCursorOnRect(opt.collision))
        {
            currentHoverBtnId = opt.id;
            if (IsMouseButtonPressed(0))
            {
                performOptionButtonAction(opt);
                director->renderVS->drawOption(opt, BS_CLICK);
            }
            else
            {
                if (lastHoverBtnId != currentHoverBtnId)
                    director->audio->playHoverSound();

                director->renderVS->drawOption(opt, BS_HOVER);
            }
        }
        else
        {
            director->renderVS->drawOption(opt, BS_ENABLED);
        }
    }
    lastHoverBtnId = currentHoverBtnId;
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
}

void SceneMenuPostgame::ssMenuOut()
{
    director->renderVS->beginPanelTexture(10 - int(deltaTime * 20));
    director->renderVS->drawOpponent(opponentFrame, BS_ENABLED);
    director->renderVS->drawMonoText(HIGHS_LOCATION, highscoreText.c_str(), 12, 1.0f);
    director->renderVS->drawHeaderText(TITLE_LOCATION, MENU_TITLE, 28);
    director->renderVS->endPanelTexture();

    // Draw VS
    //--------------------------------------------------------------------------------------
    director->renderVS->beginRT();
    director->renderVS->drawUIPanel();
    director->renderVS->drawOpponent(opponentFrame, BS_ENABLED);
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
        director->newScene = futureScene;
        deltaTime = 0.0f;
    }
}

void SceneMenuPostgame::performOppButtonAction(UIOpponent opp)
{
    director->opponentId = opp.id;
    futureScene = ST_MENU_SELECT;
    currentState = SS_MENU_OUT;
    deltaTime = 0.0f;
}

void SceneMenuPostgame::performOptionButtonAction(UIButton opt)
{
    switch (opt.id)
    {
    case 1:
        optionButtons[0] = BTN_VID_OFF;
        director->composite->switchPostProcess(false);
        director->config->currentConfig.video = 0;
        break;
    case 2:
        optionButtons[0] = BTN_VID_ON;
        director->composite->switchPostProcess(true);
        director->config->currentConfig.video = 1;
        break;
    case 3:
        optionButtons[1] = BTN_SND_OFF;
        director->audio->switchAudioOn(false);
        director->config->currentConfig.audio = 0;
        break;
    case 4:
        optionButtons[1] = BTN_SND_ON;
        director->audio->switchAudioOn(true);
        director->config->currentConfig.audio = 1;
        break;
    case 5:
        optionButtons[2] = BTN_MUS_OFF;
        director->audio->switchMusicOn(false);
        director->config->currentConfig.music = 0;
        break;
    case 6:
        optionButtons[2] = BTN_MUS_ON;
        director->audio->switchMusicOn(true);
        director->config->currentConfig.music = 1;
        break;
    case 7:
        futureScene = ST_EXIT;
        currentState = SS_MENU_OUT;
        deltaTime = 0.0f;
        break;
    default:
        break;
    }
}

void SceneMenuPostgame::performPlayButtonAction(UIButton btn)
{
    switch (btn.id)
    {
    case 1:

        break;
    case 2:

        break;
    case 3:
        futureScene = ST_MENU_SELECT;
        currentState = SS_MENU_OUT;
        deltaTime = 0.0f;
        break;
    }
}

std::string SceneMenuPostgame::formatResponse(const std::string &response)
{
    std::string res = "YOUR SCORE IS\n" + std::to_string(director->score) + "\nYOUR RANKING IS\n" + response;
    return res;
}