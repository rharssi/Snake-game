#include "render.h"
#include "ui.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

void RenderGame(SDL_Renderer* ren, GameLogic* game, SDL_FRect btnGameBack, SDL_FRect btnGamePause, SDL_FRect btnGameRetry, float totalTime) {
    // Zone de jeu avec bordure
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 50, 100, 150, 100);
    SDL_FRect gameArea = { 0, (float)HEADER_HEIGHT, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT - HEADER_HEIGHT };
    SDL_RenderRect(ren, &gameArea);

    // === RENDU DES BARRI�RES AVEC EFFETS COMPLETS ===
    for (int i = 0; i < game->barrierCount; i++) {
        if (game->barriers[i].active) {
            float anim = game->barriers[i].GenerationAnim;
            float pulse = sinf(totalTime * 3.0f + i) * 0.15f + 1.0f;

            for (int j = 0; j < game->barriers[i].length; j++) {
                float bx = (float)(game->barriers[i].x + (game->barriers[i].horizontal ? j * GRID_SIZE : 0)) + GRID_SIZE/2.0f;
                float by = (float)(game->barriers[i].y + (game->barriers[i].horizontal ? 0 : j * GRID_SIZE)) + GRID_SIZE/2.0f;

                // Anneaux lumineux autour des barri�res
                SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
                for (int ring = 4; ring > 0; ring--) {
                    SDL_SetRenderDrawColor(ren, 255, 100 - ring * 15, 50, (Uint8)(40 + ring * 10));
                    FillCircle(ren, bx, by, (GRID_SIZE/2.0f + ring * 3) * pulse * anim);
                }

                // Corps de la barri�re
                SDL_SetRenderDrawColor(ren, 255, 80, 40, 220);
                SDL_FRect barrierRect = {
                    bx - (GRID_SIZE * 0.4f) * anim,
                    by - (GRID_SIZE * 0.4f) * anim,
                    (GRID_SIZE * 0.8f) * anim,
                    (GRID_SIZE * 0.8f) * anim
                };
                SDL_RenderFillRect(ren, &barrierRect);

                // Centre lumineux
                SDL_SetRenderDrawColor(ren, 255, 180, 100, 255);
                FillCircle(ren, bx, by, (GRID_SIZE/3.5f) * anim);

                SDL_SetRenderDrawColor(ren, 255, 240, 200, 255);
                FillCircle(ren, bx, by, (GRID_SIZE/6.0f) * anim);

                // Croix centrale
                SDL_SetRenderDrawColor(ren, 255, 150, 80, 180);
                float offset = GRID_SIZE * 0.35f * anim;
                SDL_RenderLine(ren, bx - offset, by - offset, bx + offset, by + offset);
                SDL_RenderLine(ren, bx - offset, by + offset, bx + offset, by - offset);
            }

            // Lignes de connexion entre segments
            if (game->barriers[i].length > 1) {
                SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(ren, 255, 100, 50, 150);

                for (int j = 0; j < game->barriers[i].length - 1; j++) {
                    float x1 = (float)(game->barriers[i].x + (game->barriers[i].horizontal ? j * GRID_SIZE : 0)) + GRID_SIZE/2.0f;
                    float y1 = (float)(game->barriers[i].y + (game->barriers[i].horizontal ? 0 : j * GRID_SIZE)) + GRID_SIZE/2.0f;
                    float x2 = (float)(game->barriers[i].x + (game->barriers[i].horizontal ? (j+1) * GRID_SIZE : 0)) + GRID_SIZE/2.0f;
                    float y2 = (float)(game->barriers[i].y + (game->barriers[i].horizontal ? 0 : (j+1) * GRID_SIZE)) + GRID_SIZE/2.0f;

                    for (int thick = -3; thick <= 3; thick++) {
                        if (game->barriers[i].horizontal) {
                            SDL_RenderLine(ren, x1, y1 + thick, x2, y2 + thick);
                        } else {
                            SDL_RenderLine(ren, x1 + thick, y1, x2 + thick, y2);
                        }
                    }
                }
            }
        }
    }

    // === NOURRITURE NORMALE AVEC EFFETS ===
    float pulse = sinf(totalTime * 4.0f) * 0.2f + 1.0f;
    float cx = (float)game->food.x + GRID_SIZE/2.0f;
    float cy = (float)game->food.y + GRID_SIZE/2.0f;

    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    for (int ring = 5; ring > 0; ring--) {
        SDL_SetRenderDrawColor(ren, 0, 150 + ring * 20, 255, 30 + ring * 10);
        FillCircle(ren, cx, cy, (GRID_SIZE/2.0f + ring * 4) * pulse);
    }
    SDL_SetRenderDrawColor(ren, 100, 220, 255, 255);
    FillCircle(ren, cx, cy, GRID_SIZE/2.5f * pulse);
    SDL_SetRenderDrawColor(ren, 200, 250, 255, 255);
    FillCircle(ren, cx, cy, GRID_SIZE/4.5f * pulse);

    // === NOURRITURE DOR�E ===
    if (game->goldenActive) {
        float goldenPulse = sinf(totalTime * 5.0f) * 0.25f + 1.0f;
        float gx = (float)game->goldenFood.x + GRID_SIZE/2.0f;
        float gy = (float)game->goldenFood.y + GRID_SIZE/2.0f;

        for (int ring = 6; ring > 0; ring--) {
            SDL_SetRenderDrawColor(ren, 255, 180 + ring * 10, 0, 25 + ring * 8);
            FillCircle(ren, gx, gy, (GRID_SIZE/2.0f + ring * 5) * goldenPulse);
        }
        SDL_SetRenderDrawColor(ren, 255, 215, 0, 255);
        FillCircle(ren, gx, gy, GRID_SIZE/2.3f * goldenPulse);
        SDL_SetRenderDrawColor(ren, 255, 240, 150, 255);
        FillCircle(ren, gx, gy, GRID_SIZE/4.0f * goldenPulse);
    }

    // === NOURRITURE ROUGE ===
    if (game->redActive) {
        float redPulse = sinf(totalTime * 4.5f) * 0.22f + 1.0f;
        float rx = (float)game->redFood.x + GRID_SIZE/2.0f;
        float ry = (float)game->redFood.y + GRID_SIZE/2.0f;

        for (int ring = 5; ring > 0; ring--) {
            SDL_SetRenderDrawColor(ren, 255, 50 + ring * 5, 80 + ring * 10, 30 + ring * 10);
            FillCircle(ren, rx, ry, (GRID_SIZE/2.0f + ring * 4) * redPulse);
        }
        SDL_SetRenderDrawColor(ren, 255, 80, 120, 255);
        FillCircle(ren, rx, ry, GRID_SIZE/2.4f * redPulse);
        SDL_SetRenderDrawColor(ren, 255, 150, 180, 255);
        FillCircle(ren, rx, ry, GRID_SIZE/4.2f * redPulse);
    }

    // === SERPENT AVEC EFFETS ET YEUX ===
    Node* curr = game->head;
    int segIndex = 0;

    while (curr) {
        float segX = (float)curr->x + GRID_SIZE/2.0f;
        float segY = (float)curr->y + GRID_SIZE/2.0f;
        float segRadius = GRID_SIZE/2.2f;

        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

        if (segIndex == 0) {
            // T�te avec halo
            SDL_SetRenderDrawColor(ren, 100, 200, 255, 80);
            FillCircle(ren, segX, segY, segRadius + 6);
            SDL_SetRenderDrawColor(ren, 150, 220, 255, 255);
            FillCircle(ren, segX, segY, segRadius);
            SDL_SetRenderDrawColor(ren, 200, 240, 255, 255);
            FillCircle(ren, segX, segY, segRadius * 0.7f);

            // Yeux selon la direction
            float eyeOffset = 6.0f;
            if (game->dir == RIGHT) {
                SDL_SetRenderDrawColor(ren, 20, 30, 50, 255);
                FillCircle(ren, segX + eyeOffset, segY - 4, 3);
                FillCircle(ren, segX + eyeOffset, segY + 4, 3);
                SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
                FillCircle(ren, segX + eyeOffset + 1, segY - 4, 1.5f);
                FillCircle(ren, segX + eyeOffset + 1, segY + 4, 1.5f);
            } else if (game->dir == LEFT) {
                SDL_SetRenderDrawColor(ren, 20, 30, 50, 255);
                FillCircle(ren, segX - eyeOffset, segY - 4, 3);
                FillCircle(ren, segX - eyeOffset, segY + 4, 3);
                SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
                FillCircle(ren, segX - eyeOffset - 1, segY - 4, 1.5f);
                FillCircle(ren, segX - eyeOffset - 1, segY + 4, 1.5f);
            } else if (game->dir == UP) {
                SDL_SetRenderDrawColor(ren, 20, 30, 50, 255);
                FillCircle(ren, segX - 4, segY - eyeOffset, 3);
                FillCircle(ren, segX + 4, segY - eyeOffset, 3);
                SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
                FillCircle(ren, segX - 4, segY - eyeOffset - 1, 1.5f);
                FillCircle(ren, segX + 4, segY - eyeOffset - 1, 1.5f);
            } else {
                SDL_SetRenderDrawColor(ren, 20, 30, 50, 255);
                FillCircle(ren, segX - 4, segY + eyeOffset, 3);
                FillCircle(ren, segX + 4, segY + eyeOffset, 3);
                SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
                FillCircle(ren, segX - 4, segY + eyeOffset + 1, 1.5f);
                FillCircle(ren, segX + 4, segY + eyeOffset + 1, 1.5f);
            }
        } else {
            // Corps avec d�grad�
            float fade = 1.0f - (float)segIndex / 30.0f;
            if (fade < 0.5f) fade = 0.5f;
            SDL_SetRenderDrawColor(ren, (Uint8)(100 * fade), (Uint8)(200 * fade), 255, (Uint8)(60 * fade));
            FillCircle(ren, segX, segY, segRadius + 3);
            SDL_SetRenderDrawColor(ren, (Uint8)(150 * fade), (Uint8)(220 * fade), 255, 255);
            FillCircle(ren, segX, segY, segRadius);
            SDL_SetRenderDrawColor(ren, (Uint8)(200 * fade), (Uint8)(240 * fade), 255, 255);
            FillCircle(ren, segX, segY, segRadius * 0.6f);
        }

        curr = curr->next;
        segIndex++;
    }

    // === HEADER FUTURISTE N�ON ===
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

    // Fond header
    SDL_SetRenderDrawColor(ren, 5, 10, 20, 240);
    SDL_FRect headerBg = {0, 0, SCREEN_WIDTH, HEADER_HEIGHT};
    SDL_RenderFillRect(ren, &headerBg);

    // Ligne de s�paration n�on
    SDL_SetRenderDrawColor(ren, 0, 255, 255, 150);
    SDL_RenderLine(ren, 0, HEADER_HEIGHT, SCREEN_WIDTH, HEADER_HEIGHT);
    SDL_SetRenderDrawColor(ren, 0, 200, 255, 255);
    SDL_RenderLine(ren, 0, HEADER_HEIGHT - 1, SCREEN_WIDTH, HEADER_HEIGHT - 1);

    // Pr�paration des donn�es
    char scoreStr[32], timeStr[32], bestStr[32], playerStr[32], diffStr[32];
    int elapsed = (int)((SDL_GetTicks() - game->gameStartTime) / 1000);
    sprintf(scoreStr, "%05d", game->score);
    sprintf(timeStr, "%05d", elapsed);
    sprintf(bestStr, "%05d", game->bestScore);
    sprintf(playerStr, "%s", game->playerName);

    switch(game->difficulty) {
        case DIFFICULTY_EASY: sprintf(diffStr, "EASY"); break;
        case DIFFICULTY_NORMAL: sprintf(diffStr, "NORM"); break;
        case DIFFICULTY_HARD: sprintf(diffStr, "HARD"); break;
    }

    // === PANNEAU GAUCHE (Player & Diff) ===
    float panelH = HEADER_HEIGHT - 20;
    SDL_FRect leftPanel = {20, 10, 240, panelH};

    // Ombre
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 120);
    SDL_FRect leftShadow = {leftPanel.x + 8, leftPanel.y + 8, leftPanel.w, leftPanel.h};
    SDL_RenderFillRect(ren, &leftShadow);

    // Fond
    SDL_SetRenderDrawColor(ren, 10, 20, 30, 200);
    SDL_RenderFillRect(ren, &leftPanel);
    SDL_SetRenderDrawColor(ren, 0, 255, 255, 100);
    SDL_RenderRect(ren, &leftPanel);

    // Textes
    SDL_SetRenderDrawColor(ren, 0, 255, 255, 255);
    DrawMatrixText(ren, "PLAYER", leftPanel.x + 15, leftPanel.y + 10, 2.0f);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    DrawMatrixText(ren, playerStr, leftPanel.x + 15, leftPanel.y + 35, 3.5f);
    SDL_SetRenderDrawColor(ren, 0, 200, 255, 200);
    DrawMatrixText(ren, diffStr, leftPanel.x + 160, leftPanel.y + 35, 2.0f);

    // === PANNEAU DROIT (Time & Boutons) ===
    float rightW = 340;
    SDL_FRect rightPanel = {SCREEN_WIDTH - rightW - 20, 10, rightW, panelH};

    // Ombre
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 120);
    SDL_FRect rightShadow = {rightPanel.x + 8, rightPanel.y + 8, rightPanel.w, rightPanel.h};
    SDL_RenderFillRect(ren, &rightShadow);

    // Fond
    SDL_SetRenderDrawColor(ren, 10, 20, 30, 200);
    SDL_RenderFillRect(ren, &rightPanel);
    SDL_SetRenderDrawColor(ren, 0, 255, 255, 100);
    SDL_RenderRect(ren, &rightPanel);

    // Time
    SDL_SetRenderDrawColor(ren, 0, 255, 255, 255);
    DrawMatrixText(ren, "TIME", rightPanel.x + 20, rightPanel.y + 15, 2.0f);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    DrawMatrixText(ren, timeStr, rightPanel.x + 20, rightPanel.y + 35, 3.5f);

    // Boutons
    SDL_FRect* buttons[] = {&btnGameBack, &btnGamePause, &btnGameRetry};
    char buttonLabels[] = {'B', (game->isPaused ? '>' : 'P'), 'R'};

    for(int i=0; i<3; i++) {
        // Ombre bouton
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 120);
        SDL_FRect btnShadow = {buttons[i]->x + 4, buttons[i]->y + 4, buttons[i]->w, buttons[i]->h};
        SDL_RenderFillRect(ren, &btnShadow);

        // Fond bouton
        SDL_SetRenderDrawColor(ren, 5, 10, 20, 255);
        SDL_RenderFillRect(ren, buttons[i]);

        // Bordure n�on
        DrawGlowingBorder(ren, *buttons[i], 0, 255, 255);

        // Lettre
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        char lbl[2] = {buttonLabels[i], '\0'};
        DrawMatrixText(ren, lbl, buttons[i]->x + 15, buttons[i]->y + 15, 3.0f);
    }

    // === PANNEAU CENTRE (Score & Best) ===
    float gapStart = leftPanel.x + leftPanel.w;
    float gapEnd = rightPanel.x;
    float centerW = 340;
    float centerX = gapStart + ((gapEnd - gapStart) - centerW) / 2.0f;
    SDL_FRect centerPanel = {centerX, 10, centerW, panelH};

    // Ombre
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 120);
    SDL_FRect centerShadow = {centerPanel.x + 8, centerPanel.y + 8, centerPanel.w, centerPanel.h};
    SDL_RenderFillRect(ren, &centerShadow);

    // Fond
    SDL_SetRenderDrawColor(ren, 10, 20, 30, 200);
    SDL_RenderFillRect(ren, &centerPanel);
    SDL_SetRenderDrawColor(ren, 0, 255, 255, 100);
    SDL_RenderRect(ren, &centerPanel);

    // Score
    SDL_SetRenderDrawColor(ren, 0, 255, 255, 255);
    DrawMatrixText(ren, "SCORE", centerPanel.x + 30, centerPanel.y + 10, 2.0f);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    DrawMatrixText(ren, scoreStr, centerPanel.x + 30, centerPanel.y + 30, 5.0f);

    // Best
    SDL_SetRenderDrawColor(ren, 0, 200, 255, 200);
    DrawMatrixText(ren, "BEST", centerPanel.x + 230, centerPanel.y + 15, 2.0f);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 220);
    DrawMatrixText(ren, bestStr, centerPanel.x + 230, centerPanel.y + 35, 3.0f);

    // === PAUSE OVERLAY ===
    if (game->isPaused) {
        SDL_SetRenderDrawColor(ren, 0, 5, 10, 200);
        SDL_FRect pauseOverlay = {0, HEADER_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - HEADER_HEIGHT};
        SDL_RenderFillRect(ren, &pauseOverlay);

        const char* pauseTxt = "PAUSED";
        float pWidth = strlen(pauseTxt) * 6 * 7.0f;
        float pX = SCREEN_WIDTH/2.0f - pWidth/2.0f;
        float pY = SCREEN_HEIGHT/2.0f - 50;

        // Lueur cyan
        SDL_SetRenderDrawColor(ren, 0, 255, 255, 100);
        DrawMatrixText(ren, pauseTxt, pX + 2, pY + 2, 7.0f);

        // Texte blanc
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        DrawMatrixText(ren, pauseTxt, pX, pY, 7.0f);

        // Sous-titre
        SDL_SetRenderDrawColor(ren, 0, 200, 255, 255);
        DrawMatrixText(ren, "PRESS 'P' TO RESUME", SCREEN_WIDTH/2.0f - 140, SCREEN_HEIGHT/2.0f + 40, 2.5f);
    }
}
