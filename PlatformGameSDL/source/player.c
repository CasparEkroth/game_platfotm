#include <SDL.h>
#include <stdio.h>
#include "player.h"
#include "enemy.h"
#include "map.h"
#include <SDL_image.h>
#include <stdbool.h>

Player *createPlayer(SDL_Renderer *pRenderer){
    Player *pPlayer = malloc(sizeof(Player));
        if (!pPlayer) {
        fprintf(stderr, "Error: Could not allocate memory for Player.\n");
        return false;
    }
    //-- player pos
    pPlayer->onGrund = true;
    pPlayer->nrOfAktivOrbs = 0;
    pPlayer->attack = false;
    pPlayer->lives = 5;
    pPlayer->respawn = 0;
    pPlayer->player_rect.x = TILE_SIZE*(VISIBLE_WINDOW_X/2);
    pPlayer->player_rect.y = TILE_SIZE*(VISIBLE_WINDOW_Y-3);
    pPlayer->player_rect.h = TILE_SIZE*2;
    pPlayer->player_rect.w = TILE_SIZE;
    pPlayer->deltaX = 0;
    pPlayer->deltaY = 0;  // Ensure deltaY starts at 0
    pPlayer->deltaTime = 0;
    pPlayer->onGrund = true;  // Assume player starts on ground

    //--- sprits
    pPlayer->index_sprite = 0;
    SDL_Surface* playerSurface = IMG_Load("/Users/macbook/Desktop/SDL/PlatformGameSDL/resourses/player-sprite+Orb.png");
    SDL_Surface* playerSurface_left = IMG_Load("/Users/macbook/Desktop/SDL/PlatformGameSDL/resourses/player-sprite-left.png");
    if(!playerSurface||!playerSurface_left){
        fprintf(stderr,"Erorr creating surface(player): %s\n",IMG_GetError());
        return false;
    }
    pPlayer->player_shet = SDL_CreateTextureFromSurface(pRenderer,playerSurface);
    pPlayer->player_shet_left = SDL_CreateTextureFromSurface(pRenderer,playerSurface_left);
    if(!pPlayer->player_shet||!pPlayer->player_shet_left){
        fprintf(stderr,"Error creating textur(player): %s\n",SDL_GetError());
        return false;
    }
    SDL_FreeSurface(playerSurface);
    SDL_FreeSurface(playerSurface_left);
    pPlayer->death_sound = Mix_LoadWAV("/Users/macbook/Documents/GitHub/game_platfotm/PlatformGameSDL/resourses/death.wav");
    if(!pPlayer->death_sound){
        fprintf(stderr,"Falide to load sound effect!(player death) Mix_Error: %s\n",Mix_GetError());
        return false;
    }
    //-- utgångsläge
    pPlayer->diffrent_player_sprits[0] = (SDL_Rect){40,13,100,170};
    //-- left utgångsläge
    pPlayer->diffrent_player_sprits[1] = (SDL_Rect){1392,13,100,170};
    //-- springer 
    pPlayer->diffrent_player_sprits[2] = (SDL_Rect){422,595,100,170};
    //--springer åt left
    pPlayer->diffrent_player_sprits[3] = (SDL_Rect){1007,598,100,170};
    //-- spring 2
    pPlayer->diffrent_player_sprits[4] = (SDL_Rect){1190,595,100,170};
    //-- spring 2 left
    pPlayer->diffrent_player_sprits[5] = (SDL_Rect){244,598,100,170};
    //-- shift
    pPlayer->diffrent_player_sprits[6] = (SDL_Rect){599,836,120,120};
    //-- shift left
    pPlayer->diffrent_player_sprits[7] = (SDL_Rect){807,837,120,120};
    //-- hopp upp
    pPlayer->diffrent_player_sprits[8] = (SDL_Rect){421,970,100,170};
    //-- hopp upp left
    pPlayer->diffrent_player_sprits[9] = (SDL_Rect){812,967,100,170};
    //-- hopp ner 
    pPlayer->diffrent_player_sprits[10] = (SDL_Rect){995,997,100,170};
    //-- hopp ner left
    pPlayer->diffrent_player_sprits[11] = (SDL_Rect){430,1000,100,170};
    //-- ATTACK mov1
    pPlayer->diffrent_player_sprits[12] = (SDL_Rect){620,1541,100,170};
    //-- ATTACK mov1 left
    pPlayer->diffrent_player_sprits[13] = (SDL_Rect){814,1541,100,170};
    //-- ATTACK mov2 
    pPlayer->diffrent_player_sprits[14] = (SDL_Rect){805,1541,130,170};
    //-- ATTACK mov2 left
    pPlayer->diffrent_player_sprits[15] = (SDL_Rect){578,1541,130,170};
    //-- ATTACK mov3
    pPlayer->diffrent_player_sprits[16] = (SDL_Rect){995,1566,110,170};
    //-- ATTACK mov3 left
    pPlayer->diffrent_player_sprits[17] = (SDL_Rect){425,1566,110,170};
    return pPlayer;
}

