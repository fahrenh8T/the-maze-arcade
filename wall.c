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

		*texture_colour = wall_textures[4].
					  texture_buffer[(texture_width * textureoffsetY) + textureoffsetX];
		drawPixel(x, y, *texture_colour);
	}
}