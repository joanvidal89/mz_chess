#ifndef SCENE_MENU_SELECT
#define SCENE_MENU_SELECT

#include "../mz_struct.h"
#include "mz_scene.h"

class SceneMenuSelect : public MZScene
{
    //------------------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    //------------------------------------------------------------------------------------
public:
    SceneMenuSelect(MZDirector *director);
    ~SceneMenuSelect();

    //------------------------------------------------------------------------------------
    // SCENE UPDATING
    //------------------------------------------------------------------------------------
    void update() override;

    void ssMenuIn();
    void ssMenu();
    void ssMenuOut();

    void performOppButtonAction(UIOpponent opp);
    void performOptionButtonAction(UIButton opt);

private:
    SceneState currentState;
    SceneType futureScene;
    float deltaTime;

    int lastHoverOppId;
    int currentHoverOppId;
    int lastHoverBtnId;
    int currentHoverBtnId;

    const Vector2 TITLE_LOCATION = Vector2{92, 16};
    const Vector2 OPPTX_LOCATION = Vector2{32, 154};
    const char *MENU_TITLE = "SELECT MATCH";

    const UIOpponent btn_lvl_1 = {1,
                                  Rectangle{2, 58, 48, 48},
                                  Rectangle{12, 36, 56, 56},
                                  Rectangle{PANEL_OFFSET.x + 12, PANEL_OFFSET.y + 36, 56, 56},
                                  Vector2{16, 40},
                                  "MR.CHICKEN"};
    const UIOpponent btn_lvl_2 = {2,
                                  Rectangle{54, 58, 48, 48},
                                  Rectangle{72, 36, 56, 56},
                                  Rectangle{PANEL_OFFSET.x + 72, PANEL_OFFSET.y + 36, 56, 56},
                                  Vector2{76, 40},
                                  "BILLY BOY"};
    const UIOpponent btn_lvl_3 = {3,
                                  Rectangle{106, 58, 48, 48},
                                  Rectangle{132, 36, 56, 56},
                                  Rectangle{PANEL_OFFSET.x + 132, PANEL_OFFSET.y + 36, 56, 56},
                                  Vector2{136, 40},
                                  "THE SHOWMAN"};
    const UIOpponent btn_lvl_4 = {4,
                                  Rectangle{158, 58, 48, 48},
                                  Rectangle{192, 36, 56, 56},
                                  Rectangle{PANEL_OFFSET.x + 192, PANEL_OFFSET.y + 36, 56, 56},
                                  Vector2{196, 40},
                                  "SHARON PARKS"};
    const UIOpponent btn_lvl_5 = {5,
                                  Rectangle{2, 110, 48, 48},
                                  Rectangle{12, 96, 56, 56},
                                  Rectangle{PANEL_OFFSET.x + 12, PANEL_OFFSET.y + 96, 56, 56},
                                  Vector2{16, 100},
                                  "BUCKET KNIGHT"};
    const UIOpponent btn_lvl_6 = {6,
                                  Rectangle{54, 110, 48, 48},
                                  Rectangle{72, 96, 56, 56},
                                  Rectangle{PANEL_OFFSET.x + 72, PANEL_OFFSET.y + 96, 56, 56},
                                  Vector2{76, 100},
                                  "MS. ROBINSON"};
    const UIOpponent btn_lvl_7 = {7,
                                  Rectangle{106, 110, 48, 48},
                                  Rectangle{132, 96, 56, 56},
                                  Rectangle{PANEL_OFFSET.x + 132, PANEL_OFFSET.y + 96, 56, 56},
                                  Vector2{136, 100},
                                  "OLD MARCUS"};
    const UIOpponent btn_lvl_8 = {8,
                                  Rectangle{158, 110, 48, 48},
                                  Rectangle{192, 96, 56, 56},
                                  Rectangle{PANEL_OFFSET.x + 192, PANEL_OFFSET.y + 96, 56, 56},
                                  Vector2{196, 100},
                                  "????"};

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

    const UIOpponent opponentButtons[8] = {btn_lvl_1, btn_lvl_2, btn_lvl_3, btn_lvl_4,
                                           btn_lvl_5, btn_lvl_6, btn_lvl_7, btn_lvl_8};
    UIButton optionButtons[4];
};

#endif