
void thread_init(void)
{
	if( FALSE == g_thread_supported() )	/*�����Ƿ�֧�ֶ��߳�*/
	{  
		g_thread_init(NULL); 	//ʼ�����߳�֧�֣�g_thread_init()�������gdk_threads_init()ǰ��
	}
	gdk_threads_init();	/*��ʼ��GDK���߳�֧��*/
}

int main( int argc, char *argv[] )
{
	gtk_init(&argc, &argv);
	
	thread_init();//�̳߳�ʼ��
	
	gtk_widget_show_all(window);//��ʾ���пؼ�
	
	gdk_threads_enter();	// ������̻߳�������
	gtk_main();
	gdk_threads_leave();    // �˳����̻߳�������
	
	return 0;
}