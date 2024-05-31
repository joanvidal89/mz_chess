#ifndef SCENE_MAIN
#define SCENE_MAIN

#include "mz_scene.h"
#include "../mz_struct.h"

class SceneMain : public MZScene
{
    //------------------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    //------------------------------------------------------------------------------------
public:
    SceneMain(MZLauncher *launcher);
    ~SceneMain();

    //------------------------------------------------------------------------------------
    // SCENE UPDATING
    //------------------------------------------------------------------------------------
    void update() override;
    
    void performMenuIn();
    void performMenu();
    void performMenuOut();

    void performButtonAction(int id);

private:
    const UIElement btn_login = {0, Rectangle{34, 20, 95, 24}, Vector2{30, 8}, "LOG IN"};
    const UIElement btn_register = {1, Rectangle{34, 48, 95, 24}, Vector2{22, 8}, "REGISTER"};
    const UIElement btn_quit = {2, Rectangle{34, 80, 95, 24}, Vector2{36, 8}, "QUIT"};
    
    const UIElement sceneButtons[3] = {btn_login, btn_register, btn_quit};

    int lastHoverBtnId;
    int currentHoverBtnId;
};

#endif