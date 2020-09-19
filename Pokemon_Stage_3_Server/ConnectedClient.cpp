#include "ConnectedClient.h"
#include "SystemManager.h"
#include "DataBaseManager.h"

extern SystemManager* System;

// ����ͻ���
ConnectedClient::ConnectedClient(SOCKET sClient_, sockaddr_in remoteAddr_, int ID_)
	: sClient(sClient_), remoteAddr(remoteAddr_), ID(ID_), isActive(false) {
	std::cout << System->currentTime() << "�ͻ��� (ID: " << this->ID << ") ������" << std::endl;
	// �������տͻ���������߳�
	recvThread = new std::thread(&ConnectedClient::recvRequest, this);
	recvThread->detach();
}

// ɾ���ͻ���
ConnectedClient::~ConnectedClient() {
	// ɾ���߳�
	delete recvThread;
}

std::string ConnectedClient::getUserName() const {
	return this->userName;
}

bool ConnectedClient::getState() const {
	return this->isActive;
}

// ѭ������������
void ConnectedClient::recvRequest() {
	while (true) {
		char recvMsgBuffer[BUFFER_SIZE];
		// �������Կͻ��˵�����
		int retSize = recvData(recvMsgBuffer);
		// �����յ�����
		if (retSize > 0) {
			// �����ݽ��з�����ѡ����Ӧ�Ĳ���
			selectRequest(*recvMsgBuffer, recvMsgBuffer + FLAG_SIZE);
		}
		// ����⵽��ͻ��˶Ͽ�����
		else if (retSize <= 0 && errno != EINTR) {
			if (this->isActive == true) {
				std::cout << System->currentTime() << "�û� " << this->userName << " ���˳���¼" << std::endl;
			}
			// ������Ծ��ǣ����øÿͻ��˶���
			this->isActive = false;
			std::cout << System->currentTime() << "�ͻ��� (ID: " << this->ID << ") �ѶϿ�����" << std::endl;
			// �˳����ݽ����߳�
			break;
		}
	}
}

// ���ݷ��ͺ���
void ConnectedClient::sendData(const char* sendBuffer_, int sendSize_) {
	send(sClient, sendBuffer_, sendSize_, 0);
	char recvACK[BUFFER_SIZE];
	recv(sClient, recvACK, BUFFER_SIZE, 0);
}

// ���ݽ��պ���
int ConnectedClient::recvData(char* recvBuffer_) {
	int retSize = recv(sClient, recvBuffer_, BUFFER_SIZE, 0);
	char sendACK[BUFFER_SIZE]{ACK};
	send(sClient, sendACK, sizeof(sendACK), 0);
	return retSize;
}

