#ifndef MAP_H
#define MAP_H
#include <SDL.h>
#include <stdbool.h>
#include <SDL_ttf.h>

#define NUMMBER_OF_TILES_Y 7
#define NUMMBER_OF_TILES_X 200
#define TILE_SIZE 32
#define INDEX_OF_TILES 4

#define VISIBLE_WINDOW_X 30
#define VISIBLE_WINDOW_Y 15
#define START_OF_MAP 20

#define DELAY 4
#define SPEED (TILE_SIZE/DELAY)
#define GRAVITY 1
#define MAX_FALL_SPEED 10
#define JUMP_FORCE -10

//meny
#define OPTION 3
#define NUMMBER_OF_LEVELS 3

typedef struct {
    int tails[NUMMBER_OF_TILES_Y][NUMMBER_OF_TILES_X];
    SDL_Rect rect_tail [NUMMBER_OF_TILES_Y][NUMMBER_OF_TILES_X];
    SDL_Rect back_dropp_rect;
    SDL_Rect diffrent_items[INDEX_OF_TILES];
    SDL_Rect diffrent_tile[INDEX_OF_TILES];
    SDL_Texture* tail_shet;
    SDL_Texture* item_shet;
    SDL_Texture* back_dropp;
    int max_nummber_of_enemis;
} Map;

typedef struct {
    //fornts ,play, score, quite
    bool open;
    TTF_Font* font; 
    SDL_Rect many_plasment[OPTION];
    SDL_Texture* meny_option[OPTION];
    //levels
    SDL_Texture* levels[NUMMBER_OF_LEVELS];
    SDL_Texture* meny_backgrund;
    SDL_Rect meny_rect;
}Meny;


Meny *createMeny(SDL_Renderer *pRenderer);
void renderMeny(Meny *pMeny,SDL_Renderer *pRenderer);

Map *createMap(SDL_Renderer *pRenderer, int level);
void renderMap(SDL_Renderer *pRenderer, Map *pMap);

void createObstacle(Map *pMap,int start_tile);

int platforms(Map *pMap,int start);
int stairsToHeven(Map *pMap,int start);
int platformStairs(Map *pMap,int start);
int StairDrop(Map *pMap,int start);


#endif // MAP_H
