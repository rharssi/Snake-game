#include "ui.h"
#include "game.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Particle particles[MAX_PARTICLES];

const unsigned char FONT_MAP[40][7] = {
    {0x0E,0x11,0x13,0x15,0x19,0x11,0x0E}, // 0
    {0x04,0x0C,0x04,0x04,0x04,0x04,0x0E}, // 1
    {0x0E,0x11,0x01,0x02,0x04,0x08,0x1F}, // 2
    {0x1F,0x02,0x04,0x02,0x01,0x11,0x0E}, // 3
    {0x02,0x06,0x0A,0x12,0x1F,0x02,0x02}, // 4
    {0x1F,0x10,0x1E,0x01,0x01,0x11,0x0E}, // 5
    {0x0E,0x11,0x10,0x1E,0x11,0x11,0x0E}, // 6
    {0x1F,0x11,0x02,0x04,0x08,0x08,0x08}, // 7
    {0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E}, // 8
    {0x0E,0x11,0x11,0x0F,0x01,0x11,0x0E}, // 9
    {0x00,0x04,0x00,0x00,0x00,0x04,0x00}, // :
    {0x0E,0x11,0x11,0x1F,0x11,0x11,0x11}, // A
    {0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E}, // B
    {0x0E,0x11,0x10,0x10,0x10,0x11,0x0E}, // C
    {0x1E,0x11,0x11,0x11,0x11,0x11,0x1E}, // D
    {0x1F,0x10,0x10,0x1E,0x10,0x10,0x1F}, // E
    {0x1F,0x10,0x10,0x1E,0x10,0x10,0x10}, // F
    {0x0E,0x11,0x10,0x17,0x11,0x11,0x0E}, // G
    {0x11,0x11,0x11,0x1F,0x11,0x11,0x11}, // H
    {0x0E,0x04,0x04,0x04,0x04,0x04,0x0E}, // I
    {0x01,0x01,0x01,0x01,0x01,0x11,0x0E}, // J
    {0x11,0x12,0x14,0x18,0x14,0x12,0x11}, // K
    {0x10,0x10,0x10,0x10,0x10,0x10,0x1F}, // L
    {0x11,0x1B,0x15,0x11,0x11,0x11,0x11}, // M
    {0x11,0x19,0x15,0x13,0x11,0x11,0x11}, // N
    {0x0E,0x11,0x11,0x11,0x11,0x11,0x0E}, // O
    {0x1E,0x11,0x11,0x1E,0x10,0x10,0x10}, // P
    {0x0E,0x11,0x11,0x15,0x13,0x12,0x0D}, // Q
    {0x1E,0x11,0x11,0x1E,0x14,0x12,0x11}, // R
    {0x0F,0x10,0x10,0x0E,0x01,0x01,0x1E}, // S
    {0x1F,0x04,0x04,0x04,0x04,0x04,0x04}, // T
    {0x11,0x11,0x11,0x11,0x11,0x11,0x0E}, // U
    {0x11,0x11,0x11,0x11,0x11,0x0A,0x04}, // V
    {0x11,0x11,0x11,0x15,0x15,0x1B,0x11}, // W
    {0x11,0x11,0x0A,0x04,0x0A,0x11,0x11}, // X
    {0x11,0x11,0x0A,0x04,0x04,0x04,0x04}, // Y
    {0x1F,0x01,0x02,0x04,0x08,0x10,0x1F}, // Z
    {0x04,0x15,0x0E,0x1F,0x0E,0x15,0x04}  // *
};

void InitParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) particles[i].life = 0.0f;
}

void GenerationParticle(float x, float y, Uint8 r, Uint8 g, Uint8 b) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life <= 0.0f) {
            particles[i].x = x;
            particles[i].y = y;
            float angle = (float)(rand() % 360) * M_PI / 180.0f;
            float speed = 1.0f + (float)(rand() % 30) / 10.0f;
            particles[i].vx = cosf(angle) * speed;
            particles[i].vy = sinf(angle) * speed;
            particles[i].life = 1.5f + (float)(rand() % 100) / 100.0f;
            particles[i].size = 2.0f + (float)(rand() % 3);
            particles[i].r = r;
            particles[i].g = g;
            particles[i].b = b;
            break;
        }
    }
}