void renderPlayer(SDL_Renderer *pRenderer, Player *pPleyer,Map *pMap){
    if(pPleyer->index_sprite%2==0){
SDL_RenderCopy(pRenderer,pPleyer->player_shet,&pPleyer->diffrent_player_sprits[pPleyer->index_sprite],&pPleyer->player_rect);
    }else{
SDL_RenderCopy(pRenderer,pPleyer->player_shet_left,&pPleyer->diffrent_player_sprits[pPleyer->index_sprite],&pPleyer->player_rect);
    }
}

void renderHode(SDL_Renderer *pRenderer,Map *pMap,Player *pPlayer){
    SDL_Rect life={
        32,32,32,32
    };
    if(pPlayer->lives!=0){
    for (int i = 0; i < pPlayer->lives; i++){
        life.x=(TILE_SIZE+i*TILE_SIZE);
        SDL_RenderCopy(pRenderer,pMap->item_shet,&pMap->diffrent_items[0],&life);
    }
    }
}

void movePlayer(Player *pPlayer, Map *pMap) {
    if (checkCollisionAt(pPlayer, pMap)) {
        pPlayer->player_rect.x += pPlayer->deltaX;
        pPlayer->player_rect.y += pPlayer->deltaY;
    } else {
        pPlayer->deltaX = 0;
        pPlayer->deltaY = 0;
    }
}

void updateWorld(Player *pPlayer, Map *pMap,Enemy *pEnemmis[],Projectile *pProjektils[],Meny *pMeny){
    int cameraOffsetX = pPlayer->player_rect.x - (TILE_SIZE * 15);
    int cameraOffsetY = pPlayer->player_rect.y - (TILE_SIZE * 10); 
    for (int y = 0; y < NUMMBER_OF_TILES_Y; y++) {
        for (int x = 0; x < NUMMBER_OF_TILES_X; x++) {
            pMap->rect_tail[y][x].x -= cameraOffsetX;
        }
    }
    for (int i = 0; i < pMap->max_nummber_of_enemis; i++){
        pEnemmis[i]->enemy_rect.x -= cameraOffsetX;
        pProjektils[i]->projectile.x -= cameraOffsetX;
    }
    pMeny->plaesmant_in_level.x -= cameraOffsetX;
    //animeton
    static int animationTimer = 0; 
    animationTimer += pPlayer->deltaTime;
    if (animationTimer > 100) { 
            animationTimer = 0;
            if (pPlayer->attack) {
        if (pPlayer->index_sprite == 12 || pPlayer->index_sprite == 13) {
            pPlayer->index_sprite = (pPlayer->index_sprite % 2 == 0) ? 14 : 15;
        } else if (pPlayer->index_sprite == 14 || pPlayer->index_sprite == 15) {
            pPlayer->index_sprite = (pPlayer->index_sprite % 2 == 0) ? 16 : 17;
        } else {
            pPlayer->index_sprite = (pPlayer->index_sprite % 2 == 0) ? 12 : 13;
        }
        }else if (cameraOffsetX > 0) { 
            if (!pPlayer->onGrund) { 
                pPlayer->index_sprite = (cameraOffsetY > 0) ? 8 : 10;
            } else { 
                if (pPlayer->index_sprite == 2) {
                    pPlayer->index_sprite = 4; 
                } else {
                    pPlayer->index_sprite = 2;
                }
            }
        } else if (cameraOffsetX < 0) {
            if (!pPlayer->onGrund) { 
                pPlayer->index_sprite = (cameraOffsetY > 0) ? 9 : 11;
            } else { 
                if (pPlayer->index_sprite == 3) {
                    pPlayer->index_sprite = 5; 
                } else {
                    pPlayer->index_sprite = 3; 
                }
            }
        } else if (cameraOffsetX == 0) { 
            if (!pPlayer->onGrund) { 
                pPlayer->index_sprite = (cameraOffsetY > 0) ? 8 : 10;
            }else{
                pPlayer->index_sprite = (pPlayer->index_sprite%2==0) ? 0:1; // Default sprite
            }
        }
    }
    pPlayer->player_rect.x = (TILE_SIZE * 15);
    pPlayer->deltaX = 0;
}

