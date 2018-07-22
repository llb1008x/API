#include <glade/glade.h>	// ʹ��glade ��Ҫ��ͷ�ļ�
#include <gtk/gtk.h>

// �������õ�image�������һ��ͼƬ
void load_image(GtkWidget *image, const char *file_path, const int w, const int h )
{
	gtk_image_clear( GTK_IMAGE(image) );        // ���ͼ��
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file(file_path, NULL);	// ����ͼƬ��Դ
	GdkPixbuf *dest_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h, GDK_INTERP_BILINEAR); // ָ����С
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), dest_pixbuf); // ͼƬ�ؼ���������һ��ͼƬ(pixbuf)
	g_object_unref(src_pixbuf);		// �ͷ���Դ
	g_object_unref(dest_pixbuf);	// �ͷ���Դ
}

int main(int argc,char **argv)
{
	GladeXML *gxml;
	GtkWidget *window;
	GtkWidget *image;
	
	gtk_init(&argc,&argv);
	
	// ͨ�����ǵı���õ�glade����һ��glade xml
	gxml = glade_xml_new("demo.glade", NULL, NULL);

	// ��ȡ�ؼ�
	window = glade_xml_get_widget(gxml, "main_window");
	image = glade_xml_get_widget(gxml, "image3");
	
	gtk_widget_show_all(window);  
	
	load_image(image, "./1.png", image->allocation.width, image->allocation.height);
	
	gtk_main();
	
	return 0;   
} 
