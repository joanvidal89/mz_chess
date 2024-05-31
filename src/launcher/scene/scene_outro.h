#ifndef SCENE_OUTRO
#define SCENE_OUTRO

#include "mz_scene.h"
#include "../mz_struct.h"

class SceneOutro : public MZScene
{
    //------------------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    //------------------------------------------------------------------------------------
public:
    SceneOutro(MZLauncher *launcher);
    ~SceneOutro();

    //------------------------------------------------------------------------------------
    // SCENE UPDATING
    //------------------------------------------------------------------------------------
    void update() override;

private:
    const UIElement btn_m_login = {0, Rectangle{30, 20, 96, 24}, Vector2{30, 8}, "LOG IN"};
    const UIElement btn_m_register = {1, Rectangle{30, 48, 96, 24}, Vector2{22, 8}, "REGISTER"};
    const UIElement btn_m_quit = {2, Rectangle{30, 80, 96, 24}, Vector2{36, 8}, "QUIT"};
    
    const UIElement sceneButtons[3] = {btn_m_login, btn_m_register, btn_m_quit};
};

#endif