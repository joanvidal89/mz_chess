#ifndef SCENE_REQ_HINT
#define SCENE_REQ_HINT

#include "mz_scene.h"
#include "../mz_struct.h"

class SceneReqHint : public MZScene
{
    //------------------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    //------------------------------------------------------------------------------------
public:
    SceneReqHint(MZLauncher *launcher);
    ~SceneReqHint();

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
    
    const UIElement frame_hint = {2, Rectangle{17, 17, 129, 81}, Vector2{16, -12}, "PASSWORD HINT"};
    const UIElement btn_back = {2, Rectangle{34, 100, 95, 24}, Vector2{42, 8}, "OK"};

    std::string responseText;
};

#endif