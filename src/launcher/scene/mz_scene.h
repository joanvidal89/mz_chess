#ifndef MZ_SCENE
#define MZ_SCENE

#include "raylib.h"
#include "../mz_struct.h"

class MZLauncher;
class MZScene
{
public:
    MZScene(MZLauncher *launcher, SceneType type) : launcher(launcher), type(type) {}
    virtual ~MZScene() = default;
    virtual void update() {};
    
    SceneType getSceneType();

protected:
    MZLauncher *launcher;
    SceneState currentState;
    SceneType type;
    SceneType nextScene;

    float deltaTime;
};

#endif