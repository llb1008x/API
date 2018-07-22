#include <glade/glade.h>	// ʹ��glade ��Ҫ��ͷ�ļ�
#include <gtk/gtk.h>

// ��ť���µĴ�����, gpointer �൱�� void *
void deal_pressed(GtkButton *button, gpointer user_data)
{
	// ��ð�ť���ı���Ϣ
	const gchar *text = gtk_button_get_label( button );
	printf("button_text = %s; user_data = %s\n", text, (gchar *)user_data);
}

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
	button = glade_xml_get_widget(gxml, "button1");
	
	// �ź�����
	glade_xml_signal_autoconnect(gxml);
	
	g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(button, "pressed", G_CALLBACK(deal_pressed), "is pressed");
	
	gtk_widget_show_all(window);  
	gtk_main();
	
	return 0;   
} 
