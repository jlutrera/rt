/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 13:46:53 by adpachec          #+#    #+#             */
/*   Updated: 2025/03/10 20:16:14 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	get_rgb(t_color c)
{
	return (c.r << 16 | c.g << 8 | c.b);
}

int	write_color(t_point3 pixel_color)
{
	if (pixel_color.x > 255)
		pixel_color.x = 255;
	if (pixel_color.y > 255)
		pixel_color.y = 255;
	if (pixel_color.z > 255)
		pixel_color.z = 255;
	if (pixel_color.x < 0)
		pixel_color.x = 0;
	if (pixel_color.y < 0)
		pixel_color.y = 0;
	if (pixel_color.z < 0)
		pixel_color.z = 0;
	return ((int)pixel_color.x << 16
		| (int)pixel_color.y << 8 | (int)pixel_color.z);
}
