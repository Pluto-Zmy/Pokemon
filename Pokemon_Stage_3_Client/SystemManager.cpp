#include "SystemManager.h"

// ϵͳ��ʼ��
SystemManager::SystemManager(std::string IP_)
	: pokemonKindStr{"������", "�����", "������", "������"},
	attackMethodStr{"���繥��", "���湥��", "��Ѫ����", "���ٹ���", "��ͨ����", "��������", "����"},
	Network(IP_), systemPokemonList(getPokemonList("System")) {
	srand(time(nullptr));
}

// ϵͳ�ر�
SystemManager::~SystemManager() {
	// ����û���Ϣ
	for (auto pokemon : this->pokemonList) {
		delete pokemon;
	}
	pokemonList.clear();
}

void SystemManager::setUserName(std::string userName_) {
	this->userName = userName_;
}

void SystemManager::incFightWinNum() {
	this->fightWinNum++;
}

void SystemManager::incFightAllNum() {
	this->fightAllNum++;
}

std::string SystemManager::getUserName() const {
	return this->userName;
}

int SystemManager::getFightWinNum() const {
	return this->fightWinNum;
}

int SystemManager::getFightAllNum() const {
	return this->fightAllNum;
}

// ע��������
ReplyType SystemManager::signIn(std::string userName_, std::string passWord_) {
	// ����ע������
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = SIGN_IN;
	// ������Я���û���������
	memcpy((sendMsgBuffer + 1), userName_.data(), STRING_SIZE);
	memcpy((sendMsgBuffer + 17), passWord_.data(), STRING_SIZE);
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// ���������Ӧ
	char recvMsgBuffer[BUFFER_SIZE]{};
	Network.recvData(recvMsgBuffer);
	return static_cast<ReplyType>(*recvMsgBuffer);
}

// ��¼������
ReplyType SystemManager::logIn(std::string userName_, std::string passWord_) {
	// ���͵�¼����
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = LOG_IN;
	// ������Я���û���������
	memcpy((sendMsgBuffer + 1), userName_.data(), STRING_SIZE);
	memcpy((sendMsgBuffer + 17), passWord_.data(), STRING_SIZE);
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// ���������Ӧ
	char recvMsgBuffer[BUFFER_SIZE]{};
	Network.recvData(recvMsgBuffer);
	// �����¼�ɹ�������ս����Ϣ
	if (*((char*) (recvMsgBuffer)) == LOG_IN_SUCCESS) {
		memcpy(&fightWinNum, recvMsgBuffer + 1, sizeof(int));
		memcpy(&fightAllNum, recvMsgBuffer + 5, sizeof(int));
	}
	return static_cast<ReplyType>(*recvMsgBuffer);
}

// �˳���¼������
void SystemManager::logOut() {
	// �����˳���¼����
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = LOG_OUT;
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// ��յ�ǰ�û�����
	this->userName.clear();
	this->fightAllNum = 0;
	this->fightWinNum = 0;
	for (auto pokemon : pokemonList) {
		delete pokemon;
	}
	this->pokemonList.clear();
}

// ��ȡս����Ϣ����
UserFightData SystemManager::getFightData(std::string userName_) {
	// ���ͻ�ȡս����Ϣ����
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = GET_FIGHT_DATA;
	memcpy(sendMsgBuffer + 1, userName_.data(), STRING_SIZE);
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// ���ջ�Ӧ����
	char recvMsgBuffer[BUFFER_SIZE]{};
	Network.recvData(recvMsgBuffer);
	UserFightData fightData;
	memcpy(&fightData.fightWinNum, recvMsgBuffer, sizeof(int));
	memcpy(&fightData.fightAllNum, recvMsgBuffer + 4, sizeof(int));
	return fightData;
}

// ����ս����Ϣ����
void SystemManager::updateFightData() {
	// ���͸���ս����Ϣ����
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = UPDATE_FIGHT_DATA;
	// Я��ս����Ϣ
	memcpy((sendMsgBuffer + 1), &this->fightWinNum, sizeof(int));
	memcpy((sendMsgBuffer + 5), &this->fightAllNum, sizeof(int));
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
}

