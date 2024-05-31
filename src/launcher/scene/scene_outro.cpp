#include "scene_outro.h"
#include "../mz_launcher.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneOutro::SceneOutro(MZLauncher *launcher) : MZScene(launcher, SceneType::ST_OUTRO)
{
    deltaTime = 0.0f;
}

SceneOutro::~SceneOutro() {}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneOutro::update()
{
    launcher->render2D->beginRenderTexture();
    launcher->render2D->endRenderTexture();
    launcher->render2D->drawLauncher(1.0f - (deltaTime * 2));

    if (deltaTime >= 0.5f)
    {
        launcher->exitLauncher = true;
    }

    deltaTime += GetFrameTime();
}