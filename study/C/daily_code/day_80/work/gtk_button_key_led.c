#include <gtk/gtk.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "gpio_led.h"

typedef struct{
	GtkWidget *background;
	GtkWidget *background_fixed;
	GtkWidget *background_image;
	GtkWidget *toggle_button_led1;
	GtkWidget *toggle_button_led2;
	GtkWidget *toggle_button_led3;
	GtkWidget *image_led1;
	GtkWidget *image_led2;
	GtkWidget *image_led3;
}gtk_ui_t;

GdkPixbuf *pixbuf_new_from_file(const char *image_path, int w, int h)
{
	GdkPixbuf *src_pixbuf = NULL;
	GdkPixbuf *dst_pixbuf = NULL;

	src_pixbuf = gdk_pixbuf_new_from_file(image_path, NULL);
	dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h, GDK_INTERP_BILINEAR);

	g_object_unref (src_pixbuf);
	
	if(NULL == dst_pixbuf){
		return NULL;
	}else{
		return dst_pixbuf;
	}
}

GtkWidget *image_new_from_file(const char *image_path, int w, int h)
{
	GtkWidget *image = NULL;
	GdkPixbuf *pixbuf = NULL;

	pixbuf = pixbuf_new_from_file(image_path, w, h);
	image = gtk_image_new_from_pixbuf(pixbuf);
	g_object_unref(pixbuf);

	if(NULL == image){
		return NULL;
	}else{
		return image;
	}
}

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

void image_init(gtk_ui_t *ui)
{
	ui->image_led1 = image_new_from_file("../image/light_1_off.png", 200, 250);
	ui->image_led2 = image_new_from_file("../image/light_2_off.png", 200, 250);
	ui->image_led3 = image_new_from_file("../image/light_3_off.png", 200, 250);
	
	gtk_fixed_put(GTK_FIXED(ui->background_fixed), ui->image_led1, 100, 80);
	gtk_fixed_put(GTK_FIXED(ui->background_fixed), ui->image_led2, 300, 80);
	gtk_fixed_put(GTK_FIXED(ui->background_fixed), ui->image_led3, 500, 80);
}

void key_process(gpointer data)
{

}

void gtk_ui(void)
{
	static gtk_ui_t ui;
	
	led_control(1, 0);
	led_control(2, 0);
	led_control(3, 0);
	memset(&ui, 0, sizeof(ui));
	background_init(&ui);
	button_init(&ui);
	image_init(&ui);
	g_thread_create((GThreadFunc)key_process, (gpointer)&ui, FALSE, NULL);
	gtk_widget_show_all(ui.background);
}

void gtk_thread_init(void)
{
	if(FALSE == g_thread_supported())
	{ 
		g_thread_init(NULL); 
	}
	gdk_threads_init();
}

int main(int argc, char *argv[])
{
	gtk_thread_init();
	gtk_init(&argc, &argv);
	gtk_ui();
	gtk_main();
	return 0;
}
