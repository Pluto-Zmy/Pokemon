#include "ConnectedClient.h"
#include "SystemManager.h"
#include "DataBaseManager.h"

extern SystemManager* System;

// 构造客户端
ConnectedClient::ConnectedClient(SOCKET sClient_, sockaddr_in remoteAddr_, int ID_)
	: sClient(sClient_), remoteAddr(remoteAddr_), ID(ID_), isActive(false) {
	std::cout << System->currentTime() << "客户端 (ID: " << this->ID << ") 已连接" << std::endl;
	// 创建接收客户端请求的线程
	recvThread = new std::thread(&ConnectedClient::recvRequest, this);
	recvThread->detach();
}

// 删除客户端
ConnectedClient::~ConnectedClient() {
	// 删除线程
	delete recvThread;
}

std::string ConnectedClient::getUserName() const {
	return this->userName;
}

bool ConnectedClient::getState() const {
	return this->isActive;
}

// 循环接收请求函数
void ConnectedClient::recvRequest() {
	while (true) {
		char recvMsgBuffer[BUFFER_SIZE];
		// 接收来自客户端的数据
		int retSize = recvData(recvMsgBuffer);
		// 当接收到数据
		if (retSize > 0) {
			// 对数据进行分析，选择相应的操作
			selectRequest(*recvMsgBuffer, recvMsgBuffer + FLAG_SIZE);
		}
		// 当检测到与客户端断开连接
		else if (retSize <= 0 && errno != EINTR) {
			if (this->isActive == true) {
				std::cout << System->currentTime() << "用户 " << this->userName << " 已退出登录" << std::endl;
			}
			// 撤销活跃标记，弃用该客户端对象
			this->isActive = false;
			std::cout << System->currentTime() << "客户端 (ID: " << this->ID << ") 已断开连接" << std::endl;
			// 退出数据接收线程
			break;
		}
	}
}

// 数据发送函数
void ConnectedClient::sendData(const char* sendBuffer_, int sendSize_) {
	send(sClient, sendBuffer_, sendSize_, 0);
	char recvACK[BUFFER_SIZE];
	recv(sClient, recvACK, BUFFER_SIZE, 0);
}

// 数据接收函数
int ConnectedClient::recvData(char* recvBuffer_) {
	int retSize = recv(sClient, recvBuffer_, BUFFER_SIZE, 0);
	char sendACK[BUFFER_SIZE]{ACK};
	send(sClient, sendACK, sizeof(sendACK), 0);
	return retSize;
}

// 客户端请求解析函数
void ConnectedClient::selectRequest(char recvRequest_, char* recvLoad_) {
	switch (recvRequest_) {
		// 注册请求
		case SIGN_IN:
		{
			char userName[STRING_SIZE]{};
			char passWord[STRING_SIZE]{};
			memcpy(userName, recvLoad_, STRING_SIZE);
			memcpy(passWord, recvLoad_ + STRING_SIZE, STRING_SIZE);
			signIn(userName, passWord);
			break;
		}
		// 登录请求
		case LOG_IN:
		{
			char userName[STRING_SIZE]{};
			char passWord[STRING_SIZE]{};
			memcpy(userName, recvLoad_, STRING_SIZE);
			memcpy(passWord, recvLoad_ + STRING_SIZE, STRING_SIZE);
			logIn(userName, passWord);
			break;
		}
		// 退出登录请求
		case LOG_OUT:
		{
			logOut();
			break;
		}
		// 创建精灵请求
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
		// 获取精灵信息请求
		case GET_POKEMON:
		{
			std::string userName{recvLoad_};
			sendPokemon(userName);
			break;
		}
		// 更新精灵信息请求
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
		// 删除精灵请求
		case DELETE_POKEMON:
		{
			char pokemonName[STRING_SIZE]{};
			memcpy(pokemonName, recvLoad_, STRING_SIZE);
			deletePokemon(std::string(pokemonName));
			break;
		}
		// 获取战斗信息请求
		case GET_FIGHT_DATA:
		{
			char userName[STRING_SIZE]{};
			memcpy(userName, recvLoad_, STRING_SIZE);
			sendFightData(std::string(userName));
			break;
		}
		// 更新战斗信息请求
		case UPDATE_FIGHT_DATA:
		{
			int fightWinNum;
			int fightAllNum;
			memcpy(&fightWinNum, recvLoad_, sizeof(int));
			memcpy(&fightAllNum, recvLoad_ + 4, sizeof(int));
			updateFightData(fightWinNum, fightAllNum);
			break;
		}
		// 获取在线用户列表请求
		case GET_ONLINE_USER:
		{
			sendOnlineUserList();
			break;
		}
		// 获取所有用户列表请求
		case GET_ALL_USER:
		{
			sendAllUserList();
			break;
		}
		default:
			break;
	}
}

