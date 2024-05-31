#include "scene_intro.h"
#include "../mz_launcher.h"

//------------------------------------------------------------------------------------
// CONSTRUCTOR & DESTRUCTOR
//------------------------------------------------------------------------------------
SceneIntro::SceneIntro(MZLauncher *launcher) : MZScene(launcher, SceneType::ST_INTRO)
{
    nextScene = ST_MAIN;
}

SceneIntro::~SceneIntro() {}

//------------------------------------------------------------------------------------
// SCENE UPDATING
//------------------------------------------------------------------------------------
void SceneIntro::update()
{
    launcher->render2D->beginRenderTexture();
    launcher->render2D->endRenderTexture();
    launcher->render2D->drawLauncher(deltaTime * 2);

    if (deltaTime >= 0.5f)
    {
        launcher->newScene = ST_MAIN;
    }

    deltaTime += GetFrameTime();
}