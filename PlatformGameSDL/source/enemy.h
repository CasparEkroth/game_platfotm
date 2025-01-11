#ifndef ENEMY_H
#define ENEMY_H

#include<SDL.h>
#include "map.h"
//#include "player.h"

#define DIFRENT_TYPE_OF_ENEMY 2
#define DIFRENT_SPRITES 7

#define NUMMBER_OF_ENEMIS 50
#define ENEMIES_SPEED (TILE_SIZE/4)

typedef struct {
    SDL_Texture *patrollin_frog_shet;   
    SDL_Rect enemy_sprites[DIFRENT_SPRITES];
}PatrollingEnemy;

typedef struct{
    SDL_Texture *flying_bat_shet;
    SDL_Rect enemy_sprite[DIFRENT_SPRITES]; 
}FlyingEnemy;

typedef struct Enemy {
    SDL_Rect enemy_rect;         
    //
    int health;                     
    int deltaTime;
    int animationTimer;
    int attackTimer;
    int sprit_index;
    int initial_x;   
    bool onScrene,attacking;     
    // Type-specific data
    union {
        struct {
            int patrolRange;      // Range for patrolling enemies
            int direction;        // Movement direction (-1 or 1)
        } patrolling;

        struct {
            float flySpeed;       // Flying speed
            float hoverHeight;    // Height for flying enemies
        } flying;

    } typeData;

    int enemyType;                // Defines the enemy type (enum-like)
} Enemy;

// Enemy types
enum {
    ENEMY_PATROLLING,
    ENEMY_FLYING,
};

PatrollingEnemy *createPatrollingEnemy(SDL_Renderer *renderer);
FlyingEnemy *creatingFlyingEnemy(SDL_Renderer *pRenderere);
Enemy *createEnemy(SDL_Renderer *pRenderer, int enemyType,Map *pMap);

void renderEnemies(SDL_Renderer *pRenderer,Enemy *Enemise[],PatrollingEnemy *pIMG, int nrOfEnemies);
int nummber_of_enemies(Map *pMap);

#endif 