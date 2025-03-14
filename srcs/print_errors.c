/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:34:23 by adpachec          #+#    #+#             */
/*   Updated: 2025/03/11 19:32:09 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	more_errormsg(int e, int n)
{
	if (e == NUM_COMPONENTS_E)
		printf("Line %i has a bad number of components\n", n);
	else if (e == RATIO_E)
		printf("Ratio number at line %i must be betwwen 0 and 1\n", n);
	else if (e == COLOUR_E)
		printf("Bad colour number at line %i\n", n);
	else if (e == BAD_COORDINATES_E)
		printf("Bad coordinates at line %i\n", n);
	else if (e == FOV_E)
		printf("FOV number at line %i must be between 0 and 180\n", n);
	else if (e == NORM_VECTOR_E)
		printf("The vector at line %i must be normalized\n", n);
	else if (e == BAD_BRIGHT_E)
		printf("Bright number at line %i must be between 0 and 1\n", n);
	else if (e == MEMORY_E)
		printf("Memory error\n");
	else if (e == NEGATIVE_E)
		printf("Bad number at line %i\n", n);
	else if (e == EXTENSION_E)
		printf("The file must have .rt extension\n");
	else
		printf("Unknown error\n");
}

int	ft_errormsg(int e, int n)
{
	printf("Error\n");
	if (e == FILE_E)
		return (printf("It is not possible to open the file\n"), e);
	if (e == SYNTAX_E)
		return (printf("Correct syntax: ./miniRT <file.rt>\n"), e);
	if (e == MORE_THAN_ONE_AMBIENT_E)
		return (printf("There is more than one Ambient element\n"), e);
	if (e == BAD_IDENTIFIER_E)
		return (printf("Line %i has a bad identifier.", n),
			printf(" Use A, C, L, sp, pl, cy\n"), e);
	if (e == AMBIENT_NOT_DECLARED)
		return (printf("The Ambient element is not declared\n"), e);
	if (e == CAMERA_NOT_DECLARED)
		return (printf("The Camera element is not declared\n"), e);
	if (e == LIGHT_NOT_DECLARED)
		return (printf("The Light element is not declared\n"), e);
	return (more_errormsg(e, n), e);
}
