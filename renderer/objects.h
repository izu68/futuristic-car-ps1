#pragma once

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

typedef struct {
    TILE *tile;
    int x, y, w, h;
    u_char r, g, b;
} obj_tile;

typedef struct {
    int tim_mode;
    int tim_u_offset, tim_v_offset;
    RECT tim_prect, tim_crect;
    TIM_IMAGE tim_param;

    SPRT *spr;
    u_long resource;
    int x, y, w, h;
    u_char r, g, b;
} obj_sprite;

// DATA LOADING**********************************
void loadobj_sprite (obj_sprite *spr
, char *filename);

// DATA DRAWING**********************************
void drawobj_tile (obj_tile tile
, int x, int y
, int w, int h
, u_char r, u_char g, u_char b);

void drawobj_sprite (obj_sprite *spr
, int x, int y
, int w, int h);

void drawobj_rotsprite (obj_sprite *spr
, int x, int y
, int pw, int ph
, int angle, int scale);