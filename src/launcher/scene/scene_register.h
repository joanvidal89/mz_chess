#ifndef SCENE_REGISTER
#define SCENE_REGISTER

#include "mz_scene.h"
#include "../mz_struct.h"

class SceneRegister : public MZScene
{
    //------------------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    //------------------------------------------------------------------------------------
public:
    SceneRegister(MZLauncher *launcher);
    ~SceneRegister();

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

    const int hint_max_input = 64;
    char hint_text[65] = "\0";
    int hint_letterCount = 0;

    const UIElement btn_signup = {0, Rectangle{7, 102, 72, 24}, Vector2{19, 8}, "SIGN UP"};
    const UIElement btn_cancel = {1, Rectangle{84, 102, 72, 24}, Vector2{17, 8}, "CANCEL"};

    const UIElement inp_username = {0, Rectangle{17, 10, 128, 16}, Vector2{66, 16}, "USERNAME"};
    const UIElement inp_password = {1, Rectangle{17, 40, 128, 16}, Vector2{66, 16}, "PASSWORD"};
    const UIElement inp_hint = {2, Rectangle{17, 70, 130, 16}, Vector2{36, 16}, "PASSWORD HINT"};

    const UIElement sceneButtons[2] = {btn_signup, btn_cancel};
    const UIElement sceneInputs[3] = {inp_username, inp_password, inp_hint};
};

#endif