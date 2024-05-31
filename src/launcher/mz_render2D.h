#ifndef MZ_RENDER2D
#define MZ_RENDER2D

#include "raylib.h"
#include "mz_struct.h"
#include <cstring>
#include <math.h>

class MZRender2D
{
public:
    MZRender2D();
    ~MZRender2D();

    void loadResources();

    void beginRenderTexture();
    void endRenderTexture();

    void beginPanelTexture(float size);
    void endPanelTexture(float size);

    void drawLauncher(float alpha);

    void drawButton(UIElement btn, ButtonState state);
    void drawInputFrame(UIElement frame, InputState state);
    void drawTitledFrame(UIElement frame);
    void drawInputText(UIElement frame, char *text, bool hidden, bool underscored, InputState state);
    void drawTextRect(const char *text, Rectangle rec, Color tint);
    void drawLoading(float opacity);

private:
    Texture2D overlay;
    Texture2D panelGrid;
    Texture2D uiTex;
    RenderTexture2D renderTexture;
    RenderTexture2D texLauncher;

    Rectangle scissorArea;

    // NPATCH TEXTURE INFO
    const NPatchInfo panel_border = {Rectangle{0, 0, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
    const NPatchInfo panel_back = {Rectangle{24, 0, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};

    const NPatchInfo button_base = {Rectangle{0, 24, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
    const NPatchInfo button_hover = {Rectangle{24, 24, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
    const NPatchInfo button_click = {Rectangle{48, 24, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};

    const NPatchInfo input_base = {Rectangle{72, 24, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
    const NPatchInfo input_hover = {Rectangle{96, 24, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
    const NPatchInfo input_selected = {Rectangle{120, 24, 24, 24}, 8, 8, 8, 8, NPATCH_NINE_PATCH};
};

#endif