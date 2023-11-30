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
	int y, texture_height, texture_width, textureoffsetY, textureoffsetX;
	float distance, ratio;

	texture_width = wall_textures[3].width;
	texture_height = wall_textures[3].height;

	for (y = bottomwall_pixel - 1; y < SCREEN_HEIGHT; y++)
	{
		ratio = player.height / (y - SCREEN_HEIGHT / 2);
		distance = (ratio * PROJ_PLANE)
					/ cos(rays[x].rayAngle - player.rotationAngle);

		textureoffsetY = (int)abs((distance * sin(rays[x].rayAngle)) + player.y);
		textureoffsetX = (int)abs((distance * cos(rays[x].rayAngle)) + player.x);

		textureoffsetX = (int)(abs(textureoffsetX * texture_width / 30)
								% texture_width);
		textureoffsetY = (int)(abs(textureoffsetY * texture_height / 30)
								% texture_height);

		*texture_colour = wallTextures[4].
					  texture_buffer[(texture_width * textureoffsetY) + textureoffsetX];
		drawPixel(x, y, *texture_colour);
	}
}

/**
 * renderCeil - render Ceil projection
 * @WallTopPixel: wall top pixel
 * @texelColor: texture color for current pixel
 * @x: current element in the rays array
*/

void renderCeil(int wallTopPixel, colour_t *texelColor, int x)
{
	int y, texture_width, texture_height, textureOffsetY, textureOffsetX;

	texture_width = wallTextures[3].width;
	texture_height = wallTextures[3].height;

	for (y = 0; y < wallTopPixel; y++)
	{
		float distance, ratio;

		ratio = player.height / (y - SCREEN_HEIGHT / 2);
		distance = (ratio * PROJ_PLANE)
					/ cos(rays[x].rayAngle - player.rotationAngle);

		textureOffsetY = (int)abs((-distance * sin(rays[x].rayAngle)) + player.y);
		textureOffsetX = (int)abs((-distance * cos(rays[x].rayAngle)) + player.x);

		textureOffsetX = (int)(abs(textureOffsetX * texture_width / 40)
								% texture_width);
		textureOffsetY = (int)(abs(textureOffsetY * texture_height / 40)
								% texture_height);

		*texelColor = wallTextures[6].
					  texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];
		drawPixel(x, y, *texelColor);

	}
}

/**
 * renderWall - render wall projection
 *
*/
void renderWall(void)
{
	int x, y, texNum, texture_width, texture_height,
		textureOffsetX, wallBottomPixel, wallStripHeight,
		wallTopPixel, distanceFromTop, textureOffsetY;
	float perpDistance, projectedWallHeight;
	colour_t texelColor;

	for (x = 0; x < NUM_RAYS; x++)
	{
		perpDistance = rays[x].distance * cos(rays[x].rayAngle
							- player.rotationAngle);
		projectedWallHeight = (TILE_SIZE / perpDistance) * PROJ_PLANE;
		wallStripHeight = (int)projectedWallHeight;
		wallTopPixel = (SCREEN_HEIGHT / 2) - (wallStripHeight / 2);
		wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;
		wallBottomPixel = (SCREEN_HEIGHT / 2) + (wallStripHeight / 2);
		wallBottomPixel = wallBottomPixel > SCREEN_HEIGHT
							? SCREEN_HEIGHT : wallBottomPixel;
		texNum = rays[x].wallHitContent - 1;
		texture_width = wallTextures[texNum].width;
		texture_height = wallTextures[texNum].height;
		renderFloor(wallBottomPixel, &texelColor, x);
		renderCeil(wallTopPixel, &texelColor, x);

		if (rays[x].wasHitVertical)
			textureOffsetX = (int)rays[x].wallHitY % TILE_SIZE;
		else
			textureOffsetX = (int)rays[x].wallHitX % TILE_SIZE;

		for (y = wallTopPixel; y < wallBottomPixel; y++)
		{
			distanceFromTop = y + (wallStripHeight / 2) - (SCREEN_HEIGHT / 2);
			textureOffsetY = distanceFromTop *
								((float)texture_height / wallStripHeight);
			texelColor = wallTextures[texNum].
						 texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];
			if (rays[x].wasHitVertical)
				changeColorIntensity(&texelColor, 0.5);
			drawPixel(x, y, texelColor);
		}
	}
}
