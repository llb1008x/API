#!/usr/bin/python
#coding:utf-8

from socket import *
from SocketServer import ThreadingTCPServer, StreamRequestHandler
import os.path, sys, getpass
import traceback,threading
import SocketServer, socket
import select, fcntl, struct


SIGN_TYPE=""
SIGN_FILE=""
SIGN_PROJECT=""

#签名服务器----发送文件到签名服务器
if getpass.getuser() == "ranyi":
    SEND_SERVER_ADDR_PORT = ('10.8.0.23',60518)
else:
    SEND_SERVER_ADDR_PORT = ('10.8.0.18',60518)
print("SEND_SERVER_ADDR_PORT:[%s]"%str(SEND_SERVER_ADDR_PORT))
#本机服务器----用于接收签名后的文件
RECV_SERVER_ADDR_PORT = ('', 60519)

#客户端的本机IP，系统登录名
CLINT_LOCAL_IP = ""
CLINT_LOCAL_LOGNAME = ""

#保存接收签名后的文件的路径
RECV_SIGNED_PATH="gionee_sign_release"

# \033[显示方式;前景色;背景色m
# 显示方式:0（默认值）、1（高亮）、22（非粗体）、4（下划线）、24（非下划线）、5（闪烁）、25（非闪烁）、7（反显）、27（非反显）、8（不可见）
# 前景色:30（黑色）、31（红色）、32（绿色）、 33（黄色）、34（蓝色）、35（洋红）、36（青色）、37（白色）
# 背景色:40（黑色）、41（红色）、42（绿色）、 43（黄色）、44（蓝色）、45（洋红）、46（青色）、47（白色）
# \033[0m 默认
# \033[1;32;40m 绿色
# 033[1;31;40m 红色
COLOR_ERROR_PREFIX='\033[1;31;48m [ERROR]'
COLOR_WARNING_PREFIX='\033[1;33;48m [WARNING]'
COLOR_SUCCESS_PREFIX='\033[1;32;48m [SUCCESS]'
COLOR_RED_PREFIX='\033[1;31;48m'
COLOR_GREEN_PREFIX='\033[1;32;48m'
COLOR_YELLOW_PREFIX='\033[1;33;48m'
COLOR_SUFFIX='\033[0m'

class TCPServerV4(SocketServer.TCPServer):
	def __init__(self, server_address, RequestHandlerClass):
		SocketServer.TCPServer.__init__(self, server_address, RequestHandlerClass)
		self._shutdown_request = False

	def serve_forever(self, poll_interval=0.5):
		"""provide an override that can be shutdown from a request handler.
		The threading code in the BaseSocketServer class prevented this from working
		even for a non-threaded blocking server.
		"""
		#self.__is_shut_down.clear()
		try:
			while not self._shutdown_request:
				# XXX: Consider using another file descriptor or
				# connecting to the socket to wake this up instead of
				# polling. Polling reduces our responsiveness to a
				# shutdown request and wastes cpu at all other times.
				r, w, e = select.select([self], [], [], poll_interval)
				if self in r:
					self._handle_request_noblock()
		finally:
			self._shutdown_request = False
			#self.__is_shut_down.set()

#多线程socket
#使用ThreadingTCPServer和StreamRequestHandler编写socket服务的样例。
#ThreadingTCPServer从ThreadingMixIn和TCPServer继承，实现多线程。
class MyStreamRequestHandlerr(StreamRequestHandler):
    def handle(self):
        if os.path.exists(RECV_SIGNED_PATH) == False:
			os.mkdir(RECV_SIGNED_PATH)
        #get status
        statusLen = ord(self.rfile.read(1))
        statusStrings = self.rfile.read(statusLen)
        if "[SUCCESS]" in statusStrings:
		    statusStrings.split(':')[1]
		    #get filename
		    inName = statusStrings.split(':')[1]
		    outName = RECV_SIGNED_PATH+"/"+inName
		    print("开始接收文件:[%s]"%inName)
		    fd = open(outName, 'w')
		    dataBuffer = self.rfile.read(4096)
		    while dataBuffer:
		        fd.write(dataBuffer)
		        dataBuffer = self.rfile.read(4096)
		    fd.close()
		    print(COLOR_SUCCESS_PREFIX+"文件接收完成:[%s]"%outName+COLOR_SUFFIX)
        else:
		    print(COLOR_ERROR_PREFIX+statusStrings+COLOR_SUFFIX)
		#文件接收完毕后，关闭server连接，并退出
    	self.server._shutdown_request = True

