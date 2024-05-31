#include "mz_rendervs.h"

MZRenderVS::MZRenderVS() {}

MZRenderVS::~MZRenderVS()
{
    UnloadModel(mshArcadeScreen);
    UnloadRenderTexture(rtArcadeScreen);
    UnloadRenderTexture(rtUIPanel);
    UnloadTexture(texArcadeUI);
    UnloadTexture(texArcadeScreenMult);
    UnloadTexture(texArcadeScreenAdd);
    UnloadTexture(texCursor);
    UnloadFont(fontSetback);
    UnloadFont(fontMono);
}

void MZRenderVS::loadResources()
{
    mshArcadeScreen = LoadModel("res/msh_arcade_screen.obj");

    rtArcadeScreen = LoadRenderTexture(640, 720);
    rtUIPanel = LoadRenderTexture(PANEL_RECT.width, PANEL_RECT.height);
    texArcadeUI = LoadTexture("res/tex_arcadevs_ui.png");
    texArcadeScreenMult = LoadTexture("res/tex_arcadevs_mult.png");
    texArcadeScreenAdd = LoadTexture("res/tex_arcadevs_add.png");

    imgScreenAnim = LoadImageAnim("res/tex_arcadevs_anim.gif", &animFrames);
    texScreenAnim = LoadTextureFromImage(imgScreenAnim);

    fontSetback = LoadFont("res/font_setback.png");
    fontMono = LoadFont("res/font_mono.png");

    SetTextureFilter(rtArcadeScreen.texture, TEXTURE_FILTER_BILINEAR);
    SetMaterialTexture(&mshArcadeScreen.materials[0], MATERIAL_MAP_DIFFUSE,
                       rtArcadeScreen.texture);
}

void MZRenderVS::beginRT()
{
    BeginTextureMode(rtArcadeScreen);
    ClearBackground({25, 25, 25, 255});
    frameCounter++;
    if (frameCounter >= frameDelay)
    {
        currentAnimFrame++;
        if (currentAnimFrame >= animFrames)
            currentAnimFrame = 0;
        nextFrameDataOffset =
            imgScreenAnim.width * imgScreenAnim.height * 4 * currentAnimFrame;
        UpdateTexture(texScreenAnim, ((unsigned char *)imgScreenAnim.data) + nextFrameDataOffset);
        frameCounter = 0;
    }
    DrawTexture(texScreenAnim, 0, 0, WHITE);
    DrawTextureRec(texArcadeUI, Rectangle{2, 162, 392, 47}, Vector2{124, 32},
                   WHITE);
}

void MZRenderVS::endRT()
{
    BeginBlendMode(BLEND_MULTIPLIED);
    DrawTexture(texArcadeScreenMult, 0, 0, WHITE);
    EndBlendMode();
    BeginBlendMode(BLEND_ADD_COLORS);
    DrawTexture(texArcadeScreenAdd, 0, 0, WHITE);
    EndBlendMode();
    EndTextureMode();
}

void MZRenderVS::beginPanelTexture(int size)
{
    BeginTextureMode(rtUIPanel);
    ClearBackground(BLANK);
    BeginScissorMode((int)(PANEL_SIZE.x / 2) - (PANEL_SIZE.x / 2 * size / 10),
                     (int)(PANEL_SIZE.y / 2) - (PANEL_SIZE.y / 2 * size / 10),
                     (int)(PANEL_SIZE.x * size / 10),
                     (int)(PANEL_SIZE.y * size / 10));
}

void MZRenderVS::endPanelTexture()
{
    EndScissorMode();
    EndTextureMode();
}

void MZRenderVS::drawMonoText(Vector2 position, const char *text, int fontSize, float spacing)
{
    DrawTextEx(fontMono, text, position, fontSize, spacing, WHITE);
}

void MZRenderVS::drawHeaderText(Vector2 position, const char *text, int fontSize)
{
    DrawTextEx(fontSetback, text, position, fontSize, 1.0f, WHITE);
}

