#ifndef SCENE_LOGIN
#define SCENE_LOGIN

#include "mz_scene.h"
#include "../mz_struct.h"

class SceneLogin : public MZScene
{
    //------------------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    //------------------------------------------------------------------------------------
public:
    SceneLogin(MZLauncher *launcher);
    ~SceneLogin();

    //------------------------------------------------------------------------------------
    // SCENE UPDATING
    //------------------------------------------------------------------------------------
    void update() override;

    void performMenuIn();
    void performMenu();
    void performMenuOut();

    void performButtonAction(int id);

private:
    int currentSelectInput;
    int lastHoverBtnId;
    int currentHoverBtnId;

    const int username_max_input = 12;
    char username_text[13] = "\0";
    int username_letterCount = 0;

    const int password_max_input = 12;
    char password_text[13] = "\0";
    int password_letterCount = 0;

    const UIElement btn_login = {0, Rectangle{7, 102, 72, 24}, Vector2{19, 8}, "LOG IN"};
    const UIElement btn_cancel = {1, Rectangle{84, 102, 72, 24}, Vector2{17, 8}, "CANCEL"};
    const UIElement btn_hint = {2, Rectangle{120, 50, 24, 24}, Vector2{9, 8}, "?"};

    const UIElement inp_username = {0, Rectangle{14, 24, 128, 16}, Vector2{64, 16}, "USERNAME"};
    const UIElement inp_password = {1, Rectangle{14, 54, 102, 16}, Vector2{40, 16}, "PASSWORD"};

    const UIElement sceneButtons[3] = {btn_login, btn_cancel, btn_hint};
    const UIElement sceneInputs[2] = {inp_username, inp_password};
};

#endif