def getHeader(name):
	leng = len(name)
	assert leng < 250
	return chr(leng) + name

def sendFile(name):
    fileName = os.path.basename(name)
    header1 = getHeader(CLINT_LOCAL_IP)
    header2 = getHeader(CLINT_LOCAL_LOGNAME)
    header3 = getHeader(SIGN_PROJECT)
    header4 = getHeader(SIGN_TYPE)
    header5 = getHeader(fileName)
    s = socket.socket(AF_INET, SOCK_STREAM)
    socket.setdefaulttimeout(10)
    print("开始发送文件 %s 到服务器..."%name)
    try:
        data = open(name).read()
        s.connect(SEND_SERVER_ADDR_PORT)
        s.sendall(header1)
        s.sendall(header2)
        s.sendall(header3)
        s.sendall(header4)
        s.sendall(header5)
        s.sendall(data)
        print("文件发送完成！")
    except KeyboardInterrupt:
        s.close()
    except socket.timeout:
        print("连接超时，请重试！")
        s.close()
    finally:
        s.close()

def startClint():
	sendFile(SIGN_FILE)

def getLocalIP():
	global CLINT_LOCAL_IP
	"""
	Returns the actual ip of the local machine.
	This code figures out what source address would be used if some traffic
	were to be sent out to some well known address on the Internet. In this
	case, a Google DNS server is used, but the specific address does not
	matter much. No traffic is actually sent.
	"""
	try:
		csock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		csock.connect(('8.8.8.8', 80))
		(addr, port) = csock.getsockname()
		csock.close()
		CLINT_LOCAL_IP = addr.strip('\n')
	except socket.error:
		print(COLOR_ERROR_PREFIX+"本地ip地址获取错误，您将无法接收到文件，请联系管理员！"+COLOR_SUFFIX)
		sys.exit(1)
	return CLINT_LOCAL_IP

def getLocalLoginName():
	global CLINT_LOCAL_LOGNAME
	global SEND_SERVER_ADDR_PORT
	CLINT_LOCAL_LOGNAME = getpass.getuser()

def recvSignedFile():
	print("等待接收签名后的文件...")
	global RECV_SERVER
	RECV_SERVER = TCPServerV4(RECV_SERVER_ADDR_PORT, MyStreamRequestHandlerr)
	try:
		RECV_SERVER.serve_forever()
	except KeyboardInterrupt:
		RECV_SERVER.shutdown()

def checkArgv():
	global SIGN_TYPE
	global SIGN_FILE
	global SIGN_PROJECT
	if(len(sys.argv) < 4):
		print(COLOR_ERROR_PREFIX+"参数个数错误! [%s]"%len(sys.argv)+COLOR_SUFFIX)
		print(COLOR_YELLOW_PREFIX+"用法: ./gn_sign_clint.py [PROJECT_NAME] [TYPE] [File Name]"+COLOR_SUFFIX)
		print(COLOR_YELLOW_PREFIX+"参数说明："+COLOR_SUFFIX)
		print(COLOR_YELLOW_PREFIX+"PROJECT_NAME --- 需要签名的项目名称"+COLOR_SUFFIX)
		print(COLOR_YELLOW_PREFIX+"TYPE ----------- 指定签名的类型,可能的值:"+COLOR_SUFFIX)
		print(COLOR_YELLOW_PREFIX+"                 img --- 镜像文件: bin/img"+COLOR_SUFFIX)
		print(COLOR_YELLOW_PREFIX+"                 ota --- 整包或者差分包: zip"+COLOR_SUFFIX)
		print(COLOR_YELLOW_PREFIX+"File Name ------ 需要签名的文件"+COLOR_SUFFIX)
		print("")
		print(COLOR_YELLOW_PREFIX+"举例："+COLOR_SUFFIX)
		print(COLOR_YELLOW_PREFIX+"    ./gn_sign_clint.py BBL7515A img logo.bin"+COLOR_SUFFIX)
		print(COLOR_YELLOW_PREFIX+"    ./gn_sign_clint.py BBL7516A ota BBL7337A_update_amigo3.5.0_T0037.zip"+COLOR_SUFFIX)
		print("")
		sys.exit(1)
	SIGN_PROJECT=sys.argv[1]
	SIGN_TYPE=sys.argv[2]
	SIGN_FILE=sys.argv[3]

def main():
	checkArgv()
	getLocalLoginName()
	getLocalIP()
	startClint()
	recvSignedFile()

if __name__ == '__main__':
	main()
else:
	print("被调用")
