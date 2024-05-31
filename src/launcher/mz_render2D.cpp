#include "mz_render2D.h"

MZRender2D::MZRender2D()
{
}

MZRender2D::~MZRender2D()
{
    UnloadTexture(overlay);
    UnloadTexture(panelGrid);
    UnloadTexture(uiTex);
    UnloadRenderTexture(renderTexture);
    UnloadRenderTexture(texLauncher);
}

void MZRender2D::loadResources()
{
    overlay = LoadTexture("res/tex_overlay.png");
    panelGrid = LoadTexture("res/tex_grid.png");
    uiTex = LoadTexture("res/tex_launcher.png");
    renderTexture = LoadRenderTexture(BASE_WIDTH, BASE_HEIGHT);
    texLauncher = LoadRenderTexture(PANEL_SIZE.x, PANEL_SIZE.y);
}

void MZRender2D::beginRenderTexture()
{
    BeginTextureMode(renderTexture);
    ClearBackground(BLANK);
    DrawTextureRec(uiTex, Rectangle{0, 64, 416, 224}, Vector2{0, 0}, WHITE);
    DrawTexturePro(overlay, Rectangle{(float)GetTime() * 10, (float)GetTime() * 10, 384, 192}, Rectangle{16, 16, 384, 192}, Vector2{0, 0}, 0.0f, WHITE);
    DrawTexturePro(texLauncher.texture, Rectangle{0.0f, 0.0f, PANEL_SIZE.x, -PANEL_SIZE.y},
                   Rectangle{PANEL_OFFSET.x, PANEL_OFFSET.y, PANEL_SIZE.x, PANEL_SIZE.y},
                   Vector2{0, 0}, 0.0f, WHITE);
}

void MZRender2D::endRenderTexture()
{
    DrawTextureRec(uiTex, Rectangle{0, 288, 416, 224}, Vector2{0, 0}, WHITE);
    EndTextureMode();
}

void MZRender2D::beginPanelTexture(float size)
{
    BeginTextureMode(texLauncher);
    ClearBackground(BLANK);
    BeginScissorMode((int)(PANEL_SIZE.x / 2) - (PANEL_SIZE.x / 2 * size),
                     (int)(PANEL_SIZE.y / 2) - (PANEL_SIZE.y / 2 * size),
                     (int)(PANEL_SIZE.x * size),
                     (int)(PANEL_SIZE.y * size));
    DrawTextureNPatch(uiTex, panel_back,
                      Rectangle{
                          (PANEL_SIZE.x / 2) - (PANEL_SIZE.x / 2 * size),
                          (PANEL_SIZE.y / 2) - (PANEL_SIZE.y / 2 * size),
                          (PANEL_SIZE.x * size),
                          (PANEL_SIZE.y * size)},
                      Vector2{0, 0}, 0.0f, WHITE);
    DrawTexture(panelGrid, 2, 2, WHITE);
}

void MZRender2D::endPanelTexture(float size)
{
    DrawTextureNPatch(uiTex, panel_border,
                      Rectangle{
                          (float)static_cast<int>((PANEL_SIZE.x / 2) - (PANEL_SIZE.x / 2 * size)),
                          (float)static_cast<int>((PANEL_SIZE.y / 2) - (PANEL_SIZE.y / 2 * size)),
                          (float)static_cast<int>(PANEL_SIZE.x * size),
                          (float)static_cast<int>(PANEL_SIZE.y * size),
                      },
                      Vector2{0, 0},
                      0.0f, WHITE);
    EndScissorMode();
    EndTextureMode();
}

void MZRender2D::drawLauncher(float alpha)
{
    BeginDrawing();
    ClearBackground(BLANK);
    DrawTexturePro(renderTexture.texture, Rectangle{0.0f, 0.0f, (float)renderTexture.texture.width, (float)-renderTexture.texture.height},
                   Rectangle{(GetScreenWidth() - ((float)BASE_WIDTH * SCALE)) * 0.5f, (GetScreenHeight() - ((float)BASE_HEIGHT * SCALE)) * 0.5f,
                             (float)BASE_WIDTH * SCALE, (float)BASE_HEIGHT * SCALE},
                   Vector2{0, 0}, 0.0f, ColorAlpha(WHITE, alpha));
    EndDrawing();
}

void MZRender2D::drawButton(UIElement btn, ButtonState state)
{
    switch (state)
    {
    case BS_ENABLED:
        DrawTextureNPatch(uiTex, button_base, btn.rect, Vector2{0, 0}, 0.0f, WHITE);
        DrawText(btn.name.c_str(), btn.rect.x + btn.textOffset.x, btn.rect.y + btn.textOffset.y, 10, WHITE);
        break;
    case BS_DISABLED:
        DrawTextureNPatch(uiTex, button_base, btn.rect, Vector2{0, 0}, 0.0f, DARKGRAY);
        DrawText(btn.name.c_str(), btn.rect.x + btn.textOffset.x, btn.rect.y + btn.textOffset.y, 10, DARKGRAY);
        break;
    case BS_HOVER:
        DrawTextureNPatch(uiTex, button_hover, btn.rect, Vector2{0, 0}, 0.0f, WHITE);
        DrawText(btn.name.c_str(), btn.rect.x + btn.textOffset.x, btn.rect.y + btn.textOffset.y, 10, WHITE);
        break;
    case BS_CLICK:
        DrawTextureNPatch(uiTex, button_click, btn.rect, Vector2{0, 0}, 0.0f, WHITE);
        DrawText(btn.name.c_str(), btn.rect.x + btn.textOffset.x, btn.rect.y + btn.textOffset.y, 10, WHITE);
        break;
    default:
        break;
    }
}

