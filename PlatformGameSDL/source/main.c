#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include "enemy.h"
#include "player.h"
#include "map.h"

#define WINDOW_WIDTH (TILE_SIZE * VISIBLE_WINDOW_X)//fönstert vi kan se
#define WINDOW_HEIGHT (TILE_SIZE * VISIBLE_WINDOW_Y)

// Definiera Game-struktur
typedef struct {
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    Map *pMap;
    Meny *pMeny;
    Player *pPlayer;
    Projectile *pOrbs[MAX_PROJECTILES];
    Enemy *Enemies[NUMMBER_OF_ENEMIS];
    Projectile *pEnemyProjektil[NUMMBER_OF_ENEMIS];
    TexturForProjektil *pTexturProjektil;
    PatrollingEnemy *pEnemyIMG;
    bool game_is_running;
} Game;

int initialize_window(Game *pGame);
void destroy_window(Game *pGame,int max);
void printMap(Game *pGame);

void run_game(Game *pGame,SDL_Event event);
bool setup(Game *pGame);
void input(Game *pGame, SDL_Event event);

void meny_oppen(Game *pGame,SDL_Event event);
void many_input(Game *pGame,SDL_Event event);

bool keys[SDL_NUM_SCANCODES] = {false};

int main(){
    Game g = {0};
    srand(time(NULL));
    if (!initialize_window(&g)) return false;
    SDL_Event event;
    g.pMeny = createMeny(g.pRenderer);
    meny_oppen(&g,event);
    if(!setup(&g)) return false;
    int max = g.pMap->max_nummber_of_enemis;
    printMap(&g);
    run_game(&g,event);
    destroy_window(&g,max);
    return 0;
}

void meny_oppen(Game *pGame,SDL_Event event){
    while (pGame->pMeny->open){
        many_input(pGame,event);
        SDL_Delay(16);
        renderMeny(pGame->pMeny,pGame->pRenderer);
        SDL_RenderPresent(pGame->pRenderer);
    }
}

void many_input(Game *pGame,SDL_Event event){
    SDL_ShowCursor(SDL_ENABLE);
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    while (SDL_PollEvent(&event)){
        switch (event.type){
        case SDL_QUIT: pGame->pMeny->open = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            break;
        default:
            break;
        }
    }
    for (int i = 0; i < OPTION; i++){
        if(mouseX >= pGame->pMeny->many_plasment[i].x && mouseX <= pGame->pMeny->many_plasment[i].x+pGame->pMeny->many_plasment[i].w &&
        mouseY >= pGame->pMeny->many_plasment[i].y && mouseY <= pGame->pMeny->many_plasment[i].y+pGame->pMeny->many_plasment[i].h){
        pGame->pMeny->many_plasment[i].x = TILE_SIZE*2;
        if(mouseState){
            pGame->pMeny->open = false;
            if(i==0)pGame->game_is_running = true;
            //if(i==1)//score
            if(i==2)pGame->game_is_running = false;
        }
        }else{
            pGame->pMeny->many_plasment[i].x = TILE_SIZE;
        }
    }
}

bool setup(Game *pGame) {
    pGame->pMap = createMap(pGame->pRenderer, 1);
    if (!pGame->pMap) {
        fprintf(stderr, "Error creating map.\n");
        return false;
    }
    pGame->pMap->max_nummber_of_enemis = nummber_of_enemies(pGame->pMap);
    pGame->pPlayer = createPlayer(pGame->pRenderer);
    if (!pGame->pPlayer) {
        fprintf(stderr, "Error creating player.\n");
        //freeMap(pGame->pMap);  // Free map to prevent memory leaks
        return false;
    }
    pGame->pEnemyIMG = createPatrollingEnemy(pGame->pRenderer);
    for (int i = 0; i < pGame->pMap->max_nummber_of_enemis; i++){
        pGame->Enemies[i] = createEnemy(pGame->pRenderer, ENEMY_PATROLLING,pGame->pMap);
        if (!pGame->Enemies[i]) {
            fprintf(stderr, "Failed to create enemy.\n");
            return false;
        }
        pGame->pEnemyProjektil[i] = setupEnemyProjektil(i,pGame->Enemies);
        if(!pGame->pEnemyProjektil[i]){
            fprintf(stderr,"Falied to create enemy projektil.\n");
            return false;
        }
    }
    for (int  i = 0; i < MAX_PROJECTILES; i++){
        pGame->pOrbs[i] = setupOrbs();
    }
    pGame->pTexturProjektil = setupTexturs(pGame->pPlayer->player_shet,pGame->pEnemyIMG->patrollin_frog_shet);
    if(!pGame->pOrbs){
        fprintf(stderr,"Failed to create orbs.\n");
    }
    return true;
}

void run_game(Game *pGame,SDL_Event event){
    Uint32 previousTime = SDL_GetTicks(); // last frame
    Uint32 currentTime;
    while (pGame->game_is_running){
        currentTime = SDL_GetTicks();
        pGame->pPlayer->deltaTime = currentTime - previousTime;
        for (int i = 0; i < pGame->pMap->max_nummber_of_enemis; i++){
            pGame->Enemies[i]->deltaTime = currentTime - previousTime;
        }
        previousTime = currentTime;
        SDL_Delay(16);
        input(pGame,event);
        SDL_RenderClear(pGame->pRenderer);
        renderMap(pGame->pRenderer,pGame->pMap);
        renderPlayer(pGame->pRenderer,pGame->pPlayer,pGame->pMap);
        renderOrbs(pGame->pRenderer,pGame->pOrbs,pGame->pPlayer,pGame->pTexturProjektil);
        renderEnemies(pGame->pRenderer,pGame->Enemies,pGame->pEnemyIMG,pGame->pMap->max_nummber_of_enemis);
        renderProjektil(pGame->pRenderer,pGame->pMap->max_nummber_of_enemis,pGame->pTexturProjektil,pGame->pEnemyProjektil);
        renderHode(pGame->pRenderer,pGame->pMap,pGame->pPlayer);
        SDL_RenderPresent(pGame->pRenderer);
    }
}

