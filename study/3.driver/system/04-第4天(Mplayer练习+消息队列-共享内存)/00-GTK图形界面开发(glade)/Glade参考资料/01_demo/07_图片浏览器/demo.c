#include <gtk/gtk.h>
#define 	WIN_W	800
#define 	WIN_H	480
#define 	PIC_W	(960/2+15)
#define 	PIC_H	(720/2-15)

typedef struct _Window
{
	GtkWidget *main_window;
	GtkWidget *table;
	GtkWidget *progress_bar;
	GtkWidget *image;
	GtkWidget *button_previous;
	GtkWidget *button_next;
	GtkWidget *button_auto;
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

// 根据图片路径创建一个新按钮，同时指定图片大小
GtkWidget *create_button_from_file(const char *file_path, const int w, const int h)
{
	GtkWidget *temp_image = gtk_image_new_from_pixbuf(NULL);
	load_image(temp_image, file_path, w, h);
	
	GtkWidget *button = gtk_button_new(); 					// 先创建空按钮
	gtk_button_set_image(GTK_BUTTON(button), temp_image);	// 给按钮设置图标
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);	// 按钮背景色透明
	
	return button;
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
	load_image(p_temp->image, gs_bmp_name[gs_index], PIC_W, PIC_H);	// 重新加载图片
	
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
		GtkWidget *temp_image = gtk_button_get_image( GTK_BUTTON(button) ); // 获得按钮上面的图标
		if(-1 == s_timer){//pause to play
			gtk_widget_set_sensitive(p_temp->button_next, FALSE);		// 下一张按钮变灰
			gtk_widget_set_sensitive(p_temp->button_previous, FALSE);	// 上一张按钮变灰
			load_image(temp_image, "./skin/pause.bmp", 100, 62);	// 自动图标换成暂停图标
			s_timer = g_timeout_add(1000, on_time, p_temp);	// 增加定时器
		}else{
			gtk_widget_set_sensitive(p_temp->button_next, TRUE);
			gtk_widget_set_sensitive(p_temp->button_previous, TRUE);
			load_image(temp_image, "./skin/play.bmp", 100, 62);	// 暂停图标换成自动图标
			g_source_remove(s_timer);	// 删除定时器
			s_timer = -1;
		}
	
	}
	
	set_progress_bar_value(p_temp->progress_bar, 1, gs_bmp_total, gs_index+1); // 设置进度条
	load_image(p_temp->image, gs_bmp_name[gs_index], PIC_W, PIC_H);	// 重新加载图片
}

void window_demo(gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	
	// 主窗口
	p_temp->main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);		 	// 创建主窗口
	gtk_window_set_title(GTK_WINDOW(p_temp->main_window), "image");	 	// 设置窗口标题
	gtk_window_set_position(GTK_WINDOW(p_temp->main_window), GTK_WIN_POS_CENTER); // 设置窗口在显示器中的位置为居中
	gtk_widget_set_size_request(p_temp->main_window, WIN_W, WIN_H);	    // 设置窗口的最小大小
	gtk_window_set_resizable(GTK_WINDOW(p_temp->main_window), FALSE); 	// 固定窗口的大小
	g_signal_connect(p_temp->main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL); 
	//  设置主窗口背景图
	chang_background(p_temp->main_window, WIN_W, WIN_H, "./skin/background.bmp");
	
	// 表格布局
	p_temp->table = gtk_table_new(7, 7, TRUE);	 					      // 表格布局容器
	gtk_container_add(GTK_CONTAINER(p_temp->main_window), p_temp->table); // 容器加入窗口
	
	// 进度条
	p_temp->progress_bar = gtk_progress_bar_new();
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(p_temp->progress_bar), 0.0);
	GtkWidget *temp_table = gtk_table_new(3, 15, TRUE);
	gtk_table_attach_defaults(GTK_TABLE(temp_table), p_temp->progress_bar, 1, 14, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(p_temp->table), temp_table, 1, 6, 0, 1);
	
	// 图片控件
	p_temp->image = gtk_image_new_from_pixbuf(NULL);		// 创建图片控件
	load_image(p_temp->image, gs_bmp_name[gs_index], PIC_W, PIC_H);
	gtk_table_attach_defaults(GTK_TABLE(p_temp->table), p_temp->image, 1, 6, 1, 6);	// 把图片控件加入布局
	
	// 按钮
	// 上一张
	p_temp->button_previous = create_button_from_file("./skin/previous.bmp", 100, 62);
	gtk_table_attach_defaults(GTK_TABLE(p_temp->table), p_temp->button_previous, 1, 2, 6, 7);
	g_signal_connect(p_temp->button_previous, "clicked", G_CALLBACK(deal_switch_image), p_temp);
	
	// 下一张
	p_temp->button_next = create_button_from_file("./skin/next.bmp", 100, 62);
	gtk_table_attach_defaults(GTK_TABLE(p_temp->table), p_temp->button_next, 5, 6, 6, 7);
	g_signal_connect(p_temp->button_next, "clicked", G_CALLBACK(deal_switch_image), p_temp);
	
	// 自动
	p_temp->button_auto = create_button_from_file("./skin/play.bmp", 100, 62);
	gtk_table_attach_defaults(GTK_TABLE(p_temp->table), p_temp->button_auto, 3, 4, 6, 7);
	g_signal_connect(p_temp->button_auto, "clicked", G_CALLBACK(deal_switch_image), p_temp);
	
	gtk_widget_show_all(p_temp->main_window);
	
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);		// 初始化
	
	WINDOW window;
	window_demo(&window);

	gtk_main();			// 主事件循环

	return 0;
}