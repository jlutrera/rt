/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:55:04 by jutrera-          #+#    #+#             */
/*   Updated: 2025/03/10 21:13:24 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	intersect_pl(t_ray ray, t_plane *plane, t_point2 *t)
{
	t_vec	op;
	double	denom;
	double	num;

	denom = vec_dot(plane->direction, ray.dir);
	op = vec_sub(point_to_vec(plane->coordinate), point_to_vec(ray.origin));
	num = vec_dot(plane->direction, op);
	if (fabs(denom) < EPSILON)
		*t = (t_point2){INFINITY, INFINITY};
	else
		*t = (t_point2){num / denom, num / denom};
}

t_point3	compute_pl_colour_light(t_plane *pl, t_scene scene, t_vec p)
{
	t_vec		n;
	t_point3	i;
	t_point3	intensity;
	t_point3	shadow;

	n = vec_unit(pl->direction);
	i = compute_colour_lighting(scene, p, n);
	shadow = compute_shadows(scene, p, n);
	intensity.x = i.x - shadow.x + scene.ambient.ratio
		* scene.ambient.color.r / 255 ;
	intensity.y = i.y - shadow.y + scene.ambient.ratio
		* scene.ambient.color.g / 255 ;
	intensity.z = i.z - shadow.z + scene.ambient.ratio
		* scene.ambient.color.b / 255;
	return ((t_point3){fmax(10, intensity.x * pl->color.r), fmax(10,
			intensity.y * pl->color.g), fmax(10, intensity.z * pl->color.b)});
}

t_plane	*new_pl(char **s, int *e)
{
	t_plane	*new_plane;

	new_plane = calloc(sizeof(t_plane), 1);
	if (!new_plane)
	{
		*e = MEMORY_E;
		return (NULL);
	}
	*e = SUCCESS;
	if (ft_get_point(s[1], &new_plane->coordinate) == -1
		|| ft_get_vector(s[2], &new_plane->direction) == -1)
		*e = BAD_COORDINATES_E;
	else if (!is_normalized(new_plane->direction))
		*e = NORM_VECTOR_E;
	else if (ft_get_color(s[3], &new_plane->color) == -1)
		*e = COLOUR_E;
	if (*e == SUCCESS)
		return (new_plane);
	return (free(new_plane), NULL);
}

int	ft_load_pl(t_lst_obj **obj, char **s)
{
	int			e;
	t_plane		*new_plane;

	if (check_comps(s, 4))
		return (NUM_COMPONENTS_E);
	new_plane = new_pl(s, &e);
	if (!new_plane)
		return (e);
	ft_add_back_obj(obj, (void **)&new_plane, PLANE, ft_get_dist());
	return (SUCCESS);
}
