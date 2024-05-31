#ifndef MZ_INPUT
#define MZ_INPUT

#include "mz_struct.h"
#include "raylib.h"

class MZInput
{
public:
    MZInput();
    ~MZInput();

    RayCollision getRayCollision();
    Vector2 getVirtualMousePos();

    void raycastTopScreen(const Camera &camera);
    void raycastBotScreen(const Camera &camera);

    int getMouseButtonDown();

    bool isVCOnBoard();
    bool isVirtualCursorOnRect(const Rectangle &rect);

private:
    Ray ray;
    RayCollision rayCollision;
    Vector2 virtualMousePos;

    const Vector3 colTopScreen[4] = {
        Vector3{0.0f, 1.0f, -9.7f},
        Vector3{0.0f, 9.85f, -11.25f},
        Vector3{16.0f, 9.85f, -11.25f},
        Vector3{16.0f, 1.0f, -9.7f},
    };

    const Vector3 colBotScreen[4] = {
        Vector3{0.0f, 0.0f, 0.0f},
        Vector3{0.0f, 0.0f, -9.0f},
        Vector3{16.0f, 0.0f, -9.0f},
        Vector3{16.0f, 0.0f, 0.0f},
    };
};

#endif