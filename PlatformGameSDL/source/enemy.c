#include "player.h"
#include "enemy.h"
#include "map.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdbool.h>

Enemy *createEnemy(SDL_Renderer *pRenderer, int enemyType,Map *pMap) {
    Enemy *enemy = malloc(sizeof(Enemy));
    if (!enemy) {
        fprintf(stderr, "Failed to allocate memory for enemy\n");
        return NULL;
    }
    enemy->health = 30;
    enemy->deltaTime = 0;
    enemy->animationTimer = 0;
    int attackTimer = 0;
    enemy->sprit_index = 0;
    enemy->enemy_rect = (SDL_Rect){64, 64, 48, 48};
    int place_founde=0;
    enemy->onScrene = false;
    enemy->attacking = false;
    switch (enemyType) {
        case ENEMY_PATROLLING:
            enemy->enemyType = ENEMY_PATROLLING;
            enemy->typeData.patrolling.patrolRange = 300;
            enemy->typeData.patrolling.direction = 1;
            for (int y = 0; y < NUMMBER_OF_TILES_Y; y++){
                for (int x = 0; x < NUMMBER_OF_TILES_X; x++){
                    if(pMap->tails[y][x] == 4){
                        enemy->enemy_rect.x = pMap->rect_tail[y][x].x - (TILE_SIZE/2);
                        enemy->enemy_rect.y = pMap->rect_tail[y][x].y - (TILE_SIZE/2);
                        pMap->tails[y][x] = 0;
                        enemy->initial_x = enemy->enemy_rect.x;
                        printf("hitade\n");
                        place_founde++;
                    }
                    if(place_founde!=0)break;
                }
                if(place_founde!=0)break;
            }
            break;
        case ENEMY_FLYING:
            enemy->typeData.flying.flySpeed = 1.5f;
            enemy->typeData.flying.hoverHeight = 100;
            break;

        default:
            fprintf(stderr, "Unknown enemy type!\n");
            free(enemy);
            return NULL;
    }
    return enemy;
}


PatrollingEnemy *createPatrollingEnemy(SDL_Renderer *renderer) {
    PatrollingEnemy *pImige = malloc(sizeof(PatrollingEnemy));
    if (!pImige) {
        fprintf(stderr, "Failed to allocate memory for patrolling enemy\n");
        return NULL;
    }

    SDL_Surface *surface = IMG_Load("/Users/macbook/Desktop/SDL/PlatformGameSDL/resourses/player-groda.png");
    if (!surface) {
        fprintf(stderr, "Failed to load enemy sprite: %s\n", IMG_GetError());
        free(pImige);
        return NULL;
    }

    pImige->patrollin_frog_shet = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!pImige->patrollin_frog_shet) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
        free(pImige);
        return NULL;
    }
    pImige->enemy_sprites[0] = (SDL_Rect){440, 80, 120, 75};// defult
    pImige->enemy_sprites[1] = (SDL_Rect){440, 454, 120, 75};// hit
    pImige->enemy_sprites[2] = (SDL_Rect){815, 270, 120, 75};// death
    pImige->enemy_sprites[3] = (SDL_Rect){1000, 270, 120, 75};// death2
    pImige->enemy_sprites[4] = (SDL_Rect){638, 74, 115, 75};//attack 1
    pImige->enemy_sprites[5] = (SDL_Rect){811, 74, 120, 75};//attack 2
    pImige->enemy_sprites[6] = (SDL_Rect){991, 74, 120, 75}; // end of attack
    return pImige;
}

void renderEnemies(SDL_Renderer *pRenderer, Enemy *Enemies[], PatrollingEnemy *pIMG, int nrOfEnemies) {
    int spriteFrameDelay = 100;  // Delay time in ms for each sprite
    int attackDelay = 1500;       // Delay before an attack animation
    for (int i = 0; i < nrOfEnemies; i++) {
        if (Enemies[i]) {
            // Update timers
            if (Enemies[i]->onScrene) {
                Enemies[i]->attackTimer += Enemies[i]->deltaTime;
            }
            Enemies[i]->animationTimer += Enemies[i]->deltaTime;

            // Handle sprite animation based on state
            if (Enemies[i]->health <= 0) {
                if (Enemies[i]->animationTimer >= spriteFrameDelay) {
                    // Dead animation toggle between sprites 2 and 3
                    Enemies[i]->sprit_index = (Enemies[i]->sprit_index == 2) ? 3 : 2;
                    Enemies[i]->animationTimer = 0;
                }
            } else if (Enemies[i]->attacking) {
                if (Enemies[i]->sprit_index == 6) {  // End of attack animation
                    Enemies[i]->attackTimer = 0;
                    Enemies[i]->animationTimer = 0;
                    Enemies[i]->attacking = false;
                    Enemies[i]->sprit_index = 0;  
                    printf("restet time \n");// Return to idle
                } else if (Enemies[i]->animationTimer >= spriteFrameDelay) {
                    Enemies[i]->sprit_index = 5;  // Attack animation frame
                    Enemies[i]->animationTimer = 0;
                }
            } else if (Enemies[i]->attackTimer >= attackDelay) {
                Enemies[i]->sprit_index = 4;  // Start attack animation
                Enemies[i]->attacking = true;
                Enemies[i]->animationTimer = 0;
            } else if (Enemies[i]->sprit_index == 1 && Enemies[i]->animationTimer >= spriteFrameDelay) {
                Enemies[i]->sprit_index = 0;  // Return to idle animation
                Enemies[i]->animationTimer = 0;
            }

            // Render the current sprite
            SDL_RenderCopy(pRenderer, 
                           pIMG->patrollin_frog_shet, 
                           &pIMG->enemy_sprites[Enemies[i]->sprit_index], 
                           &Enemies[i]->enemy_rect);
        }
    }
}



int nummber_of_enemies(Map *pMap){
    int nummber=0;
    for (int y = 0; y < NUMMBER_OF_TILES_Y; y++) {
        for (int x = 0; x < NUMMBER_OF_TILES_X; x++){
            nummber += (pMap->tails[y][x] == 4) ?  1:0;
        }
    }
    printf("%d\n",nummber);
    return nummber;
}

/*
void enemyAttack(Enemy *Enemies[],Player *pPlayer){//patoling 
    
}*/

/*
void flyingMove(Enemy *enemy) {
    enemy->enemy_rect.y += (int)(sin(SDL_GetTicks() / 500.0) * enemy->typeData.flying.flySpeed);
}
*/
