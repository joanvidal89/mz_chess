#include "mz_input.h"

MZInput::MZInput()
{
    windowPosition = {0};
    dragWindow = false;
    panOffset = {0};
}

MZInput::~MZInput()
{
}

void MZInput::initialize(Vector2 windowPosition)
{
    this->windowPosition = windowPosition;
}

void MZInput::handleMouseInput()
{
    mousePos = GetMousePosition();
    vMousePos.x = (mousePos.x - (GetScreenWidth() - (BASE_WIDTH * SCALE)) * 0.5f) / SCALE;
    vMousePos.y = (mousePos.y - (GetScreenHeight() - (BASE_HEIGHT * SCALE)) * 0.5f) / SCALE;
    vMousePos = Vector2Clamp(vMousePos, Vector2{0, 0}, Vector2{(float)SCREEN_WIDTH, (float)SCREEN_HEIGHT});

    // CHECK DRAG WINDOW
    //------------------------------------------------------------------------------------
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !dragWindow)
    {
        if (CheckCollisionPointRec(vMousePos, Rectangle{0, 16, 416, 30}))
        {
            dragWindow = true;
            panOffset = mousePos;
        }
    }

    // PERFORM DRAG WINDOW
    //------------------------------------------------------------------------------------
    if (dragWindow)
    {
        windowPosition.x += (mousePos.x - panOffset.x);
        windowPosition.y += (mousePos.y - panOffset.y);

        SetWindowPosition((int)windowPosition.x, (int)windowPosition.y);

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            dragWindow = false;
    }
}

bool MZInput::checkMouseOver(Rectangle rect)
{
    if (CheckCollisionPointRec(vMousePos, Rectangle{rect.x + PANEL_OFFSET.x, rect.y + PANEL_OFFSET.y, rect.width, rect.height}))
        return true;

    return false;
}

bool MZInput::isKeyValid(int key)
{ 
    // allow numbers
    if ((key >= 48) && (key <= 57))
        return true;

    // allow uppercase
    if ((key >= 65) && (key <= 90))
        return true;

    // allow lowercase
    if ((key >= 97) && (key <= 122))
        return true;

    return false;
}

int MZInput::getMouseButtonDown()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return 1;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        return 2;
    }
    return 0;
}