#include <gtk/gtk.h>
#include <cairo.h>
#include <pthread.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string.h>
#include <stdlib.h>
#include "./mlx/include/mlx.h"
// Estructura para pasar argumentos al hilo
typedef struct {
    char *filename;
}  ThreadData;

// Función que se ejecutará en el hilo
void* run_rt(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    char command[2048];
    snprintf(command, sizeof(command), "./rt \"%s\"", data->filename);
    int result = system(command);

    if (result == 0) {
        g_print("rt application executed successfully.\n");
    } else {
        g_print("rt application failed to execute (return code: %d).\n", result);
    }

    g_free(data->filename);
    g_free(data); // Liberar la estructura ThreadData
    pthread_exit(NULL);
}

void load_scene(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    dialog = gtk_file_chooser_dialog_new("Load",
                                        GTK_WINDOW(data),
                                        action,
                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                        "_Open", GTK_RESPONSE_ACCEPT,
                                        NULL);
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
	gtk_file_chooser_set_current_folder(chooser, "./scenes");
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(chooser);
        g_print("Selected file: %s\n", filename);

        // Crear la estructura para pasar argumentos al hilo
        ThreadData *thread_data = g_malloc(sizeof(ThreadData));
        thread_data->filename = g_strdup(filename); // Duplicar el nombre del archivo

        // Crear el hilo para ejecutar "rt"
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, run_rt, (void *)thread_data);
        pthread_detach(thread_id); // Desvincular el hilo para que se limpie automáticamente

        g_free(filename); // Liberar el nombre del archivo original
    }
    gtk_widget_destroy(dialog);
}

void save_scene(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    GtkWidget *window = (GtkWidget *)data;

    dialog = gtk_file_chooser_dialog_new("Save Scene",
                                        GTK_WINDOW(window),
                                        action,
                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                        "_Save", GTK_RESPONSE_ACCEPT,
                                        NULL);

    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);

    // Filtro JPG
    GtkFileFilter *filter_jpg = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter_jpg, "*.jpg");
    gtk_file_filter_add_pattern(filter_jpg, "*.jpeg");
    gtk_file_filter_set_name(filter_jpg, "JPEG images");
    gtk_file_chooser_add_filter(chooser, filter_jpg);

    // Filtro PNG
    GtkFileFilter *filter_png = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter_png, "*.png");
    gtk_file_filter_set_name(filter_png, "PNG images");
    gtk_file_chooser_add_filter(chooser, filter_png);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(chooser);
        GdkPixbuf *pixbuf;

        GtkWidget *rt_window = gtk_widget_get_toplevel(widget);
        GdkWindow *rt_gdk_window = gtk_widget_get_window(rt_window);
        gint width = gdk_window_get_width(rt_gdk_window);
        gint height = gdk_window_get_height(rt_gdk_window);

        pixbuf = gdk_pixbuf_get_from_window(rt_gdk_window, 0, 0, width, height);

        if (pixbuf) {
			g_print("Pixbuf width: %d, height: %d, has alpha: %d\n",
				gdk_pixbuf_get_width(pixbuf),
				gdk_pixbuf_get_height(pixbuf),
				gdk_pixbuf_get_has_alpha(pixbuf));
	
			// Código de vista previa (opcional)
			GtkWidget *preview_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
			GtkWidget *preview_image = gtk_image_new_from_pixbuf(pixbuf);
			gtk_container_add(GTK_CONTAINER(preview_window), preview_image);
			gtk_widget_show_all(preview_window);
            
			GtkFileFilter *selected_filter = gtk_file_chooser_get_filter(chooser);
            const char *filter_name = gtk_file_filter_get_name(selected_filter);
            char *save_filename = g_strdup(filename); // Duplicar el nombre del archivo

            if (strcmp(filter_name, "JPEG images") == 0) {
                if (strstr(save_filename, ".jpg") == NULL && strstr(save_filename, ".jpeg") == NULL) {
                    save_filename = g_strconcat(save_filename, ".jpg", NULL);
                }
                gdk_pixbuf_save(pixbuf, save_filename, "jpeg", NULL, NULL);
            } else if (strcmp(filter_name, "PNG images") == 0) {
                if (strstr(save_filename, ".png") == NULL) {
                    save_filename = g_strconcat(save_filename, ".png", NULL);
                }
                gdk_pixbuf_save(pixbuf, save_filename, "png", NULL, NULL);
            }

            g_free(save_filename); // Liberar el nombre del archivo con extensión
            g_object_unref(pixbuf);
        } else {
            g_print("Failed to capture image from rt window.\n");
        }

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

void create_scene(GtkWidget *widget, gpointer data)
{
	g_print("Create: función aún no implementada.\n");
}


// Función principal
int main(int argc, char *argv[])
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
	const void *functions[] = {load_scene, save_scene, create_scene};
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

	return 0;
}
