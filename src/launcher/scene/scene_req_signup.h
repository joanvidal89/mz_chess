#ifndef SCENE_REQ_SIGNUP
#define SCENE_REQ_SIGNUP

#include "mz_scene.h"
#include "../mz_struct.h"

class SceneReqSignup : public MZScene
{
    //------------------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    //------------------------------------------------------------------------------------
public:
    SceneReqSignup(MZLauncher *launcher);
    ~SceneReqSignup();

    //------------------------------------------------------------------------------------
    // SCENE UPDATING
    //------------------------------------------------------------------------------------
    void update() override;

    void performMenuStart();
    void performMenuAwait();
    void performMenuIn();
    void performMenu();
    void performMenuOut();

private:
    int lastHoverBtnId;
    int currentHoverBtnId;

    const UIElement frame_signup = {2, Rectangle{17, 17, 129, 81}, Vector2{16, -12}, "SIGN UP"};
    const UIElement btn_back = {2, Rectangle{34, 100, 95, 24}, Vector2{42, 8}, "OK"};

    std::string responseText;
};

#endif