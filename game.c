#include "game.h"
#include "barriers.h"
#include "ui.h"
#include "food.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void LoadUserBestScore(GameLogic* game) {
    FILE* f = fopen(FILE_NAME, "r");
    if (!f) return;

    char fName[MAX_NAME_LENGTH + 1];
    char fPin[MAX_PIN_LENGTH + 1];
    int fScore;

    game->bestScore = 0;

    while (fscanf(f, "%s %s %d", fName, fPin, &fScore) == 3) {
        if (strcmp(fName, game->playerName) == 0 && strcmp(fPin, game->playerPin) == 0) {
            game->bestScore = fScore;
            break;
        }
    }
    fclose(f);
}

void SaveUserBestScore(GameLogic* game) {
    FILE* f = fopen(FILE_NAME, "r");
    UserRecord records[100];
    int count = 0;
    bool userFound = false;

    if (f) {
        while (count < 100 && fscanf(f, "%s %s %d", records[count].name, records[count].pin, &records[count].score) == 3) {
            if (strcmp(records[count].name, game->playerName) == 0 && strcmp(records[count].pin, game->playerPin) == 0) {
                if (game->score > records[count].score) {
                    records[count].score = game->score;
                }
                userFound = true;
            }
            count++;
        }
        fclose(f);
    }

    if (!userFound && count < 100) {
        strcpy(records[count].name, game->playerName);
        strcpy(records[count].pin, game->playerPin);
        records[count].score = game->score;
        count++;
    }

    f = fopen(FILE_NAME, "w");
    if (f) {
        for (int i = 0; i < count; i++) {
            fprintf(f, "%s %s %d\n", records[i].name, records[i].pin, records[i].score);
        }
        fclose(f);
    }
}

bool IsPositionOccupied(GameLogic* game, int x, int y) {
    if (x == game->food.x && y == game->food.y) return true;
    if (game->goldenActive && x == game->goldenFood.x && y == game->goldenFood.y) return true;
    if (game->redActive && x == game->redFood.x && y == game->redFood.y) return true;

    Node* curr = game->head;
    while(curr) {
        if(curr->x == x && curr->y == y) return true;
        curr = curr->next;
    }

    for (int i = 0; i < game->barrierCount; i++) {
        if (game->barriers[i].active) {
            for (int j = 0; j < game->barriers[i].length; j++) {
                int bx = game->barriers[i].x + (game->barriers[i].horizontal ? j * GRID_SIZE : 0);
                int by = game->barriers[i].y + (game->barriers[i].horizontal ? 0 : j * GRID_SIZE);
                if (bx == x && by == y) return true;
            }
        }
    }

    return false;
}


void ResetGame(GameLogic* game) {
    // Libérer l'ancien serpent
    Node* curr = game->head;
    while(curr) {
        Node* next = curr->next;
        free(curr);
        curr = next;
    }
    
    game->head = NULL;
    game->dir = RIGHT;
    game->inputCount = 0;
    game->score = 0;
    game->isPaused = false;
    game->goldenActive = false;
    game->redActive = false;
    game->barrierCount = 0;
    game->gameStartTime = SDL_GetTicks();
    game->lastStepTime = SDL_GetTicks();
    game->goldenGenerationTime = SDL_GetTicks();
    game->redGenerationTime = SDL_GetTicks();
    game->lastBarrierGenerationTime = SDL_GetTicks();

    for (int i = 0; i < MAX_BARRIERS; i++) {
        game->barriers[i].active = false;
    }

    // Créer le serpent initial (4 segments)
    for (int i = 0; i < 4; i++) {
        Node* new_node = malloc(sizeof(Node));
        if (!new_node) {  // VÉRIFICATION AJOUTÉE
            SDL_Log("ERREUR: malloc a échoué lors de la création du serpent!");
            exit(1);
        }
        new_node->x = GRID_SIZE * (10 + i);
        new_node->y = HEADER_HEIGHT + GRID_SIZE * 10;
        new_node->next = game->head;
        game->head = new_node;
    }

    // Vérifier que la tête existe
    if (!game->head) {
        SDL_Log("ERREUR CRITIQUE: game->head est NULL après création du serpent!");
        exit(1);
    }

    // Définir la vitesse selon la difficulté
    switch(game->difficulty) {
        case DIFFICULTY_EASY:   game->stepDelay = 150; break;
        case DIFFICULTY_NORMAL: game->stepDelay = 100; break;
        case DIFFICULTY_HARD:   game->stepDelay = 70;  break;
    }

    GenerationFood(game);
    
    // Vérifier que la nourriture a été créée
    if (game->food.x == 0 && game->food.y == 0) {
        SDL_Log("AVERTISSEMENT: GenerationFood n'a pas pu placer de nourriture!");
    }
}