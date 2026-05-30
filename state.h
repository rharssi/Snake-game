#ifndef STATE_H
#define STATE_H

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;
typedef enum { FOOD_NORMAL, FOOD_GOLDEN, FOOD_RED } FoodType;
typedef enum { DIFFICULTY_EASY, DIFFICULTY_NORMAL, DIFFICULTY_HARD } DifficultyLevel;
typedef enum { STATE_MENU, STATE_DIFFICULTY_SELECT, STATE_GAME, STATE_DYING, STATE_GAMEOVER } GameState;

#endif // STATE_H