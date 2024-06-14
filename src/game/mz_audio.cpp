#include "mz_audio.h"

MZAudio::MZAudio()
{
    InitAudioDevice();
}

MZAudio::~MZAudio()
{
    UnloadMusicStream(music);
    UnloadSound(intro);
    UnloadSound(hover);
    UnloadSound(accept);
    UnloadSound(cancel);
    UnloadSound(victory);
    UnloadSound(defeat);
    CloseAudioDevice();
}

void MZAudio::loadResources()
{
    music = LoadMusicStream("res/msc_launcher.mp3");
    intro = LoadSound("res/msc_intro.mp3");
    hover = LoadSound("res/snd_hover.ogg");
    accept = LoadSound("res/snd_accept.ogg");
    cancel = LoadSound("res/snd_cancel.ogg");
    victory = LoadSound("res/snd_victory.ogg");
    defeat = LoadSound("res/snd_defeat.ogg");
    SetSoundVolume(hover, 0.1f);
    SetSoundVolume(accept, 0.1f);
    SetSoundVolume(cancel, 0.1f);
    SetSoundVolume(victory, 0.1f);
    SetSoundVolume(defeat, 0.1f);
    SetMusicVolume(music, 0.8f);
}

void MZAudio::switchMusicOn(bool on)
{
    musicOn = on;
}

void MZAudio::switchAudioOn(bool on)
{
    audioOn = on;
}

void MZAudio::playIntroMusic()
{
    PlayMusicStream(music);
}

void MZAudio::setIntroMusicVolume(float vol)
{
    SetMusicVolume(music, vol);
}

void MZAudio::playHoverSound()
{
    if (audioOn)
    {
        int modularity = GetRandomValue(-10, 10);
        SetSoundPitch(hover, 1.0f + (modularity * 0.01f));
        PlaySound(hover);
    }
}

void MZAudio::playAcceptSound()
{
    if (audioOn)
    {
        int modularity = GetRandomValue(-10, 10);
        SetSoundPitch(accept, 1.0f + (modularity * 0.01f));
        PlaySound(accept);
    }
}

void MZAudio::playCancelSound()
{
    if (audioOn)
    {
        int modularity = GetRandomValue(-10, 10);
        SetSoundPitch(cancel, 1.0f + (modularity * 0.01f));
        PlaySound(cancel);
    }
}

void MZAudio::playVictorySound()
{
    if (audioOn)
    {
        PlaySound(victory);
    }
}

void MZAudio::playDefeatSound()
{
    if (audioOn)
    {
        PlaySound(defeat);
    }
}

void MZAudio::playIntroSound()
{
    if (audioOn)
        PlaySound(intro);
}

void MZAudio::updateAudio()
{
    if (musicOn)
        UpdateMusicStream(music);
}