void MZRender2D::drawInputFrame(UIElement frame, InputState state)
{
    switch (state)
    {
    case IS_ENABLED:
        DrawTextureNPatch(uiTex, input_base, frame.rect, Vector2{0, 0}, 0.0f, WHITE);
        DrawText(frame.name.c_str(), frame.rect.x + frame.textOffset.x, frame.rect.y + frame.textOffset.y, 10, DARKGRAY);
        break;
    case IS_DISABLED:
        DrawTextureNPatch(uiTex, input_base, frame.rect, Vector2{0, 0}, 0.0f, DARKGRAY);
        DrawText(frame.name.c_str(), frame.rect.x + frame.textOffset.x, frame.rect.y + frame.textOffset.y, 10, DARKGRAY);
        break;
    case IS_HOVER:
        DrawTextureNPatch(uiTex, input_hover, frame.rect, Vector2{0, 0}, 0.0f, WHITE);
        DrawText(frame.name.c_str(), frame.rect.x + frame.textOffset.x, frame.rect.y + frame.textOffset.y, 10, GRAY);
        break;
    case IS_SELECTED:
        DrawTextureNPatch(uiTex, input_selected, frame.rect, Vector2{0, 0}, 0.0f, WHITE);
        DrawText(frame.name.c_str(), frame.rect.x + frame.textOffset.x, frame.rect.y + frame.textOffset.y, 10, DARKBLUE);
        break;
    default:
        break;
    }
}

void MZRender2D::drawTitledFrame(UIElement frame)
{
    DrawTextureNPatch(uiTex, input_base, frame.rect, Vector2{0, 0}, 0.0f, WHITE);
    DrawText(frame.name.c_str(), frame.rect.x + frame.textOffset.x, frame.rect.y + frame.textOffset.y, 10, WHITE);
}

void MZRender2D::drawInputText(UIElement frame, char *text, bool hidden, bool cursor, InputState state)
{
    std::string output = "";

    if (hidden)
    {
        size_t length = strlen(text);

        char *hiddenText = new char[length + 1];
        std::string masked(length, '*');
        strncpy(hiddenText, masked.c_str(), length);
        hiddenText[length] = '\0';

        output = hiddenText;
    }
    else
    {
        output = text;
    }

    if (cursor)
    {
        output += "|";
    }

    switch (state)
    {
    case IS_ENABLED:
        DrawText(output.c_str(), (int)frame.rect.x + 6, (int)frame.rect.y + 4, 10, WHITE);
        break;
    case IS_DISABLED:
        DrawText(output.c_str(), (int)frame.rect.x + 6, (int)frame.rect.y + 4, 10, GRAY);
        break;
    default:
        break;
    }
}

void MZRender2D::drawTextRect(const char *text, Rectangle rec, Color tint)
{
    Font font = GetFontDefault();

    int length = TextLength(text);

    float textOffsetY = 0;
    float textOffsetX = 0.0f;

    enum
    {
        MEASURE_STATE = 0,
        DRAW_STATE = 1
    };
    int state = MEASURE_STATE;

    int startLine = -1;
    int endLine = -1;
    int lastk = -1;

    for (int i = 0, k = 0; i < length; i++, k++)
    {
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        if (codepoint == 0x3f)
            codepointByteCount = 1;
        i += (codepointByteCount - 1);

        float glyphWidth = 0;
        if (codepoint != '\n')
        {
            glyphWidth = (font.glyphs[index].advanceX == 0) ? font.recs[index].width : font.glyphs[index].advanceX;

            if (i + 1 < length)
                glyphWidth = glyphWidth + 1.0f;
        }

        if (state == MEASURE_STATE)
        {
            if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n'))
                endLine = i;

            if ((textOffsetX + glyphWidth) > rec.width)
            {
                endLine = (endLine < 1) ? i : endLine;
                if (i == endLine)
                    endLine -= codepointByteCount;
                if ((startLine + codepointByteCount) == endLine)
                    endLine = (i - codepointByteCount);

                state = !state;
            }
            else if ((i + 1) == length)
            {
                endLine = i;
                state = !state;
            }
            else if (codepoint == '\n')
                state = !state;

            if (state == DRAW_STATE)
            {
                textOffsetX = 0;
                i = startLine;
                glyphWidth = 0;

                int tmp = lastk;
                lastk = k - 1;
                k = tmp;
            }
        }
        else
        {
            if ((textOffsetY + font.baseSize) > rec.height)
                break;

            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                DrawTextCodepoint(font, codepoint, Vector2{rec.x + textOffsetX, rec.y + textOffsetY}, font.baseSize, tint);
            }

            if (i == endLine)
            {
                textOffsetY += (font.baseSize + font.baseSize / 2);
                textOffsetX = 0;
                startLine = endLine;
                endLine = -1;
                glyphWidth = 0;
                k = lastk;

                state = !state;
            }
        }

        if ((textOffsetX != 0) || (codepoint != ' '))
            textOffsetX += glyphWidth;
    }
}

void MZRender2D::drawLoading(float opacity)
{
    DrawText("LOADING", 240, 110, 20, ColorAlpha(WHITE, (1.0f + sinf(opacity)) / 2));
}
