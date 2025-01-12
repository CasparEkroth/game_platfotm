#include <stdio.h>
#include "player.h"
#include "enemy.h"
#include "map.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdlib.h> 
#include <stdbool.h>
#include<time.h>

Meny *createMeny(SDL_Renderer *pRenderer){
    Meny *pMeny = malloc(sizeof(Meny));  
    if(!pMeny){
        fprintf(stderr,"Error: Could not allocate memory for Meny.\n");
        return false;
    }
    pMeny->open = true;
    pMeny->gameOver = false;
    pMeny->font = TTF_OpenFont("/Users/macbook/Desktop/SDL/PlatformGameSDL/resourses/RubikMaps-Regular.ttf",24);
    if(!pMeny->font){
        fprintf(stderr,"Error: Loding font: %s\n", TTF_GetError());
        return false;
    }
    pMeny->jungulMusic= Mix_LoadMUS("/Users/macbook/Documents/GitHub/game_platfotm/PlatformGameSDL/resourses/jungle-music.mp3");
    if(!pMeny->jungulMusic){
        fprintf(stderr,"Falide to load music Mix_Error: %s\n",Mix_GetError());
        return false;
    }
    SDL_Color textColor = {0, 0, 0, 255};    
    SDL_Surface* backgrund = IMG_Load("/Users/macbook/Desktop/SDL/PlatformGameSDL/resourses/pixel-forest-many.webp"); 
    SDL_Surface* play = TTF_RenderText_Solid(pMeny->font, "PLAY", textColor);
    SDL_Surface* score = TTF_RenderText_Solid(pMeny->font, "SCORE", textColor);
    SDL_Surface* quit = TTF_RenderText_Solid(pMeny->font, "QUIT", textColor);
    SDL_Surface* gameOver = TTF_RenderText_Solid(pMeny->font, "GAME OVER", textColor);
    SDL_Surface* curenetLevel = TTF_RenderText_Solid(pMeny->font, "LEVEL 0",textColor);
    if(play == NULL || score == NULL || quit == NULL || backgrund == NULL || gameOver == NULL || curenetLevel == NULL){
        fprintf(stderr,"Error: creating Text/backgrund surface. %s\n",SDL_GetError());
        return false;
    }
    pMeny->meny_backgrund = SDL_CreateTextureFromSurface(pRenderer,backgrund);
    pMeny->meny_option[0] = SDL_CreateTextureFromSurface(pRenderer,play);
    pMeny->meny_option[1] = SDL_CreateTextureFromSurface(pRenderer,score);
    pMeny->meny_option[2] = SDL_CreateTextureFromSurface(pRenderer,quit);
    pMeny->meny_option[3] = SDL_CreateTextureFromSurface(pRenderer,gameOver);
    pMeny->levels[0] = SDL_CreateTextureFromSurface(pRenderer,curenetLevel);
    SDL_FreeSurface(curenetLevel);
    SDL_FreeSurface(backgrund);
    SDL_FreeSurface(play);
    SDL_FreeSurface(score);
    SDL_FreeSurface(quit);
    SDL_FreeSurface(gameOver);
    for (int i = 0; i < OPTION; i++){
        if(pMeny->meny_option[i] == NULL){
            fprintf(stderr,"Error: creating textrur for text: %s\n",SDL_GetError());
            return false;
        }
    }
    //-- meny backgrund
    pMeny->meny_rect.x = 0;
    pMeny->meny_rect.y = 0;
    pMeny->meny_rect.w = VISIBLE_WINDOW_X*TILE_SIZE;
    pMeny->meny_rect.h = VISIBLE_WINDOW_Y*TILE_SIZE;
    //--
    pMeny->many_plasment[0].x = TILE_SIZE;
    pMeny->many_plasment[0].y = TILE_SIZE;
    pMeny->many_plasment[0].w = TILE_SIZE*4;
    pMeny->many_plasment[0].h = TILE_SIZE;
    //--
    pMeny->many_plasment[1].x = TILE_SIZE;
    pMeny->many_plasment[1].y = TILE_SIZE*3;
    pMeny->many_plasment[1].w = TILE_SIZE*5;
    pMeny->many_plasment[1].h = TILE_SIZE;
    //--
    pMeny->many_plasment[2].x = TILE_SIZE;
    pMeny->many_plasment[2].y = TILE_SIZE*5;
    pMeny->many_plasment[2].w = TILE_SIZE*4;
    pMeny->many_plasment[2].h = TILE_SIZE;
    //-- gameOver
    int a = (VISIBLE_WINDOW_X/2)-5;
    int b = (VISIBLE_WINDOW_Y/2)-1; 
    pMeny->many_plasment[3] =(SDL_Rect){a*TILE_SIZE,b*TILE_SIZE,10*TILE_SIZE,2*TILE_SIZE};
    pMeny->plaesmant_in_level = pMeny->many_plasment[3];
    return pMeny;
}

