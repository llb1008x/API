#include <glade/glade.h>	// ʹ��glade ��Ҫ��ͷ�ļ�
#include <gtk/gtk.h>

typedef struct _Window
{
	GtkWidget *main_window;
	GtkWidget *table;
	GtkWidget *hbutton_add;
	GtkWidget *hbutton_sub;
	GtkWidget *mbutton_add;
	GtkWidget *mbutton_sub;
	GtkWidget *sbutton_add;
	GtkWidget *sbutton_sub;
	GtkWidget *button_set;
	GtkWidget *button_ok;
	GtkWidget *button_cancel;
	
}WINDOW;

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

/* ���ܣ����ð�ť״̬����ʾ�����ء�ʹ�ܷ�ʹ��(�Ƿ���)
 * flag: 1���������ð�ť
 *		 2������ȷ����ȡ����ť
 *		 3���������ӡ����ٰ�ť
 */
void set_button_state(int flag, gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	
	if(1 == flag){	// �������ð�ť
		gtk_widget_show(p_temp->hbutton_add);
		gtk_widget_show(p_temp->hbutton_sub);
		gtk_widget_show(p_temp->mbutton_add);
		gtk_widget_show(p_temp->mbutton_sub);
		gtk_widget_show(p_temp->sbutton_add);
		gtk_widget_show(p_temp->sbutton_sub);
		gtk_widget_show(p_temp->sbutton_sub);
		gtk_widget_show(p_temp->button_ok);
		gtk_widget_show(p_temp->button_cancel);
		gtk_widget_set_sensitive(p_temp->button_set, FALSE);
		gtk_widget_set_sensitive(p_temp->button_ok, FALSE);
		//gtk_widget_set_sensitive(p_temp->button_cancel, FALSE);
		//gtk_widget_hide(p_temp->button_set);
	}else if(2 == flag){// ����ȷ����ȡ����ť
		gtk_widget_hide(p_temp->hbutton_add);
		gtk_widget_hide(p_temp->hbutton_sub);
		gtk_widget_hide(p_temp->mbutton_add);
		gtk_widget_hide(p_temp->mbutton_sub);
		gtk_widget_hide(p_temp->sbutton_add);
		gtk_widget_hide(p_temp->sbutton_sub);
		gtk_widget_hide(p_temp->sbutton_sub);
		gtk_widget_hide(p_temp->button_ok);
		gtk_widget_hide(p_temp->button_cancel);
		gtk_widget_set_sensitive(p_temp->button_set, TRUE);
		//gtk_widget_show(p_temp->button_set);
	}else if(3 == flag){// �������ӡ����ٰ�ť
		gtk_widget_set_sensitive(p_temp->button_ok, TRUE);
		//gtk_widget_set_sensitive(p_temp->button_cancel, TRUE);
	}
}

/* ���ܣ�	���ð�ť�Ļص�����
 * widget�� �źŷ�����
 * data��	WINDOW�����ĵ�ַ
 */
void on_button_set(GtkWidget *widget, gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	set_button_state(1, p_temp);
}

/* ���ܣ�	���ӡ����ٰ�ť�Ļص�����
 * widget�� �źŷ�����
 * data��	WINDOW�����ĵ�ַ
 */
void deal_control(GtkWidget *widget, gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	set_button_state(3, p_temp);	// ���ð�ť״̬
}

/* ���ܣ�	ȷ����ȡ����ť�ص�����
 * widget�� �źŷ�����
 * data��	WINDOW�����ĵ�ַ
 */
void deal_result(GtkWidget *widget, gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	set_button_state(2, p_temp);	// ���ð�ť״̬
}


/* ���ܣ����ڿؼ��Ĵ��������á����֡��ź�����
 * data��WINDOW�����ĵ�ַ
 */
