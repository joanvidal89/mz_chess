#ifndef MZ_COMPOSITE
#define MZ_COMPOSITE

#include "mz_math.h"
#include "mz_struct.h"
#include "raylib.h"

class MZComposite
{
public:
    MZComposite();
    ~MZComposite();

    void loadResources();
    void switchPostProcess(bool on);

    void updateBaseCamPosition(Vector3 position);
    void updateBaseCamTarget(Vector3 target);
    void updateBaseCamFov(float fovy);
    void updateFinalCam();
    void updateFinalCamDampen();
    Camera getFinalCamera();

    void beginRT();
    void beginMode3D();
    void endMode3D();
    void endRT();

    void beginDrawing();
    void drawComposite();
    void endDrawing();

    void drawBlankOverlay(float opacity);
    void drawBlackOverlay(float opacity);
    void drawLogo(float opacity);

    void drawCursor();

private:
    Vector2 screenSize;

    Camera cameraBase;
    Camera cameraFinal;

    Texture2D texLogo;
    Texture2D texCursor;
    RenderTexture2D rtComposite;
    Shader shaderComposite;

    bool postprocess;
};

#endif