// 注册函数
void ConnectedClient::signIn(std::string userName_, std::string passWord_) {
	char sendMsgBuffer[BUFFER_SIZE];
	// 检查用户名是否已经被注册
	if (System->DataBase.checkUserExist(userName_) == true) {
		// 用户名重名，注册失败，向客户端返回注册失败信息
		*sendMsgBuffer = SIGN_IN_FAIL;
		std::cout << System->currentTime() << "用户 " << userName_ << " 注册失败，该用户名已经注册" << std::endl;
	}
	else {
		// 注册成功，在数据库中插入新用户记录
		System->DataBase.insertUser(userName_, passWord_);
		// 向客户端返回注册成功信息
		*sendMsgBuffer = SIGN_IN_SUCCESS;
		std::cout << System->currentTime() << "用户 " << userName_ << " 注册成功" << std::endl;
	}
	sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
}

// 登录函数
void ConnectedClient::logIn(std::string userName_, std::string passWord_) {
	char sendMsgBuffer[BUFFER_SIZE];
	// 检查用户名是否存在
	if (System->DataBase.checkUserExist(userName_) == false) {
		// 数据库中不存在记录，向客户端返回登录失败信息
		*sendMsgBuffer = LOG_IN_FAIL_USER_NOT_EXIST;
		std::cout << System->currentTime() << "用户名 " << userName_ << " 登录失败，该用户名不存在" << std::endl;
	}
	else {
		// 数据库中存在记录，进一步检查登录状态
		bool isOnline{false};
		// 查询用户是否已经登录
		for (auto client : System->clientList) {
			if (client->isActive && client->userName == userName_) {
				isOnline = true;
			}
		}
		if (isOnline) {
			// 已经登录，向客户端返回登录失败信息
			*sendMsgBuffer = LOG_IN_FAIL_USER_ONLINE;
			std::cout << System->currentTime() << "用户 " << userName_ << " 登录失败，该用户已经登录" << std::endl;
		}
		else {
			// 未登录，检查密码是否正确
			if (System->DataBase.getPassWord(userName_) == passWord_) {
				// 密码正确，登录成功，设置用户名以及活跃标记
				this->userName = userName_;
				this->isActive = true;
				// 向客户端返回登录成功信息
				*sendMsgBuffer = LOG_IN_SUCCESS;
				std::cout << System->currentTime() << "用户 " << userName_ << " 登录成功" << std::endl;
				// 进一步向客户端发送用户其它信息
				int fightWinNum = System->DataBase.getUserFightData(userName_).fightWinNum;
				int fightAllNum = System->DataBase.getUserFightData(userName_).fightAllNum;
				memcpy(sendMsgBuffer + 1, &fightWinNum, sizeof(int));
				memcpy(sendMsgBuffer + 5, &fightAllNum, sizeof(int));
			}
			else {
				// 密码错误，向客户端返回登录失败信息
				*sendMsgBuffer = LOG_IN_FAIL_PASSWORD_ERROR;
				std::cout << System->currentTime() << "用户 " << userName_ << " 登录失败，密码错误" << std::endl;
			}
		}
	}
	sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
}

