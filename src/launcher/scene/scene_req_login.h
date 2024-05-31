#ifndef SCENE_REQ_LOGIN
#define SCENE_REQ_LOGIN

#include "mz_scene.h"
#include "../mz_struct.h"

class SceneReqLogin : public MZScene
{
    //------------------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    //------------------------------------------------------------------------------------
public:
    SceneReqLogin(MZLauncher *launcher);
    ~SceneReqLogin();

    //------------------------------------------------------------------------------------
    // SCENE UPDATING
    //------------------------------------------------------------------------------------
    void update() override;

    void performMenuAwait();
    void performMenuIn();
    void performMenu();
    void performMenuOut();

private:
    int lastHoverBtnId;
    int currentHoverBtnId;

    const UIElement frame_login = {2, Rectangle{17, 17, 129, 81}, Vector2{16, -12}, "LOGIN ATTEMPT"};
    const UIElement btn_back = {2, Rectangle{34, 100, 95, 24}, Vector2{42, 8}, "OK"};

    std::string responseText;
};

#endif