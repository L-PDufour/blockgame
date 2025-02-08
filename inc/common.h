#ifndef COMMON_H
#define COMMON_H

#include "raylib_includes.h"
#define MV_SPEED 5 // 1
#define TILE_SIZE 16
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
typedef struct s_assetManager {
  Texture2D characterFrames[2]; // All character animation frames
  Rectangle characterRects[9];  // Source rectangles for characters
  Texture2D *grassTextures;     // Environment textures (grass etc.)
  Rectangle grassRects[12];
  Texture2D *biomeTextures;
  Rectangle biomeRects[6];
} s_AssetManager;

typedef struct s_entity {
  Rectangle entitySrc[2];
  Rectangle entityDest;
  bool isMoving;
} s_Entity;

typedef enum {
  BIOME_WATER,
  BIOME_BEACH,
  BIOME_PLAINS,
  BIOME_FOREST,
  BIOME_HILL,
  BIOME_MOUNTAIN,
} e_BiomeTypes;

typedef struct {
    e_BiomeTypes type;
} s_WorldTile;

typedef struct {
    s_WorldTile** tiles;  // 2D array of tiles
    int width;
    int height;
  RenderTexture2D mapTexture;
} s_WorldMap;

typedef struct s_gameState {
  s_AssetManager *assets;
  s_Entity *hero;
  s_WorldMap *worldMap;
  Camera2D camera;
  float frameTime;
  int currentFrame;
} s_GameState;

s_GameState *getGameState();
#endif
