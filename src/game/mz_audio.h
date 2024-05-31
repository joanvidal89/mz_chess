#ifndef MZ_AUDIO
#define MZ_AUDIO

#include "raylib.h"

class MZAudio
{
public:
    MZAudio();
    ~MZAudio();

    void loadResources();
    void switchMusicOn(bool on);
    void switchAudioOn(bool on);

    void playIntroMusic();
    void setIntroMusicVolume(float vol);
    void playHoverSound();
    void playAcceptSound();
    void playCancelSound();
    void playVictorySound();
    void playDefeatSound();
    void playIntroSound();

    void updateAudio();

private:
    Music music;

    Sound hover;
    Sound accept;
    Sound cancel;
    Sound victory;
    Sound defeat;
    Sound intro;

    bool musicOn;
    bool audioOn;
};

#endif