void death(Player *pPlayer,Map *pMap,Enemy *pEnemies[],Projectile *pProjektil[],Meny *pMeny){
    pPlayer->respawn++;
    int respawn=0;
    updet_rewspan(pPlayer,pMap,pEnemies);
    bool die = false;
    for (int i = 0; i < pMap->max_nummber_of_enemis; i++){
        if (pProjektil[i] != NULL){
            die = colitino(pPlayer->player_rect,pProjektil[i]->projectile);
            if(die)break;
        }
    }
    if(pPlayer->player_rect.y>500 ||die){//death conditions
        pPlayer->lives--;
        Mix_PlayChannel(-1, pPlayer->death_sound, 0);
            for (int x = 0; x < NUMMBER_OF_TILES_X; x++){
                if(pMap->tails[NUMMBER_OF_TILES_Y-1][x]== 3){
                    respawn =x-VISIBLE_WINDOW_X/2;
                }
            }
        for (int y = 0; y < NUMMBER_OF_TILES_Y; y++) {
            for (int x = 0; x < NUMMBER_OF_TILES_X; x++) {
                pMap->rect_tail[y][x].x =x*TILE_SIZE-TILE_SIZE*respawn;//set respan place 
            }
        }
        pPlayer->player_rect.x = TILE_SIZE*(VISIBLE_WINDOW_X/2);
        pPlayer->player_rect.y = TILE_SIZE*(VISIBLE_WINDOW_Y-5);
        for (int i = 0; i < pMap->max_nummber_of_enemis; i++) {
            if (pEnemies[i]) {
                pEnemies[i]->enemy_rect.x = pEnemies[i]->initial_x;
                pProjektil[i]->projectile.x = pEnemies[i]->initial_x;
                pProjektil[i]->projectile.w = SDL_min(pProjektil[i]->projectile.w,16);
            }
        }
        pMeny->plaesmant_in_level = pMeny->many_plasment[3];
    }
}

void updet_rewspan(Player *pPlayer,Map *pMap,Enemy *pEnemise[]){
    int playerBottom = pPlayer->player_rect.y + pPlayer->player_rect.h;
    for (int y = 0; y < NUMMBER_OF_TILES_Y; y++) {
        for (int x = 0; x < NUMMBER_OF_TILES_X; x++) {
            if (pMap->tails[y][x] == 2) { // respawn block
                SDL_Rect tileRect = pMap->rect_tail[y][x];
                if (playerBottom >= tileRect.y && 
                    playerBottom <= tileRect.y + TILE_SIZE / 4 &&
                    pPlayer->player_rect.x + pPlayer->player_rect.w > tileRect.x && 
                    pPlayer->player_rect.x < tileRect.x + TILE_SIZE) {
                        pMap->tails[y][x] = 3;
                        for (int i = 0; i < pMap->max_nummber_of_enemis; i++){
                            pEnemise[i]->initial_x = pEnemise[i]->enemy_rect.x-(TILE_SIZE/2);
                        }
                        return;
                    }
            }
        }
    }
}

bool checkCollisionAt(Player *pPlayer, Map *pMap) {
    SDL_Rect futureRect = {
        pPlayer->player_rect.x + pPlayer->deltaX,
        pPlayer->player_rect.y + pPlayer->deltaY,
        pPlayer->player_rect.w,
        pPlayer->player_rect.h
    };
    for (int y = 0; y < NUMMBER_OF_TILES_Y; y++) {
        for (int x = 0; x < NUMMBER_OF_TILES_X; x++) {
            if (pMap->tails[y][x] == 1) { 
                if (SDL_HasIntersection(&futureRect, &pMap->rect_tail[y][x])) {
                    return false; 
                }
            }
        }
    }
    return true; 
}

