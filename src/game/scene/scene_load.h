#ifndef SCENE_LOAD
#define SCENE_LOAD

#include "../mz_struct.h"
#include "mz_scene.h"

class SceneLoad : public MZScene
{
    //------------------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    //------------------------------------------------------------------------------------
public:
    SceneLoad(MZDirector *director);
    ~SceneLoad();

    //------------------------------------------------------------------------------------
    // SCENE UPDATING
    //------------------------------------------------------------------------------------
    void update() override;

private:
    float deltaTime;

    void ssInit();
    void ssLoadData();
    void ssLoadDataEnd();
    void ssTransitionIntro();

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