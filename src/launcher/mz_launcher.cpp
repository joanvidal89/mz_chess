#include "mz_launcher.h"

MZLauncher::MZLauncher()
{
    audio = new MZAudio();
    config = new MZConfig();
    comm = new MZComm();
    input = new MZInput();
    render2D = new MZRender2D();
    scene = new SceneIntro(this);

    newScene = ST_INTRO;
    exitLauncher = false;
    loginSuccessful = false;
    accountId = -1;
}

MZLauncher::~MZLauncher()
{
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
bool MZLauncher::run()
{
    // INIT WINDOW
    //------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
    SetTargetFPS(60);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MZChess Launcher");
    Vector2 windowPosition = {(float)(GetMonitorWidth(0) / 2) - (SCREEN_WIDTH / 2),
                              (float)(GetMonitorHeight(0)) / 2 - (SCREEN_HEIGHT / 2)};
    SetWindowPosition(windowPosition.x, windowPosition.y);

    audio->loadResources();
    render2D->loadResources();
    input->initialize(windowPosition);
    config->readConfig();
    comm->setServer(config->currentConfig.ip, config->currentConfig.port);

    while (!exitLauncher)
    {
        scene->update();

        if (scene->getSceneType() != newScene)
        {
            changeScene(newScene);
        }
    }

    if (loginSuccessful && accountId != -1)
        return true;
    else
        return false;
}

void MZLauncher::changeScene(SceneType newScene)
{
    delete (scene);

    switch (newScene)
    {
    case ST_MAIN:
        scene = new SceneMain(this);
        break;
    case ST_OUTRO:
        scene = new SceneOutro(this);
        break;
    case ST_LOGIN:
        scene = new SceneLogin(this);
        break;
    case ST_REGISTER:
        scene = new SceneRegister(this);
        break;
    case ST_REQUEST_LOGIN:
        scene = new SceneReqLogin(this);
        break;
    case ST_REQUEST_HINT:
        scene = new SceneReqHint(this);
        break;
    case ST_REQUEST_SIGNUP:
        scene = new SceneReqSignup(this);
        break;
    default:
        break;
    }
}