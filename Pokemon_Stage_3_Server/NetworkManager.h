#pragma once
#include "ProgramLib.h"

// ���������
class NetworkManager {
private:
	WORD sockVer;		// �׽��ְ汾
	WSADATA wsaData;
	SOCKET sListen;		// �����׽���
	sockaddr_in sin;
public:
	NetworkManager();
	~NetworkManager();
	// ��ʼ���׽���
	void initWSA();
	// ���������׽���
	void createSocket();
	// ���׽���
	void bindSocket();
	// ��ʼ����
	void startListen() const;
	// ������������
	void acceptConnect() const;
};