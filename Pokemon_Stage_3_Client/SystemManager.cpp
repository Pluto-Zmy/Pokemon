#include "SystemManager.h"

// 系统初始化
SystemManager::SystemManager(std::string IP_)
	: pokemonKindStr{"力量型", "肉盾型", "防御型", "敏捷型"},
	attackMethodStr{"闪电攻击", "火焰攻击", "吸血攻击", "快速攻击", "普通攻击", "超级暴击", "闪避"},
	Network(IP_), systemPokemonList(getPokemonList("System")) {
	srand(time(nullptr));
}

// 系统关闭
SystemManager::~SystemManager() {
	// 清除用户信息
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

// 注册请求函数
ReplyType SystemManager::signIn(std::string userName_, std::string passWord_) {
	// 发送注册请求
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = SIGN_IN;
	// 请求中携带用户名、密码
	memcpy((sendMsgBuffer + 1), userName_.data(), STRING_SIZE);
	memcpy((sendMsgBuffer + 17), passWord_.data(), STRING_SIZE);
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// 接收请求回应
	char recvMsgBuffer[BUFFER_SIZE]{};
	Network.recvData(recvMsgBuffer);
	return static_cast<ReplyType>(*recvMsgBuffer);
}

// 登录请求函数
ReplyType SystemManager::logIn(std::string userName_, std::string passWord_) {
	// 发送登录请求
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = LOG_IN;
	// 请求中携带用户名、密码
	memcpy((sendMsgBuffer + 1), userName_.data(), STRING_SIZE);
	memcpy((sendMsgBuffer + 17), passWord_.data(), STRING_SIZE);
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// 接收请求回应
	char recvMsgBuffer[BUFFER_SIZE]{};
	Network.recvData(recvMsgBuffer);
	// 如果登录成功、载入战斗信息
	if (*((char*) (recvMsgBuffer)) == LOG_IN_SUCCESS) {
		memcpy(&fightWinNum, recvMsgBuffer + 1, sizeof(int));
		memcpy(&fightAllNum, recvMsgBuffer + 5, sizeof(int));
	}
	return static_cast<ReplyType>(*recvMsgBuffer);
}

// 退出登录请求函数
void SystemManager::logOut() {
	// 发送退出登录请求
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = LOG_OUT;
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// 清空当前用户数据
	this->userName.clear();
	this->fightAllNum = 0;
	this->fightWinNum = 0;
	for (auto pokemon : pokemonList) {
		delete pokemon;
	}
	this->pokemonList.clear();
}

// 获取战斗信息函数
UserFightData SystemManager::getFightData(std::string userName_) {
	// 发送获取战斗信息请求
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = GET_FIGHT_DATA;
	memcpy(sendMsgBuffer + 1, userName_.data(), STRING_SIZE);
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// 接收回应数据
	char recvMsgBuffer[BUFFER_SIZE]{};
	Network.recvData(recvMsgBuffer);
	UserFightData fightData;
	memcpy(&fightData.fightWinNum, recvMsgBuffer, sizeof(int));
	memcpy(&fightData.fightAllNum, recvMsgBuffer + 4, sizeof(int));
	return fightData;
}

// 更新战斗信息函数
void SystemManager::updateFightData() {
	// 发送更新战斗信息请求
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = UPDATE_FIGHT_DATA;
	// 携带战斗信息
	memcpy((sendMsgBuffer + 1), &this->fightWinNum, sizeof(int));
	memcpy((sendMsgBuffer + 5), &this->fightAllNum, sizeof(int));
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
}

// 随机生成一个精灵
Pokemon* SystemManager::generateRandPokemon(std::string name_) const {
	// 随机数指定精灵种类
	PokemonKind pokemonKind = static_cast<PokemonKind>(rand() % 4);
	Pokemon* newPokemon = nullptr;
	// 创建相应的精灵
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

// 精灵创建函数
void SystemManager::createPokemon(Pokemon* pokemon_) {
	// 发送精灵创建请求
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
	// 携带精灵属性数据
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
	// 在本地增加精灵记录
	pokemonList.push_back(pokemon_);
}

// 精灵信息更新函数
void SystemManager::updatePokemon(std::string oldPokemonName_, Pokemon* pokemon_) {
	// 发送精灵信息更新请求
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
	// 携带精灵属性数据
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
	// 发送精灵删除请求
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = DELETE_POKEMON;
	memcpy(sendMsgBuffer + 1, pokemonName_.data(), STRING_SIZE);
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// 删除本地精灵
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

// 获取指定用户的精灵列表
std::vector<Pokemon*> SystemManager::getPokemonList(std::string userName_) {
	// 发送精灵获取请求
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = GET_POKEMON;
	memcpy(sendMsgBuffer + 1, userName_.data(), STRING_SIZE);
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// 接收即将接收的精灵数目
	int pokemonNum{};
	char recvNumMsgBuffer[BUFFER_SIZE]{};
	Network.recvData(recvNumMsgBuffer);
	memcpy(&pokemonNum, recvNumMsgBuffer, sizeof(int));
	std::vector<Pokemon*> pokemonList;
	// 依次接收精灵信息
	for (int i{0}; i < pokemonNum; i++) {
		// 接收精灵信息
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
		// 根据精灵类型创建精灵
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

// 获取在线用户列表
std::vector<std::string> SystemManager::getOnlineUserList() {
	// 发送获取在线用户列表请求
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = GET_ONLINE_USER;
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// 接收即将接收的用户数目
	char recvNumMsgBuffer[BUFFER_SIZE]{};
	int onlineUserNum{};
	Network.recvData(recvNumMsgBuffer);
	memcpy(&onlineUserNum, recvNumMsgBuffer, sizeof(int));
	std::vector<std::string> onlineUserList;
	// 依次接收用户名
	for (int i{0}; i < onlineUserNum; i++) {
		char recvDataMsgBuffer[BUFFER_SIZE]{};
		Network.recvData(recvDataMsgBuffer);
		std::string userName{recvDataMsgBuffer};
		onlineUserList.push_back(userName);
	}
	return onlineUserList;
}

// 获取所有用户列表
std::vector<std::string> SystemManager::getAllUserList() {
	// 发送获取所有用户列表请求
	char sendMsgBuffer[BUFFER_SIZE]{};
	*sendMsgBuffer = GET_ALL_USER;
	Network.sendData(sendMsgBuffer, sizeof(sendMsgBuffer));
	// 接收即将接收的用户数目
	char recvNumMsgBuffer[BUFFER_SIZE]{};
	int allUserNum{};
	Network.recvData(recvNumMsgBuffer);
	memcpy(&allUserNum, recvNumMsgBuffer, sizeof(int));
	std::vector<std::string> allUserList;
	// 依次接收用户名
	for (int i{0}; i < allUserNum; i++) {
		char recvDataMsgBuffer[BUFFER_SIZE]{};
		Network.recvData(recvDataMsgBuffer);
		std::string userName{recvDataMsgBuffer};
		allUserList.push_back(userName);
	}
	return allUserList;
}