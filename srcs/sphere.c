/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:49:01 by jutrera-          #+#    #+#             */
/*   Updated: 2025/03/10 21:13:27 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	ft_load_sp(t_lst_obj **obj, char **s)
{
	int			e;
	t_sphere	*new_sphere;

	if (check_comps(s, 4))
		return (NUM_COMPONENTS_E);
	new_sphere = new_sp(s, &e);
	if (!new_sphere)
		return (e);
	ft_add_back_obj(obj, (void **)&new_sphere, SPHERE, ft_get_dist());
	return (SUCCESS);
}

t_sphere	*new_sp(char **s, int *e)
{
	t_sphere	*new_sphere;

	new_sphere = calloc(sizeof(t_sphere), 1);
	if (!new_sphere)
	{
		*e = MEMORY_E;
		return (NULL);
	}
	new_sphere->radius = ft_atod(s[2]) / 2;
	*e = SUCCESS;
	if (ft_get_point(s[1], &new_sphere->center) == -1)
		*e = BAD_COORDINATES_E;
	else if (new_sphere->radius <= 0)
		*e = NEGATIVE_E;
	else if (ft_get_color(s[3], &new_sphere->color) == -1)
		*e = COLOUR_E;
	if (*e == SUCCESS)
		return (new_sphere);
	return (free(new_sphere), NULL);
}

void	intersect_sp(t_ray ray, t_sphere *sp, t_point2 *t)
{
	t_vec	co;
	double	a;
	double	b;
	double	c;
	double	discriminant;

	co = vec_sub(point_to_vec(ray.origin), point_to_vec(sp->center));
	a = vec_dot(ray.dir, ray.dir);
	b = 2 * vec_dot(co, ray.dir);
	c = vec_dot(co, co) - sp->radius * sp->radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < EPSILON)
		*t = (t_point2){INFINITY, INFINITY};
	else
		*t = (t_point2){(-b + sqrt(discriminant))
			/ (2 * a), (-b - sqrt(discriminant)) / (2 * a)};
}

/**
 * @brief Compute the color of a point on a sphere under lighting conditions.
 * 
 * This function calculates the color of a point on a sphere based on the scene's
 * lighting conditions. It takes into account the ambient light, the direct light
 * from light sources, and shadows cast by other objects. The resulting color is
 * a combination of the sphere's inherent color and the lighting conditions.
 * 
 * @param sp Pointer to the sphere object.
 * @param scene The scene containing objects, lights, and other relevant data.
 * @param p The point on the sphere for which the color is being computed.
 * 
 * @return The computed color of the point on the sphere.
 */

t_point3	compute_sp_colour_light(t_sphere *sp, t_scene scene, t_vec p)
{
	t_vec		n;
	t_point3	i;
	t_point3	shadow;
	t_point3	intensity;

	n = vec_unit(vec_sub(p, point_to_vec(sp->center)));
	i = compute_colour_lighting(scene, p, n);
	shadow = compute_shadows(scene, p, n);
	intensity.x = i.x - shadow.x + scene.ambient.ratio
		* scene.ambient.color.r / 255;
	intensity.y = i.y - shadow.y + scene.ambient.ratio
		* scene.ambient.color.g / 255;
	intensity.z = i.z - shadow.z + scene.ambient.ratio
		* scene.ambient.color.b / 255;
	return ((t_point3){fmax(10, intensity.x * sp->color.r), fmax(10,
			intensity.y * sp->color.g), fmax(10, intensity.z * sp->color.b)});
}