// �ͻ��������������
void ConnectedClient::selectRequest(char recvRequest_, char* recvLoad_) {
	switch (recvRequest_) {
		// ע������
		case SIGN_IN:
		{
			char userName[STRING_SIZE]{};
			char passWord[STRING_SIZE]{};
			memcpy(userName, recvLoad_, STRING_SIZE);
			memcpy(passWord, recvLoad_ + STRING_SIZE, STRING_SIZE);
			signIn(userName, passWord);
			break;
		}
		// ��¼����
		case LOG_IN:
		{
			char userName[STRING_SIZE]{};
			char passWord[STRING_SIZE]{};
			memcpy(userName, recvLoad_, STRING_SIZE);
			memcpy(passWord, recvLoad_ + STRING_SIZE, STRING_SIZE);
			logIn(userName, passWord);
			break;
		}
		// �˳���¼����
		case LOG_OUT:
		{
			logOut();
			break;
		}
		// ������������
		case CREATE_POKEMON:
		{
			PokemonData pokemonData;
			char ownerName[STRING_SIZE]{};
			char name[STRING_SIZE]{};
			memcpy(ownerName, recvLoad_, STRING_SIZE);
			memcpy(name, recvLoad_ + 16, STRING_SIZE);
			memcpy(&pokemonData.kind, recvLoad_ + 32, sizeof(int));
			memcpy(&pokemonData.level, recvLoad_ + 36, sizeof(int));
			memcpy(&pokemonData.exp, recvLoad_ + 40, sizeof(int));
			memcpy(&pokemonData.attack, recvLoad_ + 44, sizeof(int));
			memcpy(&pokemonData.defense, recvLoad_ + 48, sizeof(int));
			memcpy(&pokemonData.maxBlood, recvLoad_ + 52, sizeof(int));
			memcpy(&pokemonData.currentBlood, recvLoad_ + 56, sizeof(int));
			memcpy(&pokemonData.attackSpeed, recvLoad_ + 60, sizeof(int));
			memcpy(&pokemonData.attackMethod, recvLoad_ + 64, sizeof(int));
			pokemonData.ownerName = std::string(ownerName);
			pokemonData.name = std::string(name);
			createPokemon(pokemonData);
			break;
		}
		// ��ȡ������Ϣ����
		case GET_POKEMON:
		{
			std::string userName{recvLoad_};
			sendPokemon(userName);
			break;
		}
		// ���¾�����Ϣ����
		case UPDATE_POKEMON:
		{
			PokemonData pokemonData;
			char oldPokemonName[STRING_SIZE]{};
			char ownerName[STRING_SIZE]{};
			char name[STRING_SIZE]{};
			memcpy(oldPokemonName, recvLoad_, STRING_SIZE);
			memcpy(ownerName, recvLoad_ + 16, STRING_SIZE);
			memcpy(name, recvLoad_ + 32, STRING_SIZE);
			memcpy(&pokemonData.kind, recvLoad_ + 48, sizeof(int));
			memcpy(&pokemonData.level, recvLoad_ + 52, sizeof(int));
			memcpy(&pokemonData.exp, recvLoad_ + 56, sizeof(int));
			memcpy(&pokemonData.attack, recvLoad_ + 60, sizeof(int));
			memcpy(&pokemonData.defense, recvLoad_ + 64, sizeof(int));
			memcpy(&pokemonData.maxBlood, recvLoad_ + 68, sizeof(int));
			memcpy(&pokemonData.currentBlood, recvLoad_ + 72, sizeof(int));
			memcpy(&pokemonData.attackSpeed, recvLoad_ + 76, sizeof(int));
			memcpy(&pokemonData.attackMethod, recvLoad_ + 80, sizeof(int));
			pokemonData.ownerName = std::string(ownerName);
			pokemonData.name = std::string(name);
			updatePokemon(std::string(oldPokemonName), pokemonData);
			break;
		}
		// ɾ����������
		case DELETE_POKEMON:
		{
			char pokemonName[STRING_SIZE]{};
			memcpy(pokemonName, recvLoad_, STRING_SIZE);
			deletePokemon(std::string(pokemonName));
			break;
		}
		// ��ȡս����Ϣ����
		case GET_FIGHT_DATA:
		{
			char userName[STRING_SIZE]{};
			memcpy(userName, recvLoad_, STRING_SIZE);
			sendFightData(std::string(userName));
			break;
		}
		// ����ս����Ϣ����
		case UPDATE_FIGHT_DATA:
		{
			int fightWinNum;
			int fightAllNum;
			memcpy(&fightWinNum, recvLoad_, sizeof(int));
			memcpy(&fightAllNum, recvLoad_ + 4, sizeof(int));
			updateFightData(fightWinNum, fightAllNum);
			break;
		}
		// ��ȡ�����û��б�����
		case GET_ONLINE_USER:
		{
			sendOnlineUserList();
			break;
		}
		// ��ȡ�����û��б�����
		case GET_ALL_USER:
		{
			sendAllUserList();
			break;
		}
		default:
			break;
	}
}

// ע�ắ��
void ConnectedClient::signIn(std::string userName_, std::string passWord_) {
	char sendMsgBuffer[BUFFER_SIZE];
	// ����û����Ƿ��Ѿ���ע��
	if (System->DataBase.checkUserExist(userName_) == true) {
		// �û���������ע��ʧ�ܣ���ͻ��˷���ע��ʧ����Ϣ
		*sendMsgBuffer = SIGN_IN_FAIL;
		std::cout << System->currentTime() << "�û� " << userName_ << " ע��ʧ�ܣ����û����Ѿ�ע��" << std::endl;
	}
	else {
		// ע��ɹ��������ݿ��в������û���¼
		System->DataBase.insertUser(userName_, passWord_);
		// ��ͻ��˷���ע��ɹ���Ϣ
		*sendMsgBuffer = SIGN_IN_SUCCESS;
		std::cout << System->currentTime() << "�û� " << userName_ << " ע��ɹ�" << std::endl;
	}
	sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
}

