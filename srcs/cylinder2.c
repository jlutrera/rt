/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:01:36 by jutrera-          #+#    #+#             */
/*   Updated: 2025/03/10 21:13:21 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static double	intersect_disk(t_ray ray, t_vec center,	t_vec normal, double radius)
{
	double	denom;
	double	num;
	double	t;
	t_vec	p;

	denom = vec_dot(normal, ray.dir);
	if (fabs(denom) < EPSILON)
		return (INFINITY);
	num = vec_dot(normal, vec_sub(center, point_to_vec(ray.origin)));
	t = num / denom;
	if (t < EPSILON)
		return (INFINITY);
	p = vec_add(point_to_vec(ray.origin), vec_mul(ray.dir, t));
	if (vec_length(vec_sub(p, center)) - radius >= EPSILON)
		return (INFINITY);
	return (t);
}

static double	calc_tmin(double t1, double t2, t_cylinder *cy, t_ray ray)
{
	t_vec	p;
	double	h;
	t_vec	d;
	double	t_min;

	if (t1 <= EPSILON)
		t1 = INFINITY;
	if (t2 <= EPSILON)
		t2 = INFINITY;
	if (t1 < t2)
		t_min = t1;
	else
		t_min = t2;
	if (t_min == INFINITY)
		return (INFINITY);
	d = vec_unit(cy->direction);
	p = vec_add(point_to_vec(ray.origin),
			vec_mul(ray.dir, t_min));
	h = vec_dot(vec_sub(p, point_to_vec(cy->coordinate)), d);
	if (h < EPSILON || h - cy->height > EPSILON)
		return (INFINITY);
	return (t_min);
}

static double	solve_equation(t_cylinder *cy, t_vec oc, t_vec od, t_ray ray)
{
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	sqrt_discriminant;

	a = vec_dot(od, od);
	b = 2 * vec_dot(oc, od);
	c = vec_dot(oc, oc) - cy->radius * cy->radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < EPSILON)
		return (INFINITY);
	sqrt_discriminant = sqrt(discriminant);
	return (calc_tmin((-b + sqrt_discriminant) / (2 * a),
			(-b - sqrt_discriminant) / (2 * a), cy, ray));
}

void	intersect_cy(t_ray ray, t_cylinder *cy, t_point2 *t)
{
	t_vec	cd;
	t_vec	oc;
	t_vec	od;
	double	t_min;
	t_point2	t_base;

	cd = vec_unit(cy->direction);
	oc = vec_sub(point_to_vec(ray.origin), point_to_vec(cy->coordinate));
	oc = vec_sub(oc, vec_mul(cd, vec_dot(oc, cd)));
	od = vec_sub(ray.dir, vec_mul(cd, vec_dot(ray.dir, cd)));
	if (vec_length(od) < EPSILON)
		od = vec(ray.dir.y, -ray.dir.x, ray.dir.z);
	t_min = solve_equation(cy, oc, od, ray);
	t_base.x = intersect_disk(ray, point_to_vec(cy->coordinate),
			cd, cy->radius);
	if (t_base.x < t_min)
		t_min = t_base.x;
	t_base.y = intersect_disk(ray, vec_add(point_to_vec(cy->coordinate),
				vec_mul(cd, cy->height)), cd, cy->radius);
	if (t_base.y < t_min)
		t_min = t_base.y;
	*t = (t_point2){t_min, t_min};
}
