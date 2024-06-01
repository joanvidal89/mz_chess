#ifndef SCENE_GAME
#define SCENE_GAME

#include <string>

#include "../mz_struct.h"
#include "mz_scene.h"

class SceneGame : public MZScene
{
public:
    SceneGame(MZDirector *director);
    ~SceneGame();

    void update() override;

private:
    SceneState currentState;
    SceneType futureScene;
    float deltaTime;

    Cell *currentHoverCell;
    Cell *lastHoverCell;
    Cell *currentSelectedCell;
    PieceMove *currentPieceMove;
    PieceMove currentAIMove;

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

    Rectangle GAME_PANEL = Rectangle{492, 400, 96, 280};

    GameButton gameButtons[5] = {
        GameButton{1, Rectangle{302, 2, 42, 42}, Rectangle{516, 622, 48, 46}, Vector2{519, 623}, BS_DISABLED},
        GameButton{2, Rectangle{130, 214, 32, 32}, Rectangle{507, 551, 32, 32}, Vector2{507, 551}, BS_DISABLED},
        GameButton{3, Rectangle{166, 214, 32, 32}, Rectangle{541, 551, 32, 32}, Vector2{541, 551}, BS_DISABLED},
        GameButton{4, Rectangle{202, 214, 32, 32}, Rectangle{507, 584, 32, 32}, Vector2{507, 584}, BS_DISABLED},
        GameButton{5, Rectangle{238, 214, 32, 32}, Rectangle{541, 584, 32, 32}, Vector2{541, 584}, BS_DISABLED},
    };

    void ssInit();
    void ssOut();
    void preparePlayerTurn();
    void ssAwaitAI();

    void ssPlayerTurn();
    void ssPlayerTurnPromote();
    void ssPlayerAnimation();
    void ssAIAnimation();
    void drawDefaultRender();

    void handleEndgame(int result);
    void checkButtonClick();
    void performButtonClick(PieceState state);
};

#endif