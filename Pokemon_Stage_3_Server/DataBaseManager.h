#pragma once
#include "ProgramLib.h"

// ���ݿ������
class DataBaseManager {
private:
	// ���ݿ�ָ��
	sqlite3* dataBase;
public:
	// ��ʼ�����ݿ�
	DataBaseManager();
	// �ر����ݿ�
	~DataBaseManager();
	// �����ݿ��ļ�
	void initDataBase();
	// �����û����ݱ�
	void createUserTable() const;
	// �����������ݱ�
	void createPokemonTable() const;
	// ִ�д������ݱ����
	void execCreateTableSQL(std::string SQL_) const;
	// ����û��Ƿ����
	bool checkUserExist(std::string userName_) const;
	// ��ȡ�û�����
	std::string getPassWord(std::string userName_) const;
	// ����һ���û���¼
	bool insertUser(std::string userName_, std::string passWord_) const;
	// �����û�����
	void updateUser(std::string userName_, int fightWinNum_, int fightAllNum_) const;
	// ��ȡ�û�ս������
	UserFightData getUserFightData(std::string userName_) const;
	// ����һ�������¼
	void insertPokemon(PokemonData pokemonData_) const;
	// ����һ�������¼
	void updatePokemon(std::string userName_, std::string pokemonName_, PokemonData pokemonData_) const;
	// ɾ��һ�������¼
	void deletePokemon(std::string userName_, std::string pokemonName_) const;
	// ��ȡ�û����о����б�
	std::vector<PokemonData> getAllPokemon(std::string userName_) const;
	// ��ȡ������ע���û��б�
	std::vector<std::string> getAllUserList() const;
};