#pragma once
#include "ProgramLib.h"

// 网络管理类
class NetworkManager {
private:
	WORD sockVer;		// 套接字版本
	WSADATA wsaData;
	SOCKET sListen;		// 监听套接字
	sockaddr_in sin;
public:
	NetworkManager();
	~NetworkManager();
	// 初始化套接字
	void initWSA();
	// 创建监听套接字
	void createSocket();
	// 绑定套接字
	void bindSocket();
	// 开始监听
	void startListen() const;
	// 接收连接请求
	void acceptConnect() const;
};