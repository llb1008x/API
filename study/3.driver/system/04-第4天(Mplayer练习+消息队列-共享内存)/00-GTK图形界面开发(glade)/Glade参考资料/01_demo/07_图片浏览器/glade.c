#include <glade/glade.h>	// ʹ��glade ��Ҫ��ͷ�ļ�
#include <gtk/gtk.h>


typedef struct _Window
{
	GtkWidget *main_window;
	GtkWidget *table;
	GtkWidget *progress_bar;
	GtkWidget *image;
	GtkWidget *button_previous;
	GtkWidget *button_next;
	GtkWidget *button_auto;
	GtkWidget *image_auto;
}WINDOW;

static int gs_index = 0;
//���ͼƬĿ¼���ļ�����ַ��ָ������
static char *gs_bmp_name[25] = { 	
								"image/1.bmp","image/2.bmp","image/3.bmp","image/4.bmp",
								"image/5.bmp","image/6.bmp","image/7.bmp","image/8.bmp",
								"image/9.bmp","image/10.bmp","image/11.bmp","image/12.bmp",
								"image/13.bmp","image/14.bmp","image/15.bmp","image/16.bmp",
								"image/17.bmp","image/18.bmp",	"image/19.bmp","image/20.bmp",
								"image/21.bmp","image/22.bmp","image/23.bmp","image/24.bmp"
								};
static int gs_bmp_total = 24;	//ͼƬ����

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

/* ���ܣ�	���ý������İٷֱ�
 * min��	��Сֵ
 * max��	��ǰֵ
 * value��	��ǰҪ���õ�ֵ
 */
void set_progress_bar_value(GtkWidget *widget, const int min, const int max, const int value)
{
	if(max <= 0){
		return;
	}
	double temp = (min*value*1.0)/max;
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widget), temp);	// ���õ�ǰֵ
}

// ��ʱ���ص�����
gboolean on_time(gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	
	gs_index++;
	if(gs_index > gs_bmp_total - 1){
		gs_index = 0;
	}
	
	set_progress_bar_value(p_temp->progress_bar, 1, gs_bmp_total, gs_index+1); // ���ý�����
	load_image(p_temp->image, gs_bmp_name[gs_index], p_temp->image->allocation.width, p_temp->image->allocation.height);	// ���¼���ͼƬ
	
	return TRUE;
}

// ��ť�Ļص�����
void deal_switch_image(GtkWidget *button, gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	
	if(button == p_temp->button_previous){		// ��һ��
		gs_index--;
		if(gs_index < 0){
			gs_index = gs_bmp_total - 1;
		}
	}else if(button == p_temp->button_next ){	// ��һ��
		gs_index++;
		if(gs_index > gs_bmp_total - 1){
			gs_index = 0;
		}
	}else if(button == p_temp->button_auto){	// �Զ�
		static guint s_timer = -1;		// ��ʱ��id
		GtkWidget *temp_image = (GtkWidget *)p_temp->image_auto;
		if(-1 == s_timer){//pause to play
			gtk_widget_set_sensitive(p_temp->button_next, FALSE);		// ��һ�Ű�ť���
			gtk_widget_set_sensitive(p_temp->button_previous, FALSE);	// ��һ�Ű�ť���
			load_image(temp_image, "./skin/pause.bmp", temp_image->allocation.width, temp_image->allocation.height);	// �Զ�ͼ�껻����ͣͼ��
			s_timer = g_timeout_add(1000, on_time, p_temp);	// ���Ӷ�ʱ��
		}else{
			gtk_widget_set_sensitive(p_temp->button_next, TRUE);
			gtk_widget_set_sensitive(p_temp->button_previous, TRUE);
			load_image(temp_image, "./skin/play.bmp", temp_image->allocation.width, temp_image->allocation.height);	// ��ͣͼ�껻���Զ�ͼ��
			g_source_remove(s_timer);	// ɾ����ʱ��
			s_timer = -1;
		}
	
	}
	
	set_progress_bar_value(p_temp->progress_bar, 1, gs_bmp_total, gs_index+1); // ���ý�����
	load_image(p_temp->image, gs_bmp_name[gs_index], p_temp->image->allocation.width, p_temp->image->allocation.height);	// ���¼���ͼƬ
}

void window_demo(gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	
	// ͨ�����ǵı���õ�glade����һ��glade xml
	GladeXML *gxml = glade_xml_new("demo.glade", NULL, NULL);
	
	// ��ȡ�ؼ�
	p_temp->main_window = glade_xml_get_widget(gxml, "main_window");	// ������
	p_temp->progress_bar =  glade_xml_get_widget(gxml, "progressbar");	// ������
	p_temp->image = glade_xml_get_widget(gxml, "image");	// ͼƬ�ؼ�
	p_temp->button_previous = glade_xml_get_widget(gxml, "button_previous"); // ��һ�Ű�ť
	p_temp->button_next = glade_xml_get_widget(gxml, "button_next"); // ��һ�Ű�ť
	p_temp->button_auto = glade_xml_get_widget(gxml, "button_auto"); // �Զ����Ű�ť
	p_temp->image_auto = glade_xml_get_widget(gxml, "image_auto");	// �Զ����Ű�ť��ͼƬ�ؼ�
	
	//�ź�����
	glade_xml_signal_autoconnect(gxml);
	g_signal_connect(p_temp->main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL); 
	g_signal_connect(p_temp->button_previous, "clicked", G_CALLBACK(deal_switch_image), p_temp);
	g_signal_connect(p_temp->button_next, "clicked", G_CALLBACK(deal_switch_image), p_temp);
	g_signal_connect(p_temp->button_auto, "clicked", G_CALLBACK(deal_switch_image), p_temp);
	
	gtk_widget_show_all(p_temp->main_window);	// ��ʾ
	
	//  ����
	// �����ڱ���ͼ
	chang_background(p_temp->main_window, p_temp->main_window->allocation.width, p_temp->main_window->allocation.height, "./skin/background.bmp");
	// ��������ʼֵ
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(p_temp->progress_bar), 0.0);
	// ����ͼƬ�ؼ���ͼƬ
	load_image(p_temp->image, gs_bmp_name[gs_index], p_temp->image->allocation.width, p_temp->image->allocation.height);
	// ��һ�Ű�ť��ͼƬ
	GtkWidget *temp_image = glade_xml_get_widget(gxml, "image_previous");
	load_image(temp_image, "./skin/previous.bmp", temp_image->allocation.width, temp_image->allocation.height);
	// ��һ�Ű�ť��ͼƬ
	temp_image = glade_xml_get_widget(gxml, "image_next");
	load_image(temp_image, "./skin/next.bmp", temp_image->allocation.width, temp_image->allocation.height);
	// �Զ���ť��ͼƬ
	temp_image = glade_xml_get_widget(gxml, "image_auto");
	load_image(temp_image, "./skin/play.bmp", temp_image->allocation.width, temp_image->allocation.height);
	
}

int main(int argc,char **argv)
{
	
	gtk_init(&argc,&argv);
	
	WINDOW window;
	window_demo(&window);
	
	gtk_main();
	
	return 0;   
} 
