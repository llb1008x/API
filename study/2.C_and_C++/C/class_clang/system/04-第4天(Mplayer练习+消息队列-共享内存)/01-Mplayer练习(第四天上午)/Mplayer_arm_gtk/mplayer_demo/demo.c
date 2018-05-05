#include <glade/glade.h>  //使用glade需包含该文件
#include <gtk/gtk.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

char *cmd[]={"seek -5\n","pause\n","seek 5\n"};
typedef struct mp
{
	int fd;
	GladeXML *gxml;
	GtkWidget *window;
	GtkWidget *fixed;
	GtkWidget *button_backward;
	GtkWidget *button_forward;
	GtkWidget *button_pause;
}MP;

int main(int argc,char *argv[])  
{
	gtk_init(&argc,&argv); 

	MP mplayer;
	memset(&mplayer, 0, sizeof(MP));
	
	mplayer_win_init(&mplayer);//界面初始化
	
	mplayer_process(&mplayer);//启动mplayer
	
	gtk_widget_show_all(mplayer.window);
	gtk_main();
	return 0;
}