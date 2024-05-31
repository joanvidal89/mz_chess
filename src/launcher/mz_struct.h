#ifndef MZ_STRUCT
#define MZ_STRUCT

#include "raylib.h"
#include <string>

const int BASE_WIDTH = 416;
const int BASE_HEIGHT = 224;
const int SCALE = 3;
const int SCREEN_WIDTH = BASE_WIDTH * SCALE;
const int SCREEN_HEIGHT = BASE_HEIGHT * SCALE;

const Vector2 PANEL_OFFSET{186, 56};
const Vector2 PANEL_SIZE{163, 131};
const Rectangle PANEL_RECT = {0, 0, PANEL_SIZE.x, PANEL_SIZE.y};

/**
 * Rectangle rect;
 * Vector2 textOffset;
 * string name;
 * int id;
 */
struct UIElement
{
    int id;
    Rectangle rect;
    Vector2 textOffset;
    std::string name;
};

enum ButtonState
{
    BS_ENABLED,
    BS_DISABLED,
    BS_HOVER,
    BS_CLICK
};

enum InputState
{
    IS_ENABLED,
    IS_DISABLED,
    IS_HOVER,
    IS_SELECTED
};

struct Config
{
    std::string ip;
    int port;
};

enum SceneState
{
    SS_START_REQUEST,
    SS_AWAIT_REQUEST,
    SS_MENU_IN,
    SS_MENU,
    SS_MENU_OUT
};

enum SceneType
{
    ST_INTRO,
    ST_MAIN,
    ST_LOGIN,
    ST_REGISTER,
    ST_OUTRO,

    ST_REQUEST_HINT,
    ST_REQUEST_SIGNUP,
    ST_REQUEST_LOGIN
};

#endif