void renderMeny(Meny *pMeny,SDL_Renderer *pRenderer){
    SDL_RenderCopy(pRenderer,pMeny->meny_backgrund,NULL,&pMeny->meny_rect);
    SDL_RenderCopy(pRenderer,pMeny->meny_option[0],NULL,&pMeny->many_plasment[0]);
    SDL_RenderCopy(pRenderer,pMeny->meny_option[1],NULL,&pMeny->many_plasment[1]);
    SDL_RenderCopy(pRenderer,pMeny->meny_option[2],NULL,&pMeny->many_plasment[2]);
    if(pMeny->gameOver)SDL_RenderCopy(pRenderer,pMeny->meny_option[3],NULL,&pMeny->many_plasment[3]);
}

//map depending on the level 
//lv 0 no enemies 
//lv 1 only frogs
//lv 2 add flying enemies 

Map *createMap(SDL_Renderer *pRenderer, int level) {
    Map *pMap = malloc(sizeof(Map)); // Allokera Map-strukturen
    if (!pMap) {
        fprintf(stderr, "Error: Could not allocate memory for Map.\n");
        return false;
    }
    for (int y = 0; y < NUMMBER_OF_TILES_Y; y++) {
        for (int x = 0; x < NUMMBER_OF_TILES_X; x++) {
            pMap->tails[y][x] = 0;
            if(y==NUMMBER_OF_TILES_Y-1) pMap->tails[y][x] = 1;
            pMap->rect_tail[y][x].x = x*TILE_SIZE;
            pMap->rect_tail[y][x].y = (y*TILE_SIZE)+(TILE_SIZE*(VISIBLE_WINDOW_Y-NUMMBER_OF_TILES_Y));
            pMap->rect_tail[y][x].h = TILE_SIZE;
            pMap->rect_tail[y][x].w = TILE_SIZE;
        }
    }
    pMap->tails[NUMMBER_OF_TILES_Y-1][NUMMBER_OF_TILES_X-1] = 1;

    SDL_Surface* tile_surface = IMG_Load("/Users/macbook/Desktop/SDL/PlatformGameSDL/resourses/tile-map.png"); 
    SDL_Surface* item_surface = IMG_Load("/Users/macbook/Desktop/SDL/PlatformGameSDL/resourses/items.png");
    SDL_Surface* back_dropp_surface = IMG_Load("/Users/macbook/Desktop/SDL/PlatformGameSDL/resourses/forest-backedropp.png");
    if(!tile_surface||!item_surface||!back_dropp_surface){
        fprintf(stderr,"Error creating surface(tail, item, back_dropp): %s\n",IMG_GetError());
        return false;
    }
    pMap->back_dropp = SDL_CreateTextureFromSurface(pRenderer,back_dropp_surface);
    pMap->tail_shet = SDL_CreateTextureFromSurface(pRenderer,tile_surface);
    pMap->item_shet = SDL_CreateTextureFromSurface(pRenderer,item_surface);
    if(!pMap->tail_shet||!pMap->item_shet||!pMap->back_dropp){
        fprintf(stderr,"Error creating textur(tail, item, back_dropp): %s\n",SDL_GetError());
        return false;
    } 
    SDL_FreeSurface(back_dropp_surface);
    SDL_FreeSurface(tile_surface);
    SDL_FreeSurface(item_surface);
    //-- back_dropp rect
    pMap->back_dropp_rect.x = 0;
    pMap->back_dropp_rect.y = 0;
    pMap->back_dropp_rect.w = VISIBLE_WINDOW_X*TILE_SIZE;
    pMap->back_dropp_rect.h = VISIBLE_WINDOW_Y*TILE_SIZE;
    //--
    //-----block
    pMap->diffrent_tile[1].x = 305;
    pMap->diffrent_tile[1].y = 0;
    pMap->diffrent_tile[1].h = 25;
    pMap->diffrent_tile[1].w = 30;
    //-- respan
    pMap->diffrent_tile[2].x = 75;
    pMap->diffrent_tile[2].y = 66;
    pMap->diffrent_tile[2].h = 32;
    pMap->diffrent_tile[2].w = 32;
    //-- activ
    pMap->diffrent_tile[3].x = 75;
    pMap->diffrent_tile[3].y = 230;
    pMap->diffrent_tile[3].h = 32;
    pMap->diffrent_tile[3].w = 32;
    //-- next lavel
    pMap->diffrent_tile[4] =(SDL_Rect){272,32,32,32}; 
    //-- items
    pMap->diffrent_items[0].x =225;
    pMap->diffrent_items[0].y =72;
    pMap->diffrent_items[0].h = 32;
    pMap->diffrent_items[0].w = 34;
    createObstacle(pMap,START_OF_MAP,level);
    pMap->tails[NUMMBER_OF_TILES_Y-1][NUMMBER_OF_TILES_X-1] = 5;
    pMap->tails[NUMMBER_OF_TILES_Y-1][1] = 5;

    return pMap;
}

