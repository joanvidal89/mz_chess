#include "mz_audio.h"

MZAudio::MZAudio()
{
    InitAudioDevice();
}

MZAudio::~MZAudio()
{
    UnloadSound(hover);
    UnloadSound(accept);
    UnloadSound(cancel);
    UnloadSound(error);
    UnloadSound(input);
    CloseAudioDevice();
}

void MZAudio::loadResources()
{
    hover = LoadSound("res/snd_hover.ogg");
    accept = LoadSound("res/snd_accept.ogg");
    cancel = LoadSound("res/snd_cancel.ogg");
    error = LoadSound("res/snd_error.ogg");
    input = LoadSound("res/snd_input.ogg");
    SetSoundVolume(hover, 0.1f);
    SetSoundVolume(accept, 0.1f);
    SetSoundVolume(cancel, 0.1f);
    SetSoundVolume(error, 0.1f);
    SetSoundVolume(input, 0.1f);
}

void MZAudio::playHoverSound()
{
    int modularity = GetRandomValue(-10, 10);
    SetSoundPitch(hover, 1.0f + (modularity * 0.01f));
    PlaySound(hover);
}

void MZAudio::playAcceptSound()
{
    int modularity = GetRandomValue(-10, 10);
    SetSoundPitch(accept, 1.0f + (modularity * 0.01f));
    PlaySound(accept);
}

void MZAudio::playCancelSound()
{
    int modularity = GetRandomValue(-10, 10);
    SetSoundPitch(cancel, 1.0f + (modularity * 0.01f));
    PlaySound(cancel);
}

void MZAudio::playErrorSound()
{
    int modularity = GetRandomValue(-10, 10);
    SetSoundPitch(error, 1.0f + (modularity * 0.01f));
    PlaySound(error);
}

void MZAudio::playInputSound()
{
    int modularity = GetRandomValue(-10, 10);
    SetSoundPitch(input, 1.0f + (modularity * 0.01f));
    PlaySound(input);
}