#include "mz_composite.h"

MZComposite::MZComposite()
{
    cameraBase = initCamera;
    cameraFinal = initCamera;
}

MZComposite::~MZComposite()
{
    UnloadRenderTexture(rtComposite);
    UnloadShader(shaderComposite);
}

void MZComposite::loadResources()
{
    texLogo = LoadTexture("res/tex_logo.png");
    texCursor = LoadTexture("res/tex_cursor.png");

    screenSize = Vector2{(float)GetScreenWidth(), (float)GetScreenHeight()};
    rtComposite = LoadRenderTexture(screenSize.x, screenSize.y);
    shaderComposite = LoadShader(0, "res/mz_postpro.fs");

    SetTextureFilter(rtComposite.texture, TEXTURE_FILTER_BILINEAR);
    SetShaderValue(shaderComposite, GetShaderLocation(shaderComposite, "screenSize"), &screenSize, SHADER_UNIFORM_VEC2);
}

void MZComposite::switchPostProcess(bool on)
{
    postprocess = on;
}

void MZComposite::updateBaseCamPosition(Vector3 position)
{
    cameraBase.position = position;
}

void MZComposite::updateBaseCamTarget(Vector3 target)
{
    cameraBase.target = target;
}

void MZComposite::updateBaseCamFov(float fovy)
{
    cameraBase.fovy = fovy;
}

void MZComposite::updateFinalCam()
{
    cameraFinal = cameraBase;
}

void MZComposite::updateFinalCamDampen()
{
    cameraFinal.target = Vector3{
        Math::floatDamp(cameraFinal.target.x,
                        cameraBase.target.x + ((0.5f + GetMousePosition().x / GetScreenWidth())) * 0.1f, 10.0f,
                        GetFrameTime()),
        Math::floatDamp(cameraFinal.target.y,
                        cameraBase.target.y + ((0.5f - GetMousePosition().y / GetScreenHeight())) * 0.1f, 10.0f,
                        GetFrameTime()),
        cameraBase.target.z,
    };
}

Camera MZComposite::getFinalCamera()
{
    return cameraFinal;
}

void MZComposite::beginRT()
{
    BeginTextureMode(rtComposite);
    ClearBackground(DARKGRAY);
}

void MZComposite::beginMode3D()
{
    BeginMode3D(cameraFinal);
}

void MZComposite::endMode3D()
{
    EndMode3D();
}

void MZComposite::endRT()
{
    EndTextureMode();
}

void MZComposite::beginDrawing()
{
    BeginDrawing();
    ClearBackground(DARKGRAY);
}

void MZComposite::drawComposite()
{
    if (postprocess)
        BeginShaderMode(shaderComposite);
    DrawTextureRec(rtComposite.texture,
                   Rectangle{0, 0, (float)rtComposite.texture.width, (float)-rtComposite.texture.height}, Vector2{0, 0},
                   WHITE);
    if (postprocess)
        EndShaderMode();
}

void MZComposite::endDrawing()
{
    EndDrawing();
}

void MZComposite::drawBlackOverlay(float opacity)
{
    DrawRectangle(0, 0, screenSize.x, screenSize.y, ColorAlpha(BLACK, opacity));
}

void MZComposite::drawBlankOverlay(float opacity)
{
    DrawRectangle(0, 0, screenSize.x, screenSize.y, ColorAlpha(DARKGRAY, opacity));
}

void MZComposite::drawLogo(float opacity)
{
    DrawTexture(texLogo, screenSize.x / 2 - 480, screenSize.y / 2 - 270, ColorAlpha(WHITE, opacity));
}

void MZComposite::drawCursor() { DrawTexture(texCursor, GetMouseX(), GetMouseY(), WHITE); }
