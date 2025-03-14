/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 20:33:35 by jutrera-          #+#    #+#             */
/*   Updated: 2025/03/09 20:33:35 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// Función que se ejecutará en el hilo
void* run_rt(void *arg)
{
	t_data		data;
	t_scene		*scene;
	int			error;
	int			n;
    ThreadData	*fd;
	
	fd = (ThreadData *)arg;
	ft_init(&scene);
	n = 0;
	error = process_file(fd->filename, &scene, &n);
	if (error)
	{
		free_memory(scene);
		ft_errormsg(error, n);
	}
	else
	{
		init_mlx(&data, scene);
		process_img(&data, scene);
		printf("END RENDER\n");
		my_hooks(&data);
		mlx_loop(data.vars.mlx);
	}
	g_free(fd->filename);
    g_free(fd);
	return NULL;
}

void load_scene(GtkWidget *widget, gpointer data)
{
	(void)widget;
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

	dialog = gtk_file_chooser_dialog_new(
		"Load",
		GTK_WINDOW(data),
		action,
		"_Cancel", GTK_RESPONSE_CANCEL,
		"_Open", GTK_RESPONSE_ACCEPT,
		NULL);
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
	gtk_file_chooser_set_current_folder(chooser, "./scenes");
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		char *filename;
		filename = gtk_file_chooser_get_filename(chooser);
		g_print("Selected file: %s\n", filename);

		// Crear la estructura para pasar argumentos al hilo
		ThreadData *thread_data = g_malloc(sizeof(ThreadData));
		thread_data->filename = g_strdup(filename); // Duplicar el nombre del archivo

		// Crear el hilo para ejecutar "rt"
		pthread_t thread_id;
		pthread_create(&thread_id, NULL, run_rt, (void *)thread_data);

		g_free(filename); // Liberar el nombre del archivo original
	}
	gtk_widget_destroy(dialog);
}

void save_scene(GtkWidget *widget, gpointer data)
{
	(void)widget;
	(void)data;
	g_print("Save: función aún no implementada.\n");
}

void create_scene(GtkWidget *widget, gpointer data)
{
	(void)widget;
	(void)data;
	g_print("Create: función aún no implementada.\n");
}

// Función principal
int main(int argc, char **argv)
{
	// Inicializar GTK
	gtk_init(&argc, &argv);

	// Crear ventana principal
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Ray Tracing (jutrera-)");
	gtk_window_set_default_size(GTK_WINDOW(window),300, 300);

	// Conectar señal para cerrar la ventana
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Crear layout vertical
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	// Crear un estilo CSS para los botones
	GtkCssProvider *css_provider = gtk_css_provider_new();
	gtk_css_provider_load_from_data(css_provider,
		"button { "
		"   font-size: 18px; "
		"   font-weight: bold; "
		"   color: black; "
		"   background-color: #007ACC; "
		"   padding: 10px; "
		"	box-shadow: 2px 2px 5px 2px rgba(0, 0, 0, 0.25);  "
		"   border-radius: 8px; "
		"} ",
		-1, NULL);

	GtkStyleContext *context;

	// Crear botones con su respectivo estilo
	const char *button_labels[] = {"Load", "Save", "Create"};
	void (*functions[])(GtkWidget *, gpointer) = {load_scene, save_scene, create_scene};
	// Añadir un espacio vacío al final (spacer)
	GtkWidget *spacerup = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(vbox), spacerup, TRUE, TRUE, 0);

	// Crear un widget de imagen para "splash.jpg"
	GtkWidget *image = gtk_image_new_from_file("./images/splash.jpeg");
	gtk_box_pack_start(GTK_BOX(vbox), image, TRUE, TRUE, 0);

	for (int i = 0; i < 3; i++)
	{
    	// Crear un botón
		GtkWidget *button = gtk_button_new_with_label(button_labels[i]);
		g_signal_connect(button, "clicked", G_CALLBACK(functions[i]), window);

		// Asignar un tamaño fijo al botón
		gtk_widget_set_size_request(button, 200, 50); // Ancho: 200 px, Alto: 50 px

		// Añadir el botón al layout vertical
		gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

		// Centrar el botón horizontalmente
		gtk_widget_set_halign(button, GTK_ALIGN_CENTER);

		// Aplicar el estilo CSS al botón
		context = gtk_widget_get_style_context(button);
		gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
 	}

	// Añadir un espacio vacío al final (spacer)
	GtkWidget *spacerbottom = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(vbox), spacerbottom, TRUE, TRUE, 0);

	// Mostrar todo y ejecutar el loop principal
	gtk_widget_show_all(window);
	gtk_main();
	FcFini();
	return 0;
}
