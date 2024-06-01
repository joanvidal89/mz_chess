#ifndef MZ_DIRECTOR
#define MZ_DIRECTOR

#include "mz_audio.h"
#include "mz_board.h"
#include "mz_comm.h"
#include "mz_composite.h"
#include "mz_config.h"
#include "mz_input.h"
#include "mz_math.h"
#include "mz_render3d.h"
#include "mz_rendervs.h"
#include "mz_struct.h"
#include "mz_uciengine.h"
#include "raylib.h"
#include "scene/mz_scene.h"
#include "scene/scene_exit.h"
#include "scene/scene_game.h"
#include "scene/scene_load.h"
#include "scene/scene_menu_404.h"
#include "scene/scene_menu_opponent.h"
#include "scene/scene_menu_gamestart.h"
#include "scene/scene_menu_gameend.h"
#include "scene/scene_menu_select.h"

class MZDirector
{
public:
    MZDirector(int accountId, int skipIntro);
    ~MZDirector();

    void run();

    void changeScene(SceneType newScene);

    MZAudio *audio;
    MZComm *comm;
    MZComposite *composite;
    MZConfig *config;
    MZInput *input;
    MZRender3D *render3D;
    MZRenderVS *renderVS;
    MZScene *scene;
    MZUciEngine *uciEngine;

    MZBoard *board;

    int accountId;
    int opponentId;
    bool skipIntro;

    bool exitGame = false;
    SceneType newScene;

private:
    float deltaTime;
};

#endif