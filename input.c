#include "input.h"
#include "audio.h"
#include <string.h>
#include <ctype.h>

void HandleMenuInput(SDL_Event* ev, GameLogic* game, GameState* currentState, SDL_FRect btnMusicRect, SDL_FRect btnSoundRect, float totalTime) {
    float formY = SCREEN_HEIGHT/2.0f + 40;
    float boxWidth = 500;
    float boxHeight = 60;
    float boxX = SCREEN_WIDTH/2.0f - boxWidth/2.0f;
    SDL_FRect nameBoxRect = {boxX, formY, boxWidth, boxHeight};
    SDL_FRect pinBoxRect = {boxX, formY + 80, boxWidth, boxHeight};

    if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mx = ev->button.x;
        float my = ev->button.y;

        if (mx >= btnMusicRect.x && mx <= btnMusicRect.x + btnMusicRect.w &&
            my >= btnMusicRect.y && my <= btnMusicRect.y + btnMusicRect.h) {
            game->musicEnabled = !game->musicEnabled;
            PlaySFX(&wavClick, game->soundEnabled);
        }

        if (mx >= btnSoundRect.x && mx <= btnSoundRect.x + btnSoundRect.w &&
            my >= btnSoundRect.y && my <= btnSoundRect.y + btnSoundRect.h) {
            game->soundEnabled = !game->soundEnabled;
            PlaySFX(&wavClick, true);
        }

        if (mx >= nameBoxRect.x && mx <= nameBoxRect.x + nameBoxRect.w &&
            my >= nameBoxRect.y && my <= nameBoxRect.y + nameBoxRect.h) {
            game->menuInputFocus = 0;
            PlaySFX(&wavClick, game->soundEnabled);
        }

        if (mx >= pinBoxRect.x && mx <= pinBoxRect.x + pinBoxRect.w &&
            my >= pinBoxRect.y && my <= pinBoxRect.y + pinBoxRect.h) {
            game->menuInputFocus = 1;
            PlaySFX(&wavClick, game->soundEnabled);
        }
    }

    if (ev->type == SDL_EVENT_TEXT_INPUT) {
        if (game->menuInputFocus == 0) {
            if (strlen(game->playerName) < MAX_NAME_LENGTH) {
                if (strcmp(game->playerName, "NAME") == 0) game->playerName[0] = '\0';
                strcat(game->playerName, ev->text.text);
            }
        } else if (game->menuInputFocus == 1) {
            char c = ev->text.text[0];
            if (strlen(game->playerPin) < MAX_PIN_LENGTH && isdigit(c)) {
                 strcat(game->playerPin, ev->text.text);
            }
        }
    }
    if (ev->type == SDL_EVENT_KEY_DOWN) {
        if (ev->key.key == SDLK_BACKSPACE) {
            if (game->menuInputFocus == 0 && strlen(game->playerName) > 0) {
                 game->playerName[strlen(game->playerName) - 1] = '\0';
            } else if (game->menuInputFocus == 1 && strlen(game->playerPin) > 0) {
                 game->playerPin[strlen(game->playerPin) - 1] = '\0';
            }
        }
        else if (ev->key.key == SDLK_TAB) {
             game->menuInputFocus = (game->menuInputFocus == 0) ? 1 : 0;
             PlaySFX(&wavClick, game->soundEnabled);
        }
        else if (ev->key.key == SDLK_UP) {
            game->menuInputFocus = 0;
            PlaySFX(&wavClick, game->soundEnabled);
        }
        else if (ev->key.key == SDLK_DOWN) {
            game->menuInputFocus = 1;
            PlaySFX(&wavClick, game->soundEnabled);
        }
        else if (ev->key.key == SDLK_RETURN) {
            if (game->menuInputFocus == 0) {
                 game->menuInputFocus = 1;
                 PlaySFX(&wavClick, game->soundEnabled);
            }
            else {
                if (strlen(game->playerName) > 0 && strlen(game->playerPin) > 0) {
                    PlaySFX(&wavClick, game->soundEnabled);
                    LoadUserBestScore(game);
                    *currentState = STATE_DIFFICULTY_SELECT;
                }
            }
        }
    }
}

void HandleDifficultySelectInput(SDL_Event* ev, GameLogic* game, GameState* currentState) {
    if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float my = ev->button.y;
        if (my >= 210 && my <= 510) {
            float mx = ev->button.x;
            float cardWidth = 290;
            float spacing = 35;
            float startX = SCREEN_WIDTH/2.0f - (cardWidth * 3 + spacing * 2) / 2.0f;

            bool clicked = false;
            if (mx >= startX && mx <= startX + cardWidth) {
                game->difficulty = DIFFICULTY_EASY;
                clicked = true;
            }
            else if (mx >= startX + cardWidth + spacing && mx <= startX + 2*cardWidth + spacing) {
                game->difficulty = DIFFICULTY_NORMAL;
                clicked = true;
            }
            else if (mx >= startX + 2*(cardWidth + spacing) && mx <= startX + 3*cardWidth + 2*spacing) {
                game->difficulty = DIFFICULTY_HARD;
                clicked = true;
            }

            if (clicked) {
                PlaySFX(&wavClick, game->soundEnabled);
                ResetGame(game);
                *currentState = STATE_GAME;
                PlayMusic(game->musicEnabled);
            }
        }
    }
    if (ev->type == SDL_EVENT_KEY_DOWN) {
        if (ev->key.key == SDLK_1 || ev->key.key == SDLK_2 || ev->key.key == SDLK_3) {
            PlaySFX(&wavClick, game->soundEnabled);
            if (ev->key.key == SDLK_1) game->difficulty = DIFFICULTY_EASY;
            else if (ev->key.key == SDLK_2) game->difficulty = DIFFICULTY_NORMAL;
            else if (ev->key.key == SDLK_3) game->difficulty = DIFFICULTY_HARD;

            ResetGame(game);
            *currentState = STATE_GAME;
            PlayMusic(game->musicEnabled);
        }
        else if (ev->key.key == SDLK_ESCAPE) {
            PlaySFX(&wavClick, game->soundEnabled);
            *currentState = STATE_MENU;
        }
    }
}

