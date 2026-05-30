#include "render.h"
#include "ui.h"
#include <math.h>
#include <string.h>

void RenderDifficultySelect(SDL_Renderer* ren, float totalTime) {
    float breathe = sinf(totalTime * 1.2f) * 0.15f + 0.85f;
    float pulse = sinf(totalTime * 1.5f) * 0.3f + 0.7f;

    float cardY = 210;
    float cardWidth = 290;
    float cardHeight = 300;
    float spacing = 35;
    float startX = SCREEN_WIDTH/2.0f - (cardWidth * 3 + spacing * 2) / 2.0f;
    float cornerSize = 20;

    // EASY CARD
    float easyX = startX;
    SDL_SetRenderDrawColor(ren, 20, 30, 40, 240);
    SDL_FRect easyBg = {easyX + cornerSize, cardY, cardWidth - cornerSize*2, cardHeight};
    SDL_RenderFillRect(ren, &easyBg);
    SDL_FRect easyBgV = {easyX, cardY + cornerSize, cornerSize, cardHeight - cornerSize*2};
    SDL_RenderFillRect(ren, &easyBgV);
    SDL_FRect easyBgV2 = {easyX + cardWidth - cornerSize, cardY + cornerSize, cornerSize, cardHeight - cornerSize*2};
    SDL_RenderFillRect(ren, &easyBgV2);

    SDL_SetRenderDrawColor(ren, 100, 220, 150, (Uint8)(220 * breathe));
    for (int i = 0; i < 4; i++) {
        SDL_RenderLine(ren, easyX + cornerSize, cardY - i, easyX + cardWidth - cornerSize, cardY - i);
        SDL_RenderLine(ren, easyX + cornerSize, cardY + cardHeight + i, easyX + cardWidth - cornerSize, cardY + cardHeight + i);
        SDL_RenderLine(ren, easyX - i, cardY + cornerSize, easyX - i, cardY + cardHeight - cornerSize);
        SDL_RenderLine(ren, easyX + cardWidth - cornerSize/2 + i, cardY + cardHeight - cornerSize/2, easyX + cardWidth - cornerSize + i, cardY + cardHeight);
    }

    for (int i = 0; i < 4; i++) {
        SDL_RenderLine(ren, easyX + cornerSize/2 - i, cardY + cornerSize/2, easyX + cornerSize - i, cardY);
        SDL_RenderLine(ren, easyX + cornerSize/2, cardY + cornerSize/2 - i, easyX, cardY + cornerSize - i);
        SDL_RenderLine(ren, easyX + cardWidth - cornerSize/2 + i, cardY + cornerSize/2, easyX + cardWidth - cornerSize + i, cardY);
        SDL_RenderLine(ren, easyX + cardWidth - i, cardY + cornerSize, easyX + cardWidth - i, cardY + cardHeight - cornerSize);
        SDL_RenderLine(ren, easyX + cornerSize/2 - i, cardY + cardHeight - cornerSize/2, easyX + cornerSize - i, cardY + cardHeight);
        SDL_RenderLine(ren, easyX + cornerSize/2, cardY + cardHeight - cornerSize/2 + i, easyX, cardY + cardHeight - cornerSize + i);
        SDL_RenderLine(ren, easyX + cardWidth - cornerSize/2 + i, cardY + cardHeight - cornerSize/2, easyX + cardWidth - cornerSize + i, cardY + cardHeight);
        SDL_RenderLine(ren, easyX + cardWidth - cornerSize/2, cardY + cardHeight - cornerSize/2 + i, easyX + cardWidth, cardY + cardHeight - cornerSize + i);
    }

    SDL_SetRenderDrawColor(ren, 220, 255, 235, 255);
    float easyTextWidth = 4 * 6 * 7.0f;
    DrawMatrixText(ren, "EASY", easyX + cardWidth/2.0f - easyTextWidth/2.0f, cardY + 120, 7.0f);

    SDL_SetRenderDrawColor(ren, 180, 220, 200, 255);
    const char* easyDesc = "SLOW PACE";
    float easyDescWidth = strlen(easyDesc) * 6 * 2.2f;
    DrawMatrixText(ren, easyDesc, easyX + cardWidth/2.0f - easyDescWidth/2.0f, cardY + 200, 2.2f);

    // MEDIUM CARD
    float mediumX = startX + cardWidth + spacing;
    SDL_SetRenderDrawColor(ren, 20, 30, 40, 240);
    SDL_FRect medBg = {mediumX + cornerSize, cardY, cardWidth - cornerSize*2, cardHeight};
    SDL_RenderFillRect(ren, &medBg);
    SDL_FRect medBgV = {mediumX, cardY + cornerSize, cornerSize, cardHeight - cornerSize*2};
    SDL_RenderFillRect(ren, &medBgV);
    SDL_FRect medBgV2 = {mediumX + cardWidth - cornerSize, cardY + cornerSize, cornerSize, cardHeight - cornerSize*2};
    SDL_RenderFillRect(ren, &medBgV2);

    SDL_SetRenderDrawColor(ren, 255, 180, 100, (Uint8)(220 * breathe));
    for (int i = 0; i < 4; i++) {
        SDL_RenderLine(ren, mediumX + cornerSize, cardY - i, mediumX + cardWidth - cornerSize, cardY - i);
        SDL_RenderLine(ren, mediumX + cornerSize, cardY + cardHeight + i, mediumX + cardWidth - cornerSize, cardY + cardHeight + i);
        SDL_RenderLine(ren, mediumX - i, cardY + cornerSize, mediumX - i, cardY + cardHeight - cornerSize);
        SDL_RenderLine(ren, mediumX + cardWidth + i, cardY + cornerSize, mediumX + cardWidth + i, cardY + cardHeight - cornerSize);
    }

    for (int i = 0; i < 4; i++) {
        SDL_RenderLine(ren, mediumX + cornerSize/2 - i, cardY + cornerSize/2, mediumX + cornerSize - i, cardY);
        SDL_RenderLine(ren, mediumX + cornerSize/2, cardY + cornerSize/2 - i, mediumX, cardY + cornerSize - i);
        SDL_RenderLine(ren, mediumX + cardWidth - cornerSize/2 + i, cardY + cornerSize/2, mediumX + cardWidth - cornerSize + i, cardY);
        SDL_RenderLine(ren, mediumX + cardWidth - cornerSize/2, cardY + cornerSize/2 - i, mediumX + cardWidth, cardY + cornerSize - i);
        SDL_RenderLine(ren, mediumX + cornerSize/2 - i, cardY + cardHeight - cornerSize/2, mediumX + cornerSize - i, cardY + cardHeight);
        SDL_RenderLine(ren, mediumX + cornerSize/2, cardY + cardHeight - cornerSize/2 + i, mediumX, cardY + cardHeight - cornerSize + i);
        SDL_RenderLine(ren, mediumX + cardWidth - cornerSize/2 + i, cardY + cardHeight - cornerSize/2, mediumX + cardWidth - cornerSize + i, cardY + cardHeight);
        SDL_RenderLine(ren, mediumX + cardWidth - cornerSize/2, cardY + cardHeight - cornerSize/2 + i, mediumX + cardWidth, cardY + cardHeight - cornerSize + i);
    }

    SDL_SetRenderDrawColor(ren, 255, 240, 220, 255);
    float medTextWidth = 6 * 6 * 7.0f;
    DrawMatrixText(ren, "MEDIUM", mediumX + cardWidth/2.0f - medTextWidth/2.0f, cardY + 120, 7.0f);

    SDL_SetRenderDrawColor(ren, 240, 210, 180, 255);
    const char* medDesc = "SPEED UP";
    float medDescWidth = strlen(medDesc) * 6 * 2.2f;
    DrawMatrixText(ren, medDesc, mediumX + cardWidth/2.0f - medDescWidth/2.0f, cardY + 200, 2.2f);

    // HARD CARD
    float hardX = mediumX + cardWidth + spacing;
    SDL_SetRenderDrawColor(ren, 20, 30, 40, 240);
    SDL_FRect hardBg = {hardX + cornerSize, cardY, cardWidth - cornerSize*2, cardHeight};
    SDL_RenderFillRect(ren, &hardBg);
    SDL_FRect hardBgV = {hardX, cardY + cornerSize, cornerSize, cardHeight - cornerSize*2};
    SDL_RenderFillRect(ren, &hardBgV);
    SDL_FRect hardBgV2 = {hardX + cardWidth - cornerSize, cardY + cornerSize, cornerSize, cardHeight - cornerSize*2};
    SDL_RenderFillRect(ren, &hardBgV2);

    SDL_SetRenderDrawColor(ren, 255, 120, 140, (Uint8)(220 * breathe));
    for (int i = 0; i < 4; i++) {
        SDL_RenderLine(ren, hardX + cornerSize, cardY - i, hardX + cardWidth - cornerSize, cardY - i);
        SDL_RenderLine(ren, hardX + cornerSize, cardY + cardHeight + i, hardX + cardWidth - cornerSize, cardY + cardHeight + i);
        SDL_RenderLine(ren, hardX - i, cardY + cornerSize, hardX - i, cardY + cardHeight - cornerSize);
        SDL_RenderLine(ren, hardX + cardWidth + i, cardY + cornerSize, hardX + cardWidth + i, cardY + cardHeight - cornerSize);
    }

    for (int i = 0; i < 4; i++) {
        SDL_RenderLine(ren, hardX + cornerSize/2 - i, cardY + cornerSize/2, hardX + cornerSize - i, cardY);
        SDL_RenderLine(ren, hardX + cornerSize/2, cardY + cornerSize/2 - i, hardX, cardY + cornerSize - i);
        SDL_RenderLine(ren, hardX + cardWidth - cornerSize/2 + i, cardY + cornerSize/2, hardX + cardWidth - cornerSize + i, cardY);
        SDL_RenderLine(ren, hardX + cardWidth - cornerSize/2, cardY + cornerSize/2 - i, hardX + cardWidth, cardY + cornerSize - i);
        SDL_RenderLine(ren, hardX + cornerSize/2 - i, cardY + cardHeight - cornerSize/2, hardX + cornerSize - i, cardY + cardHeight);
        SDL_RenderLine(ren, hardX + cornerSize/2, cardY + cardHeight - cornerSize/2 + i, hardX, cardY + cardHeight - cornerSize + i);
        SDL_RenderLine(ren, hardX + cardWidth - cornerSize/2 + i, cardY + cardHeight - cornerSize/2, hardX + cardWidth - cornerSize + i, cardY + cardHeight);
        SDL_RenderLine(ren, hardX + cardWidth - cornerSize/2, cardY + cardHeight - cornerSize/2 + i, hardX + cardWidth, cardY + cardHeight - cornerSize + i);
    }

    SDL_SetRenderDrawColor(ren, 255, 220, 230, 255);
    float hardTextWidth = 4 * 6 * 7.0f;
    DrawMatrixText(ren, "HARD", hardX + cardWidth/2.0f - hardTextWidth/2.0f, cardY + 120, 7.0f);

    SDL_SetRenderDrawColor(ren, 240, 200, 210, 255);
    const char* hardDesc = "EXTREME";
    float hardDescWidth = strlen(hardDesc) * 6 * 2.2f;
    DrawMatrixText(ren, hardDesc, hardX + cardWidth/2.0f - hardDescWidth/2.0f, cardY + 200, 2.2f);

    float instructY = SCREEN_HEIGHT - 120;
    SDL_SetRenderDrawColor(ren, 140, 160, 190, (Uint8)(200 * pulse));
    const char* instr = "PRESS 1  2  OR 3 TO SELECT";
    float instrWidth = strlen(instr) * 6 * 2.8f;
    DrawMatrixText(ren, instr, SCREEN_WIDTH/2.0f - instrWidth/2.0f, instructY, 2.8f);

    SDL_SetRenderDrawColor(ren, 120, 140, 170, 180);
    const char* esc = "ESC OR B TO GO BACK";
    float escWidth = strlen(esc) * 6 * 2.5f;
    DrawMatrixText(ren, esc, SCREEN_WIDTH/2.0f - escWidth/2.0f, instructY + 30, 2.5f);

    for (int i = 0; i < 3; i++) {
        float dotX = SCREEN_WIDTH/2.0f - 20 + i * 20;
        float dotSize = sinf(totalTime * 2.5f + i * 0.8f) * 1.5f + 2.5f;
        SDL_SetRenderDrawColor(ren, 140, 160, 190, (Uint8)(150 * pulse));
        FillCircle(ren, dotX, instructY + 60, dotSize);
    }
}