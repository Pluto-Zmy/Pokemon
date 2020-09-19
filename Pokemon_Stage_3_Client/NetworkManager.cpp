#include "NetworkManager.h"
#include "LogIn.h"

extern LogIn* LogInWindow;

// ���캯��
NetworkManager::NetworkManager(std::string IP_) {
	// ��ʼ���׽���
	initWSA();
	// �����׽���
	createSocket();
	// ���׽���
	bindSocket(IP_);
}

// ��������
NetworkManager::~NetworkManager() {
	// �ر��׽���
	closesocket(sClient);
	WSACleanup();
}

// ��ʼ���׽���
void NetworkManager::initWSA() {
	sockVer = MAKEWORD(2, 2);
	if (WSAStartup(sockVer, &wsaData) != 0) {
		showNetworkError();
	}
}

// �����׽���
void NetworkManager::createSocket() {
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET) {
		showNetworkError();
	}
}

// ���׽���
void NetworkManager::bindSocket(std::string IP_) {
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8193);
	inet_pton(AF_INET, IP_.data(), &serAddr.sin_addr.S_un.S_addr);
	if (connect(sClient, (sockaddr*) &serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
		closesocket(sClient);
		QApplication::beep();
		QMessageBox::critical(LogInWindow,
			QString::fromLocal8Bit("�������"),
			QString::fromLocal8Bit("�޷����ӵ���Ϸ�������������������á�"),
			QString::fromLocal8Bit("ȷ��"));
		exit(-1);
	}
}

// ��������
void NetworkManager::sendData(const char* sendMsg_, int sendSize_) const {
	send(sClient, sendMsg_, sendSize_, 0);
	char recvACK[BUFFER_SIZE];
	recv(sClient, recvACK, BUFFER_SIZE, 0);
}

// ��������
int NetworkManager::recvData(char* recvMsg_) const {
	int retSize = recv(sClient, recvMsg_, BUFFER_SIZE, 0);
	if (retSize <= 0 && errno != EINTR) {
		MessageBox(nullptr,
			L"�����������Ͽ����ӣ��ͻ��˼����رա�",
			L"�������", MB_ICONERROR);
		exit(-1);
	}
	char sendACK[BUFFER_SIZE]{ACK};
	send(sClient, sendACK, sizeof(sendACK), 0);
	return retSize;
}

void NetworkManager::showNetworkError() const {
	MessageBox(nullptr,
		L"��ʼ���������ʱ�������ش���",
		L"��ʼ������", MB_ICONERROR);
	exit(-1);
}