bool playerGravity(Player *pPlayer, Map *pMap) {
    pPlayer->onGrund = false; // Assume the player is in the air
    int playerBottom = pPlayer->player_rect.y + pPlayer->player_rect.h;
    for (int y = 0; y < NUMMBER_OF_TILES_Y; y++) {
        for (int x = 0; x < NUMMBER_OF_TILES_X; x++) {
            if (pMap->tails[y][x] == 1|| pMap->tails[y][x] == 2 || pMap->tails[y][x] == 3 || pMap->tails[y][x] == 5){ 
                SDL_Rect tileRect = pMap->rect_tail[y][x];
                // Check if the player's feet are on or very near the tile's top
                if (playerBottom >= tileRect.y && 
                    playerBottom <= tileRect.y + TILE_SIZE / 4 &&
                    pPlayer->player_rect.x + pPlayer->player_rect.w > tileRect.x && 
                    pPlayer->player_rect.x < tileRect.x + TILE_SIZE) {
                    
                    pPlayer->onGrund = true;
                    pPlayer->deltaY = 0; 
                    pPlayer->player_rect.y = tileRect.y - pPlayer->player_rect.h; // Align player on top of the tile
                    if(pMap->tails[y][x] == 5){
                        return true;
                    }
                    return false;
                }
                
            }
        }
    }
    // Apply gravity if no ground is detected
    if (!pPlayer->onGrund) {
        pPlayer->deltaY += GRAVITY;
        if (pPlayer->deltaY > MAX_FALL_SPEED) {
            pPlayer->deltaY = MAX_FALL_SPEED; // Cap fall speed
        }
        pPlayer->player_rect.y += pPlayer->deltaY;
    }
    return false;
}


void playerJump(Player *pPlayer) {
    if (pPlayer->onGrund) {
        pPlayer->deltaY = JUMP_FORCE; 
        pPlayer->onGrund = false;
    }
}

Projectile *setupOrbs(void){
    Projectile *pOrb = malloc(sizeof(Projectile));
    if (!pOrb) {
        printf("Error allocating memory for Orb\n");
        return NULL; 
    }
    pOrb->projectile = (SDL_Rect){0, 0, 16, 16};
    pOrb->aktiv = false;
    pOrb->x = pOrb->y = 0;
    pOrb->distens = 0;    
    return pOrb;
}

Projectile *setupEnemyProjektil(int the_enemy,Enemy *Enemies[]){
    Projectile *pEnemyProjektil = malloc(sizeof(Projectile));
    if(!pEnemyProjektil) return NULL;
    if (pEnemyProjektil) {
        pEnemyProjektil->projectile.x = Enemies[the_enemy]->enemy_rect.x;
        pEnemyProjektil->projectile.y = Enemies[the_enemy]->enemy_rect.y;
        pEnemyProjektil->projectile.w = PROJECTILE_WIDTH; 
        pEnemyProjektil->projectile.h = PROJECTILE_HEIGHT; 
        pEnemyProjektil->aktiv = false; 
        pEnemyProjektil->x = -1;
        pEnemyProjektil->y = 0;
        pEnemyProjektil->distens = 6;
    }
    return pEnemyProjektil;
}

TexturForProjektil *setupTexturs(SDL_Texture *pPlayer_shet,SDL_Texture *pFrog_shet){
    TexturForProjektil *pTextur = malloc(sizeof(TexturForProjektil));
    pTextur->player_sprite = (SDL_Rect){676,73,40,40};
    pTextur->frog_sptites = (SDL_Rect){756,84,32,45};
    pTextur->pFrog_shet = pFrog_shet;
    pTextur->pPlayer_shet = pPlayer_shet;
    return pTextur;
}

void playerAttack(Player *pPlayer, Projectile *pOrb[]){
    if (!pOrb || pPlayer->nrOfAktivOrbs >= MAX_PROJECTILES) return;  

    if (pPlayer->attack && (pPlayer->index_sprite == 16 || pPlayer->index_sprite == 17)) {
        int currentOrb = pPlayer->nrOfAktivOrbs;
        if (pPlayer->index_sprite % 2 == 0) {  //right
            pOrb[currentOrb]->projectile.x = pPlayer->player_rect.x + TILE_SIZE;
            pOrb[currentOrb]->projectile.y = pPlayer->player_rect.y + TILE_SIZE;
            pOrb[currentOrb]->x = 1;
        } else {  // left
            pOrb[currentOrb]->projectile.x = pPlayer->player_rect.x - 16;
            pOrb[currentOrb]->projectile.y= pPlayer->player_rect.y + TILE_SIZE;
            pOrb[currentOrb]->x  = -1;
        }
        pPlayer->nrOfAktivOrbs++;
        pPlayer->attack = false;
    }
}

