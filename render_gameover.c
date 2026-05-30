#include "render.h"
#include "ui.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void RenderGameOver(SDL_Renderer* ren, GameLogic* game, float totalTime) {
    float breathe = sinf(totalTime * 1.5f) * 0.1f + 0.9f;

    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 10, 5, 15, 230);
    SDL_FRect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(ren, &overlay);

    const char* gameOverText = "GAME OVER";
    float goWidth = strlen(gameOverText) * 6 * 8.0f;
    float goY = 120;

    float glitchOff = (rand() % 10 > 7) ? (rand() % 6 - 3) : 0;
    SDL_SetRenderDrawColor(ren, 255, 0, 50, 150);
    DrawMatrixText(ren, gameOverText, SCREEN_WIDTH/2.0f - goWidth/2.0f + 4 + glitchOff, goY + 4, 8.0f);

    SDL_SetRenderDrawColor(ren, 0, 255, 255, 150);
    DrawMatrixText(ren, gameOverText, SCREEN_WIDTH/2.0f - goWidth/2.0f - 3, goY - 3, 8.0f);

    SDL_SetRenderDrawColor(ren, 240, 240, 255, 255);
    DrawMatrixText(ren, gameOverText, SCREEN_WIDTH/2.0f - goWidth/2.0f, goY, 8.0f);

    float panelW = 500;
    float panelH = 260;
    float panelX = SCREEN_WIDTH/2.0f - panelW/2.0f;
    float panelY = 280;

    SDL_SetRenderDrawColor(ren, 20, 25, 40, 240);
    SDL_FRect panel = {panelX, panelY, panelW, panelH};
    SDL_RenderFillRect(ren, &panel);

    SDL_SetRenderDrawColor(ren, 0, 180, 255, (Uint8)(200 * breathe));
    SDL_RenderRect(ren, &panel);

    SDL_SetRenderDrawColor(ren, 100, 150, 200, 255);
    DrawMatrixText(ren, game->playerName, panelX + 30, panelY + 30, 3.0f);

    SDL_SetRenderDrawColor(ren, 150, 200, 255, 255);
    DrawMatrixText(ren, "SCORE", panelX + 30, panelY + 80, 2.5f);

    char scoreTxt[32];
    sprintf(scoreTxt, "%05d", game->score);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    DrawMatrixText(ren, scoreTxt, panelX + 30, panelY + 110, 6.0f);

    SDL_SetRenderDrawColor(ren, 50, 100, 150, 100);
    SDL_RenderLine(ren, panelX + 300, panelY + 30, panelX + 300, panelY + panelH - 30);

    SDL_SetRenderDrawColor(ren, 255, 215, 0, 200);
    DrawMatrixText(ren, "BEST", panelX + 330, panelY + 80, 2.5f);

    char bestTxt[32];
    sprintf(bestTxt, "%05d", game->bestScore);
    SDL_SetRenderDrawColor(ren, 255, 215, 0, 255);
    DrawMatrixText(ren, bestTxt, panelX + 330, panelY + 110, 4.0f);

    if (game->score >= game->bestScore && game->score > 0) {
         float pulseRec = sinf(totalTime * 8.0f) * 0.5f + 0.5f;
         if (pulseRec > 0.5f) {
             SDL_SetRenderDrawColor(ren, 100, 255, 100, 255);
             DrawMatrixText(ren, "NEW RECORD!", panelX + 140, panelY + 200, 2.5f);
         }
    }

    float btnY = 620;

    SDL_SetRenderDrawColor(ren, 100, 255, 150, 200);
    SDL_FRect boxR = {SCREEN_WIDTH/2.0f - 180, btnY, 140, 50};
    SDL_RenderFillRect(ren, &boxR);
    SDL_SetRenderDrawColor(ren, 20, 40, 20, 255);
    SDL_RenderRect(ren, &boxR);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    DrawMatrixText(ren, "[R] RESTART", SCREEN_WIDTH/2.0f - 170, btnY + 15, 2.0f);

    SDL_SetRenderDrawColor(ren, 255, 100, 100, 200);
    SDL_FRect boxE = {SCREEN_WIDTH/2.0f + 40, btnY, 140, 50};
    SDL_RenderFillRect(ren, &boxE);
    SDL_SetRenderDrawColor(ren, 40, 20, 20, 255);
    SDL_RenderRect(ren, &boxE);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    DrawMatrixText(ren, "[ESC] MENU", SCREEN_WIDTH/2.0f + 55, btnY + 15, 2.0f);
}