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
void con_backward(GtkButton *button, gpointer data)
{
  MP *mplayer=(MP *)data;
  
 write(mplayer->fd,cmd[0],strlen(cmd[0]));
 printf("bbbbbbbbbbbbbbbbbbbbbbbb\n");
  printf("%s\n",cmd[0]);
}
void con_pause(GtkButton *button,gpointer data)
{
  MP *mplayer=(MP *)data;
  printf("fd======%d\n",mplayer->fd);
  write(mplayer->fd,cmd[1],strlen(cmd[1]));
  printf("%s\n",cmd[1]);
}
void con_forward(GtkButton *button,gpointer data)
{
  MP *mplayer=(MP *)data;
  write(mplayer->fd,cmd[2],strlen(cmd[2]));
  printf("ffffffffffffffffffff\n");
   printf("%s\n",cmd[2]);
}
void mplayer_win_init(gpointer data)
{
    MP *mplayer=(MP *)data;
	
	//通过我们的保存好的glade创建一个glade xml
	mplayer->gxml=glade_xml_new("mplayer.glade",NULL,NULL);
	
    //获取控件
	mplayer->window=glade_xml_get_widget(mplayer->gxml,"window_back");  //主窗口
	mplayer->button_backward=glade_xml_get_widget(mplayer->gxml,"button_backward"); 
	mplayer->button_pause=glade_xml_get_widget(mplayer->gxml,"button_pause"); //播放按钮
	mplayer->button_forward=glade_xml_get_widget(mplayer->gxml,"button_forward"); 
	
	//信号连接
	glade_xml_signal_autoconnect(mplayer->gxml);
	g_signal_connect(mplayer->window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	
	
}

int main(int argc,char *argv[])  
{
   gtk_init(&argc,&argv); 
	MP mplayer;
	memset(&mplayer, 0, sizeof(MP));
	mplayer_win_init(&mplayer);//界面初始化
    mkfifo("fifo_cmd", S_IRUSR|S_IWUSR);
    mplayer.fd=open("fifo_cmd",O_RDWR);	
	
	printf("in main fun fd=%d\n",mplayer.fd);
	pid_t pid;
    pid = fork();
	if(pid == 0)
	{
		execlp("mplayer",
		" mplayer", "-ac", "mad",
		"-slave", "-quiet","-idle",
		"-input", "file=./fifo_cmd",
		"./coffee.mp3", NULL);
    }
    else
	{
    g_signal_connect(mplayer.button_backward,"clicked",G_CALLBACK(con_backward),&mplayer);
	g_signal_connect(mplayer.button_pause,"clicked",G_CALLBACK(con_pause),&mplayer);
	g_signal_connect(mplayer.button_forward,"clicked",G_CALLBACK(con_forward),&mplayer);
	}
	gtk_widget_show_all(mplayer.window);
	gtk_main();
	return 0;
}