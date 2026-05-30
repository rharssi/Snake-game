#ifndef UI_H
#define UI_H

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct { float x, y, vx, vy, life, size; Uint8 r, g, b; } Particle;

extern Particle particles[];

void InitParticles();
void GenerationParticle(float x, float y, Uint8 r, Uint8 g, Uint8 b);
void FillCircle(SDL_Renderer* ren, float cx, float cy, float radius);
void DrawMatrixText(SDL_Renderer* ren, const char* txt, float x, float y, float scale);
void DrawToggleButton(SDL_Renderer* ren, float x, float y, float w, float h, const char* label, bool state);
void DrawSpaceBackground(SDL_Renderer* ren, bool paused);
void DrawGlowingBorder(SDL_Renderer* ren, SDL_FRect rect, Uint8 r, Uint8 g, Uint8 b);

#endif // UI_H