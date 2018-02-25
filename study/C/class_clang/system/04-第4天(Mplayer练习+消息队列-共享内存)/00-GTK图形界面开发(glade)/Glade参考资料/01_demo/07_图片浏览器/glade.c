#include <glade/glade.h>	// 使用glade 需要的头文件
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
//存放图片目录和文件名地址的指针数组
static char *gs_bmp_name[25] = { 	
								"image/1.bmp","image/2.bmp","image/3.bmp","image/4.bmp",
								"image/5.bmp","image/6.bmp","image/7.bmp","image/8.bmp",
								"image/9.bmp","image/10.bmp","image/11.bmp","image/12.bmp",
								"image/13.bmp","image/14.bmp","image/15.bmp","image/16.bmp",
								"image/17.bmp","image/18.bmp",	"image/19.bmp","image/20.bmp",
								"image/21.bmp","image/22.bmp","image/23.bmp","image/24.bmp"
								};
static int gs_bmp_total = 24;	//图片总数

/* 功能：  设置背景图
 * widget: 主窗口
 * w, h:   图片的大小
 * path：  图片路径
 */
static void chang_background(GtkWidget *widget, int w, int h, const gchar *path)
{
	gtk_widget_set_app_paintable(widget, TRUE);		//允许窗口可以绘图
    gtk_widget_realize(widget);	
	
	/* 更改背景图时，图片会重叠
	 * 这时要手动调用下面的函数，让窗口绘图区域失效，产生窗口重绘制事件（即 expose 事件）。
	 */
	gtk_widget_queue_draw(widget);
	
    GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file(path, NULL);	// 创建图片资源对象
    // w, h是指定图片的宽度和高度
    GdkPixbuf *dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h, GDK_INTERP_BILINEAR);

	GdkPixmap *pixmap = NULL;
	
	/* 创建pixmap图像; 
	 * NULL：不需要蒙版; 
	 * 123： 0~255，透明到不透明
	 */
    gdk_pixbuf_render_pixmap_and_mask(dst_pixbuf, &pixmap, NULL, 128);
    // 通过pixmap给widget设置一张背景图，最后一个参数必须为: FASLE
	gdk_window_set_back_pixmap(widget->window, pixmap, FALSE);
    
	// 释放资源
	g_object_unref(src_pixbuf);
    g_object_unref(dst_pixbuf);
    g_object_unref(pixmap);
}

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

/* 功能：	设置进度条的百分比
 * min：	最小值
 * max：	当前值
 * value：	当前要设置的值
 */
void set_progress_bar_value(GtkWidget *widget, const int min, const int max, const int value)
{
	if(max <= 0){
		return;
	}
	double temp = (min*value*1.0)/max;
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widget), temp);	// 设置当前值
}

// 定时器回调函数
gboolean on_time(gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	
	gs_index++;
	if(gs_index > gs_bmp_total - 1){
		gs_index = 0;
	}
	
	set_progress_bar_value(p_temp->progress_bar, 1, gs_bmp_total, gs_index+1); // 设置进度条
	load_image(p_temp->image, gs_bmp_name[gs_index], p_temp->image->allocation.width, p_temp->image->allocation.height);	// 重新加载图片
	
	return TRUE;
}

// 按钮的回调函数
void deal_switch_image(GtkWidget *button, gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	
	if(button == p_temp->button_previous){		// 上一张
		gs_index--;
		if(gs_index < 0){
			gs_index = gs_bmp_total - 1;
		}
	}else if(button == p_temp->button_next ){	// 下一张
		gs_index++;
		if(gs_index > gs_bmp_total - 1){
			gs_index = 0;
		}
	}else if(button == p_temp->button_auto){	// 自动
		static guint s_timer = -1;		// 定时器id
		GtkWidget *temp_image = (GtkWidget *)p_temp->image_auto;
		if(-1 == s_timer){//pause to play
			gtk_widget_set_sensitive(p_temp->button_next, FALSE);		// 下一张按钮变灰
			gtk_widget_set_sensitive(p_temp->button_previous, FALSE);	// 上一张按钮变灰
			load_image(temp_image, "./skin/pause.bmp", temp_image->allocation.width, temp_image->allocation.height);	// 自动图标换成暂停图标
			s_timer = g_timeout_add(1000, on_time, p_temp);	// 增加定时器
		}else{
			gtk_widget_set_sensitive(p_temp->button_next, TRUE);
			gtk_widget_set_sensitive(p_temp->button_previous, TRUE);
			load_image(temp_image, "./skin/play.bmp", temp_image->allocation.width, temp_image->allocation.height);	// 暂停图标换成自动图标
			g_source_remove(s_timer);	// 删除定时器
			s_timer = -1;
		}
	
	}
	
	set_progress_bar_value(p_temp->progress_bar, 1, gs_bmp_total, gs_index+1); // 设置进度条
	load_image(p_temp->image, gs_bmp_name[gs_index], p_temp->image->allocation.width, p_temp->image->allocation.height);	// 重新加载图片
}

void window_demo(gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	
	// 通过我们的保存好的glade创建一个glade xml
	GladeXML *gxml = glade_xml_new("demo.glade", NULL, NULL);
	
	// 获取控件
	p_temp->main_window = glade_xml_get_widget(gxml, "main_window");	// 主窗口
	p_temp->progress_bar =  glade_xml_get_widget(gxml, "progressbar");	// 进度条
	p_temp->image = glade_xml_get_widget(gxml, "image");	// 图片控件
	p_temp->button_previous = glade_xml_get_widget(gxml, "button_previous"); // 上一张按钮
	p_temp->button_next = glade_xml_get_widget(gxml, "button_next"); // 下一张按钮
	p_temp->button_auto = glade_xml_get_widget(gxml, "button_auto"); // 自动播放按钮
	p_temp->image_auto = glade_xml_get_widget(gxml, "image_auto");	// 自动播放按钮的图片控件
	
	//信号连接
	glade_xml_signal_autoconnect(gxml);
	g_signal_connect(p_temp->main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL); 
	g_signal_connect(p_temp->button_previous, "clicked", G_CALLBACK(deal_switch_image), p_temp);
	g_signal_connect(p_temp->button_next, "clicked", G_CALLBACK(deal_switch_image), p_temp);
	g_signal_connect(p_temp->button_auto, "clicked", G_CALLBACK(deal_switch_image), p_temp);
	
	gtk_widget_show_all(p_temp->main_window);	// 显示
	
	//  设置
	// 主窗口背景图
	chang_background(p_temp->main_window, p_temp->main_window->allocation.width, p_temp->main_window->allocation.height, "./skin/background.bmp");
	// 进度条初始值
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(p_temp->progress_bar), 0.0);
	// 设置图片控件的图片
	load_image(p_temp->image, gs_bmp_name[gs_index], p_temp->image->allocation.width, p_temp->image->allocation.height);
	// 上一张按钮的图片
	GtkWidget *temp_image = glade_xml_get_widget(gxml, "image_previous");
	load_image(temp_image, "./skin/previous.bmp", temp_image->allocation.width, temp_image->allocation.height);
	// 下一张按钮的图片
	temp_image = glade_xml_get_widget(gxml, "image_next");
	load_image(temp_image, "./skin/next.bmp", temp_image->allocation.width, temp_image->allocation.height);
	// 自动按钮的图片
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
