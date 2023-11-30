#include "header.h"

/**
 * change_colour_intens - this changes the colour intensity
 * @factor: the intensity factor
 * @colour: the colour for intensity
*/

void change_colour_intensity(colour_t *colour, float factor)
{
	colour_t a = (*colour & 0xFF000000); /*alpha*/
	colour_t r = (*colour & 0x00FF0000) * factor; /*red*/
	colour_t g = (*colour & 0x0000FF00) * factor; /*green*/
	colour_t b = (*colour & 0x000000FF) * factor; /*blue*/

	*colour = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
}

/**
 * render_floor - render floor projection
 *
 * @bottomwall_pixel: wall bottom pixel
 * @texture_colour: texture color for current pixel
 * @x: current element in the rays array
*/

void render_floor(int bottomwall_pixel, colour_t *texture_colour, int x)
{
	int y, texture_height, texture_width, o, textureoffsetX;
	float distance, ratio;

	texture_width = wall_textures[3].width;
	texture_height = wall_textures[3].height;

	for (y = bottomwall_pixel - 1; y < SCREEN_HEIGHT; y++)
	{
		ratio = player.height / (y - SCREEN_HEIGHT / 2);
		distance = (ratio * PROJ_PLANE)
					/ cos(rays[x].rayAngle - player.rotationAngle);

		o = (int)abs((distance * sin(rays[x].rayAngle)) + player.y);
		textureoffsetX = (int)abs((distance * cos(rays[x].rayAngle)) + player.x);

		textureoffsetX = (int)(abs(textureoffsetX * texture_width / 30)
								% texture_width);
		o = (int)(abs(o * texture_height / 30)
								% texture_height);

		*texture_colour = wall_textures[4].
					  texture_buffer[(texture_width * o) + textureoffsetX];
		drawPixel(x, y, *texture_colour);
	}
}

/**
 * render_ceil - renders the ceil projection
 * @topwall_pixel: wall top pixel
 * texture_colour: texture color for current pixel
 * @x: current element in the rays array
*/

void render_ceil(int topwall_pixel, colour_t texture_colour, int x)
{
	int y, texture_width, texture_height, o, textureoffsetX,
    textureoffsetY;

	texture_width = wall_textures[3].width;
	texture_height = wall_textures[3].height;

	for (y = 0; y < topwall_pixel; y++)
	{
		float distance, ratio;

		ratio = player.height / (y - SCREEN_HEIGHT / 2);
		distance = (ratio * PROJ_PLANE)
					/ cos(rays[x].rayAngle - player.rotationAngle);

		textureoffsetY = (int)abs((-distance * sin(rays[x].rayAngle)) + player.y);
		textureoffsetX = (int)abs((-distance * cos(rays[x].rayAngle)) + player.x);

		textureoffsetX = (int)(abs(textureoffsetX * texture_width / 40)
								% texture_width);
		o = (int)(abs(o * texture_height / 40)
								% texture_height);

	 texture_colour = wall_textures[6].
					  texture_buffer[(texture_width * o) + textureoffsetX];
		drawPixel(x, y, texture_colour);

	}
}

/**
 * render_wall - renders the wall pseuodo 3D
 *
*/
void render_wall(void)
{
	int x, y, texNum, texture_width, texture_height,
		textureoffsetX, wallBottomPixel, wallStripHeight,
		topwall_pixel, distanceFromTop, o;
	float perpDistance, projectedWallHeight;
	colour_t texture_colour;

	for (x = 0; x < NUM_RAYS; x++)
	{
		perpDistance = rays[x].distance * cos(rays[x].rayAngle
							- player.rotationAngle);
		projectedWallHeight = (TILE_SIZE / perpDistance) * PROJ_PLANE;
		wallStripHeight = (int)projectedWallHeight;
		topwall_pixel = (SCREEN_HEIGHT / 2) - (wallStripHeight / 2);
		topwall_pixel = topwall_pixel < 0 ? 0 : topwall_pixel;
		wallBottomPixel = (SCREEN_HEIGHT / 2) + (wallStripHeight / 2);
		wallBottomPixel = wallBottomPixel > SCREEN_HEIGHT
							? SCREEN_HEIGHT : wallBottomPixel;
		texNum = rays[x].wallHitContent - 1;
		texture_width = wall_textures[texNum].width;
		texture_height = wall_textures[texNum].height;
		render_floor(wallBottomPixel, texture_colour, x);
		render_ceil(topwall_pixel, texture_colour, x);

		if (rays[x].wasHitVertical)
			textureoffsetX = (int)rays[x].wallHitY % TILE_SIZE;
		else
			textureoffsetX = (int)rays[x].wallHitX % TILE_SIZE;

		for (y = topwall_pixel; y < wallBottomPixel; y++)
		{
			distanceFromTop = y + (wallStripHeight / 2) - (SCREEN_HEIGHT / 2);
			o = distanceFromTop *
								((float)texture_height / wallStripHeight);
	 texture_colour = wall_textures[texNum].
						 texture_buffer[(texture_width * o) + textureoffsetX];
			if (rays[x].wasHitVertical)
				change_colour_intensity texture_colour, 0.5);
			drawPixel(x, y texture_colour);
		}
	}
}
