#include "NetworkManager.h"
#include "SystemManager.h"
#include "ConnectedClient.h"

extern SystemManager* System;

NetworkManager::NetworkManager() {
	initWSA();
	createSocket();
	bindSocket();
	startListen();
	std::cout << System->currentTime() << "������������������ɹ�" << std::endl;
}

NetworkManager::~NetworkManager() {
	closesocket(sListen);
	WSACleanup();
	std::cout << System->currentTime() << "��������������ѹر�" << std::endl;
}

// ��ʼ���׽���
void NetworkManager::initWSA() {
	sockVer = MAKEWORD(2, 2);
	if (WSAStartup(sockVer, &wsaData) != 0) {
		std::cout << System->currentTime() << "��ʼ���׽���ʧ��" << std::endl;
		exit(-1);
	}
}

// ���������׽���
void NetworkManager::createSocket() {
	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen == INVALID_SOCKET) {
		std::cout << System->currentTime() << "���������׽���ʧ��" << std::endl;
		exit(-1);
	}
}

// ���׽���
void NetworkManager::bindSocket() {
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8193);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(sListen, (LPSOCKADDR) &sin, sizeof(sin)) == SOCKET_ERROR) {
		std::cout << System->currentTime() << "���׽���ʧ��" << std::endl;
		exit(-1);
	}
}

// ��ʼ����
void NetworkManager::startListen() const {
	if (listen(sListen, 5) == SOCKET_ERROR) {
		std::cout << System->currentTime() << "�����׽���ʧ��" << std::endl;
		exit(-1);
	}
}

// ������������
void NetworkManager::acceptConnect() const {
	while (true) {
		SOCKET sClient;
		sockaddr_in remoteAddr;
		int nAddrlen = sizeof(remoteAddr);
		sClient = accept(sListen, (SOCKADDR*) &remoteAddr, &nAddrlen);
		if (sClient != INVALID_SOCKET) {
			System->createClient(sClient, remoteAddr);
		}
	}
}