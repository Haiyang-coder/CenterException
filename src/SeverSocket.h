/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-11-17 16:16:44
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-11-20 10:59:59
 * @FilePath: /ExceptionnalOperaAggregteSystem/src/SeverSocket.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include <list>
#include "Packet.h"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ThreadPool.h"
#include "DealTask.h"
#include <mutex>
#include "ThreadSafeQueue.h"
#include <iomanip>

#define D_WORK_THREADS_MIN_NUM 5  // 最少工作线程数
#define D_WORK_THREADS_MAX_NUM 15 // 最大工作线程数
#define D_SERVER_PORT 8856		  // 作为服务端的端口
#define D_CLIENT_PORT 7877		  // 作为客户端的端口
#define D_CLIENT_IP 0x7F000001	  // 作为客户端需要连接的IP
#define D_MAXEVENTS_NUM 1024	  // 最大的连接数量
#define D_EPOLL_THREADS_NUM 10	  // 最大的epolltree数量
#define D_MAX_PACKET (1024 * 5)

/// @brief 接收链接需要的参数
struct dataEpollAccept
{
	int fd;		 // epoll文件描述符
	void *m_arg; // 网络模块的对象指针
	dataEpollAccept(int nfd, void *arg)
	{
		m_arg = arg;
		fd = nfd;
	};
};

/// @brief 接收数据需要的参数
struct dataEpollRecv
{
	int epoll_fd;	 // epoll文件描述符
	int recv_fd;	 // 接收套接字
	void *arg;		 // 网络模块的对象指针
	void *taskqueue; // 任务队列
	dataEpollRecv(int ep_fd, int re_fd, void *ptr, void *taskq)
	{
		epoll_fd = ep_fd;
		recv_fd = re_fd;
		arg = ptr;
		taskqueue = taskq;
	};
};

class CSeverSocket
{
public:
	// static CSeverSocket* getInstance();
	CSeverSocket();
	~CSeverSocket();

	// 当作服务端时的初始化
	bool InitSockEnv();
	// 作为客户端时的初始化
	bool initClientEnv();

	// epoll接收连接线程（只会提醒有连接来到，不会建立连接）
	void EpollAceppt_thead();
	// epoll监听数据传入的线程（只会提醒有数据传入，不会接收）
	void EpollRecv_thead(int epoll_fd, void *arg);

	// 关闭套接字
	int CloseEpollfd(int number);
	// 关闭服务端
	void CloseSocket();

private:
	// 接收连接的入口线程
	static void *AcceptClient_thread(void *arg);
	// 接收连接的具体实现
	int AcceptClien();
	// 接收数据的线程入口
	static void *RecvData_thead(void *arg);
	// 接收线程的具体实现
	int RecvData(int epoll_fd, int recv_fd, CThreadSafeQueue<CPacket> *taskQueue);
	// 安全的epollCtl
	int EpollSafeCtrl(int epfd, int opera, int fd, struct epoll_event *__event);
	// 安全的epollWait
	int EpollSafeWait(int __epfd, struct epoll_event *__events,
					  int __maxevents, int __timeout);

private:
	int m_serv_fd;						  // 用于和其他系统通信的套接字
	int m_client_sock;					  // 用于和汇聚系统通信的套接字
	int m_epoll_fds[D_EPOLL_THREADS_NUM]; // epoll文件描述符
	std::mutex myMutex;					  // 保护共享资源,在这里是保护epoll
	CThreadPool *m_pthreadPool;			  // 线程池
};
