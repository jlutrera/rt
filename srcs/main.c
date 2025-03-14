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

void load_scene(GtkWidget *widget, gpointer data)
{
	int error = 0;

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
		gtk_widget_destroy(dialog);
		filename = gtk_file_chooser_get_filename(chooser);
		error = run_rt(filename);
		if (!error)
			printf("File %s rendered successfuly.n", filename);
		free(filename);
	}
}

void save_scene(GtkWidget *widget, gpointer data)
{
	(void)widget;
	(void)data;
	printf("Save: función aún no implementada.\n");
}

void create_scene(GtkWidget *widget, gpointer data)
{
	(void)widget;
	(void)data;
	printf("Create: función aún no implementada.\n");
}

void quit_button(GtkWidget *widget, gpointer data)
{
	(void)widget;
	(void)data;
	printf("Quit: función aún no implementada.\n");
	gtk_main_quit();
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
	const char *button_labels[] = {"Load", "Save", "Create", "Exit"};
	void (*functions[])(GtkWidget *, gpointer) = {load_scene, save_scene, create_scene, quit_button};
	// Añadir un espacio vacío al final (spacer)
	GtkWidget *spacerup = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(vbox), spacerup, TRUE, TRUE, 0);

	// Crear un widget de imagen para "splash.jpg"
	GtkWidget *image = gtk_image_new_from_file("./images/splash.jpeg");
	gtk_box_pack_start(GTK_BOX(vbox), image, TRUE, TRUE, 0);

	for (int i = 0; i < 4; i++)
	{
    	// Crear un botón
		GtkWidget *button = gtk_button_new_with_label(button_labels[i]);
		g_signal_connect(button, "clicked", G_CALLBACK(functions[i]), NULL);

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

	return 0;
}
