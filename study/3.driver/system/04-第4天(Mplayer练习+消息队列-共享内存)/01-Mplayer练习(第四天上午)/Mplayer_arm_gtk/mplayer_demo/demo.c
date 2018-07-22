#include <glade/glade.h>  //ʹ��glade��������ļ�
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
	
	mplayer_win_init(&mplayer);//�����ʼ��
	
	mplayer_process(&mplayer);//����mplayer
	
	gtk_widget_show_all(mplayer.window);
	gtk_main();
	return 0;
}