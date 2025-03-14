/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 14:30:48 by jutrera-          #+#    #+#             */
/*   Updated: 2025/03/10 21:13:30 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/**
 * @brief Checks if a point lies inside a triangle.
 * 
 * @param tr Pointer to the triangle.
 * @param p Point to be checked.
 * @param n Normal vector of the triangle.
 * 
 * @return true if the point is inside the triangle, false otherwise.
 */

static bool	point_is_inside_tr(t_triangle *tr, t_vec p, t_vec n)
{
	t_vec	edge1;
	t_vec	edge2;
	t_vec	c;

	edge1 = vec_sub(point_to_vec(tr->p2), point_to_vec(tr->p1));
	edge2 = vec_sub(p, point_to_vec(tr->p1));
	c = vec_cross(edge1, edge2);
	if (vec_dot(n, c) < EPSILON)
		return (false);
	edge1 = vec_sub(point_to_vec(tr->p3), point_to_vec(tr->p2));
	edge2 = vec_sub(p, point_to_vec(tr->p2));
	c = vec_cross(edge1, edge2);
	if (vec_dot(n, c) < EPSILON)
		return (false);
	edge1 = vec_sub(point_to_vec(tr->p1), point_to_vec(tr->p3));
	edge2 = vec_sub(p, point_to_vec(tr->p3));
	c = vec_cross(edge1, edge2);
	if (vec_dot(n, c) < EPSILON)
		return (false);
	return (true);
}

/**
 * @brief Checks if a ray intersects with a triangle.
 * 
 * If there's an intersection, it calculates the intersection point and checks 
 * if this point lies inside the triangle.
 * 
 * @param ray The ray to check.
 * @param tr Pointer to the triangle.
 * @param t Pointer to store the intersection point.
 */

void	intersect_tr(t_ray ray, t_triangle *tr, t_point2 *t)
{
	t_vec	edge1;
	t_vec	edge2;
	t_vec	n;
	t_point2	v;
	t_vec	p;

	edge1 = vec_sub(point_to_vec(tr->p2), point_to_vec(tr->p1));
	edge2 = vec_sub(point_to_vec(tr->p3), point_to_vec(tr->p1));
	n = vec_cross(edge1, edge2);
	v.x = vec_dot(ray.dir, n);
	if (fabs(v.x) < EPSILON)
		return ;
	v.y = vec_dot(point_to_vec(tr->p1), n);
	t->x = (v.y - vec_dot(point_to_vec(ray.origin), n)) / v.x;
	if (t->x < 0)
	{
		*t = (t_point2){INFINITY, INFINITY};
		return ;
	}
	p = vec_add(point_to_vec(ray.origin), vec_mul(ray.dir, t->x));
	if (point_is_inside_tr(tr, p, n))
		*t = (t_point2){t->x, INFINITY};
	else
		*t = (t_point2){INFINITY, INFINITY};
}

/**
 * @brief Compute the color of a point on a triangle under lighting conditions.
 * 
 * @param tr Pointer to the triangle.
 * @param scene The scene containing objects, lights, and other relevant data.
 * @param p The point on the triangle for which the color is being computed.
 * 
 * @return The computed color of the point on the triangle.
 */

t_point3	compute_tr_colour_light(t_triangle *tr, t_scene scene, t_vec p)
{
	t_vec		n;
	t_point3	i;
	t_point3	shadow;
	t_point3	intensity;

	n = vec_cross(vec_sub(point_to_vec(tr->p2), point_to_vec(tr->p1)),
			vec_sub(point_to_vec(tr->p3), point_to_vec(tr->p1)));
	i = compute_colour_lighting(scene, p, n);
	shadow = compute_shadows(scene, p, n);
	intensity.x = i.x - shadow.x + scene.ambient.ratio
		* scene.ambient.color.r / 255;
	intensity.y = i.y - shadow.y + scene.ambient.ratio
		* scene.ambient.color.g / 255;
	intensity.z = i.z - shadow.z + scene.ambient.ratio
		* scene.ambient.color.b / 255;
	return ((t_point3){fmax(10, intensity.x * tr->color.r), fmax(10,
			intensity.y * tr->color.g), fmax(10, intensity.z * tr->color.b)});
}

/**
 * @brief Creates a new triangle object from the given string parameters.
 * 
 * @param s String parameters containing triangle data.
 * @param e Pointer to store the error code.
 * 
 * @return Pointer to the new triangle object, or NULL if there's an error.
 */

t_triangle	*new_tr(char **s, int *e)
{
	t_triangle	*new_triangle;

	new_triangle = calloc(sizeof(t_triangle), 1);
	if (!new_triangle)
	{
		*e = MEMORY_E;
		return (NULL);
	}
	*e = SUCCESS;
	if (ft_get_point(s[1], &new_triangle->p1) == -1)
		*e = BAD_COORDINATES_E;
	else if (ft_get_point(s[2], &new_triangle->p2) == -1)
		*e = BAD_COORDINATES_E;
	else if (ft_get_point(s[3], &new_triangle->p3) == -1)
		*e = BAD_COORDINATES_E;
	else if (ft_get_color(s[4], &new_triangle->color) == -1)
		*e = COLOUR_E;
	if (*e == SUCCESS)
		return (new_triangle);
	return (free(new_triangle), NULL);
}

int	ft_load_tr(t_lst_obj **obj, char **s)
{
	int			e;
	t_triangle	*new_triangle;

	if (check_comps(s, 5))
		return (NUM_COMPONENTS_E);
	new_triangle = new_tr(s, &e);
	if (!new_triangle)
		return (e);
	ft_add_back_obj(obj, (void **)&new_triangle, TRIANGLE, ft_get_dist());
	return (SUCCESS);
}
