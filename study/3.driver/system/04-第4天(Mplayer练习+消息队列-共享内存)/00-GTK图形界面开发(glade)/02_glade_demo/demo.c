#include <glade/glade.h>  //使用glade需包含该文件
#include <gtk/gtk.h>
#include "sungtk_interface.h"

void callback_button(GtkWidget *button, gpointer data)
{
	sungtk_button_set_image(button, "./picture/pause.png", 100, 80);//重新按钮设置照片
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
	gxml = glade_xml_new("./glade_file/glade_demo.glade",NULL,NULL);	//第一步，打开glade文件，返回类型GladeXML

	window = glade_xml_get_widget(gxml,"window_back");//第二步，通过该接口，获取创建的控件
	sungtk_background_set_picture(window, "./picture/background.jpg", 400, 300);//设置窗口背景图片
	
	
	//fixed = glade_xml_get_widget(gxml,"fixed_back");//获取到固定布局，通过代码可以继续添加控件
	
	button = glade_xml_get_widget(gxml,"button_pause");//获取按钮控件
	gtk_button_set_label(GTK_BUTTON(button), "");
	sungtk_button_inset_image(button, "./picture/play.png", 100, 80);//按钮第一次插入一张图片
	g_signal_connect(button, "pressed", G_CALLBACK(callback_button), button);
	
	label = glade_xml_get_widget(gxml,"label_song_name");//获取label控件
	gtk_label_set_text(GTK_LABEL(label), "coffee.mp3");
	
	image = glade_xml_get_widget(gxml,"image_logo");//获取图片空间
	sungtk_image_load_picture(image, "./picture/volume.jpg", 100, 80);//更改图片
	
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}