// �������һ������
Pokemon* SystemManager::generateRandPokemon(std::string name_) const {
	// �����ָ����������
	PokemonKind pokemonKind = static_cast<PokemonKind>(rand() % 4);
	Pokemon* newPokemon = nullptr;
	// ������Ӧ�ľ���
	switch (pokemonKind) {
		case HIGH_ATTACK:
			newPokemon = new HighAttack(name_);
			break;
		case HIGH_BLOOD:
			newPokemon = new HighBlood(name_);
			break;
		case HIGH_DEFENSE:
			newPokemon = new HighDefense(name_);
			break;
		case HIGH_SPEED:
			newPokemon = new HighSpeed(name_);
			break;
	}
	return newPokemon;
}

// ���鴴������
void SystemManager::createPokemon(Pokemon* pokemon_) {
	// ���;��鴴������
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = CREATE_POKEMON;
	std::string name = pokemon_->getName();
	PokemonKind kind = pokemon_->getKind();
	int level = pokemon_->getLevel();
	int exp = pokemon_->getExp();
	int attack = pokemon_->getAttack();
	int defense = pokemon_->getDefense();
	int maxBlood = pokemon_->getMaxBlood();
	int currentBlood = pokemon_->getCurrentBlood();
	int attackSpeed = pokemon_->getAttackSpeed();
	AttackMethod attackMethod = pokemon_->getAttackMethod();
	// Я��������������
	memcpy(sendMsgBuffer + 1, userName.data(), STRING_SIZE);
	memcpy(sendMsgBuffer + 17, name.data(), STRING_SIZE);
	memcpy(sendMsgBuffer + 33, &kind, sizeof(int));
	memcpy(sendMsgBuffer + 37, &level, sizeof(int));
	memcpy(sendMsgBuffer + 41, &exp, sizeof(int));
	memcpy(sendMsgBuffer + 45, &attack, sizeof(int));
	memcpy(sendMsgBuffer + 49, &defense, sizeof(int));
	memcpy(sendMsgBuffer + 53, &maxBlood, sizeof(int));
	memcpy(sendMsgBuffer + 57, &currentBlood, sizeof(int));
	memcpy(sendMsgBuffer + 61, &attackSpeed, sizeof(int));
	memcpy(sendMsgBuffer + 65, &attackMethod, sizeof(int));
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// �ڱ������Ӿ����¼
	pokemonList.push_back(pokemon_);
}

// ������Ϣ���º���
void SystemManager::updatePokemon(std::string oldPokemonName_, Pokemon* pokemon_) {
	// ���;�����Ϣ��������
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = UPDATE_POKEMON;
	std::string name = pokemon_->getName();
	PokemonKind kind = pokemon_->getKind();
	int level = pokemon_->getLevel();
	int exp = pokemon_->getExp();
	int attack = pokemon_->getAttack();
	int defense = pokemon_->getDefense();
	int maxBlood = pokemon_->getMaxBlood();
	int currentBlood = pokemon_->getCurrentBlood();
	int attackSpeed = pokemon_->getAttackSpeed();
	AttackMethod attackMethod = pokemon_->getAttackMethod();
	// Я��������������
	memcpy(sendMsgBuffer + 1, oldPokemonName_.data(), STRING_SIZE);
	memcpy(sendMsgBuffer + 17, userName.data(), STRING_SIZE);
	memcpy(sendMsgBuffer + 33, name.data(), STRING_SIZE);
	memcpy(sendMsgBuffer + 49, &kind, sizeof(int));
	memcpy(sendMsgBuffer + 53, &level, sizeof(int));
	memcpy(sendMsgBuffer + 57, &exp, sizeof(int));
	memcpy(sendMsgBuffer + 61, &attack, sizeof(int));
	memcpy(sendMsgBuffer + 65, &defense, sizeof(int));
	memcpy(sendMsgBuffer + 69, &maxBlood, sizeof(int));
	memcpy(sendMsgBuffer + 73, &currentBlood, sizeof(int));
	memcpy(sendMsgBuffer + 77, &attackSpeed, sizeof(int));
	memcpy(sendMsgBuffer + 81, &attackMethod, sizeof(int));
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
}

void SystemManager::deletePokemon(std::string pokemonName_) {
	// ���;���ɾ������
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = DELETE_POKEMON;
	memcpy(sendMsgBuffer + 1, pokemonName_.data(), STRING_SIZE);
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// ɾ�����ؾ���
	auto pokemonIter = pokemonList.begin();
	while (pokemonIter != pokemonList.end()) {
		if ((*pokemonIter)->getName() == pokemonName_) {
			delete (*pokemonIter);
			pokemonList.erase(pokemonIter);
			break;
		}
		else {
			pokemonIter++;
		}
	}
}

