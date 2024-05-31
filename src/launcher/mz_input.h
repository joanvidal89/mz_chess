#ifndef MZ_INPUT
#define MZ_INPUT

#include "raylib.h"
#include "raymath.h"
#include "mz_struct.h"

class MZInput
{
public:
    MZInput();
    ~MZInput();

    void initialize(Vector2 windowPosition);

    void handleMouseInput();

    bool checkMouseOver(Rectangle rect);
    bool isKeyValid(int key);

private:
    Vector2 mousePos = {0};
    Vector2 vMousePos = {0};
    Vector2 windowPosition;
    Vector2 panOffset;

    bool dragWindow;
};

#endif