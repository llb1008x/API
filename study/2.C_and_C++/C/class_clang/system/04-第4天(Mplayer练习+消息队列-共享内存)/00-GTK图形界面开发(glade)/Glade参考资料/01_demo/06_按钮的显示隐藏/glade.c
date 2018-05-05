#include <glade/glade.h>	// 使用glade 需要的头文件
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

/* 功能：设置按钮状态：显示或隐藏、使能非使能(是否变灰)
 * flag: 1，按下设置按钮
 *		 2，按下确定、取消按钮
 *		 3，按下增加、减少按钮
 */
void set_button_state(int flag, gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	
	if(1 == flag){	// 按下设置按钮
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
	}else if(2 == flag){// 按下确定、取消按钮
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
	}else if(3 == flag){// 按下增加、减少按钮
		gtk_widget_set_sensitive(p_temp->button_ok, TRUE);
		//gtk_widget_set_sensitive(p_temp->button_cancel, TRUE);
	}
}

/* 功能：	设置按钮的回调函数
 * widget： 信号发出者
 * data：	WINDOW变量的地址
 */
void on_button_set(GtkWidget *widget, gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	set_button_state(1, p_temp);
}

/* 功能：	增加、减少按钮的回调函数
 * widget： 信号发出者
 * data：	WINDOW变量的地址
 */
void deal_control(GtkWidget *widget, gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	set_button_state(3, p_temp);	// 设置按钮状态
}

/* 功能：	确定、取消按钮回调函数
 * widget： 信号发出者
 * data：	WINDOW变量的地址
 */
void deal_result(GtkWidget *widget, gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	set_button_state(2, p_temp);	// 设置按钮状态
}


/* 功能：窗口控件的创建、设置、布局、信号连接
 * data：WINDOW变量的地址
 */
void window_demo(gpointer data)
{
	WINDOW *p_temp = (WINDOW *)data;
	
	// 通过我们的保存好的glade创建一个glade xml
	GladeXML *gxml = glade_xml_new("demo.glade", NULL, NULL);
	
	// 获取控件
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
	
	/////////////////////////信号连接
	glade_xml_signal_autoconnect(gxml);
	
	g_signal_connect(p_temp->main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL); 			// 关闭
	
	// 时
	g_signal_connect(p_temp->hbutton_add, "clicked", G_CALLBACK(deal_control), p_temp);
	g_signal_connect(p_temp->hbutton_sub, "clicked", G_CALLBACK(deal_control), p_temp);
	// 分
	g_signal_connect(p_temp->mbutton_add, "clicked", G_CALLBACK(deal_control), p_temp);
	g_signal_connect(p_temp->mbutton_sub, "clicked", G_CALLBACK(deal_control), p_temp);
	// 秒
	g_signal_connect(p_temp->sbutton_add, "clicked", G_CALLBACK(deal_control), p_temp);
	g_signal_connect(p_temp->sbutton_sub, "clicked", G_CALLBACK(deal_control), p_temp);
	
	g_signal_connect(p_temp->button_ok, "clicked", G_CALLBACK(deal_result), p_temp);
	g_signal_connect(p_temp->button_cancel, "clicked", G_CALLBACK(deal_result), p_temp);
	g_signal_connect(p_temp->button_set, "clicked", G_CALLBACK(on_button_set), p_temp);
	
	
	////////////////////////控件的显示隐藏
	gtk_widget_show_all(p_temp->main_window);
	//set_button_state(1, p_temp);
	set_button_state(2, p_temp);
	
	// 设置图标
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

	window_demo(&window);	// 窗口的相关操作
	
	gtk_main();
	
	return 0;   
} 

