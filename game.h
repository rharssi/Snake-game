#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "state.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define HEADER_HEIGHT 90
#define GRID_SIZE 25
#define MAX_BARRIERS 20
#define MAX_PARTICLES 200
#define MAX_NAME_LENGTH 12
#define MAX_PIN_LENGTH 4
#define FILE_NAME "stockage.txt"

typedef struct Node {
    int x, y;
    struct Node* next;
} Node;

typedef struct {
    int x, y;
    bool active;
    float GenerationAnim;
    int length;
    bool horizontal;
} Barrier;

typedef struct {
    char name[MAX_NAME_LENGTH + 1];
    char pin[MAX_PIN_LENGTH + 1];
    int score;
} UserRecord;

typedef struct {
    Node* head;
    Direction dir;
    Direction inputBuffer[3];
    int inputCount;
    SDL_Point food;
    FoodType foodType;
    SDL_Point goldenFood;
    bool goldenActive;
    SDL_Point redFood;
    bool redActive;
    Barrier barriers[MAX_BARRIERS];
    int barrierCount;
    int score;
    int bestScore;
    bool isPaused;
    Uint64 gameStartTime;
    Uint64 lastStepTime;
    Uint64 goldenGenerationTime;
    Uint64 redGenerationTime;
    Uint64 lastBarrierGenerationTime;
    Uint64 dyingStartTime;
    char playerName[MAX_NAME_LENGTH + 1];
    char playerPin[MAX_PIN_LENGTH + 1];
    int menuInputFocus;
    DifficultyLevel difficulty;
    int stepDelay;
    bool musicEnabled;
    bool soundEnabled;
} GameLogic;

void LoadUserBestScore(GameLogic* game);
void SaveUserBestScore(GameLogic* game);
bool IsPositionOccupied(GameLogic* game, int x, int y);
void GenerationFood(GameLogic* game);
void GenerationGoldenFood(GameLogic* game);
void GenerationRedFood(GameLogic* game);
void GenerationBarrier(GameLogic* game);
void ResetGame(GameLogic* game);

#endif // GAME_H