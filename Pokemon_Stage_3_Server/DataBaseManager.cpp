#include "DataBaseManager.h"
#include "SystemManager.h"

extern SystemManager* System;

// 初始化数据库
DataBaseManager::DataBaseManager() {
	// 打开数据库文件
	initDataBase();
	// 创建用户数据表
	createUserTable();
	// 创建精灵数据表
	createPokemonTable();
	std::cout << System->currentTime() << "服务器数据库服务启动成功" << std::endl;
}

// 关闭数据库
DataBaseManager::~DataBaseManager() {
	sqlite3_close(this->dataBase);
	std::cout << System->currentTime() << "服务器数据库服务已关闭" << std::endl;
}

// 打开数据库文件
void DataBaseManager::initDataBase() {
	sqlite3_open("DataBase.db", &this->dataBase);
}

// 创建用户数据表
void DataBaseManager::createUserTable() const {
	std::string SQL = "CREATE TABLE IF NOT EXISTS User (\n\
		UserName VARCHAR(15) PRIMARY KEY NOT NULL, \n\
		PassWord VARCHAR(15) NOT NULL, \n\
		FightWinNum INTEGER NOT NULL, \n\
		FightAllNum INTEGER NOT NULL);";
	// 执行 SQL 语句
	execCreateTableSQL(SQL);
}

// 创建精灵数据表
void DataBaseManager::createPokemonTable() const {
	std::string SQL = "CREATE TABLE IF NOT EXISTS Pokemon (\n\
		Owner VARCHAR(15) NOT NULL, \n\
		Name VARCHAR(15) NOT NULL, \n\
		Kind INTEGER NOT NULL, \n\
		Level INTEGER NOT NULL, \n\
		Exp INTEGER NOT NULL, \n\
		Attack INTEGER NOT NULL, \n\
		Defense INTEGER NOT NULL, \n\
		MaxBlood INTEGER NOT NULL, \n\
		CurrentBlood INTEGER NOT NULL, \n\
		AttackSpeed INTEGER NOT NULL, \n\
		AttackMethod INTEGER NOT NULL);";
	// 执行 SQL 语句
	execCreateTableSQL(SQL);
}

