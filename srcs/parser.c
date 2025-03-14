/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 20:34:36 by jutrera-          #+#    #+#             */
/*   Updated: 2025/03/09 20:34:36 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	ft_free_data(char **data)
{
	int	i;

	i = -1;
	while (data[++i])
		free(data[i]);
	free(data);
}

static bool	bad_line(char *line_aux)
{
	return (strlen(line_aux) > 1
		&& (line_aux[strlen(line_aux) - 1] == 13
			|| line_aux[strlen(line_aux) - 1] == 10));
}

static bool	data_is_empty(char **data)
{
	return (!data || !*data || data[0][0] == 0 ||
		data[0][0] == 13 || data[0][0] == 10);
}

static char	**load_data(char *line)
{
	char	*line_aux;
	char	**data;

	line_aux = ft_strtrim(line, " ");
	while (bad_line(line_aux))
		line_aux[strlen(line_aux) - 1] = '\0';
	data = ft_split(line_aux, ' ');
	if (!data)
		exit(1);
	free(line_aux);
	return (data);
}

int	parse_line(char *line, t_scene **scene)
{
	char	**data;
	int		error;

	data = load_data(line);
	if (data_is_empty(data))
		error = SUCCESS;
	else if (!strcmp("A", data[0]))
		error = ft_load_ambient(&(*scene)->ambient, data);
	else if (!strcmp("C", data[0]))
		error = ft_load_camera(&(*scene)->camera, data);
	else if (!strcmp("L", data[0]))
		error = ft_load_light(&(*scene)->light, data);
	else if (!strcmp("sp", data[0]))
		error = ft_load_sp(&((*scene)->obj), data);
	else if (!strcmp("pl", data[0]))
		error = ft_load_pl(&((*scene)->obj), data);
	else if (!strcmp("cy", data[0]))
		error = ft_load_cy(&((*scene)->obj), data);
	else if (!strcmp("tr", data[0]))
		error = ft_load_tr(&((*scene)->obj), data);
	else if (!strcmp("co", data[0]))
		error = ft_load_co(&((*scene)->obj), data);
	else
		error = BAD_IDENTIFIER_E;
	return (ft_free_data(data), error);
}
