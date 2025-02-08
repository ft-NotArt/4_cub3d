/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albillie <albillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 21:45:18 by kaveo             #+#    #+#             */
/*   Updated: 2025/02/08 00:00:51 by albillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	is_map_charset(char *line)
{
	if (ft_strchr(line, '0') || ft_strchr(line, '1'))
	{
		return (true);
	}
	return (false);
}

bool	is_path_charset(char *line)
{
	int	len;

	len = ft_strlen(line);
	if (ft_strnstr(line, "NO", len) || ft_strnstr(line, "SO", len)
		|| ft_strnstr(line, "WE", len) || ft_strnstr(line, "EA", len)
		|| ft_strnstr(line, "F", len) || ft_strnstr(line, "C", len)
		|| ft_strnstr(line, "DO", len))
	{
		return (true);
	}
	return (false);
}

bool	is_playable_map(t_parsing *parsing)
{
	char	**flood_map;

	flood_map = create_floodfill_map(parsing);
	print_map(flood_map);
	// flood_fill(flood_map, parsing->player_x, parsing->player_y, parsing->map_height);
	return (true);
}
