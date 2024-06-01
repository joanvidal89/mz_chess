#ifndef SCENE_MENU_GAMEEND
#define SCENE_MENU_GAMEEND

#include <iostream>
#include <sstream>
#include <string>

#include "../mz_struct.h"
#include "mz_scene.h"

class SceneMenuGameEnd : public MZScene
{
    //------------------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    //------------------------------------------------------------------------------------
public:
    SceneMenuGameEnd(MZDirector *director);
    ~SceneMenuGameEnd();

    //------------------------------------------------------------------------------------
    // SCENE UPDATING
    //------------------------------------------------------------------------------------
    void update() override;

    void ssMenuAwait();
    void ssMenuIn();
    void ssMenu();
    void ssMenuOut();

    void performOptionButtonAction(UIButton opt);
    void performBackButtonAction();

private:
    SceneState currentState;
    SceneType futureScene;
    float deltaTime;

    int lastHoverPlayId;
    int currentHoverPlayId;
    int lastHoverBtnId;
    int currentHoverBtnId;

    const Vector2 TITLE_LOCATION = Vector2{76, 4};
    const Vector2 RESULT_LOCATION = Vector2{82, 48};
    const Vector2 SCORE_TEXT_LOCATION = Vector2{82, 80};
    const Vector2 SCORE_LOCATION = Vector2{82, 92};
    const Vector2 RANK_TEXT_LOCATION = Vector2{82, 120};
    const Vector2 RANK_LOCATION = Vector2{82, 132};
    const char *SCORE_TEXT = "SCORE: ";
    const char *RANK_TEXT = "RANKED:";

    std::string gameResult = "";
    std::string opponentId = "";
    std::string score = "";
    std::string rank;

    const char *OPP_NAMES[9] = {
        "", "MR.CHICKEN", "BILLI BOY", "THE SHOWMAN", "SHARON PARKS", "BUCKET KNIGHT", "MS ROBINSON",
        "OLD MARCUS", "????"};
    const char *opponentName = OPP_NAMES[0];

    const Rectangle FRAME_LOCATION = Rectangle{12, 4, 56, 56};
    const Vector2 ICON_OFFSET = Vector2{16, 8};
    const UIOpponent LVL_0 = {0, Rectangle{2, 58, 48, 48}, FRAME_LOCATION, Rectangle{0}, ICON_OFFSET, "MR.CHICKEN"};
    const UIOpponent LVL_1 = {1, Rectangle{2, 58, 48, 48}, FRAME_LOCATION, Rectangle{0}, ICON_OFFSET, "MR.CHICKEN"};
    const UIOpponent LVL_2 = {2, Rectangle{54, 58, 48, 48}, FRAME_LOCATION, Rectangle{0}, ICON_OFFSET, "BILLY BOY"};
    const UIOpponent LVL_3 = {3, Rectangle{106, 58, 48, 48}, FRAME_LOCATION, Rectangle{0}, ICON_OFFSET, "THE SHOWMAN"};
    const UIOpponent LVL_4 = {4, Rectangle{158, 58, 48, 48}, FRAME_LOCATION, Rectangle{0}, ICON_OFFSET, "SHARON PARKS"};
    const UIOpponent LVL_5 = {5, Rectangle{2, 110, 48, 48}, FRAME_LOCATION, Rectangle{0}, ICON_OFFSET, "BUCKET KNIGHT"};
    const UIOpponent LVL_6 = {6, Rectangle{54, 110, 48, 48}, FRAME_LOCATION, Rectangle{0}, ICON_OFFSET, "MS. ROBINSON"};
    const UIOpponent LVL_7 = {7, Rectangle{106, 110, 48, 48}, FRAME_LOCATION, Rectangle{0}, ICON_OFFSET, "OLD MARCUS"};
    const UIOpponent LVL_8 = {8, Rectangle{158, 110, 48, 48}, FRAME_LOCATION, Rectangle{0}, ICON_OFFSET, "????"};
    const UIOpponent OPP_FRAMES[9] = {LVL_0, LVL_1, LVL_2, LVL_3, LVL_4, LVL_5, LVL_6, LVL_7, LVL_8};
    UIOpponent opponentFrame;

    const UIButton BTN_BACK = {1, Rectangle{390, 94, 32, 32}, Rectangle{8, 64, 64, 32},
                               Rectangle{PANEL_OFFSET.x + 8, PANEL_OFFSET.y + 64, 64, 32}, Vector2{8, 64}, " OK"};

    int id;
    Rectangle iconRect;
    Rectangle drawRect;
    Rectangle collision;
    Vector2 iconOffset;

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
};

#endif