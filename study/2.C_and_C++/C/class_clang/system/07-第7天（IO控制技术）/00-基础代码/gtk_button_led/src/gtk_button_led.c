#include <gtk/gtk.h>
#include <string.h>
#include <pthread.h>

#include "gpio_led.h"

typedef struct{
	GtkWidget *background;
	GtkWidget *background_fixed;
	GtkWidget *background_image;
	GtkWidget *toggle_button_led1;
	GtkWidget *toggle_button_led2;
	GtkWidget *toggle_button_led3;
}gtk_ui_t;

void background_init(gtk_ui_t *ui)
{
	GdkPixbuf *imge_buf_src;
	GdkPixbuf *imge_buf_dest;
	
	ui->background = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(ui->background, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_position(GTK_WINDOW(ui->background), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(ui->background), 800, 480);
	
	ui->background_fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(ui->background), ui->background_fixed);

	imge_buf_src = gdk_pixbuf_new_from_file("../image/back.jpg", NULL);
	imge_buf_dest = gdk_pixbuf_scale_simple(imge_buf_src, 800, 480, GDK_INTERP_BILINEAR);
	g_object_unref(imge_buf_src);
	ui->background_image = gtk_image_new_from_pixbuf(imge_buf_dest);
	g_object_unref(imge_buf_dest);
	gtk_fixed_put(GTK_FIXED(ui->background_fixed), ui->background_image, 0, 0);
}

void button_callback(GtkWidget *widget, gpointer data)
{
	gtk_ui_t *ui = data;
	const gchar *str = NULL;

	str = gtk_button_get_label(GTK_BUTTON(widget));
	if(0 == strncmp(str, "LED1", strlen("LED1")))
	{
		printf("toggle_button_led1 pressed\n");
		
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->toggle_button_led1)) == TRUE)
		{
			led_control(3, 1);
		}
		else
		{
			led_control(3, 0);
		}
	}
	else if(0 == strncmp(str, "LED2", strlen("LED2")))
	{
		printf("toggle_button_led2 pressed\n");
		
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->toggle_button_led2)) == TRUE)
		{
			led_control(2, 1);
		}
		else
		{
			led_control(2, 0);
		}
	}
	if(0 == strncmp(str, "LED3", strlen("LED3")))
	{
		printf("toggle_button_led3 pressed\n");
		
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->toggle_button_led3)) == TRUE)
		{
			led_control(1, 1);
		}
		else
		{
			led_control(1, 0);
		}
	}
}

void button_init(gtk_ui_t *ui)
{
	ui->toggle_button_led1 = gtk_toggle_button_new_with_label("LED1");
	ui->toggle_button_led2 = gtk_toggle_button_new_with_label("LED2");
	ui->toggle_button_led3 = gtk_toggle_button_new_with_label("LED3");
	gtk_fixed_put(GTK_FIXED(ui->background_fixed), ui->toggle_button_led1, 100, 350);
	gtk_fixed_put(GTK_FIXED(ui->background_fixed), ui->toggle_button_led2, 350, 350);
	gtk_fixed_put(GTK_FIXED(ui->background_fixed), ui->toggle_button_led3, 600, 350);
	gtk_widget_set_size_request(ui->toggle_button_led1, 100, 60);
	gtk_widget_set_size_request(ui->toggle_button_led2, 100, 60);
	gtk_widget_set_size_request(ui->toggle_button_led3, 100, 60);
	g_signal_connect(G_OBJECT(ui->toggle_button_led1), "clicked",	G_CALLBACK(button_callback), ui);
	g_signal_connect(G_OBJECT(ui->toggle_button_led2), "clicked",	G_CALLBACK(button_callback), ui);
	g_signal_connect(G_OBJECT(ui->toggle_button_led3), "clicked",	G_CALLBACK(button_callback), ui);
}

void gtk_ui(void)
{
	static gtk_ui_t ui;
	
	memset(&ui, 0, sizeof(ui));
	led_control(1, 0);
	led_control(2, 0);
	led_control(3, 0);
	background_init(&ui);
	button_init(&ui);
	
	gtk_widget_show_all(ui.background);
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	gtk_ui();
	gtk_main();
	return 0;
}