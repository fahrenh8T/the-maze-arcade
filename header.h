#ifndef HEADER_H
#define HEADER_H

#include <SDL2/SDL.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include "upng.h"

#define PI 3.14159265
#define TILE_SIZE 64
#define MAP_NUM_ROWS 13
#define MAP_NUM_COLS 20
#define SCREEN_WIDTH (MAP_NUM_COLS * TILE_SIZE)
#define SCREEN_HEIGHT (MAP_NUM_ROWS * TILE_SIZE)
#define FOV_ANGLE (60 * (PI / 180))
#define NUM_RAYS SCREEN_WIDTH
#define PROJ_PLANE ((SCREEN_WIDTH / 2) / tan(FOV_ANGLE / 2))
#define NUM_TEXTURES 8
typedef uint32_t colour_t;

/* Functions-variables-structs for textures */

/**
 * struct texture_k - struct for the textures
 * @width: texture width
 * @height: texture height
 * @texture_buffer: pointer to texture buffer
 * @upng_texture: pointer to the upng buffer
 *
 */
typedef struct texture_k
{
	int width;
	int height;
	colour_t *texture_buffer;
	upng_t *upng_texture;
} texture_t;

texture_t wall_textures[NUM_TEXTURES];

void change_colour_intensity(colour_t *colour, float factor);
void render_floor(int bottomwall_pixel, colour_t *texture_colour, int x);
void render_ceil(int topwall_pixel, colour_t texture_colour, int x);
void render_wall(void);

#endif /*HEADER_H*/