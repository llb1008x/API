#include <glade/glade.h>  //ʹ��glade��������ļ�
#include <gtk/gtk.h>
#include "sungtk_interface.h"

void callback_button(GtkWidget *button, gpointer data)
{
	sungtk_button_set_image(button, "./picture/pause.png", 100, 80);//���°�ť������Ƭ
}

int main(int argc,char *argv[])  
{
	GladeXML *gxml; 
	GtkWidget *window;
	//GtkWidget *fixed;
	GtkWidget *button;
	GtkWidget *label;
	GtkWidget *image;
	
	gtk_init(&argc,&argv); 
	gxml = glade_xml_new("./glade_file/glade_demo.glade",NULL,NULL);	//��һ������glade�ļ�����������GladeXML

	window = glade_xml_get_widget(gxml,"window_back");//�ڶ�����ͨ���ýӿڣ���ȡ�����Ŀؼ�
	sungtk_background_set_picture(window, "./picture/background.jpg", 400, 300);//���ô��ڱ���ͼƬ
	
	
	//fixed = glade_xml_get_widget(gxml,"fixed_back");//��ȡ���̶����֣�ͨ��������Լ�����ӿؼ�
	
	button = glade_xml_get_widget(gxml,"button_pause");//��ȡ��ť�ؼ�
	gtk_button_set_label(GTK_BUTTON(button), "");
	sungtk_button_inset_image(button, "./picture/play.png", 100, 80);//��ť��һ�β���һ��ͼƬ
	g_signal_connect(button, "pressed", G_CALLBACK(callback_button), button);
	
	label = glade_xml_get_widget(gxml,"label_song_name");//��ȡlabel�ؼ�
	gtk_label_set_text(GTK_LABEL(label), "coffee.mp3");
	
	image = glade_xml_get_widget(gxml,"image_logo");//��ȡͼƬ�ռ�
	sungtk_image_load_picture(image, "./picture/volume.jpg", 100, 80);//����ͼƬ
	
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}