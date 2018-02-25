#include <glade/glade.h>	// 使用glade 需要的头文件
#include <gtk/gtk.h>

int main(int argc,char **argv)
{
	GladeXML *gxml;
	GtkWidget *window;
	GtkWidget *button;
	
	gtk_init(&argc,&argv);
	
	// 通过我们的保存好的glade创建一个glade xml
	gxml = glade_xml_new("demo.glade", NULL, NULL);

	// 获取控件
	window = glade_xml_get_widget(gxml, "window1"); /* window1 是glade3 中窗口的名字*/
	
	gtk_widget_show_all(window);  
	gtk_main();
	
	return 0;   
} 
