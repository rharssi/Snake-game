#include "food.h"
#include "game.h"
#include "ui.h"
#include "math_utils.h"
#include <SDL3/SDL.h>
#include <stdlib.h>

void GenerationFood(GameLogic* game) {  // Enlevez le cast inutile
    SDL_Point freeCells[ (SCREEN_WIDTH/GRID_SIZE) * ((SCREEN_HEIGHT-HEADER_HEIGHT)/GRID_SIZE) ];
    int freeCount = 0;

    for (int x = 0; x < SCREEN_WIDTH; x += GRID_SIZE) {
        for (int y = HEADER_HEIGHT; y < SCREEN_HEIGHT; y += GRID_SIZE) {
            if (!IsPositionOccupied(game, x, y)) {
                freeCells[freeCount++] = (SDL_Point){x, y};
            }
        }
    }

    if (freeCount > 0) {
        int choice = rand() % freeCount;
        game->food = freeCells[choice];
    } else {
        SDL_Log("AVERTISSEMENT: Aucune case libre pour la nourriture!");
        game->food = (SDL_Point){GRID_SIZE * 5, HEADER_HEIGHT + GRID_SIZE * 5};
    }
}

void GenerationGoldenFood(GameLogic* game) {
    int attempts = 0;
    do {
        game->goldenFood.x = (rand() % (SCREEN_WIDTH / GRID_SIZE)) * GRID_SIZE;
        game->goldenFood.y = HEADER_HEIGHT + (rand() % ((SCREEN_HEIGHT - HEADER_HEIGHT) / GRID_SIZE)) * GRID_SIZE;
        attempts++;
    } while (IsPositionOccupied(game, game->goldenFood.x, game->goldenFood.y) && attempts < 100);
    game->goldenActive = true;
    game->goldenGenerationTime = SDL_GetTicks();
}

void GenerationRedFood(GameLogic* game) {
    int attempts = 0;
    do {
        game->redFood.x = (rand() % (SCREEN_WIDTH / GRID_SIZE)) * GRID_SIZE;
        game->redFood.y = HEADER_HEIGHT + (rand() % ((SCREEN_HEIGHT - HEADER_HEIGHT) / GRID_SIZE)) * GRID_SIZE;
        attempts++;
    } while (IsPositionOccupied(game, game->redFood.x, game->redFood.y) && attempts < 100);
    game->redActive = true;
    game->redGenerationTime = SDL_GetTicks();
}

void GenerationBarrier(GameLogic* game) {
    if (game->barrierCount >= MAX_BARRIERS) return;

    int length = 3 + (rand() % 4);
    bool horizontal = rand() % 2;

    int foodGridX = game->food.x / GRID_SIZE;
    int foodGridY = (game->food.y - HEADER_HEIGHT) / GRID_SIZE;

    int maxAttempts = 50;
    int x = 0, y = 0;
    bool validPosition = false;

    for (int attempt = 0; attempt < maxAttempts; attempt++) {
        int offsetX = -8 + (rand() % 17);
        int offsetY = -8 + (rand() % 17);

        int gridX = foodGridX + offsetX;
        int gridY = foodGridY + offsetY;

        if (horizontal) {
            if (gridX < 0) gridX = 0;
            if (gridX + length >= SCREEN_WIDTH / GRID_SIZE)
                gridX = SCREEN_WIDTH / GRID_SIZE - length;
        } else {
            if (gridY < 0) gridY = 0;
            if (gridY + length >= (SCREEN_HEIGHT - HEADER_HEIGHT) / GRID_SIZE)
                gridY = (SCREEN_HEIGHT - HEADER_HEIGHT) / GRID_SIZE - length;
        }

        x = gridX * GRID_SIZE;
        y = HEADER_HEIGHT + gridY * GRID_SIZE;

        validPosition = true;
        for (int i = 0; i < length; i++) {
            int checkX = x + (horizontal ? i * GRID_SIZE : 0);
            int checkY = y + (horizontal ? 0 : i * GRID_SIZE);

            if (IsPositionOccupied(game, checkX, checkY)) {
                validPosition = false;
                break;
            }
        }

        if (validPosition) break;
    }

    if (validPosition) {
        game->barriers[game->barrierCount].x = x;
        game->barriers[game->barrierCount].y = y;
        game->barriers[game->barrierCount].length = length;
        game->barriers[game->barrierCount].horizontal = horizontal;
        game->barriers[game->barrierCount].active = true;
        game->barriers[game->barrierCount].GenerationAnim = 0.0f;
        game->barrierCount++;

        for (int i = 0; i < length; i++) {
            float px = (float)(x + (horizontal ? i * GRID_SIZE : 0)) + GRID_SIZE/2;
            float py = (float)(y + (horizontal ? 0 : i * GRID_SIZE)) + GRID_SIZE/2;
            for(int j=0; j<10; j++) {
                GenerationParticle(px, py, 255, 100, 50);
            }
        }
    }
}