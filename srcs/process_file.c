/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:35:58 by adpachec          #+#    #+#             */
/*   Updated: 2025/03/10 20:58:18 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	check_file_extension(const char *filename)
{
	const char	*dot;

	dot = strrchr(filename, '.');
	if (dot && !strcmp(dot, ".rt"))
		return (0);
	return (-1);
}

static void	change_tabs_for_spaces(char *line)
{
	int	i;

	i = -1;
	while ((line)[++i])
	{
		if ((line)[i] == '\t')
			(line)[i] = ' ';
	}
}

static int	end_msg(int error, t_scene **scene)
{
	if (error)
		return (error);
	if (!(*scene)->ambient.declared)
		return (AMBIENT_NOT_DECLARED);
	if (!(*scene)->camera)
		return (CAMERA_NOT_DECLARED);
	if (!(*scene)->light)
		return (LIGHT_NOT_DECLARED);
	return (SUCCESS);
}

int	process_file(char *file, t_scene **scene, int *n)
{
	int			fd;
	int			error;
	char		*line;
	static char	*buf;

	if (check_file_extension(file) == -1)
		return (EXTENSION_E);
	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (FILE_E);
	error = SUCCESS;
	line = get_next_line(fd, &buf);
	while (line && !error)
	{
		(*n)++;
		change_tabs_for_spaces(line);
		error = parse_line(line, scene);
		if (line)
			free(line);
		if (!error)
			line = get_next_line(fd, &buf);
	}
	free(buf);
	close(fd);
	return (end_msg(error, scene));
}
