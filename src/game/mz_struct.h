#ifndef MZ_STRUCT
#define MZ_STRUCT

#include <string>
#include <vector>

#include "raylib.h"

const Vector2 PANEL_OFFSET{190, 96};
const Vector2 PANEL_SIZE{260, 212};
const Rectangle PANEL_RECT = {0, 0, PANEL_SIZE.x, PANEL_SIZE.y};
const Rectangle BOARD_RECT = {160.0f, 380.0f, 480.0f, 700.0f};
const Vector3 WORLD_ORIGIN = {0.0f, 0.0f, 0.0f};

enum ButtonState
{
    BS_ENABLED,
    BS_DISABLED,
    BS_HOVER,
    BS_CLICK
};

enum ChessColor
{
    CC_NONE,
    CC_WHITE,
    CC_BLACK,
};

enum PieceType
{
    PT_NONE,
    PT_PAWN,
    PT_ROOK,
    PT_KNIGHT,
    PT_BISHOP,
    PT_QUEEN,
    PT_KING,
};

enum PieceState
{
    PS_NONE,
    PS_IDLE,
    PS_MOVE,
    PS_CASTLE_SHORT,
    PS_CASTLE_LONG,
    PS_PROMOTE,
    PS_PROMOTE_ROOK,
    PS_PROMOTE_KNIGHT,
    PS_PROMOTE_BISHOP,
    PS_PROMOTE_QUEEN,
    PS_DIE,
    PS_DISABLED,
};

enum MoveType
{
    MT_MOVE,
    MT_DOUBLEMOVE,
    MT_ATTACK,
    MT_ENPASSANT,
    MT_PROMOTE,
    MT_ATTACKPROMOTE,
    MT_LONG_CASTLE,
    MT_SHORT_CASTLE,
};

enum CellState
{
    CS_DISABLED,
    CS_HOVER_DISABLED,
    CS_SELECTABLE,
    CS_HOVER_SELECTABLE,
    CS_SELECTED,
    CS_HOVER_SELECTED,
    CS_MOVABLE,
    CS_HOVER_MOVABLE,
    CS_ATTACKED,
    CS_HOVER_ATTACKED,
    CS_CHECK,
    CS_HOVER_CHECK,

};

enum SceneType
{
    ST_LOAD,
    ST_MENU_SELECT,
    ST_MENU_OPPONENT,
    ST_MENU_POSTGAME,
    ST_MENU_404,
    ST_GAME,
    ST_EXIT,
};

enum SceneState
{
    SS_INIT,
    SS_OUT,

    SS_LOAD_DATA,
    SS_LOAD_DATA_END,
    SS_TRANSITION_INTRO,

    SS_REQUEST_HTTP,
    SS_AWAIT_HTTP,

    SS_MENU_IN,
    SS_MENU,
    SS_MENU_OUT,

    SS_REQUEST_MOVES,
    SS_AWAIT_MOVES,
    SS_REQUEST_AI,
    SS_AWAIT_AI,
    SS_PLAYER_TURN,
    SS_PLAYER_TURN_PROMOTE,
    SS_PLAYER_ANIM_START,
    SS_PLAYER_ANIM,
    SS_AI_ANIM_START,
    SS_AI_ANIM,
    SS_ENDGAME,

};

struct Config
{
    std::string ip;
    std::string port;
    bool video;
    bool music;
    bool audio;
};

struct UIOpponent
{
    int id;
    Rectangle iconRect;
    Rectangle drawRect;
    Rectangle collision;
    Vector2 iconOffset;
    const char *name;
};

struct UIButton
{
    int id;
    Rectangle iconRect;
    Rectangle drawRect;
    Rectangle collision;
    Vector2 iconOffset;
    const char *text;
};

struct BoardPosition
{
    int file;
    int rank;
};

struct PieceMove
{
    BoardPosition origin;
    BoardPosition target;
    MoveType type;
};

struct GameButton
{
    int id;
    Rectangle iconRect;
    Rectangle drawRect;
    Vector2 iconOffset;
    ButtonState state;
};

struct Cell
{
    const BoardPosition position;
    const Rectangle drawRect;
    const Vector2 drawModel;
    const ChessColor cellColor;
    CellState cellState;
    PieceType pieceType;
    ChessColor pieceColor;
    PieceState pieceState;
    std::vector<PieceMove> pieceMoves;
};

const Camera3D initCamera = {
    Vector3{8.0f, 15.0f, 100.0f}, Vector3{8.0f, 5.0f, -10.0f},
    Vector3{0.0f, 1.0f, 0.0f}, 30.0f, CAMERA_PERSPECTIVE};

const Camera3D menuCamera = {
    Vector3{8.0f, 10.0f, 25.0f}, Vector3{8.0f, 5.5f, -10.0f},
    Vector3{0.0f, 1.0f, 0.0f}, 15.0f, CAMERA_PERSPECTIVE};

const Camera3D gameCamera = {
    Vector3{8.0f, 25.0f, 15.0f}, Vector3{8.0f, 0.0f, -4.5f},
    Vector3{0.0f, 1.0f, 0.0f}, 15.0f, CAMERA_PERSPECTIVE};

#endif