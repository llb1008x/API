#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// 线程处理函数
gpointer deal_thread(gpointer data)
{
	int i = 0;
	while(1){
		
		char buf[20] = "";
		sprintf(buf, "%d", i++);
		printf("i=====%d\n", i);
		
		usleep(500*1000);
		
		//使用gdk_thread_*()将要刷新的界面
		gdk_threads_enter();	// 进入多线程互斥区域
		gtk_label_set_text( GTK_LABEL(data), buf );
		gdk_threads_leave();	// 退出多线程互斥区域	
	}
}

//线程处理函数2
gpointer deal_thread1(gpointer data)
{
	int k = 10;
	while(1)
	{
		char buf[20] = "";
		sprintf(buf, "%d", k++);
		printf("k=====%d\n", k);
		
		sleep(1);
		
		gdk_threads_enter();	// 进入多线程互斥区域
		gtk_label_set_text( GTK_LABEL(data), buf );
		gdk_threads_leave();	// 退出多线程互斥区域
	}
}

void gtk_thread_init(void)
{
	if(FALSE == g_thread_supported())
	{ 
		g_thread_init(NULL); 
	}
	gdk_threads_init();
}

int main( int argc, char *argv[] )
{
	gtk_thread_init();//
	
	gtk_init(&argc, &argv);
	// 创建新窗口 
	GtkWidget *window = NULL;
	GtkWidget *vbox = NULL;
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"thread number");
	gtk_widget_set_size_request(window, 200, 60);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	vbox = gtk_vbox_new(TRUE, 5);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	// 标签
	GtkWidget *label = gtk_label_new("");
	GtkWidget *label1 = gtk_label_new("");
	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), label1, TRUE, TRUE, 5);
	
	
	/* 创建线程
	GThread *g_thread_create(GThreadFunc func,
							 gpointer data,
							 gboolean joinable,
							 GError **error);
	这是创建线程函数，func是线程执行的外部函数，data是传给该外部函数的参数，joinable标志线程是否可分离，error是出错代码返回地址。
	*/
	g_thread_create( (GThreadFunc)deal_thread,  label, FALSE, NULL );
	g_thread_create( (GThreadFunc)deal_thread1,  label1, FALSE, NULL );
	
	//pthread_t tid, tid1;
	//pthread_create(&tid, NULL, deal_thread, label);
	//pthread_create(&tid1, NULL, deal_thread1, label1);
	
	
	gtk_widget_show_all(window);//显示所有控件
	
	gtk_main();
	return 0;
}