void input(Game *pGame, SDL_Event event){
    SDL_ShowCursor(SDL_DISABLE);
    while (SDL_PollEvent(&event)){
        switch (event.type)
        {
        case SDL_QUIT: 
            pGame->game_is_running = false;
            break;
        case SDL_KEYDOWN:
            keys[event.key.keysym.scancode] = true;
            break;
        case SDL_KEYUP:
            keys[event.key.keysym.scancode] = false;
            break;
        default:
            break;
        }
    }
    if(keys[SDL_SCANCODE_ESCAPE]) pGame->game_is_running = false;
    if(keys[SDL_SCANCODE_LEFT]){
        pGame->pPlayer->deltaX = -SPEED;
    }
    if(keys[SDL_SCANCODE_RIGHT]){
        pGame->pPlayer->deltaX = SPEED;
    }if(keys[SDL_SCANCODE_SPACE]){
        playerJump(pGame->pPlayer);
    }
    if(keys[SDL_SCANCODE_D]) pGame->pPlayer->attack = true;

    enemyAttack(pGame->Enemies,pGame->pPlayer,pGame->pMap->max_nummber_of_enemis,pGame->pEnemyProjektil);
    terminateEnemy(pGame->Enemies,pGame->pMap,pGame->pEnemyProjektil);
    death(pGame->pPlayer,pGame->pMap,pGame->Enemies,pGame->pEnemyProjektil);
    movePlayer(pGame->pPlayer,pGame->pMap);
    playerGravity(pGame->pPlayer,pGame->pMap);
    playerAttack(pGame->pPlayer,pGame->pOrbs);
    updateOrbs(pGame->pOrbs,pGame->pMap,pGame->pPlayer);
    enemyHit(pGame->Enemies,pGame->pOrbs,pGame->pMap,pGame->pPlayer);
    updateWorld(pGame->pPlayer,pGame->pMap,pGame->Enemies,pGame->pEnemyProjektil);
}

// Skriver ut kartan
void printMap(Game *pGame) {
    for (int i = 0; i < NUMMBER_OF_TILES_Y; i++) {
        for (int x = 0; x < NUMMBER_OF_TILES_X; x++) {
            printf("%d", pGame->pMap->tails[i][x]);
        }
        printf("\n");
    }
}

// Initialiserar SDL och skapar fönster
int initialize_window(Game *pGame) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0 || TTF_Init() != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        fprintf(stderr, "Error initializing SDL_image: %s\n", IMG_GetError());
        return false;
    }
    pGame->pWindow = SDL_CreateWindow(
        NULL, // Titel
        SDL_WINDOWPOS_CENTERED, // x
        SDL_WINDOWPOS_CENTERED, // y
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        0 // Flags
    );
    if (!pGame->pWindow) {
        fprintf(stderr, "Error creating SDL Window: %s\n", SDL_GetError());
        return false;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, render_flags);
    if (!pGame->pRenderer) {
        fprintf(stderr, "Error creating SDL Renderer: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void destroy_window(Game *pGame, int max) {
    if (pGame->Enemies) {
        for (int i = 0; i < max; i++) {
            if (pGame->Enemies[i]) {
                free(pGame->Enemies[i]);
                pGame->Enemies[i] = NULL;
            }
            if (pGame->pEnemyProjektil[i] != NULL) { 
                free(pGame->pEnemyProjektil[i]);
                pGame->pEnemyProjektil[i] = NULL;
            }
        }
    }

    if (pGame->pEnemyIMG->patrollin_frog_shet) SDL_DestroyTexture(pGame->pEnemyIMG->patrollin_frog_shet);
    if (pGame->pMap) {
        SDL_DestroyTexture(pGame->pMap->tail_shet);
        SDL_DestroyTexture(pGame->pMap->item_shet);
        SDL_DestroyTexture(pGame->pMap->back_dropp);
        free(pGame->pMap);
    }

    if (pGame->pOrbs) {
        for (int i = 0; i < pGame->pPlayer->nrOfAktivOrbs; i++) {
            if (pGame->pOrbs[i] != NULL) {
                free(pGame->pOrbs[i]);
                pGame->pOrbs[i] = NULL;
            }
        }
    }

    if (pGame->pPlayer) {
        SDL_DestroyTexture(pGame->pPlayer->player_shet);
        SDL_DestroyTexture(pGame->pPlayer->player_shet_left);
        free(pGame->pPlayer);
    }

    if (pGame->pTexturProjektil) {
        SDL_DestroyTexture(pGame->pTexturProjektil->pFrog_shet);
        SDL_DestroyTexture(pGame->pTexturProjektil->pPlayer_shet);
        free(pGame->pTexturProjektil);
    }

    if (pGame->pMeny) {
        for (int i = 0; i < OPTION; i++) {
            SDL_DestroyTexture(pGame->pMeny->meny_option[i]);
        }
        SDL_DestroyTexture(pGame->pMeny->meny_backgrund);
        TTF_CloseFont(pGame->pMeny->font);
        free(pGame->pMeny);
    }

    if (pGame->pRenderer) SDL_DestroyRenderer(pGame->pRenderer);
    if (pGame->pWindow) SDL_DestroyWindow(pGame->pWindow);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

