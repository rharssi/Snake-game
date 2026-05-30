#ifndef AUDIO_H
#define AUDIO_H

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
    Uint8 *buffer;
    Uint32 length;
    SDL_AudioSpec spec;
} AudioData;

extern SDL_AudioStream *streamMusic;
extern SDL_AudioStream *streamSfx;
extern AudioData wavJeu, wavNour, wavCol, wavClick, wavGameOver;

void LoadWAVFile(const char* filename, AudioData* data);
void InitAudio();
void PlayMusic(bool enabled);
void StopMusic();
void PlaySFX(AudioData* wav, bool enabled);
void CleanAudio();

#endif // AUDIO_H