#include "scene_register.h"
#include "../mz_launcher.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneRegister::SceneRegister(MZLauncher *launcher) : MZScene(launcher, SceneType::ST_REGISTER)
{
    currentState = SS_MENU_IN;
    nextScene = ST_MAIN;
    deltaTime = 0.0f;
    currentSelectInput = 0;
}

SceneRegister::~SceneRegister()
{
}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneRegister::update()
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

void SceneRegister::performMenuIn()
{
    launcher->render2D->beginPanelTexture(deltaTime * 4);
    for (UIElement btn : sceneButtons)
    {
        launcher->render2D->drawButton(btn, BS_DISABLED);
    }
    for (UIElement input : sceneInputs)
    {
        launcher->render2D->drawInputFrame(input, IS_ENABLED);
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

void SceneRegister::performMenu()
{
    launcher->render2D->beginPanelTexture(1.0f);

    // HANDLE KEYBOARD INPUT
    if (currentSelectInput == 0)
    {
        int key = GetCharPressed();
        int key2 = GetKeyPressed();

        if (launcher->input->isKeyValid(key) && (username_letterCount < username_max_input))
        {
            launcher->audio->playInputSound();
            username_text[username_letterCount] = (char)key;
            username_text[username_letterCount + 1] = '\0';
            username_letterCount++;
        }
        if (key2 == KEY_BACKSPACE)
        {
            launcher->audio->playInputSound();
            username_letterCount--;
            if (username_letterCount < 0)
                username_letterCount = 0;
            username_text[username_letterCount] = '\0';
        }
        if (key2 == KEY_TAB)
        {
            launcher->audio->playHoverSound();
            currentSelectInput++;
        }
    }
    else if (currentSelectInput == 1)
    {
        int key = GetCharPressed();
        int key2 = GetKeyPressed();

        if (launcher->input->isKeyValid(key) && (password_letterCount < password_max_input))
        {
            launcher->audio->playInputSound();
            password_text[password_letterCount] = (char)key;
            password_text[password_letterCount + 1] = '\0';
            password_letterCount++;
        }
        if (key2 == KEY_BACKSPACE)
        {
            launcher->audio->playInputSound();
            password_letterCount--;
            if (password_letterCount < 0)
                password_letterCount = 0;
            password_text[password_letterCount] = '\0';
        }
        if (key2 == KEY_TAB)
        {
            launcher->audio->playHoverSound();
            currentSelectInput++;
        }
    }
    else if (currentSelectInput == 2)
    {
        int key = GetCharPressed();
        int key2 = GetKeyPressed();

        if ((key == 32 || launcher->input->isKeyValid(key)) && (hint_letterCount < hint_max_input))
        {
            if (key == 32 && hint_letterCount != 0)
            {
                hint_text[hint_letterCount] = (char)key;
                hint_text[hint_letterCount + 1] = '\0';
                hint_letterCount++;
            }
            launcher->audio->playInputSound();
            hint_text[hint_letterCount] = (char)key;
            hint_text[hint_letterCount + 1] = '\0';
            hint_letterCount++;
        }
        if (key2 == KEY_BACKSPACE)
        {
            hint_letterCount--;
            launcher->audio->playInputSound();
            if (hint_letterCount < 0)
                hint_letterCount = 0;
            hint_text[hint_letterCount] = '\0';
        }
        if (key2 == KEY_TAB)
        {
            launcher->audio->playHoverSound();
            currentSelectInput = 0;
        }
    }

    currentHoverBtnId = -1;
    // HANDLE BUTTONS
    for (UIElement btn : sceneButtons)
    {
        if ((btn.id == 0 && (username_letterCount == 0 || password_letterCount == 0 || hint_letterCount == 0)))
            launcher->render2D->drawButton(btn, BS_DISABLED);
        else
        {
            if (launcher->input->checkMouseOver(btn.rect))
            {
                currentHoverBtnId = btn.id;
                if (launcher->input->getMouseButtonDown() == 1)
                {
                    performButtonAction(btn.id);
                    launcher->render2D->drawButton(btn, BS_CLICK);
                }
                else
                {
                    launcher->render2D->drawButton(btn, BS_HOVER);
                    if (lastHoverBtnId != currentHoverBtnId)
                        launcher->audio->playHoverSound();
                }
            }
            else
                launcher->render2D->drawButton(btn, BS_ENABLED);
        }
    }
    // HANDLE INPUTS
    for (UIElement input : sceneInputs)
    {
        if (input.id == currentSelectInput)
            launcher->render2D->drawInputFrame(input, IS_SELECTED);
        else if (launcher->input->checkMouseOver(input.rect))
        {
            currentHoverBtnId = input.id;

            if (launcher->input->getMouseButtonDown() == 1)
            {
                launcher->audio->playAcceptSound();
                currentSelectInput = input.id;
                launcher->render2D->drawInputFrame(input, IS_SELECTED);
            }
            else
            {
                launcher->render2D->drawInputFrame(input, IS_HOVER);
                if (lastHoverBtnId != currentHoverBtnId)
                    launcher->audio->playHoverSound();
            }
            launcher->render2D->drawInputFrame(input, IS_HOVER);
        }
        else
            launcher->render2D->drawInputFrame(input, IS_ENABLED);
    }
    lastHoverBtnId = currentHoverBtnId;

    if (currentSelectInput == 0 && ((int)(deltaTime * 2) % 2 == 0))
        launcher->render2D->drawInputText(inp_username, username_text, false, true, IS_ENABLED);
    else
        launcher->render2D->drawInputText(inp_username, username_text, false, false, IS_ENABLED);

    if (currentSelectInput == 1 && ((int)(deltaTime * 2) % 2 == 0))
        launcher->render2D->drawInputText(inp_password, password_text, true, true, IS_ENABLED);
    else
        launcher->render2D->drawInputText(inp_password, password_text, true, false, IS_ENABLED);

    int hint_text_length = strlen(hint_text);
    char *hint_text_cropped = hint_text + (hint_text_length > 22 ? hint_text_length - 22 : 0);
    if (currentSelectInput == 2 && ((int)(deltaTime * 2) % 2 == 0))
        launcher->render2D->drawInputText(inp_hint, hint_text_cropped, false, true, IS_ENABLED);
    else
        launcher->render2D->drawInputText(inp_hint, hint_text_cropped, false, false, IS_ENABLED);

    launcher->render2D->endPanelTexture(1.0f);
    launcher->render2D->beginRenderTexture();
    launcher->render2D->endRenderTexture();
    launcher->render2D->drawLauncher(1.0f);
}

void SceneRegister::performMenuOut()
{
    launcher->render2D->beginPanelTexture(1.0f - (deltaTime * 4));
    for (UIElement btn : sceneButtons)
    {
        launcher->render2D->drawButton(btn, BS_DISABLED);
    }
    for (UIElement input : sceneInputs)
    {
        launcher->render2D->drawInputFrame(input, IS_ENABLED);
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

void SceneRegister::performButtonAction(int id)
{
    switch (id)
    {
    case 0:
        launcher->audio->playAcceptSound();
        launcher->comm->currentUsername = username_text;
        launcher->comm->currentPassword = password_text;
        launcher->comm->currentHint = hint_text;
        currentState = SS_MENU_OUT;
        nextScene = ST_REQUEST_SIGNUP;
        deltaTime = 0.0f;
        break;
    case 1:
        launcher->audio->playCancelSound();
        currentState = SS_MENU_OUT;
        nextScene = ST_MAIN;
        deltaTime = 0.0f;
        break;
    default:
        break;
    }
}
