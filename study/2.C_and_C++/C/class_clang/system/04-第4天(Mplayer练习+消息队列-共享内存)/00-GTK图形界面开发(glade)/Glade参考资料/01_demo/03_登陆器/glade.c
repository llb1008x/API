#include <glade/glade.h>	// 使用glade 需要的头文件
#include <gtk/gtk.h>
#include <string.h>
#define 	PWD			"123456"

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

 /* 功能:      设置控件字体大小
  * widget:    需要改变字体的控件
  * size:      字体大小
  * is_button: TRUE代表控件为按钮，FALSE为其它控件
  */
static void set_widget_font_size(GtkWidget *widget, int size, gboolean is_button)
{
	GtkWidget *labelChild;  
	PangoFontDescription *font;  
	gint fontSize = size;  
  
	font = pango_font_description_from_string("Sans");          //"Sans"字体名   
	pango_font_description_set_size(font, fontSize*PANGO_SCALE);//设置字体大小   
	
	if(is_button){
		labelChild = gtk_bin_get_child(GTK_BIN(widget));//取出GtkButton里的label  
	}else{
		labelChild = widget;
	}
	
	//设置label的字体，这样这个GtkButton上面显示的字体就变了
	gtk_widget_modify_font(GTK_WIDGET(labelChild), font);
	pango_font_description_free(font);
}

// 数字按钮的处理函数
static void deal_number(GtkWidget *widget, gpointer data)
{
	char result_buf[25] = {0};
	
	const char *buf = gtk_entry_get_text( GTK_ENTRY(data) );	// 获得行编辑内容
	if(NULL != buf){
		strcpy(result_buf, buf);  // 拷贝
	}
	
	// 获得按钮上的内容，追加
	const char *temp_buf = gtk_button_get_label( GTK_BUTTON(widget) );
	strcat(result_buf, temp_buf);
	
	if(18 < strlen(result_buf) ){	// 超过18个字符，清空
		memset( result_buf, '\0', sizeof(result_buf) );	// 清空
	}
	
	gtk_entry_set_text( GTK_ENTRY(data), result_buf );  // 重新设置行编辑的内容

}

// 退格按钮的处理函数
static void back_space(GtkWidget *widget, gpointer data)
{
	char result_buf[25] = {0};
	const char *temp_buf = gtk_entry_get_text( GTK_ENTRY(data) );	// 获得行编辑内容
	if(NULL != temp_buf){
		strcpy(result_buf, temp_buf);  // 拷贝
	}else{
		return;
	}
	
	result_buf[ strlen(result_buf) - 1 ] = '\0';
	
	gtk_entry_set_text( GTK_ENTRY(data), result_buf );  // 重新设置行编辑的内容
}

// 确定按钮的处理函数
static void deal_sure(GtkWidget *widget, gpointer data)
{	char result_buf[25] = {0};
	const char *temp_buf = gtk_entry_get_text( GTK_ENTRY(data) );	// 获得行编辑内容
	if(NULL != temp_buf){
		strcpy(result_buf, temp_buf);
	}else{	// 没有内容，中断函数
		return;
	}
	
	if( 0 == strcmp(PWD, result_buf) ){			// 密码正确时
		printf("right password\n");
	}else{
		printf("wrong password\n");
	}
	
	memset( result_buf, '\0', sizeof(result_buf) );		// 清空
	gtk_entry_set_text( GTK_ENTRY(data), result_buf );  // 获得行编辑内容
}

int main(int argc,char **argv)
{
	GladeXML *gxml;
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *entry;
	GtkWidget *label;
	
	gtk_init(&argc,&argv);
	
	// 通过我们的保存好的glade创建一个glade xml
	gxml = glade_xml_new("demo.glade", NULL, NULL);

	// 获取控件
	window = glade_xml_get_widget(gxml, "main_window"); /* main_window是glade3 中窗口的名字*/
	entry = glade_xml_get_widget(gxml, "entry");
	set_widget_font_size(entry, 18, FALSE);
	label = glade_xml_get_widget(gxml, "label");
	set_widget_font_size(label, 18, FALSE);
	
	//信号连接
	glade_xml_signal_autoconnect(gxml);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);	// 确保能关闭窗口
	g_signal_connect(entry, "activate", G_CALLBACK(deal_sure), entry);
	
	// 操作1到9的按钮
	char buf[10] = {0};
	int i = 0;
	for(i = 0; i < 10; i++){
		sprintf(buf, "button%d", i);
		button = glade_xml_get_widget(gxml, buf); 
		set_widget_font_size(button, 30, TRUE);
		g_signal_connect(button, "clicked", G_CALLBACK(deal_number), entry);
	}
	
	// 退格按钮
	button = glade_xml_get_widget(gxml, "button_del"); 
	set_widget_font_size(button, 20, TRUE);
	g_signal_connect(button, "clicked", G_CALLBACK(back_space), entry);
	
	// 确定按钮
	button = glade_xml_get_widget(gxml, "button_sure"); 
	set_widget_font_size(button, 20, TRUE);
	g_signal_connect(button, "clicked", G_CALLBACK(deal_sure), entry);
	
	gtk_widget_show_all(window);
	
	chang_background(window, window->allocation.width, window->allocation.height, "./skin/bg.png");
	
	gtk_main();
	
	return 0;   
} 
