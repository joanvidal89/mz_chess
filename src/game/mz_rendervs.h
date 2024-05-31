#ifndef MZ_RENDERVS
#define MZ_RENDERVS

#include <math.h>

#include "mz_struct.h"
#include "raylib.h"

class MZRenderVS
{
public:
    MZRenderVS();
    ~MZRenderVS();

    void loadResources();

    void beginPanelTexture(int size);
    void endPanelTexture();
    void drawMonoText(Vector2 position, const char *text, int fontSize, float spacing);
    void drawHeaderText(Vector2 position, const char *text, int fontSize);
    void drawWarningSign();
    void drawUIPanel();

    void beginRT();
    void endRT();

    void drawArcadeVS();
    void drawGamePanel(const Rectangle &rec);
    void drawOpponent(const UIOpponent &btn, ButtonState state);
    void drawOption(const UIButton &btn, ButtonState state);
    void drawButton(const UIButton &btn, ButtonState state);
    void drawGameButton(const GameButton &btn);
    void drawButtonWithText(const UIButton &btn, ButtonState state);
    void drawLoading(float opacity);
    void drawCell(const Cell &cell);

private:
    Model mshArcadeScreen;

    Font fontSetback;
    Font fontMono;

    Texture2D texScreenAnim;
    Texture2D texArcadeUI;
    Texture2D texArcadeScreenAdd;
    Texture2D texArcadeScreenMult;
    Texture2D texCursor;

    RenderTexture2D rtUIPanel;
    RenderTexture2D rtArcadeScreen;

    const NPatchInfo game_panel = {Rectangle{90, 214, 36, 36}, 12, 12, 12, 12, NPATCH_NINE_PATCH};

    const NPatchInfo opponent_base = {Rectangle{2, 2, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
    const NPatchInfo opponent_hover = {Rectangle{30, 2, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
    const NPatchInfo opponent_click = {Rectangle{58, 2, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};

    const NPatchInfo button_base = {Rectangle{2, 30, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
    const NPatchInfo button_hover = {Rectangle{30, 30, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
    const NPatchInfo button_click = {Rectangle{58, 30, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};

    const NPatchInfo option_base = {Rectangle{86, 30, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
    const NPatchInfo option_hover = {Rectangle{114, 30, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
    const NPatchInfo option_click = {Rectangle{142, 30, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};

    const NPatchInfo gamebtn_disabled = {Rectangle{86, 2, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
    const NPatchInfo gamebtn_enabled = {Rectangle{114, 2, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
    const NPatchInfo gamebtn_hover = {Rectangle{142, 2, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
    const NPatchInfo gamebtn_click = {Rectangle{170, 2, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};

    const Rectangle cell_selected = Rectangle{2, 214, 40, 40};
    const Rectangle cell_selectable = Rectangle{2, 258, 40, 40};
    const Rectangle cell_attack = Rectangle{2, 302, 40, 40};
    const Rectangle cell_move = Rectangle{2, 346, 40, 40};

    const Rectangle cell_hover = Rectangle{46, 214, 40, 40};

    const Color C_WHITE = Color{200, 200, 200, 255};
    const Color C_BLACK = Color{150, 150, 150, 255};
    const Color C_DIS_WHITE = Color{75, 75, 75, 255};
    const Color C_DIS_BLACK = Color{25, 25, 25, 255};
    const Color C_SEL_WHITE = Color{75, 100, 125, 255};
    const Color C_SEL_BLACK = Color{25, 50, 75, 255};
    const Color C_ATK_WHITE = Color{125, 75, 75, 255};
    const Color C_ATK_BLACK = Color{75, 25, 25, 255};
    const Color C_MOV_WHITE = Color{25, 75, 75, 255};
    const Color C_MOV_BLACK = Color{75, 125, 125, 255};

    int animFrames = 0;
    Image imgScreenAnim;
    unsigned int nextFrameDataOffset = 0;
    int currentAnimFrame = 0;
    int frameDelay = 8;
    int frameCounter = 0;
};

#endif