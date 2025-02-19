/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoteris <noterisarthur42@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 04:39:22 by anoteris          #+#    #+#             */
/*   Updated: 2025/02/19 17:01:58 by anoteris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	frame_handler(t_cub3d *cub3d)
{
	cub3d->frames[cub3d->frame]->enabled = false ;
	cub3d->frame++ ;
	if (cub3d->frame == NB_FRAMES)
		cub3d->frame = -1 ;
	cub3d->frames[cub3d->frame + (cub3d->frame == -1)]->enabled = true ;
}

static void	key_handler(t_cub3d *cub3d)
{
	if (mlx_is_key_down(cub3d->mlx, MLX_KEY_W))
		move(cub3d,
			cub3d->raycast->dir->x,
			-cub3d->raycast->dir->y,
			MOVE_SPEED);
	if (mlx_is_key_down(cub3d->mlx, MLX_KEY_S))
		move(cub3d,
			-cub3d->raycast->dir->x,
			cub3d->raycast->dir->y,
			MOVE_SPEED);
	if (mlx_is_key_down(cub3d->mlx, MLX_KEY_D))
		move(cub3d,
			cub3d->raycast->dir->y,
			cub3d->raycast->dir->x,
			MOVE_SPEED * 0.75);
	if (mlx_is_key_down(cub3d->mlx, MLX_KEY_A))
		move(cub3d,
			-cub3d->raycast->dir->y,
			-cub3d->raycast->dir->x,
			MOVE_SPEED * 0.75);
	if (mlx_is_key_down(cub3d->mlx, MLX_KEY_RIGHT))
		rotate(cub3d, -ROT_SPEED);
	if (mlx_is_key_down(cub3d->mlx, MLX_KEY_LEFT))
		rotate(cub3d, ROT_SPEED);
}

void	frame_loop(void *param)
{
	t_cub3d	*cub3d ;
	int32_t	x ;
	int32_t	y ;

	cub3d = (t_cub3d *) param ;
	if (cub3d->frame >= 0)
		frame_handler(cub3d);
	key_handler(cub3d);
	mlx_get_mouse_pos(cub3d->mlx, &x, &y);
	if (x - (SCREENWIDTH / 2))
	{
		mlx_set_mouse_pos(cub3d->mlx, (SCREENWIDTH / 2), (SCREENHEIGHT / 2));
		rotate(cub3d, -ROT_SPEED * (x - (SCREENWIDTH / 2)) * 0.03);
	}
	if ((mlx_get_time() - cub3d->last_frame_time) > (1.0 / 60.0))
	{
		raycasting(cub3d);
		cub3d->last_frame_time = mlx_get_time();
	}
}

void	keyboard_hook(mlx_key_data_t keydata, void *param)
{
	t_cub3d	*cub3d ;

	cub3d = (t_cub3d *) param ;
	if (keydata.key == MLX_KEY_ESCAPE)
		close_success(cub3d);
	if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_PRESS)
		raycast_to_door(cub3d, false);
}

void	mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods,
	void *param)
{
	t_cub3d	*cub3d ;

	(void) mods ;
	cub3d = (t_cub3d *) param ;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS
		&& cub3d->frame == -1)
	{
		raycast_to_door(cub3d, true);
		cub3d->frame = 0 ;
	}
}
