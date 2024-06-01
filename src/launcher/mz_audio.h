#ifndef MZ_AUDIO
#define MZ_AUDIO

#include "raylib.h"

class MZAudio
{
public:
    MZAudio();
    ~MZAudio();

    void loadResources();

    void playHoverSound();
    void playAcceptSound();
    void playCancelSound();
    void playErrorSound();
    void playInputSound();

private:
    Music music;
    Sound hover;
    Sound accept;
    Sound cancel;
    Sound error;
    Sound input;
};

#endif