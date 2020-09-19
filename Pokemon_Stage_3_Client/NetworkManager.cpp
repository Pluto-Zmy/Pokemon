#include "NetworkManager.h"
#include "LogIn.h"

extern LogIn* LogInWindow;

// 构造函数
NetworkManager::NetworkManager(std::string IP_) {
	// 初始化套接字
	initWSA();
	// 创建套接字
	createSocket();
	// 绑定套接字
	bindSocket(IP_);
}

// 析构函数
NetworkManager::~NetworkManager() {
	// 关闭套接字
	closesocket(sClient);
	WSACleanup();
}

// 初始化套接字
void NetworkManager::initWSA() {
	sockVer = MAKEWORD(2, 2);
	if (WSAStartup(sockVer, &wsaData) != 0) {
		showNetworkError();
	}
}

// 创建套接字
void NetworkManager::createSocket() {
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET) {
		showNetworkError();
	}
}

// 绑定套接字
void NetworkManager::bindSocket(std::string IP_) {
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8193);
	inet_pton(AF_INET, IP_.data(), &serAddr.sin_addr.S_un.S_addr);
	if (connect(sClient, (sockaddr*) &serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
		closesocket(sClient);
		QApplication::beep();
		QMessageBox::critical(LogInWindow,
			QString::fromLocal8Bit("网络错误"),
			QString::fromLocal8Bit("无法连接到游戏服务器，请检查网络设置。"),
			QString::fromLocal8Bit("确定"));
		exit(-1);
	}
}

// 发送数据
void NetworkManager::sendData(const char* sendMsg_, int sendSize_) const {
	send(sClient, sendMsg_, sendSize_, 0);
	char recvACK[BUFFER_SIZE];
	recv(sClient, recvACK, BUFFER_SIZE, 0);
}

// 接收数据
int NetworkManager::recvData(char* recvMsg_) const {
	int retSize = recv(sClient, recvMsg_, BUFFER_SIZE, 0);
	if (retSize <= 0 && errno != EINTR) {
		MessageBox(nullptr,
			L"与服务器意外断开连接，客户端即将关闭。",
			L"网络错误", MB_ICONERROR);
		exit(-1);
	}
	char sendACK[BUFFER_SIZE]{ACK};
	send(sClient, sendACK, sizeof(sendACK), 0);
	return retSize;
}

void NetworkManager::showNetworkError() const {
	MessageBox(nullptr,
		L"初始化网络服务时遇到严重错误。",
		L"初始化错误", MB_ICONERROR);
	exit(-1);
}