void HandleGameInput(SDL_Event* ev, GameLogic* game, GameState* currentState, SDL_FRect btnGameBack, SDL_FRect btnGamePause, SDL_FRect btnGameRetry) {
    if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mx = ev->button.x;
        float my = ev->button.y;

        if (mx >= btnGameBack.x && mx <= btnGameBack.x + btnGameBack.w &&
            my >= btnGameBack.y && my <= btnGameBack.y + btnGameBack.h) {
             PlaySFX(&wavClick, game->soundEnabled);
             StopMusic();
             *currentState = STATE_DIFFICULTY_SELECT;
        }

        if (mx >= btnGamePause.x && mx <= btnGamePause.x + btnGamePause.w &&
            my >= btnGamePause.y && my <= btnGamePause.y + btnGamePause.h) {
             game->isPaused = !game->isPaused;
             PlaySFX(&wavClick, game->soundEnabled);
        }

        if (mx >= btnGameRetry.x && mx <= btnGameRetry.x + btnGameRetry.w &&
            my >= btnGameRetry.y && my <= btnGameRetry.y + btnGameRetry.h) {
             PlaySFX(&wavClick, game->soundEnabled);
             ResetGame(game);
        }
    }

    if (ev->type == SDL_EVENT_KEY_DOWN) {
        Direction next_input = -1;
        switch (ev->key.key) {
            case SDLK_UP:    next_input = UP;    break;
            case SDLK_DOWN:  next_input = DOWN;  break;
            case SDLK_LEFT:  next_input = LEFT;  break;
            case SDLK_RIGHT: next_input = RIGHT; break;

            case SDLK_ESCAPE:
                PlaySFX(&wavClick, game->soundEnabled);
                StopMusic();
                *currentState = STATE_MENU;
                break;
            case SDLK_B:
                PlaySFX(&wavClick, game->soundEnabled);
                StopMusic();
                *currentState = STATE_DIFFICULTY_SELECT;
                break;
            case SDLK_P:
                game->isPaused = !game->isPaused;
                break;
            case SDLK_R:
                ResetGame(game);
                break;
        }

        if (next_input != -1 && game->inputCount < 3) {
            Direction last = (game->inputCount == 0) ? game->dir : game->inputBuffer[game->inputCount - 1];
            if (!((next_input == UP && last == DOWN) || (next_input == DOWN && last == UP) ||
                  (next_input == LEFT && last == RIGHT) || (next_input == RIGHT && last == LEFT) ||
                  (next_input == last))) {
                game->inputBuffer[game->inputCount++] = next_input;
            }
        }
    }
}

void HandleGameOverInput(SDL_Event* ev, GameLogic* game, GameState* currentState) {
    float btnY = 620;
    SDL_FRect rectRestart = {SCREEN_WIDTH/2.0f - 180, btnY, 140, 50};
    SDL_FRect rectMenu = {SCREEN_WIDTH/2.0f + 40, btnY, 140, 50};

    if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mx = ev->button.x;
        float my = ev->button.y;

        if (mx >= rectRestart.x && mx <= rectRestart.x + rectRestart.w &&
            my >= rectRestart.y && my <= rectRestart.y + rectRestart.h) {
            PlaySFX(&wavClick, game->soundEnabled);
            ResetGame(game);
            *currentState = STATE_GAME;
            PlayMusic(game->musicEnabled);
        }

        if (mx >= rectMenu.x && mx <= rectMenu.x + rectMenu.w &&
            my >= rectMenu.y && my <= rectMenu.y + rectMenu.h) {
            PlaySFX(&wavClick, game->soundEnabled);
            *currentState = STATE_MENU;
        }
    }

    if (ev->type == SDL_EVENT_KEY_DOWN) {
        if (ev->key.key == SDLK_R || ev->key.key == SDLK_RETURN) {
            PlaySFX(&wavClick, game->soundEnabled);
            ResetGame(game);
            *currentState = STATE_GAME;
            PlayMusic(game->musicEnabled);
        }
        if (ev->key.key == SDLK_ESCAPE) {
            PlaySFX(&wavClick, game->soundEnabled);
            *currentState = STATE_MENU;
        }
    }
}