// 执行创建数据表语句
void DataBaseManager::execCreateTableSQL(std::string SQL_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	// 执行 SQL 语句
	sqlite3_get_table(this->dataBase, SQL_.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	sqlite3_free_table(resultTable);
}

// 检查用户是否存在
bool DataBaseManager::checkUserExist(std::string userName_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::string SQL = "SELECT UserName FROM User WHERE UserName=\"" + userName_ + "\";";
	// 执行 SQL 语句
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	sqlite3_free_table(resultTable);
	// 存在返回 true，否则返回 false
	return (rowNum != 0) ? true : false;
}

// 获取用户密码
std::string DataBaseManager::getPassWord(std::string userName_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::string SQL = "SELECT PassWord FROM User WHERE UserName=\"" + userName_ + "\";";
	// 执行 SQL 语句
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	std::string passWord{resultTable[1]};
	sqlite3_free_table(resultTable);
	return passWord;
}

// 插入一条用户记录
bool DataBaseManager::insertUser(std::string userName_, std::string passWord_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::string SQL = "INSERT INTO User VALUES (\"" + userName_ + "\", " +
		"\"" + passWord_ + "\", " + "0, 0);";
	// 执行 SQL 语句
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	sqlite3_free_table(resultTable);
	return true;
}

// 更新用户数据
void DataBaseManager::updateUser(std::string userName_, int fightWinNum_, int fightAllNum_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::string SQL = "UPDATE User SET FightWinNum=" + std::to_string(fightWinNum_) +
		", FightAllNum=" + std::to_string(fightAllNum_) +
		" WHERE UserName=\"" + userName_ + "\";";
	// 执行 SQL 语句
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	sqlite3_free_table(resultTable);
}

// 获取用户战斗数据
UserFightData DataBaseManager::getUserFightData(std::string userName_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::string SQL = "SELECT FightWinNum, FightAllNum FROM User WHERE UserName=\"" + userName_ + "\";";
	// 执行 SQL 语句
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	UserFightData userFightData{atoi(resultTable[2]), atoi(resultTable[3])};
	sqlite3_free_table(resultTable);
	return userFightData;
}

// 插入一条精灵记录
void DataBaseManager::insertPokemon(PokemonData pokemonData_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::string SQL = "INSERT INTO Pokemon VALUES (\"" +
		pokemonData_.ownerName + "\", " +
		"\"" + pokemonData_.name + "\", " +
		std::to_string(pokemonData_.kind) + ", " +
		std::to_string(pokemonData_.level) + ", " +
		std::to_string(pokemonData_.exp) + ", " +
		std::to_string(pokemonData_.attack) + ", " +
		std::to_string(pokemonData_.defense) + ", " +
		std::to_string(pokemonData_.maxBlood) + ", " +
		std::to_string(pokemonData_.currentBlood) + ", " +
		std::to_string(pokemonData_.attackSpeed) + ", " +
		std::to_string(pokemonData_.attackMethod) + ");";
	// 执行 SQL 语句
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	sqlite3_free_table(resultTable);
}

// 更新一条精灵记录
void DataBaseManager::updatePokemon(std::string userName_, std::string pokemonName_, PokemonData pokemonData_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::string SQL = "UPDATE Pokemon SET Name=\"" + pokemonData_.name + "\"" +
		", Kind=" + std::to_string(pokemonData_.kind) +
		", Level=" + std::to_string(pokemonData_.level) +
		", Exp=" + std::to_string(pokemonData_.exp) +
		", Attack=" + std::to_string(pokemonData_.attack) +
		", Defense=" + std::to_string(pokemonData_.defense) +
		", MaxBlood=" + std::to_string(pokemonData_.maxBlood) +
		", CurrentBlood=" + std::to_string(pokemonData_.currentBlood) +
		", AttackSpeed=" + std::to_string(pokemonData_.attackSpeed) +
		", AttackMethod=" + std::to_string(pokemonData_.attackMethod) +
		" WHERE Owner=\"" + userName_ + "\" AND Name=\"" + pokemonName_ + "\";";
	// 执行 SQL 语句
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	sqlite3_free_table(resultTable);
}

// 删除一条精灵记录
void DataBaseManager::deletePokemon(std::string userName_, std::string pokemonName_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::string SQL = "DELETE FROM Pokemon WHERE Owner=\"" + userName_ + "\" AND Name=\"" + pokemonName_ + "\";";
	// 执行 SQL 语句
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	sqlite3_free_table(resultTable);
}

// 获取用户所有精灵列表
std::vector<PokemonData> DataBaseManager::getAllPokemon(std::string userName_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::vector<PokemonData> pokemonList;
	std::string SQL = "SELECT * FROM Pokemon WHERE Owner=\"" + userName_ + "\";";
	// 执行 SQL 语句
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	char** dataTable = resultTable + colNum;
	for (int row{0}; row < rowNum; row++) {
		// colNum*row+i
		PokemonData newData{dataTable[colNum * row], dataTable[colNum * row + 1],
			atoi(dataTable[colNum * row + 2]), atoi(dataTable[colNum * row + 3]),
			atoi(dataTable[colNum * row + 4]), atoi(dataTable[colNum * row + 5]),
			atoi(dataTable[colNum * row + 6]), atoi(dataTable[colNum * row + 7]),
			atoi(dataTable[colNum * row + 8]), atoi(dataTable[colNum * row + 9]),
			atoi(dataTable[colNum * row + 10])};
		pokemonList.push_back(newData);
	}
	sqlite3_free_table(resultTable);
	return pokemonList;
}

// 获取所有已注册用户列表
std::vector<std::string> DataBaseManager::getAllUserList() const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::vector<std::string> userList;
	std::string SQL = "SELECT UserName FROM User;";
	// 执行 SQL 语句
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	char** dataTable = resultTable + colNum;
	for (int row{0}; row < rowNum; row++) {
		userList.push_back(std::string{dataTable[row]});
	}
	sqlite3_free_table(resultTable);
	return userList;
}