// 退出登录函数
void ConnectedClient::logOut() {
	std::cout << System->currentTime() << "用户 " << this->userName << " 已退出登录" << std::endl;
	// 清空用户名
	this->userName.clear();
	// 撤销活跃标记
	isActive = false;
}

// 战斗信息发送函数
void ConnectedClient::sendFightData(std::string userName_) {
	char sendMsgBuffer[BUFFER_SIZE];
	UserFightData fightData = System->DataBase.getUserFightData(userName_);
	memcpy(sendMsgBuffer, &fightData.fightWinNum, sizeof(int));
	memcpy(sendMsgBuffer + 4, &fightData.fightAllNum, sizeof(int));
	sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
}

// 更新用户战斗信息函数
void ConnectedClient::updateFightData(int fightWinNum_, int fightAllNum_) {
	// 在数据库中更新记录
	System->DataBase.updateUser(this->userName, fightWinNum_, fightAllNum_);
}

// 精灵创建函数
void ConnectedClient::createPokemon(PokemonData pokemonData_) {
	// 在数据库中更新记录
	System->DataBase.insertPokemon(pokemonData_);
}

void ConnectedClient::updatePokemon(std::string pokemonName_, PokemonData pokemonData_) {
	System->DataBase.updatePokemon(this->userName, pokemonName_, pokemonData_);
}

// 精灵删除函数
void ConnectedClient::deletePokemon(std::string pokemonName_) {
	// 在数据库中删除精灵记录
	System->DataBase.deletePokemon(this->userName, pokemonName_);
}

// 精灵信息发送函数
void ConnectedClient::sendPokemon(std::string userName_) {
	// 在数据中检索精灵列表
	std::vector<PokemonData> pokemonList{System->DataBase.getAllPokemon(userName_)};
	// 获取要发送的精灵个数
	int pokemonNum = pokemonList.size();
	// 先告知客户端要发送的精灵数目
	char sendNumMsgBuffer[BUFFER_SIZE];
	memcpy(sendNumMsgBuffer, &pokemonNum, sizeof(int));
	sendData(sendNumMsgBuffer, sizeof(sendNumMsgBuffer));
	// 依次发送精灵信息
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

// 在线用户列表发送函数
void ConnectedClient::sendOnlineUserList() {
	// 获取在线用户列表
	std::vector<std::string> onlineUserList{System->getOnlineUserList()};
	// 获取要发送的用户个数
	int onlineUserNum = onlineUserList.size();
	// 先告知客户端要发送的用户数目
	char sendNumMsgBuffer[BUFFER_SIZE];
	memcpy(sendNumMsgBuffer, &onlineUserNum, sizeof(int));
	sendData(sendNumMsgBuffer, sizeof(sendNumMsgBuffer));
	// 依次发送用户名
	for (int i{0}; i < onlineUserNum; i++) {
		char sendDataMsgBuffer[BUFFER_SIZE]{};
		memcpy(sendDataMsgBuffer, onlineUserList[i].data(), STRING_SIZE);
		sendData(sendDataMsgBuffer, sizeof(sendDataMsgBuffer));
	}
}

// 所有用户列表发送函数
void ConnectedClient::sendAllUserList() {
	// 获取所有用户列表
	std::vector<std::string> allUserList{System->DataBase.getAllUserList()};
	// 获取要发送的用户个数
	int allUserNum = allUserList.size();
	// 先告知客户端要发送的用户数目
	char sendNumMsgBuffer[BUFFER_SIZE];
	memcpy(sendNumMsgBuffer, &allUserNum, sizeof(int));
	sendData(sendNumMsgBuffer, sizeof(sendNumMsgBuffer));
	// 依次发送用户名
	for (int i{0}; i < allUserNum; i++) {
		char sendDataMsgBuffer[BUFFER_SIZE]{};
		memcpy(sendDataMsgBuffer, allUserList[i].data(), STRING_SIZE);
		sendData(sendDataMsgBuffer, sizeof(sendDataMsgBuffer));
	}
}