void updateOrbs(Projectile *pOrb[],Map *pMap, Player *pPlayer){
    if(pPlayer->nrOfAktivOrbs == 0) return;
    for (int i = 0; i < pPlayer->nrOfAktivOrbs; i++){
        if (!pOrb[i]) continue;
        pOrb[i]->projectile.x += pOrb[i]->x * PROJECTILE_SPEED;
    }
    //kolla om de träffar ett block
    for (int i = 0; i < pPlayer->nrOfAktivOrbs; i++){
        if (!pOrb[i]) continue;
        for (int y = 0; y < NUMMBER_OF_TILES_Y; y++) {
            for (int x = 0; x < NUMMBER_OF_TILES_X; x++){
            if(pOrb[i]->projectile.x >= pMap->rect_tail[y][x].x && 
                pOrb[i]->projectile.x < pMap->rect_tail[y][x].x + TILE_SIZE &&
                pOrb[i]->projectile.y >= pMap->rect_tail[y][x].y && 
                pOrb[i]->projectile.y < pMap->rect_tail[y][x].y + TILE_SIZE){
                    if(pMap->tails[y][x] == 1){
                        removeOrbs(pOrb, i, pPlayer);
                        i--;  
                        break;  
                    }
                }
            }
        }
    }
}

void removeOrbs(Projectile *pOrbs[], int theOrb,Player *pPlayer){
    if (theOrb < 0 || theOrb >= pPlayer->nrOfAktivOrbs) return;
    for (int i = theOrb; i < pPlayer->nrOfAktivOrbs - 1; i++){
        pOrbs[i]->projectile = pOrbs[i+1]->projectile;
        pOrbs[i]->distens = pOrbs[i+1]->distens;
        pOrbs[i]->x = pOrbs[i+1]->x;
        pOrbs[i]->y = pOrbs[i+1]->y;
        pOrbs[i]->aktiv = pOrbs[i+1]->aktiv;
    }
    pPlayer->nrOfAktivOrbs--;
}

void renderOrbs(SDL_Renderer *pRenderer, Projectile *pOrb[],Player *pPlayer,TexturForProjektil *pTextur){
    if (!pOrb || pPlayer->nrOfAktivOrbs == 0) return;  // Safety check
    // Render orbs
    for (int i = 0; i < pPlayer->nrOfAktivOrbs; i++) {
        SDL_RenderCopy(pRenderer,pTextur->pPlayer_shet, 
                        &pTextur->player_sprite, 
                        &pOrb[i]->projectile);
    }
}

void enemyHit(Enemy *pEnemies[],Projectile *pOrbs[],Map *pMap,Player *pPlayer){
    for (int i = 0; i < pPlayer->nrOfAktivOrbs; i++){
        for (int j = 0; j < pMap->max_nummber_of_enemis; j++){
            if(pOrbs[i]->projectile.x<pEnemies[j]->enemy_rect.x+pEnemies[j]->enemy_rect.w &&
                pOrbs[i]->projectile.x+pOrbs[i]->projectile.w > pEnemies[j]->enemy_rect.x &&
                pOrbs[i]->projectile.y<pEnemies[j]->enemy_rect.y+pEnemies[j]->enemy_rect.h &&
                pOrbs[i]->projectile.y+pOrbs[i]->projectile.h > pEnemies[j]->enemy_rect.y
                ){
                    //updatera fiende sprites samt hp
                    removeOrbs(pOrbs,i,pPlayer);
                    pEnemies[j]->sprit_index = 1;
                    pEnemies[j]->animationTimer = 0;
                    pEnemies[j]->health -= PLAYER_DAMEGE; 
                    printf("treff %d\n",pEnemies[j]->health);
            }else if (pOrbs[i]->projectile.x<0 || pOrbs[i]->projectile.x > VISIBLE_WINDOW_X*TILE_SIZE){
                    removeOrbs(pOrbs,i,pPlayer);
            }
        }
        
    }
}

void enemy_redar(Enemy *Enemies[],Player *pPlayer,int nrOfEnemies){
    for (int i = 0; i <nrOfEnemies ; i++){
        if(Enemies[i]->enemy_rect.x>=0 &&
            Enemies[i]->enemy_rect.x <= VISIBLE_WINDOW_X*TILE_SIZE){
                Enemies[i]->onScrene = true;
        }else{
                Enemies[i]->onScrene = false;
        }
    }
}

