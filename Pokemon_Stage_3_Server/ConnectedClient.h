#pragma once
#include "ProgramLib.h"

// 已连接的客户端类
class ConnectedClient {
private:
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int ID;
	// 请求接收线程
	std::thread* recvThread;
	// 客户端已登录的用户名
	std::string userName;
	// 连接活跃标记
	bool isActive;
public:
	// 客户端构造函数、析构函数
	ConnectedClient(SOCKET sClient_, sockaddr_in remoteAddr_, int ID_);
	~ConnectedClient();
	std::string getUserName() const;
	bool getState() const;
	// 循环接收请求函数
	void recvRequest();
	// 数据发送函数
	void sendData(const char* sendBuffer_, int sendSize_);
	// 数据接收函数
	int recvData(char* recvBuffer_);
	// 客户端请求解析函数
	void selectRequest(char recvRequest_, char* recvLoad_);
	// 注册函数
	void signIn(std::string userName_, std::string passWord_);
	// 登录函数
	void logIn(std::string userName_, std::string passWord_);
	// 退出登录函数
	void logOut();
	// 战斗信息发送函数
	void sendFightData(std::string userName_);
	// 更新用户战斗信息函数
	void updateFightData(int fightWinNum_, int fightAllNum_);
	// 精灵创建函数
	void createPokemon(PokemonData pokemonData_);
	// 精灵信息更新函数
	void updatePokemon(std::string pokemonName_, PokemonData pokemonData_);
	// 精灵删除函数
	void deletePokemon(std::string pokemonName_);
	// 精灵信息发送函数
	void sendPokemon(std::string userName_);
	// 在线用户列表发送函数
	void sendOnlineUserList();
	// 所有用户列表发送函数
	void sendAllUserList();
};