void renderMap(SDL_Renderer *pRenderer, Map *pMap) {
    SDL_RenderCopy(pRenderer,pMap->back_dropp,NULL,&pMap->back_dropp_rect);
    for (int y = 0; y < NUMMBER_OF_TILES_Y; y++){
        for (int x = 0; x < NUMMBER_OF_TILES_X; x++){
            switch (pMap->tails[y][x]){
            case 1:
                SDL_RenderCopy(pRenderer,pMap->tail_shet,&pMap->diffrent_tile[1],&pMap->rect_tail[y][x]);
                break;
            case 2:
                SDL_RenderCopy(pRenderer,pMap->tail_shet,&pMap->diffrent_tile[2],&pMap->rect_tail[y][x]);
                break;
            case 3:
                SDL_RenderCopy(pRenderer,pMap->tail_shet,&pMap->diffrent_tile[3],&pMap->rect_tail[y][x]);
                break;
            case 5:
                SDL_RenderCopy(pRenderer,pMap->tail_shet,&pMap->diffrent_tile[4],&pMap->rect_tail[y][x]);
                break;
            default:
                break;
            }
        }
    }
}

void createObstacle(Map *pMap,int start_tile,int level){
    do
    {
        int slump = rand()%4+1;
    switch (slump)
    {
    case 1: start_tile = platforms(pMap,start_tile,level);break;
    case 2: start_tile = stairsToHeven(pMap,start_tile,level);break;
    case 3: start_tile = platformStairs(pMap,start_tile,level);break;
    case 4: start_tile = StairDrop(pMap,start_tile,level);
    default:
        break;
    }
    } while (start_tile<=NUMMBER_OF_TILES_X-25);
    pMap->tails[NUMMBER_OF_TILES_Y-1][194] = 2;
}

