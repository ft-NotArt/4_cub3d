/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaveo <kaveo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 18:25:18 by kaveo             #+#    #+#             */
/*   Updated: 2025/02/07 03:46:55 by kaveo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	count_path_words(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		while (ft_isspace(line[i]))
		{
			i++;
		}
		if (line[i])
		{
			count++;
		}
		while (line[i] && !ft_isspace(line[i]))
		{
			i++;
		}
	}
	if (count != 2)
		return (false);
	return (true);
}

static int	get_path_len(char *temp)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (!ft_isspace(temp[i]))
	{
		i++;
	}
	while (ft_isspace(temp[i]))
	{
		i++;
	}
	while (!ft_isspace(temp[i]))
	{
		i++;
		len++;
	}
	return (len);
}

char	*get_texture_path(char *line, char c, char c1)
{
	int		i;
	int		j;
	char	*temp;
	char	*path;

	if (!count_path_words(line))
	{
		ft_printf_fd(2, "Error\nInvalid words count on %c%c texture line\n", c, c1);
		exit(1);
	}
	temp = ft_strtrim(line, " 	");
	free(line);
	i = 0;
	if (temp[i] != c || temp[i + 1] != c1)
	{
		ft_printf_fd(2, "Error\nInvalid character on %c%c texture line\n", c, c1);
		exit(1);
	}
	i += 2;
	while (temp[i] && ft_isspace(temp[i]))
		i++;
	path = malloc(sizeof(char *) + (get_path_len(temp)));
	j = 0;
	while (temp[i] && !ft_isspace(temp[i]))
		path[j++] = temp[i++];
	path[j] = '\0';
	free(temp);
	return (path);
}

bool	get_path_by_id(t_txtr_id id, t_parsing *parsing, char *line)
{
	if (id == NO && !parsing->paths[NO])
		parsing->paths[NO] = get_texture_path(line, 'N', 'O');
	else if (id == SO && !parsing->paths[SO])
		parsing->paths[SO] = get_texture_path(line, 'S', 'O');
	else if (id == WE && !parsing->paths[WE])
		parsing->paths[WE] = get_texture_path(line, 'W', 'E');
	else if (id == EA && !parsing->paths[EA])
		parsing->paths[EA] = get_texture_path(line, 'E', 'A');
	else if (id == DO && !parsing->paths[DO])
		parsing->paths[DO] = get_texture_path(line, 'D', 'O');
	else if (id == FL && !parsing->paths[FL])
		parsing->paths[FL] = get_texture_path(line, 'F', ' ');
	else if (id == CE && !parsing->paths[CE])
		parsing->paths[CE] = get_texture_path(line, 'C', ' ');
	else
	{
		ft_printf_fd(2, "Error\nPath duplication in map !\n");
		return (false);
	}
	return (true);
}

int	get_identifier_id(char *line)
{
	if (ft_strnstr(line, "NO", ft_strlen(line)))
		return (NO);
	else if (ft_strnstr(line, "SO", ft_strlen(line)))
		return (SO);
	else if (ft_strnstr(line, "WE", ft_strlen(line)))
		return (WE);
	else if (ft_strnstr(line, "EA", ft_strlen(line)))
		return (EA);
	else if (ft_strnstr(line, "DO", ft_strlen(line)))
		return (DO);
	else if (ft_strnstr(line, "F", ft_strlen(line)))
		return (FL);
	else if (ft_strnstr(line, "C", ft_strlen(line)))
		return (CE);
	return (-1);
}

int	get_higher_len(t_parsing *parsing)
{
	int		i;
	size_t	len;

	i = 0;
	len = 0;
	while (parsing->map[i])
	{
		if (ft_strlen(parsing->map[i]) > len)
		{
			len = ft_strlen(parsing->map[i]);
		}
		i++;
	}
	return (len);
}

void	flood_fill(char **map, int x, int y, int height)
{
	if ((x == -1 || y == -1) || (x == (height - 1) && map[x][y] == '0'))
	{
		printf("Map is not closed\n");
		exit(1);
	}
	if (map[x][y] == ' ' || map[x][y] == '\n')
	{
		printf("Map is not closed\n");
		exit(1);
	}
	if (map[x][y] == '1' || map[x][y] == 'V')
		return ;
	map[x][y] = 'V';
	flood_fill(map, x - 1, y, height);
	flood_fill(map, x + 1, y, height);
	flood_fill(map, x, y - 1, height);
	flood_fill(map, x, y + 1, height);
}

int	flood_map_height(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		i++;
	}
	return (i);
}

bool	is_playable_map(t_parsing *parsing)
{
	char	**flood_map;

	flood_map = create_floodfill_map(parsing);
	get_player_pos(flood_map, parsing);
	flood_fill(flood_map, parsing->player_x, parsing->player_y, flood_map_height(flood_map));
	return (true);
}

char	**create_floodfill_map(t_parsing *parsing)
{
	char	**map;
	int 	i;
	int 	j;

	map = malloc(sizeof(char **) * (parsing->map_height + 1));
	i = 0;
	while (parsing->map[i])
	{
		j = 0;
		map[i] = malloc(sizeof(char *) * (get_higher_len(parsing) + 2));
		ft_strlcpy(map[i], parsing->map[i], ft_strlen(parsing->map[i]) + 1);
		while (map[i][j])
			j++;
		while (j < get_higher_len(parsing))
		{
			map[i][j] = ' ';
			j++;
		}
		map[i][j] = '\n';
		map[i][j + 1] = '\0';
		i++;
	}
	map[i] = NULL;
	return (map);
}

void	get_player_pos(char **map, t_parsing *parsing)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if ((map[i][j] == 'N' || map[i][j] == 'S'
				|| map[i][j] == 'W' || map[i][j] == 'E'))
			{
				parsing->player_x = i;
				parsing->player_y = j;
			}
			j++;
		}
		i++;
	}
}
static int	handle_open(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror(filename);
		return (-1);
	}
	return (fd);
}

static char	*handle_map(char *map_in_line, char *line)
{
	char	*temp;

	if (line[0] == '\n' || !is_map_charset(line))
	{
		ft_printf_fd(2, "Error\nInvalid characters in the map !\n");
		exit(1);
	}
	temp = ft_strjoin(map_in_line, line);
	map_in_line = temp;
	return (map_in_line);
}


// TODO make this functions shorted and the rest of the parsing greater
char	**get_map_data(char *filename, t_parsing *parsing)
{
	int		fd;
	char	**map = NULL;
	char	*line;
	char	*map_in_line;
	char	*temp;
	char	**id = malloc(sizeof(char **) * (PATHS_COUNT + 1));
	int i = 0;

	fd = handle_open(filename);
	line = get_next_line(fd);
	map_in_line = ft_strdup("");
	while (line)
	{
		if (is_map_identifier(line))
			id[i++] = ft_strdup(line);
		else if (is_map_charset(line))
		{
			while (line)
			{
				temp = map_in_line;
				map_in_line = handle_map(map_in_line, line);
				free(temp);
				parsing->map_height++;
				free(line);
				line = get_next_line(fd);
			}
		}
		free(line);
		line = get_next_line(fd);
	}
	id[i] = NULL;
	map = ft_split(map_in_line, '\n');
	free(map_in_line);
	free_str_array(id);
	return (map);
}
