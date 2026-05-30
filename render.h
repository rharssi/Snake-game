#ifndef RENDER_H
#define RENDER_H

#include <SDL3/SDL.h>
#include "game.h"

void RenderMenu(SDL_Renderer* ren, GameLogic* game, SDL_FRect btnMusicRect, SDL_FRect btnSoundRect, float totalTime);
void RenderDifficultySelect(SDL_Renderer* ren, float totalTime);
void RenderGame(SDL_Renderer* ren, GameLogic* game, SDL_FRect btnGameBack, SDL_FRect btnGamePause, SDL_FRect btnGameRetry, float totalTime);
void RenderGameOver(SDL_Renderer* ren, GameLogic* game, float totalTime);

#endif // RENDER_H