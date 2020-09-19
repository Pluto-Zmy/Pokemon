#pragma once
#include "ProgramLib.h"
#include "NetworkManager.h"
#include "DataBaseManager.h"

// 系统控制类
class SystemManager {
private:
	// 生成系统精灵列表
	void generateSystemPokemonList();
public:
	// 数据库对象
	DataBaseManager DataBase;
	// 网络对象
	NetworkManager Network;
	// 已连接的客户端列表
	std::vector<ConnectedClient*> clientList;
	SystemManager();
	// 系统时间
	std::string currentTime() const;
	// 创建一个客户端
	void createClient(SOCKET sClient_, sockaddr_in remoteAddr_);
	// 获取在线用户列表
	std::vector<std::string> getOnlineUserList() const;
};