// ��¼����
void ConnectedClient::logIn(std::string userName_, std::string passWord_) {
	char sendMsgBuffer[BUFFER_SIZE];
	// ����û����Ƿ����
	if (System->DataBase.checkUserExist(userName_) == false) {
		// ���ݿ��в����ڼ�¼����ͻ��˷��ص�¼ʧ����Ϣ
		*sendMsgBuffer = LOG_IN_FAIL_USER_NOT_EXIST;
		std::cout << System->currentTime() << "�û��� " << userName_ << " ��¼ʧ�ܣ����û���������" << std::endl;
	}
	else {
		// ���ݿ��д��ڼ�¼����һ������¼״̬
		bool isOnline{false};
		// ��ѯ�û��Ƿ��Ѿ���¼
		for (auto client : System->clientList) {
			if (client->isActive && client->userName == userName_) {
				isOnline = true;
			}
		}
		if (isOnline) {
			// �Ѿ���¼����ͻ��˷��ص�¼ʧ����Ϣ
			*sendMsgBuffer = LOG_IN_FAIL_USER_ONLINE;
			std::cout << System->currentTime() << "�û� " << userName_ << " ��¼ʧ�ܣ����û��Ѿ���¼" << std::endl;
		}
		else {
			// δ��¼����������Ƿ���ȷ
			if (System->DataBase.getPassWord(userName_) == passWord_) {
				// ������ȷ����¼�ɹ��������û����Լ���Ծ���
				this->userName = userName_;
				this->isActive = true;
				// ��ͻ��˷��ص�¼�ɹ���Ϣ
				*sendMsgBuffer = LOG_IN_SUCCESS;
				std::cout << System->currentTime() << "�û� " << userName_ << " ��¼�ɹ�" << std::endl;
				// ��һ����ͻ��˷����û�������Ϣ
				int fightWinNum = System->DataBase.getUserFightData(userName_).fightWinNum;
				int fightAllNum = System->DataBase.getUserFightData(userName_).fightAllNum;
				memcpy(sendMsgBuffer + 1, &fightWinNum, sizeof(int));
				memcpy(sendMsgBuffer + 5, &fightAllNum, sizeof(int));
			}
			else {
				// ���������ͻ��˷��ص�¼ʧ����Ϣ
				*sendMsgBuffer = LOG_IN_FAIL_PASSWORD_ERROR;
				std::cout << System->currentTime() << "�û� " << userName_ << " ��¼ʧ�ܣ��������" << std::endl;
			}
		}
	}
	sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
}

// �˳���¼����
void ConnectedClient::logOut() {
	std::cout << System->currentTime() << "�û� " << this->userName << " ���˳���¼" << std::endl;
	// ����û���
	this->userName.clear();
	// ������Ծ���
	isActive = false;
}

// ս����Ϣ���ͺ���
void ConnectedClient::sendFightData(std::string userName_) {
	char sendMsgBuffer[BUFFER_SIZE];
	UserFightData fightData = System->DataBase.getUserFightData(userName_);
	memcpy(sendMsgBuffer, &fightData.fightWinNum, sizeof(int));
	memcpy(sendMsgBuffer + 4, &fightData.fightAllNum, sizeof(int));
	sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
}

// �����û�ս����Ϣ����
void ConnectedClient::updateFightData(int fightWinNum_, int fightAllNum_) {
	// �����ݿ��и��¼�¼
	System->DataBase.updateUser(this->userName, fightWinNum_, fightAllNum_);
}

// ���鴴������
void ConnectedClient::createPokemon(PokemonData pokemonData_) {
	// �����ݿ��и��¼�¼
	System->DataBase.insertPokemon(pokemonData_);
}

void ConnectedClient::updatePokemon(std::string pokemonName_, PokemonData pokemonData_) {
	System->DataBase.updatePokemon(this->userName, pokemonName_, pokemonData_);
}

