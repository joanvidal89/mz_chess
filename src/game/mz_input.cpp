#include "mz_input.h"

MZInput::MZInput()
{
    ray = {0};
    rayCollision = {0};
    virtualMousePos = Vector2{0.0f, 0.0f};
}

MZInput::~MZInput() {}

RayCollision MZInput::getRayCollision() { return rayCollision; }

Vector2 MZInput::getVirtualMousePos() { return virtualMousePos; }

void MZInput::raycastTopScreen(const Camera &camera)
{
    rayCollision = {0};

    ray = GetMouseRay(GetMousePosition(), camera);
    rayCollision = GetRayCollisionQuad(ray, colTopScreen[0], colTopScreen[1], colTopScreen[2], colTopScreen[3]);

    if (rayCollision.hit)
        virtualMousePos = Vector2{rayCollision.point.x * 40, 412 - rayCollision.point.y * 41};
    else
        virtualMousePos = Vector2{-1, -1};
}

void MZInput::raycastBotScreen(const Camera &camera)
{
    rayCollision = {0};

    ray = GetMouseRay(GetMousePosition(), camera);
    rayCollision = GetRayCollisionQuad(ray, colBotScreen[0], colBotScreen[1], colBotScreen[2], colBotScreen[3]);
    if (rayCollision.hit)
        virtualMousePos = Vector2{rayCollision.point.x * 40, 720 + rayCollision.point.z * 40};
    else
        virtualMousePos = Vector2{-1.0f, -1.0f};
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

bool MZInput::isVCOnBoard()
{
    if (CheckCollisionPointRec(virtualMousePos, BOARD_RECT))
        return true;

    return false;
}

bool MZInput::isVirtualCursorOnRect(const Rectangle &rect)
{
    if (CheckCollisionPointRec(virtualMousePos, rect))
        return true;

    return false;
}