void FillCircle(SDL_Renderer* ren, float cx, float cy, float radius) {
    if (radius < 0.5f) return;
    for (float dy = -radius; dy <= radius; dy += 1.0f) {
        float dx = sqrtf(radius * radius - dy * dy);
        SDL_RenderLine(ren, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

void DrawMatrixText(SDL_Renderer* ren, const char* txt, float x, float y, float scale) {
    if (!txt) return;
    for (int i = 0; txt[i]; i++) {
        int idx = -1;
        char c = txt[i];
        if (c >= 'a' && c <= 'z') c -= 32;
        if (c >= '0' && c <= '9') idx = c - '0';
        else if (c == ':') idx = 10;
        else if (c >= 'A' && c <= 'Z') idx = 11 + (c - 'A');
        else if (c == '*') idx = 37;
        else if (c == ' ') continue;

        if (idx != -1 && idx <= 37) {
            for (int r = 0; r < 7; r++) {
                for (int col = 0; col < 5; col++) {
                    if ((FONT_MAP[idx][r] >> (4 - col)) & 1) {
                        SDL_FRect p = { x + (i * 6 * scale) + (col * scale), y + (r * scale), scale, scale };
                        SDL_RenderFillRect(ren, &p);
                    }
                }
            }
        }
    }
}

void DrawToggleButton(SDL_Renderer* ren, float x, float y, float w, float h, const char* label, bool state) {
    SDL_SetRenderDrawColor(ren, 20, 30, 40, 200);
    SDL_FRect bg = {x, y, w, h};
    SDL_RenderFillRect(ren, &bg);

    if (state) SDL_SetRenderDrawColor(ren, 0, 255, 100, 255);
    else SDL_SetRenderDrawColor(ren, 255, 50, 50, 155);
    SDL_RenderRect(ren, &bg);

    SDL_FRect led = {x + 10, y + h/2 - 5, 10, 10};
    SDL_RenderFillRect(ren, &led);

    if (state) SDL_SetRenderDrawColor(ren, 200, 255, 200, 255);
    else SDL_SetRenderDrawColor(ren, 150, 150, 150, 255);
    DrawMatrixText(ren, label, x + 30, y + h/2 - 7, 2.0f);

    const char* status = state ? "ON" : "OFF";
    DrawMatrixText(ren, status, x + w - 40, y + h/2 - 7, 2.0f);
}

void DrawSpaceBackground(SDL_Renderer* ren, bool paused) {
    for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
        float f = (float)i / (float)SCREEN_HEIGHT;
        SDL_SetRenderDrawColor(ren, (Uint8)(5 + f * 10), (Uint8)(10 + f * 15), (Uint8)(25 + f * 30), 255);
        SDL_FRect r = {0, (float)i, (float)SCREEN_WIDTH, 4};
        SDL_RenderFillRect(ren, &r);
    }

    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life > 0) {
            if (!paused) {
                particles[i].x += particles[i].vx * 0.3f;
                particles[i].y += particles[i].vy * 0.3f;
                particles[i].life -= 0.008f;
                particles[i].vx *= 0.98f;
                particles[i].vy *= 0.98f;
            }
            SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
            Uint8 alpha = (Uint8)(particles[i].life * 200);
            SDL_SetRenderDrawColor(ren, particles[i].r, particles[i].g, particles[i].b, alpha);
            FillCircle(ren, particles[i].x, particles[i].y, particles[i].size);
        }
    }

    static int stars[100][2] = {0};
    static bool starsInit = false;
    if (!starsInit) {
        for (int i = 0; i < 100; i++) {
            stars[i][0] = rand() % SCREEN_WIDTH;
            stars[i][1] = rand() % SCREEN_HEIGHT;
        }
        starsInit = true;
    }

    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    for (int i = 0; i < 100; i++) {
        Uint8 brightness = 100 + (rand() % 155);
        SDL_SetRenderDrawColor(ren, brightness, brightness, brightness + 50, 180);
        SDL_FRect star = {(float)stars[i][0], (float)stars[i][1], 2, 2};
        SDL_RenderFillRect(ren, &star);
    }
}

void DrawGlowingBorder(SDL_Renderer* ren, SDL_FRect rect, Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    for(int i=0; i<3; i++) {
        SDL_SetRenderDrawColor(ren, r, g, b, (Uint8)(150 - i*40));
        SDL_FRect glowRect = {rect.x - i, rect.y - i, rect.w + i*2, rect.h + i*2};
        SDL_RenderRect(ren, &glowRect);
    }
}