// ����ɾ������
void ConnectedClient::deletePokemon(std::string pokemonName_) {
	// �����ݿ���ɾ�������¼
	System->DataBase.deletePokemon(this->userName, pokemonName_);
}

// ������Ϣ���ͺ���
void ConnectedClient::sendPokemon(std::string userName_) {
	// �������м��������б�
	std::vector<PokemonData> pokemonList{System->DataBase.getAllPokemon(userName_)};
	// ��ȡҪ���͵ľ������
	int pokemonNum = pokemonList.size();
	// �ȸ�֪�ͻ���Ҫ���͵ľ�����Ŀ
	char sendNumMsgBuffer[BUFFER_SIZE];
	memcpy(sendNumMsgBuffer, &pokemonNum, sizeof(int));
	sendData(sendNumMsgBuffer, sizeof(sendNumMsgBuffer));
	// ���η��;�����Ϣ
	for (int i{0}; i < pokemonNum; i++) {
		char sendDataMsgBuffer[BUFFER_SIZE];
		PokemonData currentData = pokemonList[i];
		memcpy(sendDataMsgBuffer, currentData.ownerName.data(), STRING_SIZE);
		memcpy(sendDataMsgBuffer + 16, currentData.name.data(), STRING_SIZE);
		memcpy(sendDataMsgBuffer + 32, &currentData.kind, sizeof(int));
		memcpy(sendDataMsgBuffer + 36, &currentData.level, sizeof(int));
		memcpy(sendDataMsgBuffer + 40, &currentData.exp, sizeof(int));
		memcpy(sendDataMsgBuffer + 44, &currentData.attack, sizeof(int));
		memcpy(sendDataMsgBuffer + 48, &currentData.defense, sizeof(int));
		memcpy(sendDataMsgBuffer + 52, &currentData.maxBlood, sizeof(int));
		memcpy(sendDataMsgBuffer + 56, &currentData.currentBlood, sizeof(int));
		memcpy(sendDataMsgBuffer + 60, &currentData.attackSpeed, sizeof(int));
		memcpy(sendDataMsgBuffer + 64, &currentData.attackMethod, sizeof(int));
		sendData(sendDataMsgBuffer, sizeof(sendDataMsgBuffer));
	}
}

// �����û��б��ͺ���
void ConnectedClient::sendOnlineUserList() {
	// ��ȡ�����û��б�
	std::vector<std::string> onlineUserList{System->getOnlineUserList()};
	// ��ȡҪ���͵��û�����
	int onlineUserNum = onlineUserList.size();
	// �ȸ�֪�ͻ���Ҫ���͵��û���Ŀ
	char sendNumMsgBuffer[BUFFER_SIZE];
	memcpy(sendNumMsgBuffer, &onlineUserNum, sizeof(int));
	sendData(sendNumMsgBuffer, sizeof(sendNumMsgBuffer));
	// ���η����û���
	for (int i{0}; i < onlineUserNum; i++) {
		char sendDataMsgBuffer[BUFFER_SIZE]{};
		memcpy(sendDataMsgBuffer, onlineUserList[i].data(), STRING_SIZE);
		sendData(sendDataMsgBuffer, sizeof(sendDataMsgBuffer));
	}
}

// �����û��б��ͺ���
void ConnectedClient::sendAllUserList() {
	// ��ȡ�����û��б�
	std::vector<std::string> allUserList{System->DataBase.getAllUserList()};
	// ��ȡҪ���͵��û�����
	int allUserNum = allUserList.size();
	// �ȸ�֪�ͻ���Ҫ���͵��û���Ŀ
	char sendNumMsgBuffer[BUFFER_SIZE];
	memcpy(sendNumMsgBuffer, &allUserNum, sizeof(int));
	sendData(sendNumMsgBuffer, sizeof(sendNumMsgBuffer));
	// ���η����û���
	for (int i{0}; i < allUserNum; i++) {
		char sendDataMsgBuffer[BUFFER_SIZE]{};
		memcpy(sendDataMsgBuffer, allUserList[i].data(), STRING_SIZE);
		sendData(sendDataMsgBuffer, sizeof(sendDataMsgBuffer));
	}
}