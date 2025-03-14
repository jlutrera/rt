/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 12:06:47 by adpachec          #+#    #+#             */
/*   Updated: 2025/03/13 20:22:10 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include 	<string.h>
# include 	<stdlib.h>
# include 	<unistd.h>
# include 	<stdio.h>
# include 	<stdint.h>
# include 	<stdbool.h>
# include 	<fcntl.h>
# include 	<stdarg.h>
# include 	<math.h>
# include 	<limits.h>
# include 	"errors.h"
# include 	"structs.h"

// para usar gtk
#include <gtk/gtk.h>
#include <cairo.h>
#include <pthread.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <fontconfig/fontconfig.h>

# include 	"../mlx/include/mlx.h"

# define 	K_ESC   	0xFF1B
# define 	K_LEFT  	0xFF51
# define 	K_RIGHT 	0xFF53

// Window options
# define 	WIDTH 		1080
# define 	ASPECTRATIO	1.78  //16:9
# define	TOOLBAR_H 	50

// Math constants
# define 	EPSILON 	0.000001
# define 	SPECULAR 	100
# define	M_PI 		3.14159265358979323846

// color.c
int			get_rgb(t_color c);
int			write_color(t_point3 pixel_color);
// cone.c
t_point3	compute_co_colour_light(t_cone *co, t_scene scene, t_vec p);
int			ft_load_co(t_lst_obj **obj, char **s);
t_cone		*new_co(char **s, int *e);
// cone2.c
void		intersect_co(t_ray ray, t_cone *co, t_point2 *t);
// cylinder.c
t_point3	compute_cy_colour_light(t_cylinder *cy, t_scene scene, t_vec p);
int			ft_load_cy(t_lst_obj **obj, char **s);
t_cylinder	*new_cy(char **s, int *e);
//cylinder2.c
void		intersect_cy(t_ray ray, t_cylinder *cy, t_point2 *t);
// ft_atoi.c
int			ft_atoi(const char *str);
// ft_atol.c
long		ft_atol(const char *str);
// ft_gnl.c
char		*get_next_line(int fd, char **buff);
// ft_split.c
char		**ft_split(char const *s, char c);
// ft_strlen.c
int			ft_strlen(char *s);
// ft_strtrim.c
char		*ft_strtrim(char const *s1, char const *set);
// light_shadow.c
t_point3	compute_shadows(t_scene scene, t_vec p, t_vec n);
t_point3	compute_colour_lighting(t_scene scene, t_vec p, t_vec n);
// my_hooks.c
void		my_hooks(t_data *vars);
void		free_memory(t_scene *scene);
// parser_gets.c
int			ft_get_vector(char *s, t_vec *direction);
int			ft_get_point(char *s, t_point3 *position);
int			ft_get_color(char *s, t_color *color);
void		ft_free(char **aux);
// parser_lists.c
t_lst_obj	*ft_init_obj(void *object, t_obj_type type, double last_dist);
t_lst_obj	*ft_obj_last(t_lst_obj *obj);
void		ft_add_back_obj(t_lst_obj **obj, void **object, t_obj_type type, double last_dist);
// parser_loads.c
int			ft_load_ambient(t_ambient *ambient, char **s);
int			ft_load_camera(t_camera **camera, char **s);
int			ft_load_light(t_light **light, char **s);
// parser_utils.c
double		ft_get_dist(void);
int			is_normalized(t_vec vector);
double		ft_atod(char *s);
int			check_comps(char **s, int n);
// parser.c
int			parse_line(char *line, t_scene **scene);
// plane.c
void		intersect_pl(t_ray ray, t_plane *plane, t_point2 *t);
t_point3	compute_pl_colour_light(t_plane *pl, t_scene scene, t_vec p);
int			ft_load_pl(t_lst_obj **obj, char **s);
t_plane		*new_pl(char **s, int *e);
// print_errors.c
int			ft_errormsg(int e, int n);
// process_file.c
int			process_file(char *file, t_scene **scene, int *n);
// process_img.c
void		process_img(t_data *data, t_scene *scene);
// ray.c
t_vec		ray_ex(t_ray *ray, double t);
t_ray		ray(t_point3 origin, t_vec dir);
// sphere.c
void		intersect_sp(t_ray ray, t_sphere *sp, t_point2 *t);
t_point3	compute_sp_colour_light(t_sphere *sp, t_scene scene, t_vec p);
t_sphere	*new_sp(char **s, int *e);
int			ft_load_sp(t_lst_obj **obj, char **s);
// triangle.c
void		intersect_tr(t_ray ray, t_triangle *tr, t_point2 *t);
t_point3	compute_tr_colour_light(t_triangle *tr, t_scene scene, t_vec p);
int			ft_load_tr(t_lst_obj **obj, char **s);
t_triangle	*new_tr(char **s, int *e);
// vectors1.c
t_vec		vec(double x, double y, double z);
t_vec		vec_add(t_vec v1, t_vec v2);
t_vec		vec_sub(t_vec v1, t_vec v2);
t_vec		vec_mul(t_vec v, double t);
t_vec		vec_rotate(t_vec v, t_vec d);
// vectors2.c
double		vec_dot(t_vec v1, t_vec v2);
t_vec		vec_cross(t_vec v1, t_vec v2);
double		vec_length(t_vec v);
t_vec		vec_unit(t_vec v);
t_vec		vec_divition(t_vec v1, double t);
// vectors3.c
t_vec		point_to_vec(t_point3 P);
t_point3	vec_to_point(t_vec v);

#endif
