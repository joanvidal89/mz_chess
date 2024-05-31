#include "scene_menu_opponent.h"

#include "../mz_director.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneMenuOpponent::SceneMenuOpponent(MZDirector *director) : MZScene(director, SceneType::ST_MENU_OPPONENT)
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

SceneMenuOpponent::~SceneMenuOpponent() {}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneMenuOpponent::update()
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
        director->comm->startRequestGET();
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

void SceneMenuOpponent::ssMenuAwait()
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
    if (director->comm->isReadyGET())
    {
        deltaTime = 0.0f;

        std::string response = director->comm->getAnswerGET();

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

void SceneMenuOpponent::ssMenuIn()
{
    director->renderVS->beginPanelTexture(int(deltaTime * 20));
    for (UIButton play : PLAY_BTNS)
    {
        director->renderVS->drawButtonWithText(play, BS_DISABLED);
    }
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

void SceneMenuOpponent::ssMenu()
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
                if (lastHoverPlayId != currentHoverPlayId)
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

void SceneMenuOpponent::ssMenuOut()
{
    director->renderVS->beginPanelTexture(10 - int(deltaTime * 20));
    for (UIButton play : PLAY_BTNS)
    {
        director->renderVS->drawButtonWithText(play, BS_DISABLED);
    }
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

void SceneMenuOpponent::performOppButtonAction(UIOpponent opp)
{
    director->opponentId = opp.id;
    futureScene = ST_MENU_SELECT;
    currentState = SS_MENU_OUT;
    deltaTime = 0.0f;
}

void SceneMenuOpponent::performOptionButtonAction(UIButton opt)
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

void SceneMenuOpponent::performPlayButtonAction(UIButton btn)
{
    switch (btn.id)
    {
    case 1:
        director->audio->playAcceptSound();
        director->board->initializeBoard();
        futureScene = ST_GAME;
        currentState = SS_MENU_OUT;
        deltaTime = 0.0f;
        break;
    case 2:
        director->audio->playAcceptSound();

        break;
    case 3:
        director->audio->playCancelSound();
        futureScene = ST_MENU_SELECT;
        currentState = SS_MENU_OUT;
        deltaTime = 0.0f;
        break;
    }
}

std::string SceneMenuOpponent::formatResponse(const std::string &response)
{
    std::stringstream formattedResponse;
    std::istringstream iss(response);
    std::string line;

    int count = 1;
    while (std::getline(iss, line))
    {
        // Remove any trailing carriage return characters
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }
        
        std::istringstream lineStream(line);
        std::string name, score;
        std::getline(lineStream, name, ';');
        std::getline(lineStream, score);

        std::string formattedName = name;
        formattedName.append(12 - name.length(), '.');
        std::string formattedScore = score;
        formattedScore.append(5 - score.length(), '0');

        // Construct the formatted line
        formattedResponse << count << "." << formattedName << score << "\n";
        count++;
    }

    return formattedResponse.str();
}