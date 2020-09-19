#pragma once
#include "ProgramLib.h"

// 网络管理类
class NetworkManager {
private:
	// 套接字相关对象
	WORD sockVer;
	WSADATA wsaData;
	SOCKET sClient;
	sockaddr_in serAddr;
public:
	NetworkManager(std::string IP_);
	~NetworkManager();
	// 初始化套接字
	void initWSA();
	// 创建套接字
	void createSocket();
	// 绑定套接字
	void bindSocket(std::string IP_);
	// 发送数据
	void sendData(const char* sendMsg_, int sendSize_) const;
	// 接收数据
	int recvData(char* recvMsg_) const;
	// 错误提示框
	void showNetworkError() const;
};