void MZRenderVS::drawWarningSign()
{
    DrawTextureRec(texArcadeUI, Rectangle{396, 162, 96, 96}, Vector2{86, 24}, WHITE);
    drawHeaderText(Vector2{76, 64}, "ERROR 404", 28);
    DrawText("No Server Connection\n\n  Game will close down", 26, 96, 20, WHITE);
}

void MZRenderVS::drawUIPanel()
{
    DrawTexturePro(
        rtUIPanel.texture, Rectangle{0.0f, 0.0f, PANEL_SIZE.x, -PANEL_SIZE.y},
        Rectangle{PANEL_OFFSET.x, PANEL_OFFSET.y, PANEL_SIZE.x, PANEL_SIZE.y},
        Vector2{0, 0}, 0.0f, WHITE);
}

void MZRenderVS::drawArcadeVS()
{
    DrawModel(mshArcadeScreen, WORLD_ORIGIN, 1.0f, ColorAlpha(WHITE, 0.6f));
}

void MZRenderVS::drawGamePanel(const Rectangle &rec)
{
    DrawTextureNPatch(texArcadeUI, game_panel, rec, Vector2{0, 0}, 0.0f, WHITE);
    DrawTextureRec(texArcadeUI, Rectangle{314, 214, 80, 144}, Vector2{500, 408}, WHITE);
}

void MZRenderVS::drawOpponent(const UIOpponent &btn, ButtonState state)
{
    switch (state)
    {
    case BS_ENABLED:
        DrawTextureNPatch(texArcadeUI, opponent_base, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);
        break;
    case BS_DISABLED:
        DrawTextureNPatch(texArcadeUI, opponent_base, btn.drawRect, Vector2{0, 0}, 0.0f, DARKGRAY);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, DARKGRAY);
        break;
    case BS_HOVER:
        DrawTextureNPatch(texArcadeUI, opponent_hover, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);
        break;
    case BS_CLICK:
        DrawTextureNPatch(texArcadeUI, opponent_click, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);

        break;
    default:
        break;
    }
}

void MZRenderVS::drawOption(const UIButton &btn, ButtonState state)
{
    switch (state)
    {
    case BS_ENABLED:
        DrawTextureNPatch(texArcadeUI, option_base, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);
        break;
    case BS_DISABLED:
        DrawTextureNPatch(texArcadeUI, option_base, btn.drawRect, Vector2{0, 0}, 0.0f, DARKGRAY);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, DARKGRAY);
        break;
    case BS_HOVER:
        DrawTextureNPatch(texArcadeUI, option_hover, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);
        break;
    case BS_CLICK:
        DrawTextureNPatch(texArcadeUI, option_click, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);

        break;
    default:
        break;
    }
}

void MZRenderVS::drawButton(const UIButton &btn, ButtonState state)
{
    switch (state)
    {
    case BS_ENABLED:
        DrawTextureNPatch(texArcadeUI, button_base, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);
        break;
    case BS_DISABLED:
        DrawTextureNPatch(texArcadeUI, button_base, btn.drawRect, Vector2{0, 0}, 0.0f, DARKGRAY);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, DARKGRAY);
        break;
    case BS_HOVER:
        DrawTextureNPatch(texArcadeUI, button_hover, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);
        break;
    case BS_CLICK:
        DrawTextureNPatch(texArcadeUI, button_click, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);

        break;
    default:
        break;
    }
}

void MZRenderVS::drawGameButton(const GameButton &btn)
{
    switch (btn.state)
    {
    case BS_ENABLED:
        DrawTextureNPatch(texArcadeUI, button_base, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);
        break;
    case BS_DISABLED:
        DrawTextureNPatch(texArcadeUI, button_base, btn.drawRect, Vector2{0, 0}, 0.0f, DARKGRAY);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, DARKGRAY);
        break;
    case BS_HOVER:
        DrawTextureNPatch(texArcadeUI, button_hover, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);
        break;
    case BS_CLICK:
        DrawTextureNPatch(texArcadeUI, button_click, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);

        break;
    default:
        break;
    }
}

