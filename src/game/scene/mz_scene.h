#ifndef MZ_SCENE
#define MZ_SCENE

#include "../mz_struct.h"
#include "raylib.h"

class MZDirector;
class MZScene
{
public:
    MZScene(MZDirector *director, SceneType type) : director(director), type(type) {}
    virtual ~MZScene() = default;
    virtual void update() {};

    SceneType getSceneType();

protected:
    MZDirector *director;
    SceneState currentState;
    SceneType type;

    float deltaTime;
};

#endif