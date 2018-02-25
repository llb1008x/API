#include <glade/glade.h>	// 使用glade 需要的头文件
#include <gtk/gtk.h>

// 给创建好的image重新设计一张图片
void load_image(GtkWidget *image, const char *file_path, const int w, const int h )
{
	gtk_image_clear( GTK_IMAGE(image) );        // 清除图像
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file(file_path, NULL);	// 创建图片资源
	GdkPixbuf *dest_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h, GDK_INTERP_BILINEAR); // 指定大小
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), dest_pixbuf); // 图片控件重新设置一张图片(pixbuf)
	g_object_unref(src_pixbuf);		// 释放资源
	g_object_unref(dest_pixbuf);	// 释放资源
}

int main(int argc,char **argv)
{
	GladeXML *gxml;
	GtkWidget *window;
	GtkWidget *image;
	
	gtk_init(&argc,&argv);
	
	// 通过我们的保存好的glade创建一个glade xml
	gxml = glade_xml_new("demo.glade", NULL, NULL);

	// 获取控件
	window = glade_xml_get_widget(gxml, "main_window");
	image = glade_xml_get_widget(gxml, "image3");
	
	gtk_widget_show_all(window);  
	
	load_image(image, "./1.png", image->allocation.width, image->allocation.height);
	
	gtk_main();
	
	return 0;   
} 
