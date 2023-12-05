#include "header.h"


/**
 * move_player - moves plaayer to the next position
 * @delta_time: time elapsed since the last frame
*/

void move_player(float delta_time)
{
	float moveStep, newPlayerX, newPlayerY;

	player.rotationAngle += player.turnDirection * player.turnSpeed * delta_time;
	moveStep = player.walkDirection * player.walkSpeed * delta_time;

	newPlayerX = player.x + cos(player.rotationAngle) * moveStep;
	newPlayerY = player.y + sin(player.rotationAngle) * moveStep;

	if (!detect_collision(newPlayerX, newPlayerY))
	{
		player.x = newPlayerX;
		player.y = newPlayerY;
	}
}

/**
 * renderPlayer - render the player
 *
*/

void render_player(void)
{
	draw_rect(
		player.x * MINIMAP_SCALE_FACTOR,
		player.y * MINIMAP_SCALE_FACTOR,
		player.width * MINIMAP_SCALE_FACTOR,
		player.height * MINIMAP_SCALE_FACTOR,
		0xFFFFFFFF
	);
}