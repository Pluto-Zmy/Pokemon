#include "DataBaseManager.h"
#include "SystemManager.h"

extern SystemManager* System;

// ��ʼ�����ݿ�
DataBaseManager::DataBaseManager() {
	// �����ݿ��ļ�
	initDataBase();
	// �����û����ݱ�
	createUserTable();
	// �����������ݱ�
	createPokemonTable();
	std::cout << System->currentTime() << "���������ݿ���������ɹ�" << std::endl;
}

// �ر����ݿ�
DataBaseManager::~DataBaseManager() {
	sqlite3_close(this->dataBase);
	std::cout << System->currentTime() << "���������ݿ�����ѹر�" << std::endl;
}

// �����ݿ��ļ�
void DataBaseManager::initDataBase() {
	sqlite3_open("DataBase.db", &this->dataBase);
}

// �����û����ݱ�
void DataBaseManager::createUserTable() const {
	std::string SQL = "CREATE TABLE IF NOT EXISTS User (\n\
		UserName VARCHAR(15) PRIMARY KEY NOT NULL, \n\
		PassWord VARCHAR(15) NOT NULL, \n\
		FightWinNum INTEGER NOT NULL, \n\
		FightAllNum INTEGER NOT NULL);";
	// ִ�� SQL ���
	execCreateTableSQL(SQL);
}

// �����������ݱ�
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
	// ִ�� SQL ���
	execCreateTableSQL(SQL);
}

// ִ�д������ݱ����
void DataBaseManager::execCreateTableSQL(std::string SQL_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	// ִ�� SQL ���
	sqlite3_get_table(this->dataBase, SQL_.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	sqlite3_free_table(resultTable);
}

// ����û��Ƿ����
bool DataBaseManager::checkUserExist(std::string userName_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::string SQL = "SELECT UserName FROM User WHERE UserName=\"" + userName_ + "\";";
	// ִ�� SQL ���
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	sqlite3_free_table(resultTable);
	// ���ڷ��� true�����򷵻� false
	return (rowNum != 0) ? true : false;
}

// ��ȡ�û�����
std::string DataBaseManager::getPassWord(std::string userName_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::string SQL = "SELECT PassWord FROM User WHERE UserName=\"" + userName_ + "\";";
	// ִ�� SQL ���
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	std::string passWord{resultTable[1]};
	sqlite3_free_table(resultTable);
	return passWord;
}

// ����һ���û���¼
bool DataBaseManager::insertUser(std::string userName_, std::string passWord_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::string SQL = "INSERT INTO User VALUES (\"" + userName_ + "\", " +
		"\"" + passWord_ + "\", " + "0, 0);";
	// ִ�� SQL ���
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	sqlite3_free_table(resultTable);
	return true;
}

// �����û�����
void DataBaseManager::updateUser(std::string userName_, int fightWinNum_, int fightAllNum_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::string SQL = "UPDATE User SET FightWinNum=" + std::to_string(fightWinNum_) +
		", FightAllNum=" + std::to_string(fightAllNum_) +
		" WHERE UserName=\"" + userName_ + "\";";
	// ִ�� SQL ���
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	sqlite3_free_table(resultTable);
}

// ��ȡ�û�ս������
UserFightData DataBaseManager::getUserFightData(std::string userName_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::string SQL = "SELECT FightWinNum, FightAllNum FROM User WHERE UserName=\"" + userName_ + "\";";
	// ִ�� SQL ���
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	UserFightData userFightData{atoi(resultTable[2]), atoi(resultTable[3])};
	sqlite3_free_table(resultTable);
	return userFightData;
}

// ����һ�������¼
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
	// ִ�� SQL ���
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	sqlite3_free_table(resultTable);
}

// ����һ�������¼
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
	// ִ�� SQL ���
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	sqlite3_free_table(resultTable);
}

// ɾ��һ�������¼
void DataBaseManager::deletePokemon(std::string userName_, std::string pokemonName_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::string SQL = "DELETE FROM Pokemon WHERE Owner=\"" + userName_ + "\" AND Name=\"" + pokemonName_ + "\";";
	// ִ�� SQL ���
	sqlite3_get_table(this->dataBase, SQL.data(), &resultTable, &rowNum, &colNum, &errMsg);
	if (errMsg) {
		std::cout << System->currentTime() << errMsg << std::endl;
	}
	sqlite3_free_table(resultTable);
}

// ��ȡ�û����о����б�
std::vector<PokemonData> DataBaseManager::getAllPokemon(std::string userName_) const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::vector<PokemonData> pokemonList;
	std::string SQL = "SELECT * FROM Pokemon WHERE Owner=\"" + userName_ + "\";";
	// ִ�� SQL ���
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

// ��ȡ������ע���û��б�
std::vector<std::string> DataBaseManager::getAllUserList() const {
	int colNum, rowNum;
	char** resultTable;
	char* errMsg = nullptr;
	std::vector<std::string> userList;
	std::string SQL = "SELECT UserName FROM User;";
	// ִ�� SQL ���
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