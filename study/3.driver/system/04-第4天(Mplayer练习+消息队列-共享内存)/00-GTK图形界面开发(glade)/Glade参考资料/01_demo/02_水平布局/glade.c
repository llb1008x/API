#include <glade/glade.h>	// ʹ��glade ��Ҫ��ͷ�ļ�
#include <gtk/gtk.h>

int main(int argc,char **argv)
{
	GladeXML *gxml;
	GtkWidget *window;
	GtkWidget *button;
	
	gtk_init(&argc,&argv);
	
	// ͨ�����ǵı���õ�glade����һ��glade xml
	gxml = glade_xml_new("demo.glade", NULL, NULL);

	// ��ȡ�ؼ�
	window = glade_xml_get_widget(gxml, "window1"); /* window1 ��glade3 �д��ڵ�����*/
	
	gtk_widget_show_all(window);  
	gtk_main();
	
	return 0;   
} 