// ��ȡָ���û��ľ����б�
std::vector<Pokemon*> SystemManager::getPokemonList(std::string userName_) {
	// ���;����ȡ����
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = GET_POKEMON;
	memcpy(sendMsgBuffer + 1, userName_.data(), STRING_SIZE);
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// ���ռ������յľ�����Ŀ
	int pokemonNum{};
	char recvNumMsgBuffer[BUFFER_SIZE]{};
	Network.recvData(recvNumMsgBuffer);
	memcpy(&pokemonNum, recvNumMsgBuffer, sizeof(int));
	std::vector<Pokemon*> pokemonList;
	// ���ν��վ�����Ϣ
	for (int i{0}; i < pokemonNum; i++) {
		// ���վ�����Ϣ
		char recvDataMsgBuffer[BUFFER_SIZE]{};
		Network.recvData(recvDataMsgBuffer);
		char name[STRING_SIZE]{};
		memcpy(name, recvDataMsgBuffer + 16, STRING_SIZE);
		PokemonKind kind = static_cast<PokemonKind>(*((int*) (recvDataMsgBuffer + 32)));
		int level = *((int*) (recvDataMsgBuffer + 36));
		int exp = *((int*) (recvDataMsgBuffer + 40));
		int attack = *((int*) (recvDataMsgBuffer + 44));
		int defense = *((int*) (recvDataMsgBuffer + 48));
		int maxBlood = *((int*) (recvDataMsgBuffer + 52));
		int currentBlood = *((int*) (recvDataMsgBuffer + 56));
		int attackSpeed = *((int*) (recvDataMsgBuffer + 60));
		AttackMethod attackMethod = static_cast<AttackMethod>(*((int*) (recvDataMsgBuffer + 64)));
		Pokemon* newPokemon{nullptr};
		// ���ݾ������ʹ�������
		switch (*((int*) (recvDataMsgBuffer + 32))) {
			case HIGH_ATTACK:
				newPokemon = new HighAttack(name, level, exp, attack, defense, maxBlood, currentBlood, attackSpeed);
				break;
			case HIGH_BLOOD:
				newPokemon = new HighBlood(name, level, exp, attack, defense, maxBlood, currentBlood, attackSpeed);
				break;
			case HIGH_DEFENSE:
				newPokemon = new HighDefense(name, level, exp, attack, defense, maxBlood, currentBlood, attackSpeed);
				break;
			case HIGH_SPEED:
				newPokemon = new HighSpeed(name, level, exp, attack, defense, maxBlood, currentBlood, attackSpeed);
				break;
		}
		pokemonList.push_back(newPokemon);
	}
	return pokemonList;
}

// ��ȡ�����û��б�
std::vector<std::string> SystemManager::getOnlineUserList() {
	// ���ͻ�ȡ�����û��б�����
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = GET_ONLINE_USER;
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// ���ռ������յ��û���Ŀ
	char recvNumMsgBuffer[BUFFER_SIZE]{};
	int onlineUserNum{};
	Network.recvData(recvNumMsgBuffer);
	memcpy(&onlineUserNum, recvNumMsgBuffer, sizeof(int));
	std::vector<std::string> onlineUserList;
	// ���ν����û���
	for (int i{0}; i < onlineUserNum; i++) {
		char recvDataMsgBuffer[BUFFER_SIZE]{};
		Network.recvData(recvDataMsgBuffer);
		std::string userName{recvDataMsgBuffer};
		onlineUserList.push_back(userName);
	}
	return onlineUserList;
}

// ��ȡ�����û��б�
std::vector<std::string> SystemManager::getAllUserList() {
	// ���ͻ�ȡ�����û��б�����
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = GET_ALL_USER;
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// ���ռ������յ��û���Ŀ
	char recvNumMsgBuffer[BUFFER_SIZE]{};
	int allUserNum{};
	Network.recvData(recvNumMsgBuffer);
	memcpy(&allUserNum, recvNumMsgBuffer, sizeof(int));
	std::vector<std::string> allUserList;
	// ���ν����û���
	for (int i{0}; i < allUserNum; i++) {
		char recvDataMsgBuffer[BUFFER_SIZE]{};
		Network.recvData(recvDataMsgBuffer);
		std::string userName{recvDataMsgBuffer};
		allUserList.push_back(userName);
	}
	return allUserList;
}