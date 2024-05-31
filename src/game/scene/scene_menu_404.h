#ifndef SCENE_MENU_404
#define SCENE_MENU_404

#include <iostream>
#include <sstream>
#include <string>

#include "../mz_struct.h"
#include "mz_scene.h"

class SceneMenu404 : public MZScene
{
    //------------------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    //------------------------------------------------------------------------------------
public:
    SceneMenu404(MZDirector *director);
    ~SceneMenu404();

    //------------------------------------------------------------------------------------
    // SCENE UPDATING
    //------------------------------------------------------------------------------------
    void update() override;

    void ssMenuIn();
    void ssMenu();
    void ssMenuOut();

private:
    SceneState currentState;
    SceneType futureScene;
    float deltaTime;

    int lastHoverPlayId;
    int currentHoverPlayId;
    int lastHoverBtnId;
    int currentHoverBtnId;

    const UIButton BTN_OK = {1,
                             Rectangle{318, 94, 32, 32},
                             Rectangle{8, 64, 64, 32},
                             Rectangle{PANEL_OFFSET.x + 8, PANEL_OFFSET.y + 64, 64, 32},
                             Vector2{8, 64},
                             "PLAY"};

    const UIButton BTN_VID_ON = {
        1, Rectangle{210, 58, 32, 32}, Rectangle{224, 302, 32, 32}, Rectangle{224, 302, 32, 32}, Vector2{224, 302}, ""};
    const UIButton BTN_VID_OFF = {
        2, Rectangle{210, 94, 32, 32}, Rectangle{224, 302, 32, 32}, Rectangle{224, 302, 32, 32}, Vector2{224, 302}, ""};
    const UIButton BTN_SND_ON = {
        3, Rectangle{246, 58, 32, 32}, Rectangle{260, 302, 32, 32}, Rectangle{260, 302, 32, 32}, Vector2{260, 302}, ""};
    const UIButton BTN_SND_OFF = {
        4, Rectangle{246, 94, 32, 32}, Rectangle{260, 302, 32, 32}, Rectangle{260, 302, 32, 32}, Vector2{260, 302}, ""};
    const UIButton BTN_MUS_ON = {
        5, Rectangle{282, 58, 32, 32}, Rectangle{296, 302, 32, 32}, Rectangle{296, 302, 32, 32}, Vector2{296, 302}, ""};
    const UIButton BTN_MUS_OFF = {
        6, Rectangle{282, 94, 32, 32}, Rectangle{296, 302, 32, 32}, Rectangle{296, 302, 32, 32}, Vector2{296, 302}, ""};
    const UIButton BTN_EXIT = {
        7, Rectangle{318, 58, 96, 32}, Rectangle{332, 302, 96, 32}, Rectangle{332, 302, 96, 32}, Vector2{332, 302}, ""};

    UIButton optionButtons[4];

    std::string formatResponse(const std::string &response);
};

#endif