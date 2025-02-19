/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoteris <noterisarthur42@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 22:25:53 by anoteris          #+#    #+#             */
/*   Updated: 2025/02/19 17:03:12 by anoteris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static mlx_texture_t	**get_txtrs_txtrs(t_parsing *pars)
{
	mlx_texture_t	**txtrs ;
	int				i ;

	txtrs = malloc((NB_TXTRS + 1) * sizeof(mlx_texture_t *));
	i = -1 ;
	while (++i < NB_TXTRS)
	{
		txtrs[i] = mlx_load_png(pars->paths[i]);
		if (!txtrs[i])
		{
			txtrs[i] = NULL ;
			delete_textures_array(txtrs);
			return (NULL);
		}
	}
	txtrs[i] = NULL ;
	return (txtrs);
}

static mlx_texture_t	**get_txtrs_anim(void)
{
	mlx_texture_t	**anim ;
	int				i ;
	char			file[100];
	char			*itoa_res ;

	anim = malloc((NB_FRAMES + 1) * sizeof(mlx_texture_t *));
	i = -1 ;
	while (++i < NB_FRAMES)
	{
		ft_bzero(file, 100 * sizeof(char));
		ft_strlcpy(file, "textures/AWP_anim/AWP_",
			ft_strlen("textures/AWP_anim/AWP_") + 1);
		itoa_res = ft_itoa(i);
		ft_strlcat(file, itoa_res, ft_strlen(file) + ft_strlen(itoa_res) + 1);
		free(itoa_res);
		ft_strlcat(file, ".png", ft_strlen(file) + ft_strlen(".png") + 1);
		anim[i] = mlx_load_png(file);
		if (!anim[i])
		{
			anim[i] = NULL ;
			return (delete_textures_array(anim), NULL);
		}
	}
	anim[i] = NULL ;
	return (anim);
}

static void	cub3d_txtrs_to_img(t_cub3d *cub3d,
	mlx_texture_t **txtrs, mlx_texture_t **anim)
{
	int		i ;

	i = -1 ;
	while (++i < NB_TXTRS)
		cub3d->txtrs[i] = mlx_texture_to_image(cub3d->mlx, txtrs[i]);
	cub3d->txtrs[i] = NULL ;
	mlx_resize_image(cub3d->txtrs[FL], 256, 256);
	mlx_resize_image(cub3d->txtrs[CE], 256, 256);
	i = -1 ;
	while (++i < NB_FRAMES)
	{
		cub3d->frames[i] = mlx_texture_to_image(cub3d->mlx, anim[i]);
		mlx_resize_image(cub3d->frames[i],
			SCREENWIDTH * (3.0 / 4.0), SCREENHEIGHT * (2.0 / 3.0));
	}
	cub3d->frames[i] = NULL ;
}

static void	cub3d_init_mlx_part(t_cub3d *cub3d, t_parsing *pars,
	mlx_texture_t **txtrs, mlx_texture_t **anim)
{
	cub3d->mlx = mlx_init(SCREENWIDTH, SCREENHEIGHT, "CUB3D", false);
	if (!cub3d->mlx)
	{
		error_mlx();
		delete_textures_array(txtrs);
		delete_textures_array(anim);
		free_parsing(pars);
		free_cub3d(cub3d);
		exit(EXIT_FAILURE);
	}
	cub3d->screen = mlx_new_image(cub3d->mlx, SCREENWIDTH, SCREENHEIGHT);
	mlx_image_to_window(cub3d->mlx, cub3d->screen, 0, 0);
	mlx_set_instance_depth(cub3d->screen->instances, 0);
	cub3d_txtrs_to_img(cub3d, txtrs, anim);
	(delete_textures_array(txtrs), delete_textures_array(anim));
}

t_cub3d	*cub3d_init(t_parsing *pars)
{
	t_cub3d			*cub3d ;
	mlx_texture_t	**txtrs ;
	mlx_texture_t	**anim ;

	txtrs = get_txtrs_txtrs(pars);
	if (!txtrs)
		(error_mlx(), free_parsing(pars), exit(EXIT_FAILURE));
	anim = get_txtrs_anim();
	if (!anim)
		(error_mlx(), delete_textures_array(txtrs),
			free_parsing(pars), exit(EXIT_FAILURE));
	cub3d = malloc(sizeof(t_cub3d));
	cub3d->txtrs = malloc(sizeof(mlx_image_t *) * (NB_TXTRS + 1));
	ft_bzero(cub3d->txtrs, sizeof(mlx_image_t *) * (NB_TXTRS + 1));
	cub3d->frames = malloc(sizeof(mlx_image_t *) * (NB_FRAMES + 1));
	ft_bzero(cub3d->frames, sizeof(mlx_image_t *) * (NB_FRAMES + 1));
	cub3d->map = pars->map ;
	pars->map = NULL ;
	cub3d->map_width = pars->map_width ;
	cub3d->map_height = pars->map_height ;
	cub3d->raycast = raycast_init(pars);
	(cub3d_init_mlx_part(cub3d, pars, txtrs, anim), cub3d->last_frame_time = 0);
	cub3d->frame = -1 ;
	cub3d->minimap = minimap_init(cub3d);
	return (cub3d);
}
