#ifndef MZ_LAUNCHER
#define MZ_LAUNCHER

#include "raylib.h"
#include "raymath.h"
#include "mz_audio.h"
#include "mz_comm.h"
#include "mz_config.h"
#include "mz_input.h"
#include "mz_render2d.h"
#include "scene/mz_scene.h"
#include "scene/scene_intro.h"
#include "scene/scene_main.h"
#include "scene/scene_outro.h"
#include "scene/scene_login.h"
#include "scene/scene_register.h"
#include "scene/scene_req_hint.h"
#include "scene/scene_req_login.h"
#include "scene/scene_req_signup.h"

class MZLauncher
{
public:
    MZLauncher();
    ~MZLauncher();

    bool run();

    void changeScene(SceneType newScene);

    MZAudio *audio;
    MZComm *comm;
    MZConfig *config;
    MZInput *input;
    MZRender2D *render2D;
    MZScene *scene;

    SceneType newScene;

    bool exitLauncher;
    bool loginSuccessful;

    int accountId;
    
private:
  
};

#endif