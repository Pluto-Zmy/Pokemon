#include "SystemManager.h"
#include "ConnectedClient.h"

SystemManager::SystemManager() {
	generateSystemPokemonList();
	std::cout << currentTime() << "宠物小精灵服务器开始运行..." << std::endl;
}

std::string SystemManager::currentTime() const {
	time_t nowTime = time(0);
	char* timeStr = ctime(&nowTime);
	return ("> " + std::string{timeStr}.substr(0, strlen(timeStr) - 1) + " ··· ");
}

// 创建一个客户端
void SystemManager::createClient(SOCKET sClient_, sockaddr_in remoteAddr_) {
	// 为客户端对象申请内存
	ConnectedClient* newCilent = new ConnectedClient(sClient_, remoteAddr_, clientList.size());
	// 将申请到的客户端对象存储到客户端列表
	clientList.push_back(newCilent);
}

// 获取在线用户列表
std::vector<std::string> SystemManager::getOnlineUserList() const {
	// 结果列表
	std::vector<std::string> onlineUserList;
	// 遍历客户端列表
	for (auto user : this->clientList) {
		// 搜索所有具有活跃标记的客户端
		if (user->getState() == true) {
			// 存储到结果列表
			onlineUserList.push_back(user->getUserName());
		}
	}
	return onlineUserList;
}

// 生成系统精灵列表
void SystemManager::generateSystemPokemonList() {
	srand(time(nullptr));
	// 如果数据库中没有系统精灵记录，则生成系统精灵列表
	if (DataBase.getAllPokemon("System").empty()) {
		std::cout << currentTime() << "游戏服务器初次启动，正在准备数据。请稍候片刻..." << std::endl;
		PokemonData* pokemon{nullptr};
		std::string ownerName{"System"};
		for (int i{0}; i < 100; i++) {
			std::string name{ownerName + std::to_string(i)};
			int kind = rand() % 4;
			int attackMethod = kind;
			int exp = rand() % 1500 + 100;
			int level = exp / 100;
			switch (kind) {
				case 0:
					pokemon = new PokemonData{ownerName, name, kind, level, exp,
						100 + 150 * (level - 1), 100 * level, 100 * level, 100 * level, 100 + 20 * (level - 1), attackMethod};
					break;
				case 1:
					pokemon = new PokemonData{ownerName, name, kind, level, exp,
						100 * level, 100 * level, 100 + 150 * (level - 1), 100 + 150 * (level - 1), 100 + 20 * (level - 1), attackMethod};
					break;
				case 2:
					pokemon = new PokemonData{ownerName, name, kind, level, exp,
						100 * level, 100 + 150 * (level - 1), 100 * level, 100 * level, 100 + 20 * (level - 1), attackMethod};
					break;
				case 3:
					pokemon = new PokemonData{ownerName, name, kind, level, exp,
						100 * level, 100 * level, 100 * level, 100 * level, 100 + 40 * (level - 1), attackMethod};
					break;
				default:
					break;
			}
			if (pokemon) {
				DataBase.insertPokemon(*pokemon);
				delete pokemon;
			}
		}
	}
}