#include <glade/glade.h>	// 使用glade 需要的头文件
#include <gtk/gtk.h>

// 按钮按下的处理函数, gpointer 相当于 void *
void deal_pressed(GtkButton *button, gpointer user_data)
{
	// 获得按钮的文本信息
	const gchar *text = gtk_button_get_label( button );
	printf("button_text = %s; user_data = %s\n", text, (gchar *)user_data);
}

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
	button = glade_xml_get_widget(gxml, "button1");
	
	// 信号连接
	glade_xml_signal_autoconnect(gxml);
	
	g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(button, "pressed", G_CALLBACK(deal_pressed), "is pressed");
	
	gtk_widget_show_all(window);  
	gtk_main();
	
	return 0;   
} 
