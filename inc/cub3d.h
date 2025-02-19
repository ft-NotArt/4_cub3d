/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoteris <noterisarthur42@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:40:26 by anoteris          #+#    #+#             */
/*   Updated: 2025/02/19 16:54:33 by anoteris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/*** INCLUDE ***/

# include "libft.h"
# include "get_next_line.h"
# include "MLX42/MLX42.h"

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <math.h>
# define PI 3.14159265358979323846
# include <stdbool.h>

/*** DEFINE ***/

# ifndef SCREENWIDTH
#  define SCREENWIDTH 800
# endif

# ifndef SCREENHEIGHT
#  define SCREENHEIGHT 600
# endif

# define NB_TXTRS 7
# define NB_FRAMES 18

# define SPACES_SET " \t\r\v\f"

# define MOVE_SPEED 0.15
# define ROT_SPEED 0.05

# define MINIMAP_SIZE 250

// Colors
# define BLACK 0x000000FF
# define GRAY 0xC5C6C780
# define RED 0xFF0000FF
# define GREEN 0x00FF00FF
# define BLUE 0x0000FFFF

/*** TYPEDEF ***/

typedef enum s_txtr_id
{
	NO,
	SO,
	WE,
	EA,
	DO,
	FL,
	CE
}	t_txtr_id;

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

typedef struct s_vector
{
	double	x;
	double	y;
}	t_vector;

typedef struct s_parsing
{
	char			**map;
	char			**paths;
	char			*map_in_line;
	int				map_height;
	int				map_width;
	t_txtr_id		player_dir;
	struct s_vector	*pos;
}				t_parsing;

typedef struct s_raycasting
{
	struct s_vector	*pos;
	struct s_vector	*dir;
	struct s_vector	*plane;
	struct s_vector	*ray_dir;
	struct s_vector	*delta_dist;
	struct s_vector	*side_dist;
	struct s_vector	*tex;
	struct s_vector	*floor;
	struct s_point	*map;
	struct s_point	*step;
	int				line_height;
	int				draw_start;
	int				draw_end;
	double			perp_wall_dist;
	double			row_dist;
	t_txtr_id		side;
	bool			door;
}	t_raycast;

typedef struct s_minimap
{
	mlx_image_t	*img;
	int			x;
	int			y;
	double		play_x;
	double		play_y;
	double		angle;
	int			radius;
	int			center;
	double		map_size;
	double		rot_x;
	double		rot_y;
	int			dist_x;
	int			dist_y;
}	t_minimap;

typedef struct s_cub3d
{
	char				**map;
	int					map_height;
	int					map_width;
	mlx_t				*mlx;
	mlx_image_t			*screen;
	mlx_image_t			**txtrs;
	mlx_image_t			**frames;
	int					frame;
	struct s_raycasting	*raycast;
	double				last_frame_time ;
	struct s_minimap	*minimap;
}	t_cub3d;

/*** FUNCTIONS ***/

// Parsing

t_parsing	*init_parsing(void);
int			handle_open(char *filename);

// Args

bool		args_checker(int ac, char **av);

// Check

bool		check_map_spawn(char **map);
bool		check_path_order(char *line, t_txtr_id id);
bool		check_paths_count(t_parsing *parsing);

// Map

size_t		get_map_height(char **map);
size_t		get_higher_len(char **map);
bool		get_map_data(char *filename, t_parsing *parsing);
bool		check_map_chars(char **map);

// Path

bool		get_path_by_id(t_txtr_id id, t_parsing *parsing, char *line);
int			get_path_id(char *line);

// Floodfill

void		flood_fill(char **map, int y, int x, t_parsing *parsing);
char		**create_floodfill_map(t_parsing *parsing);

// Player

size_t		get_player_x_pos(char **map);
size_t		get_player_y_pos(char **map);
t_txtr_id	get_player_direction(char **map);

// Init
t_cub3d		*cub3d_init(t_parsing *pars);
t_raycast	*raycast_init(t_parsing *pars);
t_minimap	*minimap_init(t_cub3d *cub3d);
t_point		*point_init(int x, int y);
t_vector	*vector_init(double x, double y);

// Game
void		frame_loop(void *param);
void		keyboard_hook(mlx_key_data_t keydata, void *param);
void		mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods,
				void *param);
void		move(t_cub3d *cub3d, double dir1, double dir2, double move_speed);
void		rotate(t_cub3d *cub3d, double rot_speed);

// Display
void		set_visuals(t_cub3d *cub3d);
void		draw_crosshair(t_cub3d *cub3d);
void		put_awp_anim_to_window(t_cub3d *cub3d);
void		minimap(t_minimap *minimap, t_cub3d *cub3d);

// Raycasting
void		raycasting(t_cub3d *cub3d);
void		raycast_to_door(t_cub3d *cub3d, bool AWP);
void		draw_background(t_cub3d *cub3d, t_raycast *raycast,
				mlx_image_t *floor, mlx_image_t *ceilling);
void		calc_delta_dist(t_raycast *raycast);
void		calc_step_and_side_dist(t_raycast *raycast);
void		calc_line(t_raycast *raycast);
int			calc_tex_x(t_raycast *raycast, mlx_image_t *img);

// Is

bool		is_map_charset(char *line);
bool		is_path_charset(char *line);
bool		is_valid_map(char *filename, t_parsing *parsing);
bool		is_space_line(char *line);

// Utils
void		error_mlx(void);
void		delete_textures_array(mlx_texture_t **to_delete);
uint32_t	get_rgba(uint8_t r, uint8_t g, uint8_t b, bool shadow);

// Free
void		free_parsing(t_parsing *parsing);
void		free_raycast(t_raycast *raycast);
void		free_cub3d(t_cub3d *cub3d);

// Close
void		close_success(void *param);
void		close_failure(void *param);

#endif
