#ifndef SERIAL_H
#define SERIAL_H

#include <QThread>
#include <QQueue>

class Serial : public QThread
{
	Q_OBJECT
	public:
	Serial();

	static char g_Msg[5][500];		// store message from serial
	static QQueue <int> g_Queue;		// queue store Number of g_Msg
	int g_fd_serial;			// 串口设备指针
	signals:
	void infoStart(int info);		// 发送信号
	
	protected:
	void run();				// 串口线程
	
	private:
	int info;
	char ch;
	int i,j;


};


#endif
