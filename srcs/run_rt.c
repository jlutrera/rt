
#include "rt.h"

static void	ft_init(t_scene **scene)
{
	*scene = (t_scene *) malloc (sizeof(t_scene));
	if (!(*scene))
		exit (EXIT_FAILURE);
	(*scene)->ambient.declared = false;
	(*scene)->light = NULL;
	(*scene)->obj = NULL;
	(*scene)->camera = NULL;
}

static void	free_light_camera(t_light *light, t_camera *camera)
{
	t_light		*l;
	t_camera	*c;

	while (light)
	{
		l = light;
		light = light->next;
		free(l);
	}
	free(light);
	while (camera)
	{
		c = camera;
		camera = camera->next;
		free(c);
	}
	free(camera);
}

void	free_memory(t_scene *scene)
{
	t_lst_obj	*aux;

	if (scene->obj)
	{
		while (scene->obj)
		{
			aux = scene->obj;
			scene->obj = scene->obj->next;
			free(aux->object);
			free(aux);
		}
	}
	free_light_camera(scene->light, scene->camera);
	free(scene);
}

static void	init_mlx(t_data *data, t_scene *scene)
{
	data->image.width = WIDTH;
	if (WIDTH > 2500 || WIDTH < 100)
		data->image.width = 2500;
	data->image.height = floor(data->image.width / ASPECTRATIO);
	if (data->image.height < 1)
		data->image.height = 1;
	data->vars.mlx = mlx_init();
	if (!data->vars.mlx)
		exit (EXIT_FAILURE);
	data->vars.win = mlx_new_window(data->vars.mlx, data->image.width, data->image.height, "RT");
	if (!data->vars.win)
		exit (EXIT_FAILURE);
	data->image.img = mlx_new_image(data->vars.mlx, data->image.width, data->image.height);
	if (!data->image.img)
		exit (EXIT_FAILURE);
	data->image.addr = mlx_get_data_addr(data->image.img, &data->image.bits_per_pixel, &data->image.line_length, &data->image.endian);
	if (!data->image.addr)
		exit (EXIT_FAILURE);
	data->scene = scene;
}

// int	main(int argc, char **argv)
// {
// 	t_data		data;
// 	t_scene		*scene;
// 	int			error;
// 	int			n;

// 	if (argc != 2)
// 		return (ft_errormsg(SYNTAX_E, 0));
// 	if (WIDTH <= 0)
// 		return (printf("Bad resolution !\n"), 0);
// 	ft_init(&scene);
// 	n = 0;
// 	error = process_file(argv[1], &scene, &n);
// 	if (error)
// 	{
// 		free_memory(scene);
// 		return (ft_errormsg(error, n));
// 	}
// 	init_mlx(&data, scene);
// 	process_img(&data, scene);
// 	printf("END RENDER\n");
// 	my_hooks(&data);
// 	mlx_loop(data.vars.mlx);

// 	return (SUCCESS);
// }


// Función que ejecuta el renderizado
int run_rt(char *filename)
{
	t_data		data;
	t_scene		*scene;
	int			error;
	int			n;

	ft_init(&scene);
	n = 0;
	error = process_file(filename, &scene, &n);
	if (error)
	{
		free_memory(scene);
		return (ft_errormsg(error, n));
	}

	init_mlx(&data, scene);
	process_img(&data, scene);
	printf("END RENDER\n");
	my_hooks(&data);
	mlx_loop(data.vars.mlx);

	return (SUCCESS);
}