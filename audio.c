#include "audio.h"
#include <SDL3/SDL.h>

SDL_AudioStream *streamMusic = NULL;
SDL_AudioStream *streamSfx = NULL;
AudioData wavJeu = {0}, wavNour = {0}, wavCol = {0}, wavClick = {0}, wavGameOver = {0};

void LoadWAVFile(const char* filename, AudioData* data) {
    if (!SDL_LoadWAV(filename, &data->spec, &data->buffer, &data->length)) {
        SDL_Log("Erreur chargement WAV %s: %s", filename, SDL_GetError());
    }
}

void InitAudio() {
    SDL_AudioSpec wantedSpec = { SDL_AUDIO_S16, 2, 44100 };

    streamMusic = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &wantedSpec, NULL, NULL);
    streamSfx = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &wantedSpec, NULL, NULL);

    if (!streamMusic || !streamSfx) {
        SDL_Log("Erreur creation stream audio: %s", SDL_GetError());
    } else {
        SDL_ResumeAudioStreamDevice(streamMusic);
        SDL_ResumeAudioStreamDevice(streamSfx);
    }

    LoadWAVFile("jeu.wav", &wavJeu);
    LoadWAVFile("nour.wav", &wavNour);
    LoadWAVFile("col.wav", &wavCol);
    LoadWAVFile("click.wav", &wavClick);
    LoadWAVFile("gameover.wav", &wavGameOver);
}

void PlayMusic(bool enabled) {
    if (!streamMusic || !wavJeu.buffer || !enabled) return;
    SDL_SetAudioStreamFormat(streamMusic, &wavJeu.spec, NULL);
    SDL_ClearAudioStream(streamMusic);
    SDL_PutAudioStreamData(streamMusic, wavJeu.buffer, wavJeu.length);
}

void StopMusic() {
    if (streamMusic) SDL_ClearAudioStream(streamMusic);
}

void PlaySFX(AudioData* wav, bool enabled) {
    if (!streamSfx || !wav->buffer || !enabled) return;
    SDL_SetAudioStreamFormat(streamSfx, &wav->spec, NULL);
    SDL_PutAudioStreamData(streamSfx, wav->buffer, wav->length);
}

void CleanAudio() {
    if (wavJeu.buffer) SDL_free(wavJeu.buffer);
    if (wavNour.buffer) SDL_free(wavNour.buffer);
    if (wavCol.buffer) SDL_free(wavCol.buffer);
    if (wavClick.buffer) SDL_free(wavClick.buffer);
    if (wavGameOver.buffer) SDL_free(wavGameOver.buffer);
    SDL_DestroyAudioStream(streamMusic);
    SDL_DestroyAudioStream(streamSfx);
}