void window_demo(gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	
	// ͨ�����ǵı���õ�glade����һ��glade xml
	GladeXML *gxml = glade_xml_new("demo.glade", NULL, NULL);
	
	// ��ȡ�ؼ�
	p_temp->main_window = glade_xml_get_widget(gxml, "main_window");	
	p_temp->hbutton_add =  glade_xml_get_widget(gxml, "hbutton_add");	
	p_temp->hbutton_sub = glade_xml_get_widget(gxml, "hbutton_sub");	
	p_temp->mbutton_add = glade_xml_get_widget(gxml, "mbutton_add"); 
	p_temp->mbutton_sub = glade_xml_get_widget(gxml, "mbutton_sub"); 
	p_temp->sbutton_add = glade_xml_get_widget(gxml, "sbutton_add"); 
	p_temp->sbutton_sub = glade_xml_get_widget(gxml, "sbutton_sub");
	p_temp->button_set = glade_xml_get_widget(gxml, "button_set"); 
	p_temp->button_ok = glade_xml_get_widget(gxml, "button_ok"); 
	p_temp->button_cancel = glade_xml_get_widget(gxml, "button_cancel");
	
	/////////////////////////�ź�����
	glade_xml_signal_autoconnect(gxml);
	
	g_signal_connect(p_temp->main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL); 			// �ر�
	
	// ʱ
	g_signal_connect(p_temp->hbutton_add, "clicked", G_CALLBACK(deal_control), p_temp);
	g_signal_connect(p_temp->hbutton_sub, "clicked", G_CALLBACK(deal_control), p_temp);
	// ��
	g_signal_connect(p_temp->mbutton_add, "clicked", G_CALLBACK(deal_control), p_temp);
	g_signal_connect(p_temp->mbutton_sub, "clicked", G_CALLBACK(deal_control), p_temp);
	// ��
	g_signal_connect(p_temp->sbutton_add, "clicked", G_CALLBACK(deal_control), p_temp);
	g_signal_connect(p_temp->sbutton_sub, "clicked", G_CALLBACK(deal_control), p_temp);
	
	g_signal_connect(p_temp->button_ok, "clicked", G_CALLBACK(deal_result), p_temp);
	g_signal_connect(p_temp->button_cancel, "clicked", G_CALLBACK(deal_result), p_temp);
	g_signal_connect(p_temp->button_set, "clicked", G_CALLBACK(on_button_set), p_temp);
	
	
	////////////////////////�ؼ�����ʾ����
	gtk_widget_show_all(p_temp->main_window);
	//set_button_state(1, p_temp);
	set_button_state(2, p_temp);
	
	// ����ͼ��
	GtkWidget *temp_image = glade_xml_get_widget(gxml, "hbutton_add_image");
	load_image(temp_image, "./image/button_add.png", temp_image->allocation.width, temp_image->allocation.height);
	temp_image = glade_xml_get_widget(gxml, "hbutton_sub_image");
	load_image(temp_image, "./image/button_sub.png", temp_image->allocation.width, temp_image->allocation.height);
	temp_image = glade_xml_get_widget(gxml, "mbutton_add_image");
	load_image(temp_image, "./image/button_add.png", temp_image->allocation.width, temp_image->allocation.height);
	temp_image = glade_xml_get_widget(gxml, "mbutton_sub_image");
	load_image(temp_image, "./image/button_sub.png", temp_image->allocation.width, temp_image->allocation.height);
	temp_image = glade_xml_get_widget(gxml, "sbutton_add_image");
	load_image(temp_image, "./image/button_add.png", temp_image->allocation.width, temp_image->allocation.height);
	temp_image = glade_xml_get_widget(gxml, "sbutton_sub_image");
	load_image(temp_image, "./image/button_sub.png", temp_image->allocation.width, temp_image->allocation.height);
	
	temp_image = glade_xml_get_widget(gxml, "button_set_image");
	load_image(temp_image, "./image/button_set.png", temp_image->allocation.width, temp_image->allocation.height);
	temp_image = glade_xml_get_widget(gxml, "button_ok_image");
	load_image(temp_image, "./image/button_ok.png", temp_image->allocation.width, temp_image->allocation.height);
	temp_image = glade_xml_get_widget(gxml, "button_cancel_image");
	load_image(temp_image, "./image/button_cancel.png", temp_image->allocation.width, temp_image->allocation.height);
}

int main(int argc,char **argv)
{
	gtk_init(&argc,&argv);
	
	WINDOW window;

	window_demo(&window);	// ���ڵ���ز���
	
	gtk_main();
	
	return 0;   
} 