void MZRenderVS::drawButtonWithText(const UIButton &btn, ButtonState state)
{
    switch (state)
    {
    case BS_ENABLED:
        DrawTextureNPatch(texArcadeUI, button_base, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);
        DrawText(btn.text, btn.drawRect.x + 30, btn.drawRect.y + 10, 10, WHITE);
        break;
    case BS_DISABLED:
        DrawTextureNPatch(texArcadeUI, button_base, btn.drawRect, Vector2{0, 0}, 0.0f, DARKGRAY);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, DARKGRAY);
        DrawText(btn.text, btn.drawRect.x + 30, btn.drawRect.y + 10, 10,
                 DARKGRAY);
        break;
    case BS_HOVER:
        DrawTextureNPatch(texArcadeUI, button_hover, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);
        DrawText(btn.text, btn.drawRect.x + 30, btn.drawRect.y + 10, 10, WHITE);
        break;
    case BS_CLICK:
        DrawTextureNPatch(texArcadeUI, button_click, btn.drawRect, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureRec(texArcadeUI, btn.iconRect, btn.iconOffset, WHITE);
        DrawText(btn.text, btn.drawRect.x + 30, btn.drawRect.y + 10, 10, WHITE);
        break;
    default:
        break;
    }
}

void MZRenderVS::drawLoading(float opacity)
{
    DrawText("LOADING", 260, 180, 20,
             ColorAlpha(WHITE, (1.0f + sinf(opacity)) / 2));
}

void MZRenderVS::drawCell(const Cell &cell)
{
    switch (cell.cellState)
    {
    case CS_DISABLED:
        DrawRectangle(cell.drawRect.x, cell.drawRect.y, cell.drawRect.width,
                      cell.drawRect.height,
                      cell.cellColor == CC_WHITE ? DARKGRAY : BLACK);
        break;
    case CS_ENABLED:
        DrawRectangle(cell.drawRect.x, cell.drawRect.y, cell.drawRect.width,
                      cell.drawRect.height,
                      cell.cellColor == CC_WHITE ? GRAY : DARKGRAY);
        break;
    case CS_SELECTED:
        DrawRectangle(cell.drawRect.x, cell.drawRect.y, cell.drawRect.width,
                      cell.drawRect.height, WHITE);
        DrawTextureRec(texArcadeUI, cell_selected,
                       Vector2{cell.drawRect.x, cell.drawRect.y}, GRAY);
        break;
    case CS_ATTACKED:
        DrawRectangle(cell.drawRect.x, cell.drawRect.y, cell.drawRect.width,
                      cell.drawRect.height, MAROON);
        break;
    case CS_MOVABLE:
        DrawRectangle(cell.drawRect.x, cell.drawRect.y, cell.drawRect.width,
                      cell.drawRect.height, BLUE);
        break;
    case CS_HOVER_ATTACKED:
        DrawRectangle(cell.drawRect.x, cell.drawRect.y, cell.drawRect.width,
                      cell.drawRect.height, MAROON);
        DrawTextureRec(texArcadeUI, cell_hover,
                       Vector2{cell.drawRect.x, cell.drawRect.y}, WHITE);
        break;
    case CS_HOVER_MOVABLE:
        DrawRectangle(cell.drawRect.x, cell.drawRect.y, cell.drawRect.width,
                      cell.drawRect.height, BLUE);
        DrawTextureRec(texArcadeUI, cell_hover,
                       Vector2{cell.drawRect.x, cell.drawRect.y}, WHITE);
        break;
    case CS_HOVER_ENABLED:
        DrawRectangle(cell.drawRect.x, cell.drawRect.y, cell.drawRect.width,
                      cell.drawRect.height,
                      (cell.pieceColor == CC_WHITE && !cell.pieceMoves.empty()) ? LIGHTGRAY
                      : cell.cellColor == CC_WHITE                              ? GRAY
                                                                                : DARKGRAY);
        DrawTextureRec(texArcadeUI, cell_hover,
                       Vector2{cell.drawRect.x, cell.drawRect.y}, WHITE);
        break;
    default:
        break;
    }
}
