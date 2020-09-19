#pragma once
#include "ProgramLib.h"
#include "NetworkManager.h"
#include "DataBaseManager.h"

// ϵͳ������
class SystemManager {
private:
	// ����ϵͳ�����б�
	void generateSystemPokemonList();
public:
	// ���ݿ����
	DataBaseManager DataBase;
	// �������
	NetworkManager Network;
	// �����ӵĿͻ����б�
	std::vector<ConnectedClient*> clientList;
	SystemManager();
	// ϵͳʱ��
	std::string currentTime() const;
	// ����һ���ͻ���
	void createClient(SOCKET sClient_, sockaddr_in remoteAddr_);
	// ��ȡ�����û��б�
	std::vector<std::string> getOnlineUserList() const;
};