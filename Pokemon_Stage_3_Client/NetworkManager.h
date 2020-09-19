#pragma once
#include "ProgramLib.h"

// ���������
class NetworkManager {
private:
	// �׽�����ض���
	WORD sockVer;
	WSADATA wsaData;
	SOCKET sClient;
	sockaddr_in serAddr;
public:
	NetworkManager(std::string IP_);
	~NetworkManager();
	// ��ʼ���׽���
	void initWSA();
	// �����׽���
	void createSocket();
	// ���׽���
	void bindSocket(std::string IP_);
	// ��������
	void sendData(const char* sendMsg_, int sendSize_) const;
	// ��������
	int recvData(char* recvMsg_) const;
	// ������ʾ��
	void showNetworkError() const;
};