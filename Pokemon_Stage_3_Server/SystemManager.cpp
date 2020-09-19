#include "SystemManager.h"
#include "ConnectedClient.h"

SystemManager::SystemManager() {
	generateSystemPokemonList();
	std::cout << currentTime() << "����С�����������ʼ����..." << std::endl;
}

std::string SystemManager::currentTime() const {
	time_t nowTime = time(0);
	char* timeStr = ctime(&nowTime);
	return ("> " + std::string{timeStr}.substr(0, strlen(timeStr) - 1) + " ������ ");
}

// ����һ���ͻ���
void SystemManager::createClient(SOCKET sClient_, sockaddr_in remoteAddr_) {
	// Ϊ�ͻ��˶��������ڴ�
	ConnectedClient* newCilent = new ConnectedClient(sClient_, remoteAddr_, clientList.size());
	// �����뵽�Ŀͻ��˶���洢���ͻ����б�
	clientList.push_back(newCilent);
}

// ��ȡ�����û��б�
std::vector<std::string> SystemManager::getOnlineUserList() const {
	// ����б�
	std::vector<std::string> onlineUserList;
	// �����ͻ����б�
	for (auto user : this->clientList) {
		// �������о��л�Ծ��ǵĿͻ���
		if (user->getState() == true) {
			// �洢������б�
			onlineUserList.push_back(user->getUserName());
		}
	}
	return onlineUserList;
}

// ����ϵͳ�����б�
void SystemManager::generateSystemPokemonList() {
	srand(time(nullptr));
	// ������ݿ���û��ϵͳ�����¼��������ϵͳ�����б�
	if (DataBase.getAllPokemon("System").empty()) {
		std::cout << currentTime() << "��Ϸ��������������������׼�����ݡ����Ժ�Ƭ��..." << std::endl;
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