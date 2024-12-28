#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <stdbool.h>
#include "map.h"
#include "enemy.h"

#define INDEX_OF_PLEYER_SPRITS 20
#define PLAYER_SIZE_X 32
#define PLAYER_SIZE_y 64  

#define ANIMATION_DELAY 100      // Delay between frames in milliseconds
#define NUM_RUN_FRAMES 4         // Number of running frames in the sprite sheet

#define MAX_PROJECTILES 10
#define PROJECTILE_SPEED (TILE_SIZE/4)
#define PLAYER_DAMEGE 10
#define ORB_INDEX 10

#define PROJECTILE_WIDTH 16
#define PROJECTILE_HEIGHT 16

typedef struct {
    SDL_Rect player_rect;
    SDL_Rect diffrent_player_sprits[INDEX_OF_PLEYER_SPRITS];
    SDL_Texture* player_shet;
    SDL_Texture* player_shet_left;
    int index_sprite;
    bool onGrund,attack;
    int deltaX,deltaY,deltaTime;;
    int lives,respawn,nrOfAktivOrbs;
} Player;

typedef struct {
    bool aktiv;
    SDL_Rect projectile;
    float x,y;//rikting 
    int distens; 
}Projectile;

typedef struct{
    SDL_Texture *pFrog_shet;
    SDL_Rect frog_sptites;
    SDL_Texture *pPlayer_shet;
    SDL_Rect player_sprite;
    //fågel
}TexturForProjektil;


Player *createPlayer(SDL_Renderer *pRenderer);
void renderPlayer(SDL_Renderer *pRenderer, Player *pPleyer, Map *pMap);
void renderHode(SDL_Renderer *pRenderer,Map *pMap,Player *pPlayer);

void movePlayer(Player *pPlayer, Map *pMap);
void updateWorld(Player *pPlayer, Map *pMap,Enemy *pEnemmis[],Projectile *pProjektils[]);

bool checkCollisionAt(Player *pPlayer, Map *pMap);
void playerGravity(Player *pPlayer,Map *pMap);
void playerJump(Player *pPlayer);

void death(Player *pPlayer,Map *pMap,Enemy *pEnemies[],Projectile *pProjektil[]);
void updet_rewspan(Player *pPlayer,Map *pMap,Enemy *pEnemise[]);

Projectile *setupOrbs(void);
Projectile *setupEnemyProjektil(int the_enemy,Enemy *Enemies[]);

TexturForProjektil *setupTexturs(SDL_Texture *pPlayer_shet,SDL_Texture *pFrog_shet);
void playerAttack(Player *pPlayer, Projectile *pOrb[]);
void updateOrbs(Projectile *pOrb[],Map *pMap, Player *pPlayer);
void removeOrbs(Projectile *pOrbs[], int theOrb,Player *pPlayer);
void renderOrbs(SDL_Renderer *pRenderer, Projectile *pOrb[],Player *pPlayer,TexturForProjektil *pTextur);
void enemyHit(Enemy *pEnemies[],Projectile *pOrbs[],Map *pMap,Player *pPlayer);

void terminateEnemy(Enemy *Enemies[], Map *pMap,Projectile *pEnemyProjektil[]);
void enemy_redar(Enemy *Enemies[],Player *pPlayer,int nrOfEnemies);
void enemyAttack(Enemy *Enemies[],Player *pPlayer,int enemy,Projectile *pEnemyProjektil[]);
void renderProjektil(SDL_Renderer *pRenderer,int nrOfProjektil,TexturForProjektil *pTextur,Projectile *pProjektil[]);

//hållbart
bool colitino(SDL_Rect A,SDL_Rect B);


#endif