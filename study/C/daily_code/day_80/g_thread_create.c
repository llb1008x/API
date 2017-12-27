#include <gtk/gtk.h>

void thread_demo(gpointer data)
{
	//...
}

void gtk_thread_init(void)
{
	if(FALSE == g_thread_supported())
	{ 
		g_thread_init(NULL); 
	}
	gdk_threads_init();
}

int main(int argc, char *argv[])
{
	gtk_thread_init();
	gtk_init(&argc, &argv);
	
	/* 代码 */
	g_thread_create((GThreadFunc)thread_demo, (gpointer)&ui, FALSE, NULL);
	//...
	
	gtk_main();
	return 0;
}
