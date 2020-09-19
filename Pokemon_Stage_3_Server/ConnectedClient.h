#pragma once
#include "ProgramLib.h"

// �����ӵĿͻ�����
class ConnectedClient {
private:
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int ID;
	// ��������߳�
	std::thread* recvThread;
	// �ͻ����ѵ�¼���û���
	std::string userName;
	// ���ӻ�Ծ���
	bool isActive;
public:
	// �ͻ��˹��캯������������
	ConnectedClient(SOCKET sClient_, sockaddr_in remoteAddr_, int ID_);
	~ConnectedClient();
	std::string getUserName() const;
	bool getState() const;
	// ѭ������������
	void recvRequest();
	// ���ݷ��ͺ���
	void sendData(const char* sendBuffer_, int sendSize_);
	// ���ݽ��պ���
	int recvData(char* recvBuffer_);
	// �ͻ��������������
	void selectRequest(char recvRequest_, char* recvLoad_);
	// ע�ắ��
	void signIn(std::string userName_, std::string passWord_);
	// ��¼����
	void logIn(std::string userName_, std::string passWord_);
	// �˳���¼����
	void logOut();
	// ս����Ϣ���ͺ���
	void sendFightData(std::string userName_);
	// �����û�ս����Ϣ����
	void updateFightData(int fightWinNum_, int fightAllNum_);
	// ���鴴������
	void createPokemon(PokemonData pokemonData_);
	// ������Ϣ���º���
	void updatePokemon(std::string pokemonName_, PokemonData pokemonData_);
	// ����ɾ������
	void deletePokemon(std::string pokemonName_);
	// ������Ϣ���ͺ���
	void sendPokemon(std::string userName_);
	// �����û��б��ͺ���
	void sendOnlineUserList();
	// �����û��б��ͺ���
	void sendAllUserList();
};