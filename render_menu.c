#include "render.h"
#include "ui.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

void RenderMenu(SDL_Renderer* ren, GameLogic* game, SDL_FRect btnMusicRect, SDL_FRect btnSoundRect, float totalTime) {
    DrawToggleButton(ren, btnMusicRect.x, btnMusicRect.y, btnMusicRect.w, btnMusicRect.h, "MUSIC", game->musicEnabled);
    DrawToggleButton(ren, btnSoundRect.x, btnSoundRect.y, btnSoundRect.w, btnSoundRect.h, "SOUND", game->soundEnabled);

    float titleY = 120;
    float breathe = sinf(totalTime * 1.2f) * 0.15f + 0.85f;
    float snakeWidth = 5 * 6 * 28.0f;
    float centerX = SCREEN_WIDTH/2.0f - snakeWidth/2.0f;

    float colorWave = sinf(totalTime * 1.5f) * 0.5f + 0.5f;
    SDL_SetRenderDrawColor(ren, 5, 10, 20, 220);
    DrawMatrixText(ren, "SNAKE", centerX + 5, titleY + 5, 28.0f);

    Uint8 baseR = (Uint8)(10 + colorWave * 15);
    Uint8 baseG = (Uint8)(20 + colorWave * 25);
    Uint8 baseB = (Uint8)(50 + colorWave * 40);
    SDL_SetRenderDrawColor(ren, baseR, baseG, baseB, 255);
    DrawMatrixText(ren, "SNAKE", centerX, titleY + 2, 28.0f);

    Uint8 topR = (Uint8)(40 + colorWave * 30);
    Uint8 topG = (Uint8)(100 + colorWave * 60);
    Uint8 topB = (Uint8)(160 + colorWave * 50);
    SDL_SetRenderDrawColor(ren, topR, topG, topB, 255);
    DrawMatrixText(ren, "SNAKE", centerX, titleY, 28.0f);

    float formY = SCREEN_HEIGHT/2.0f + 40;
    float boxWidth = 500;
    float boxHeight = 60;
    float boxX = SCREEN_WIDTH/2.0f - boxWidth/2.0f;

    char nameDisplay[64];
    sprintf(nameDisplay, "USER: %s", game->playerName);

    SDL_SetRenderDrawColor(ren, 20, 30, 45, 230);
    SDL_FRect nameBox = {boxX, formY, boxWidth, boxHeight};
    SDL_RenderFillRect(ren, &nameBox);

    if (game->menuInputFocus == 0) SDL_SetRenderDrawColor(ren, 50, 255, 100, 255);
    else SDL_SetRenderDrawColor(ren, 40, 60, 85, 255);
    SDL_RenderRect(ren, &nameBox);

    float nameWidth = strlen(nameDisplay) * 6 * 3.0f;
    SDL_SetRenderDrawColor(ren, 200, 220, 240, 255);
    DrawMatrixText(ren, nameDisplay, boxX + 20, formY + 15, 3.0f);

    if (game->menuInputFocus == 0 && (int)(totalTime * 2.0f) % 2 == 0) {
        float cursorX = boxX + 20 + nameWidth + 5;
        SDL_SetRenderDrawColor(ren, 70, 130, 180, 255);
        SDL_FRect cursor = {cursorX, formY + 15, 3, 20};
        SDL_RenderFillRect(ren, &cursor);
    }

    float pinY = formY + 80;
    SDL_SetRenderDrawColor(ren, 20, 30, 45, 230);
    SDL_FRect pinBox = {boxX, pinY, boxWidth, boxHeight};
    SDL_RenderFillRect(ren, &pinBox);

    if (game->menuInputFocus == 1) SDL_SetRenderDrawColor(ren, 50, 255, 100, 255);
    else SDL_SetRenderDrawColor(ren, 40, 60, 85, 255);
    SDL_RenderRect(ren, &pinBox);

    char pinDisplay[64] = "PIN: ";
    for(int k=0; k<strlen(game->playerPin); k++) {
        strcat(pinDisplay, "*");
    }

    float pinWidth = strlen(pinDisplay) * 6 * 3.0f;
    SDL_SetRenderDrawColor(ren, 200, 220, 240, 255);
    DrawMatrixText(ren, pinDisplay, boxX + 20, pinY + 15, 3.0f);

    if (game->menuInputFocus == 1 && (int)(totalTime * 2.0f) % 2 == 0) {
        float cursorX = boxX + 20 + pinWidth + 5;
        SDL_SetRenderDrawColor(ren, 70, 130, 180, 255);
        SDL_FRect cursor = {cursorX, pinY + 15, 3, 20};
        SDL_RenderFillRect(ren, &cursor);
    }

    float instructY = SCREEN_HEIGHT - 100;
    float pulse = sinf(totalTime * 1.5f) * 0.15f + 0.85f;

    const char* instr1 = "TAB OR UP/DOWN TO SWITCH FIELDS";
    float w1 = strlen(instr1) * 6 * 2.0f;
    SDL_SetRenderDrawColor(ren, 150, 150, 180, 200);
    DrawMatrixText(ren, instr1, SCREEN_WIDTH/2.0f - w1/2.0f, instructY - 30, 2.0f);

    const char* instr2 = "PRESS ENTER TO LOGIN";
    float w2 = strlen(instr2) * 6 * 2.5f;
    SDL_SetRenderDrawColor(ren, 200, 220, 240, (Uint8)(255 * pulse));
    DrawMatrixText(ren, instr2, SCREEN_WIDTH/2.0f - w2/2.0f, instructY, 2.5f);
}