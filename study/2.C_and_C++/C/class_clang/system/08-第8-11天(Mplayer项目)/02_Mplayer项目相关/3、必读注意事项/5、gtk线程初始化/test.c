
void thread_init(void)
{
	if( FALSE == g_thread_supported() )	/*测试是否支持多线程*/
	{  
		g_thread_init(NULL); 	//始化多线程支持，g_thread_init()必须放在gdk_threads_init()前面
	}
	gdk_threads_init();	/*初始化GDK多线程支持*/
}

int main( int argc, char *argv[] )
{
	gtk_init(&argc, &argv);
	
	thread_init();//线程初始化
	
	gtk_widget_show_all(window);//显示所有控件
	
	gdk_threads_enter();	// 进入多线程互斥区域
	gtk_main();
	gdk_threads_leave();    // 退出多线程互斥区域
	
	return 0;
}