void terminateEnemy(Enemy *Enemies[], Map *pMap, Projectile *pEnemyProjektil[]) {
    if (pMap->max_nummber_of_enemis <= 0) {
        return;
    }
    for (int i = 0; i < pMap->max_nummber_of_enemis; i++) {
        if (Enemies[i] && Enemies[i]->health <= 0 && Enemies[i]->sprit_index == 3) {
            printf("Terminating enemy %d\n", i);
            printf("Before freeing: Enemies[%d] = %p, Projectiles[%d] = %p\n", 
                   i, (void *)Enemies[i], i, (void *)pEnemyProjektil[i]);

            for (int j = i; j < pMap->max_nummber_of_enemis - 1; j++) {
                Enemies[j] = Enemies[j + 1];
                pEnemyProjektil[j] = pEnemyProjektil[j + 1];
            }
            Enemies[pMap->max_nummber_of_enemis - 1] = NULL;
            pEnemyProjektil[pMap->max_nummber_of_enemis - 1] = NULL;
            pMap->max_nummber_of_enemis--;
            i--;
        }
    }
}




void enemyAttack(Enemy *Enemies[], Player *pPlayer, int nrOfEnemies, Projectile *pEnemyProjektil[]) {
    int attackDelay = 150; 
    enemy_redar(Enemies, pPlayer, nrOfEnemies); // Find the attacking enemy
    for (int i = 0; i < nrOfEnemies; i++) {
        if (Enemies[i] && Enemies[i]->onScrene && Enemies[i]->sprit_index >= 5) {
            Enemies[i]->attackTimer += Enemies[i]->deltaTime;
            if (pEnemyProjektil[i]) {
                pEnemyProjektil[i]->aktiv = true;
                if (Enemies[i]->attackTimer >= attackDelay && !Enemies[i]->attacking) {
                    Enemies[i]->attackTimer = 0;  
                    Enemies[i]->attacking = true;
                    Enemies[i]->sprit_index = 4;  
                    Enemies[i]->animationTimer = 0; 
                }
                // Handle the projectile's movement and reversal
                if (Enemies[i]->attacking) {
                    if (pEnemyProjektil[i]->x == -1 && 
                        pEnemyProjektil[i]->projectile.x <= Enemies[i]->enemy_rect.x - (pEnemyProjektil[i]->distens * TILE_SIZE)) {
                        pEnemyProjektil[i]->x = 1; 
                    } else if (pEnemyProjektil[i]->x == 1 && pEnemyProjektil[i]->projectile.x >= Enemies[i]->enemy_rect.x) {
                        pEnemyProjektil[i]->x = -1; 
                        pEnemyProjektil[i]->aktiv = false;
                        Enemies[i]->sprit_index = 6;  
                        Enemies[i]->animationTimer = 0; 
                    }
                    // Safely adjust projectile width based on direction
                    if (pEnemyProjektil[i]->x == -1) {
                        pEnemyProjektil[i]->projectile.w = SDL_max(pEnemyProjektil[i]->projectile.w + PROJECTILE_SPEED, 0);
                    } else {
                        pEnemyProjektil[i]->projectile.w = SDL_max(pEnemyProjektil[i]->projectile.w - PROJECTILE_SPEED, 0);
                    }
                    pEnemyProjektil[i]->projectile.x += PROJECTILE_SPEED * pEnemyProjektil[i]->x;
                    pEnemyProjektil[i]->projectile.y += PROJECTILE_SPEED * pEnemyProjektil[i]->y;
                }
            }
        }
    }
}

void renderProjektil(SDL_Renderer *pRenderer,int nrOfProjektil,TexturForProjektil *pTextur,Projectile *pProjektil[]){
    for (int i = 0; i < nrOfProjektil; i++){//enemy
        if(pProjektil[i]->aktiv){
            SDL_RenderCopy(pRenderer,pTextur->pFrog_shet,&pTextur->frog_sptites,&pProjektil[i]->projectile);
            /*SDL_SetRenderDrawColor(pRenderer,255,0,0,255);
            SDL_Rect A = pProjektil[i]->projectile;
            SDL_RenderDrawRect(pRenderer,&A);*/
        }
    }   
}
//kolla om spelaren träffas 

bool colitino(SDL_Rect A,SDL_Rect B){
    if(A.x + A.w >= B.x && 
        A.x <= B.x + B.w &&
        A.y + A.h >= B.y &&
        A.y <= B.y + B.h){
            return true;
        }else{
            return false;
        }
}
/*SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255); // Red for player
SDL_RenderDrawRect(pRenderer, &pPlayer->player_rect);

SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, 255); // Green for tiles
SDL_RenderDrawRect(pRenderer, &pMap->rect_tail[y][x]);*/

//som printf fast med grafik

//SDL_RenderCopyEx()