#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "game.h"
#include "state.h"
#include "audio.h"
#include "ui.h"
#include "input.h"
#include "render.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Window* win = SDL_CreateWindow("Snake - Pro Login System", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);
    srand((unsigned int)time(NULL));
    InitParticles();

    InitAudio();

    GameLogic game = {0};
    strcpy(game.playerName, "NAME");
    game.playerPin[0] = '\0';
    game.menuInputFocus = 0;

    game.bestScore = 0;
    game.difficulty = DIFFICULTY_NORMAL;

    game.musicEnabled = true;
    game.soundEnabled = true;

    GameState currentState = STATE_MENU;
    bool run = true;
    SDL_Event ev;

    SDL_StartTextInput(win);

    SDL_FRect btnMusicRect = { SCREEN_WIDTH/2.0f - 220, 20, 200, 40 };
    SDL_FRect btnSoundRect = { SCREEN_WIDTH/2.0f + 20, 20, 200, 40 };

    float headerRightStart = SCREEN_WIDTH - 240;
    float btnSize = 50;
    float btnSpacing = 10;
    SDL_FRect btnGameBack   = { headerRightStart + 40, 20, btnSize, btnSize };
    SDL_FRect btnGamePause  = { headerRightStart + 40 + btnSize + btnSpacing, 20, btnSize, btnSize };
    SDL_FRect btnGameRetry  = { headerRightStart + 40 + (btnSize + btnSpacing)*2, 20, btnSize, btnSize };

    while (run) {
        float totalTime = SDL_GetTicks() / 1000.0f;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_QUIT) run = false;

            if (currentState == STATE_MENU) {
                HandleMenuInput(&ev, &game, &currentState, btnMusicRect, btnSoundRect, totalTime);
            }
            else if (currentState == STATE_DIFFICULTY_SELECT) {
                HandleDifficultySelectInput(&ev, &game, &currentState);
            }
            else if (currentState == STATE_GAME) {
                HandleGameInput(&ev, &game, &currentState, btnGameBack, btnGamePause, btnGameRetry);
            }
            else if (currentState == STATE_GAMEOVER) {
                HandleGameOverInput(&ev, &game, &currentState);
            }
        }

        if (currentState == STATE_GAME && game.musicEnabled && !game.isPaused) {
             if (streamMusic && SDL_GetAudioStreamAvailable(streamMusic) == 0) {
                 SDL_PutAudioStreamData(streamMusic, wavJeu.buffer, wavJeu.length);
             }
        }

        if (currentState == STATE_DYING) {
             Uint64 now = SDL_GetTicks();
             if (now - game.dyingStartTime > 1000) {
                 PlaySFX(&wavGameOver, game.soundEnabled);
                 SaveUserBestScore(&game);
                 currentState = STATE_GAMEOVER;
             }
        }

        if (currentState == STATE_GAME && !game.isPaused) {
            Uint64 now = SDL_GetTicks();

            if (!game.goldenActive && (now - game.goldenGenerationTime) > 10000) {
                GenerationGoldenFood(&game);
            }

            if (!game.redActive && (now - game.redGenerationTime) > 15000) {
                GenerationRedFood(&game);
            }

            int expectedBarriers = game.score / 50;
            if (expectedBarriers > MAX_BARRIERS) expectedBarriers = MAX_BARRIERS;

            switch(game.difficulty) {
                case DIFFICULTY_EASY:   expectedBarriers = (int)(expectedBarriers * 0.5); break;
                case DIFFICULTY_HARD:   expectedBarriers = (int)(expectedBarriers * 1.5); break;
                default: break;
            }

            if (game.barrierCount < expectedBarriers && (now - game.lastBarrierGenerationTime) > 3000) {
                GenerationBarrier(&game);
                game.lastBarrierGenerationTime = now;
            }

            for (int i = 0; i < game.barrierCount; i++) {
                if (game.barriers[i].GenerationAnim < 1.0f) {
                    game.barriers[i].GenerationAnim += 0.02f;
                }
            }

            if (now - game.lastStepTime > game.stepDelay) {
                if (game.inputCount > 0) {
                    game.dir = game.inputBuffer[0];
                    for (int i = 0; i < game.inputCount - 1; i++) game.inputBuffer[i] = game.inputBuffer[i+1];
                    game.inputCount--;
                }
                int next_x = game.head->x + (game.dir == RIGHT ? GRID_SIZE : game.dir == LEFT ? -GRID_SIZE : 0);
                int next_y = game.head->y + (game.dir == DOWN ? GRID_SIZE : game.dir == UP ? -GRID_SIZE : 0);

                bool collision = false;

                if (next_x < 0 || next_x >= SCREEN_WIDTH ||
                    next_y < HEADER_HEIGHT || next_y >= SCREEN_HEIGHT) {
                    collision = true;
                }

                if (!collision) {
                    Node* curr = game.head;
                    while(curr) {
                        if(curr->x == next_x && curr->y == next_y) {
                            collision = true;
                            break;
                        }
                        curr = curr->next;
                    }
                }

                if (!collision) {
                    for (int i = 0; i < game.barrierCount; i++) {
                        if (game.barriers[i].active) {
                            for (int j = 0; j < game.barriers[i].length; j++) {
                                int bx = game.barriers[i].x + (game.barriers[i].horizontal ? j * GRID_SIZE : 0);
                                int by = game.barriers[i].y + (game.barriers[i].horizontal ? 0 : j * GRID_SIZE);

                                if (bx == next_x && by == next_y) {
                                    collision = true;
                                    for(int k=0; k<30; k++) {
                                        GenerationParticle((float)next_x + GRID_SIZE/2, (float)next_y + GRID_SIZE/2, 255, 80, 50);
                                    }
                                    break;
                                }
                            }
                            if (collision) break;
                        }
                    }
                }

                if (collision) {
                    StopMusic();
                    PlaySFX(&wavCol, game.soundEnabled);

                    float headX = (float)game.head->x + GRID_SIZE/2.0f;
                    float headY = (float)game.head->y + GRID_SIZE/2.0f;
                    for(int k=0; k<80; k++) {
                        GenerationParticle(headX, headY, 200 + rand()%55, 0, 0);
                    }

                    game.dyingStartTime = SDL_GetTicks();
                    currentState = STATE_DYING;

                } else {
                    Node* new_head = malloc(sizeof(Node));
                    new_head->x = next_x;
                    new_head->y = next_y;
                    new_head->next = game.head;
                    game.head = new_head;

                    bool ateFood = false;
                    int segmentsToAdd = 0;

                    if (next_x == game.food.x && next_y == game.food.y) {
                        PlaySFX(&wavNour, game.soundEnabled);
                        game.score += 10;
                        if(game.score > game.bestScore) game.bestScore = game.score;
                        GenerationFood(&game);
                        for(int i=0; i<15; i++) GenerationParticle((float)next_x + GRID_SIZE/2, (float)next_y + GRID_SIZE/2, 0, 200, 255);
                        ateFood = true;
                    }

                    if (game.goldenActive && next_x == game.goldenFood.x && next_y == game.goldenFood.y) {
                        PlaySFX(&wavNour, game.soundEnabled);
                        game.score += 30;
                        if(game.score > game.bestScore) game.bestScore = game.score;
                        game.goldenActive = false;
                        game.goldenGenerationTime = SDL_GetTicks();
                        for(int i=0; i<25; i++) GenerationParticle((float)next_x + GRID_SIZE/2, (float)next_y + GRID_SIZE/2, 255, 215, 0);
                        ateFood = true;
                    }

                    if (game.redActive && next_x == game.redFood.x && next_y == game.redFood.y) {
                        PlaySFX(&wavNour, game.soundEnabled);
                        game.score += 10;
                        if(game.score > game.bestScore) game.bestScore = game.score;
                        game.redActive = false;
                        game.redGenerationTime = SDL_GetTicks();
                        segmentsToAdd = 3;
                        for(int i=0; i<20; i++) GenerationParticle((float)next_x + GRID_SIZE/2, (float)next_y + GRID_SIZE/2, 255, 80, 100);
                        ateFood = true;
                    }

                    if (!ateFood) {
                        Node* temp = game.head;
                        while (temp->next->next) temp = temp->next;
                        free(temp->next);
                        temp->next = NULL;
                    }

                    for (int i = 0; i < segmentsToAdd; i++) {
                        Node* tail = game.head;
                        while (tail->next) tail = tail->next;
                        Node* new_segment = malloc(sizeof(Node));
                        new_segment->x = tail->x;
                        new_segment->y = tail->y;
                        new_segment->next = NULL;
                        tail->next = new_segment;
                    }
                }
                game.lastStepTime = now;
            }
        }

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        DrawSpaceBackground(ren, game.isPaused);

        if (currentState == STATE_MENU) {
            RenderMenu(ren, &game, btnMusicRect, btnSoundRect, totalTime);
        }
        else if (currentState == STATE_DIFFICULTY_SELECT) {
            RenderDifficultySelect(ren, totalTime);
        }
        else if (currentState == STATE_GAME || currentState == STATE_DYING) {
            RenderGame(ren, &game, btnGameBack, btnGamePause, btnGameRetry, totalTime);
        }
        else if (currentState == STATE_GAMEOVER) {
            RenderGameOver(ren, &game, totalTime);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    CleanAudio();

    Node* curr = game.head;
    while(curr) { Node* next = curr->next; free(curr); curr = next; }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}