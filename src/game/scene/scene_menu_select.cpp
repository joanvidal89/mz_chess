#include "scene_menu_select.h"

#include "../mz_director.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneMenuSelect::SceneMenuSelect(MZDirector *director) : MZScene(director, SceneType::ST_MENU_SELECT)
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

SceneMenuSelect::~SceneMenuSelect() {}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneMenuSelect::update()
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

void SceneMenuSelect::ssMenuIn()
{
    director->renderVS->beginPanelTexture(int(deltaTime * 20));
    for (UIOpponent opp : opponentButtons)
    {
        director->renderVS->drawOpponent(opp, BS_ENABLED);
    }
    director->renderVS->drawHeaderText(TITLE_LOCATION, MENU_TITLE, 14);
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

void SceneMenuSelect::ssMenu()
{
    // Panel Update
    currentHoverOppId = -1;
    director->input->raycastTopScreen(director->composite->getFinalCamera());

    director->renderVS->beginPanelTexture(10);
    for (UIOpponent opp : opponentButtons)
    {
        if (director->input->isVirtualCursorOnRect(opp.collision))
        {
            director->renderVS->drawHeaderText(OPPTX_LOCATION, opp.name, 28);

            currentHoverOppId = opp.id;
            if (director->input->getMouseButtonDown() == 1)
            {
                performOppButtonAction(opp);
                director->renderVS->drawOpponent(opp, BS_CLICK);
            }
            else
            {
                if (lastHoverOppId != currentHoverOppId)
                    director->audio->playHoverSound();

                director->renderVS->drawOpponent(opp, BS_HOVER);
            }
        }
        else
        {
            director->renderVS->drawOpponent(opp, BS_ENABLED);
        }
    }
    director->renderVS->drawHeaderText(TITLE_LOCATION, MENU_TITLE, 14);
    director->renderVS->endPanelTexture();
    lastHoverOppId = currentHoverOppId;

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

void SceneMenuSelect::ssMenuOut()
{
    director->renderVS->beginPanelTexture(10 - int(deltaTime * 20));
    for (UIOpponent opp : opponentButtons)
    {
        director->renderVS->drawOpponent(opp, BS_ENABLED);
    }
    director->renderVS->drawHeaderText(TITLE_LOCATION, MENU_TITLE, 14);
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

void SceneMenuSelect::performOppButtonAction(UIOpponent opp)
{
    director->audio->playAcceptSound();
    director->opponentId = opp.id;
    futureScene = ST_MENU_OPPONENT;
    currentState = SS_MENU_OUT;
    deltaTime = 0.0f;
}

void SceneMenuSelect::performOptionButtonAction(UIButton opt)
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