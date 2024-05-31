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
    UnloadMusicStream(music);
    CloseAudioDevice();
}

void MZAudio::loadResources()
{
    music = LoadMusicStream("res/msc_launcher.mp3");
    hover = LoadSound("res/snd_hover.ogg");
    accept = LoadSound("res/snd_accept.ogg");
    cancel = LoadSound("res/snd_cancel.ogg");
    error = LoadSound("res/snd_error.ogg");
    input = LoadSound("res/snd_input.ogg");
    SetMusicVolume(music, 0.5);
    SetSoundVolume(hover, 0.2f);
    SetSoundVolume(accept, 0.2f);
    SetSoundVolume(cancel, 0.2f);
    SetSoundVolume(error, 0.2f);
    SetSoundVolume(input, 0.1f);

    PlayMusicStream(music);
}

void MZAudio::updateAudio()
{
    UpdateMusicStream(music);
}

void MZAudio::setMusicVolume(float vol)
{
    SetMusicVolume(music, vol);
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