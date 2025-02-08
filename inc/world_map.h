#ifndef WORLD_MAP_H
#define WORLD_MAP_H

typedef enum {
  GREEN_TREE,
  GREEN_DARK_TREE,
  PINE_TREE,
  GREEN_PINE_TREE,
  LEAF_TREE,
  LEAF_DARK_TREE,
  BLUE_MTN,
  RED_MTN,
  WHITE_MTN,
  SMALL_WHITE_MTN,
  GREEN_HILL,
  YELLOW_HILL,
} e_BiomeTextures;

typedef enum {
  NE_GRASS,
  N_GRASS,
  NW_GRASS,
  SINGLE_N_GRASS,
  E_GRASS,
  M_GRASS,
  W_GRASS,
  SINGLE_M_GRASS,
  SE_GRASS,
  S_GRASS,
  SW_GRASS,
  SINGLE_S_GRASS,
} e_GrassTextures;

void drawWorldMap();


#endif
