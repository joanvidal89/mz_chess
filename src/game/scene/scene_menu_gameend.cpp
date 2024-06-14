#include "scene_menu_gameend.h"

#include "../mz_director.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneMenuGameEnd::SceneMenuGameEnd(MZDirector *director) : MZScene(director, SceneType::ST_MENU_GAME_END)
{
    deltaTime = 0.0f;
    currentState = SS_REQUEST_HTTP;
    futureScene = ST_MENU_SELECT;
    lastHoverBtnId = -1;
    currentHoverBtnId = -1;
    opponentName = OPP_NAMES[director->opponentId];
    opponentFrame = OPP_FRAMES[director->opponentId];
    score = std::to_string(director->board->calculateScore());
    opponentId = std::to_string(director->opponentId);
    switch (director->board->getGameResult())
    {
    case GS_LOSS:
        gameResult = "YOU LOST";
        break;
    case GS_WIN:
        gameResult = "YOU WON!";
        break;
    case GS_DRAW:
        gameResult = "DRAW";
        break;
    default:
        break;
    }

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

SceneMenuGameEnd::~SceneMenuGameEnd() {}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneMenuGameEnd::update()
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
        director->comm->levelId = opponentId;
        director->comm->score = score;
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

void SceneMenuGameEnd::ssMenuAwait()
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
            // WE REMOVE THE "DONE" PART OF THE CLIENT RESPONSE.
            response.erase(response.size() - 6, 6);
            rank = response;
            currentState = SS_MENU_IN;
        }
    }

    deltaTime += GetFrameTime();
}

void SceneMenuGameEnd::ssMenuIn()
{
    director->renderVS->beginPanelTexture(int(deltaTime * 20));
    director->renderVS->drawOpponent(opponentFrame, BS_ENABLED);
    director->renderVS->drawHeaderText(TITLE_LOCATION, opponentName, 28);
    director->renderVS->drawHeaderText(RESULT_LOCATION, gameResult.c_str(), 28);
    director->renderVS->drawMonoText(SCORE_TEXT_LOCATION, SCORE_TEXT, 12, 1.0f);
    director->renderVS->drawMonoText(RANK_TEXT_LOCATION, RANK_TEXT, 12, 1.0f);
    director->renderVS->drawMonoText(SCORE_LOCATION, score.c_str(), 24, 1.0f);
    director->renderVS->drawMonoText(RANK_LOCATION, rank.c_str(), 24, 1.0f);
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

void SceneMenuGameEnd::ssMenu()
{
    // Panel Update
    currentHoverPlayId = -1;
    director->input->raycastTopScreen(director->composite->getFinalCamera());

    director->renderVS->beginPanelTexture(10);
    if (director->input->isVirtualCursorOnRect(BTN_BACK.collision))
    {
        currentHoverPlayId = BTN_BACK.id;

        if (director->input->getMouseButtonDown() == 1)
        {
            performBackButtonAction();
            director->renderVS->drawButtonWithText(BTN_BACK, BS_CLICK);
        }
        else
        {
            if (currentHoverPlayId != currentHoverPlayId)
                director->audio->playHoverSound();

            director->renderVS->drawButtonWithText(BTN_BACK, BS_HOVER);
        }
    }
    else
    {
        director->renderVS->drawButtonWithText(BTN_BACK, BS_ENABLED);
    }
    director->renderVS->drawOpponent(opponentFrame, BS_ENABLED);
    director->renderVS->drawHeaderText(TITLE_LOCATION, opponentName, 28);
    director->renderVS->drawHeaderText(RESULT_LOCATION, gameResult.c_str(), 28);
    director->renderVS->drawMonoText(SCORE_TEXT_LOCATION, SCORE_TEXT, 12, 1.0f);
    director->renderVS->drawMonoText(RANK_TEXT_LOCATION, RANK_TEXT, 12, 1.0f);
    director->renderVS->drawMonoText(SCORE_LOCATION, score.c_str(), 24, 1.0f);
    director->renderVS->drawMonoText(RANK_LOCATION, rank.c_str(), 24, 1.0f);
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
            if (director->input->getMouseButtonDown() == 1)
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

void SceneMenuGameEnd::ssMenuOut()
{
    director->renderVS->beginPanelTexture(10 - int(deltaTime * 20));
    director->renderVS->drawOpponent(opponentFrame, BS_ENABLED);
    director->renderVS->drawHeaderText(TITLE_LOCATION, opponentName, 28);
    director->renderVS->drawHeaderText(RESULT_LOCATION, gameResult.c_str(), 28);
    director->renderVS->drawMonoText(SCORE_TEXT_LOCATION, SCORE_TEXT, 12, 1.0f);
    director->renderVS->drawMonoText(RANK_TEXT_LOCATION, RANK_TEXT, 12, 1.0f);
    director->renderVS->drawMonoText(SCORE_LOCATION, score.c_str(), 24, 1.0f);
    director->renderVS->drawMonoText(RANK_LOCATION, rank.c_str(), 24, 1.0f);
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
        director->newScene = futureScene;
        deltaTime = 0.0f;
    }
}

void SceneMenuGameEnd::performOptionButtonAction(UIButton opt)
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
        director->audio->playCancelSound();
        futureScene = ST_EXIT;
        currentState = SS_MENU_OUT;
        deltaTime = 0.0f;
        break;
    default:
        break;
    }
}

void SceneMenuGameEnd::performBackButtonAction()
{
    director->audio->playAcceptSound();
    futureScene = ST_MENU_SELECT;
    currentState = SS_MENU_OUT;
    deltaTime = 0.0f;
}