int platforms(Map *pMap,int start,int level){
    for (int y = 0; y < NUMMBER_OF_TILES_Y; y++){
        for(int x = start;x < start+15; x++){
            pMap->tails[y][x]=0;
        } 
    }
    for (int i = start; i < start+4; i++){
        pMap->tails[NUMMBER_OF_TILES_Y-3][i+2] = 1;
        pMap->tails[NUMMBER_OF_TILES_Y-3][i+9] = 1;
    }
    pMap->tails[NUMMBER_OF_TILES_Y-1][start+16] = 2;
    if(level != 0) pMap->tails[NUMMBER_OF_TILES_Y-4][start+9] = 4;
    return (start+15+3);
}

int stairsToHeven(Map *pMap,int start,int level){
    for (int y = 0; y < NUMMBER_OF_TILES_Y; y++){
        for(int x = start+2;x < start+7; x++){
            pMap->tails[y][x]=0;
        } 
    }
    for (int i = start; i < start+3; i++){
        pMap->tails[NUMMBER_OF_TILES_Y-2][i]=1;
        pMap->tails[NUMMBER_OF_TILES_Y-2][i+6]=1;
    }
    if(level != 0) pMap->tails[NUMMBER_OF_TILES_Y-4][start+6] = 4;
    pMap->tails[NUMMBER_OF_TILES_Y-3][start+2]=1;
    pMap->tails[NUMMBER_OF_TILES_Y-3][start+6]=1;
    pMap->tails[NUMMBER_OF_TILES_Y-1][start+15] = 2;
    return (start+13+3);
}

int platformStairs(Map *pMap,int start,int level){
    for (int  y = 0; y < NUMMBER_OF_TILES_Y; y++){
        for (int x = start; x < start+22; x++){
            pMap->tails[y][x]=0;
        }
    }
    for (int i = start+3; i < start+7; i++){
        pMap->tails[NUMMBER_OF_TILES_Y-4][i]=1;
        if(i<start+6){
            pMap->tails[NUMMBER_OF_TILES_Y-6][i+7]=1;
            pMap->tails[NUMMBER_OF_TILES_Y-1][i+12]=1;
        }
    }
    pMap->tails[NUMMBER_OF_TILES_Y-1][start+23] = 2;
    if(level != 0) pMap->tails[NUMMBER_OF_TILES_Y-5][start+6] = 4;//fiende 
    return (start+22+3);
}

int StairDrop(Map *pMap,int start,int level){
    for (int  y = 0; y < NUMMBER_OF_TILES_Y; y++){
        for (int x = start; x < start+15; x++){
            pMap->tails[y][x]=0;
        }
    }
    for (int i = start; i < start+2; i++){
        pMap->tails[NUMMBER_OF_TILES_Y-2][i+3]=1;
        pMap->tails[NUMMBER_OF_TILES_Y-3][i+8]=1;
    }
    pMap->tails[NUMMBER_OF_TILES_Y-1][start+15] = 2;
    return (start+15+3);
}

void renderFont(SDL_Renderer *pRenderer,Meny * pMeny){
    if(!pMeny->gameOver)SDL_RenderCopy(pRenderer,pMeny->levels[0],NULL,&pMeny->plaesmant_in_level);

}

void levelFornt(SDL_Renderer *pRenderer,Meny * pMeny,int level){
    pMeny->levels[0] = updateText(pRenderer,level,pMeny->levels[0],pMeny->font);
    pMeny->plaesmant_in_level = pMeny->many_plasment[3];
}

SDL_Texture* updateText(SDL_Renderer *pRenderer,int chenge,SDL_Texture* pText,TTF_Font *font){
    char textBuffer[128];
    snprintf(textBuffer, sizeof(textBuffer), "Level %d", chenge);
    SDL_Color textColor1 = {0, 0, 0, 255}; //färg
    SDL_Surface *pTmpSurface = TTF_RenderText_Solid(font, textBuffer, textColor1);
    pText = SDL_CreateTextureFromSurface(pRenderer, pTmpSurface);
    SDL_FreeSurface(pTmpSurface);
    return pText;
}
