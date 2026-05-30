#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>
#include "game.h"
#include "state.h"

void HandleMenuInput(SDL_Event* ev, GameLogic* game, GameState* currentState, SDL_FRect btnMusicRect, SDL_FRect btnSoundRect, float totalTime);
void HandleDifficultySelectInput(SDL_Event* ev, GameLogic* game, GameState* currentState);
void HandleGameInput(SDL_Event* ev, GameLogic* game, GameState* currentState, SDL_FRect btnGameBack, SDL_FRect btnGamePause, SDL_FRect btnGameRetry);
void HandleGameOverInput(SDL_Event* ev, GameLogic* game, GameState* currentState);

#endif // INPUT_H