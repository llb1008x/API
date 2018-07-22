#include <glade/glade.h>	// ʹ��glade ��Ҫ��ͷ�ļ�
#include <gtk/gtk.h>
#include <string.h>
#define 	PWD			"123456"

/* ���ܣ�  ���ñ���ͼ
 * widget: ������
 * w, h:   ͼƬ�Ĵ�С
 * path��  ͼƬ·��
 */
static void chang_background(GtkWidget *widget, int w, int h, const gchar *path)
{
	gtk_widget_set_app_paintable(widget, TRUE);		//�����ڿ��Ի�ͼ
    gtk_widget_realize(widget);	
	
	/* ���ı���ͼʱ��ͼƬ���ص�
	 * ��ʱҪ�ֶ���������ĺ������ô��ڻ�ͼ����ʧЧ�����������ػ����¼����� expose �¼�����
	 */
	gtk_widget_queue_draw(widget);
	
    GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file(path, NULL);	// ����ͼƬ��Դ����
    // w, h��ָ��ͼƬ�Ŀ�Ⱥ͸߶�
    GdkPixbuf *dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h, GDK_INTERP_BILINEAR);

	GdkPixmap *pixmap = NULL;
	
	/* ����pixmapͼ��; 
	 * NULL������Ҫ�ɰ�; 
	 * 123�� 0~255��͸������͸��
	 */
    gdk_pixbuf_render_pixmap_and_mask(dst_pixbuf, &pixmap, NULL, 128);
    // ͨ��pixmap��widget����һ�ű���ͼ�����һ����������Ϊ: FASLE
	gdk_window_set_back_pixmap(widget->window, pixmap, FALSE);
    
	// �ͷ���Դ
	g_object_unref(src_pixbuf);
    g_object_unref(dst_pixbuf);
    g_object_unref(pixmap);
}

 /* ����:      ���ÿؼ������С
  * widget:    ��Ҫ�ı�����Ŀؼ�
  * size:      �����С
  * is_button: TRUE����ؼ�Ϊ��ť��FALSEΪ�����ؼ�
  */
static void set_widget_font_size(GtkWidget *widget, int size, gboolean is_button)
{
	GtkWidget *labelChild;  
	PangoFontDescription *font;  
	gint fontSize = size;  
  
	font = pango_font_description_from_string("Sans");          //"Sans"������   
	pango_font_description_set_size(font, fontSize*PANGO_SCALE);//���������С   
	
	if(is_button){
		labelChild = gtk_bin_get_child(GTK_BIN(widget));//ȡ��GtkButton���label  
	}else{
		labelChild = widget;
	}
	
	//����label�����壬�������GtkButton������ʾ������ͱ���
	gtk_widget_modify_font(GTK_WIDGET(labelChild), font);
	pango_font_description_free(font);
}

// ���ְ�ť�Ĵ�����
static void deal_number(GtkWidget *widget, gpointer data)
{
	char result_buf[25] = {0};
	
	const char *buf = gtk_entry_get_text( GTK_ENTRY(data) );	// ����б༭����
	if(NULL != buf){
		strcpy(result_buf, buf);  // ����
	}
	
	// ��ð�ť�ϵ����ݣ�׷��
	const char *temp_buf = gtk_button_get_label( GTK_BUTTON(widget) );
	strcat(result_buf, temp_buf);
	
	if(18 < strlen(result_buf) ){	// ����18���ַ������
		memset( result_buf, '\0', sizeof(result_buf) );	// ���
	}
	
	gtk_entry_set_text( GTK_ENTRY(data), result_buf );  // ���������б༭������

}

// �˸�ť�Ĵ�����
static void back_space(GtkWidget *widget, gpointer data)
{
	char result_buf[25] = {0};
	const char *temp_buf = gtk_entry_get_text( GTK_ENTRY(data) );	// ����б༭����
	if(NULL != temp_buf){
		strcpy(result_buf, temp_buf);  // ����
	}else{
		return;
	}
	
	result_buf[ strlen(result_buf) - 1 ] = '\0';
	
	gtk_entry_set_text( GTK_ENTRY(data), result_buf );  // ���������б༭������
}

// ȷ����ť�Ĵ�����
static void deal_sure(GtkWidget *widget, gpointer data)
{	char result_buf[25] = {0};
	const char *temp_buf = gtk_entry_get_text( GTK_ENTRY(data) );	// ����б༭����
	if(NULL != temp_buf){
		strcpy(result_buf, temp_buf);
	}else{	// û�����ݣ��жϺ���
		return;
	}
	
	if( 0 == strcmp(PWD, result_buf) ){			// ������ȷʱ
		printf("right password\n");
	}else{
		printf("wrong password\n");
	}
	
	memset( result_buf, '\0', sizeof(result_buf) );		// ���
	gtk_entry_set_text( GTK_ENTRY(data), result_buf );  // ����б༭����
}

int main(int argc,char **argv)
{
	GladeXML *gxml;
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *entry;
	GtkWidget *label;
	
	gtk_init(&argc,&argv);
	
	// ͨ�����ǵı���õ�glade����һ��glade xml
	gxml = glade_xml_new("demo.glade", NULL, NULL);

	// ��ȡ�ؼ�
	window = glade_xml_get_widget(gxml, "main_window"); /* main_window��glade3 �д��ڵ�����*/
	entry = glade_xml_get_widget(gxml, "entry");
	set_widget_font_size(entry, 18, FALSE);
	label = glade_xml_get_widget(gxml, "label");
	set_widget_font_size(label, 18, FALSE);
	
	//�ź�����
	glade_xml_signal_autoconnect(gxml);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);	// ȷ���ܹرմ���
	g_signal_connect(entry, "activate", G_CALLBACK(deal_sure), entry);
	
	// ����1��9�İ�ť
	char buf[10] = {0};
	int i = 0;
	for(i = 0; i < 10; i++){
		sprintf(buf, "button%d", i);
		button = glade_xml_get_widget(gxml, buf); 
		set_widget_font_size(button, 30, TRUE);
		g_signal_connect(button, "clicked", G_CALLBACK(deal_number), entry);
	}
	
	// �˸�ť
	button = glade_xml_get_widget(gxml, "button_del"); 
	set_widget_font_size(button, 20, TRUE);
	g_signal_connect(button, "clicked", G_CALLBACK(back_space), entry);
	
	// ȷ����ť
	button = glade_xml_get_widget(gxml, "button_sure"); 
	set_widget_font_size(button, 20, TRUE);
	g_signal_connect(button, "clicked", G_CALLBACK(deal_sure), entry);
	
	gtk_widget_show_all(window);
	
	chang_background(window, window->allocation.width, window->allocation.height, "./skin/bg.png");
	
	gtk_main();
	
	return 0;   
} 
