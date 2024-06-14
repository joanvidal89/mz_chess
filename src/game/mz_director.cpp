#include "mz_director.h"

MZDirector::MZDirector(int accountId, int skipIntro)
{
    audio = new MZAudio();
    board = new MZBoard();
    comm = new MZComm();
    composite = new MZComposite();
    config = new MZConfig();
    input = new MZInput();
    render3D = new MZRender3D();
    renderVS = new MZRenderVS();
    uciEngine = new MZUciEngine();
    opponentId = 0;

    newScene = ST_LOAD;
    this->accountId = accountId;
    this->skipIntro = skipIntro;
}

MZDirector::~MZDirector()
{
    config->writeConfig();
    delete (audio);
    delete (board);
    delete (comm);
    delete (composite);
    delete (config);
    delete (input);
    delete (render3D);
    delete (renderVS);
    delete (uciEngine);
}

void MZDirector::run()
{
    // INIT WINDOW
    //------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    // SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_VSYNC_HINT);
    // SetConfigFlags(FLAG_WINDOW_MAXIMIZED);
    // SetConfigFlags(FLAG_WINDOW_TOPMOST);
    // SetTraceLogLevel(LOG_ALL);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "MZ Chess");
    //InitWindow(1600, 900, "MZ Chess");
    //SetTargetFPS(60);
    HideCursor();

    // INIT CONFIG
    //------------------------------------------------------------------------------------

    config->readConfig();
    composite->loadResources();
    composite->switchPostProcess(config->currentConfig.video);
    audio->switchMusicOn(config->currentConfig.music);
    audio->switchAudioOn(config->currentConfig.audio);
    comm->address = config->currentConfig.ip;
    comm->port = config->currentConfig.port;
    comm->accountId = std::to_string(accountId);
    comm->launchProcess();
    uciEngine->launchProcess();

    scene = new SceneLoad(this);

    // GAME LOOP
    //------------------------------------------------------------------------------------
    while (!exitGame)
    {
        scene->update();

        if (scene->getSceneType() != newScene)
        {
            changeScene(newScene);
        }
    }
}

/**
 * @brief Deletes the current scene and creates a new one.
 *
 * @param newScene The new SceneType to create.
 * @return void
 */
void MZDirector::changeScene(SceneType newScene)
{
    delete (scene);

    switch (newScene)
    {
    case ST_MENU_404:
        scene = new SceneMenu404(this);
        break;
    case ST_MENU_SELECT:
        scene = new SceneMenuSelect(this);
        break;
    case ST_MENU_OPPONENT:
        scene = new SceneMenuOpponent(this);
        break;
    case ST_MENU_GAME_END:
        scene = new SceneMenuGameEnd(this);
        break;
    case ST_MENU_GAME_START:
        scene = new SceneMenuGameStart(this);
        break;
    case ST_GAME:
        scene = new SceneGame(this);
        break;
    case ST_EXIT:
        scene = new SceneExit(this);
        break;
    default:
        break;
    }
}
