#pragma once
#include "ProgramLib.h"

// 数据库管理类
class DataBaseManager {
private:
	// 数据库指针
	sqlite3* dataBase;
public:
	// 初始化数据库
	DataBaseManager();
	// 关闭数据库
	~DataBaseManager();
	// 打开数据库文件
	void initDataBase();
	// 创建用户数据表
	void createUserTable() const;
	// 创建精灵数据表
	void createPokemonTable() const;
	// 执行创建数据表语句
	void execCreateTableSQL(std::string SQL_) const;
	// 检查用户是否存在
	bool checkUserExist(std::string userName_) const;
	// 获取用户密码
	std::string getPassWord(std::string userName_) const;
	// 插入一条用户记录
	bool insertUser(std::string userName_, std::string passWord_) const;
	// 更新用户数据
	void updateUser(std::string userName_, int fightWinNum_, int fightAllNum_) const;
	// 获取用户战斗数据
	UserFightData getUserFightData(std::string userName_) const;
	// 插入一条精灵记录
	void insertPokemon(PokemonData pokemonData_) const;
	// 更新一条精灵记录
	void updatePokemon(std::string userName_, std::string pokemonName_, PokemonData pokemonData_) const;
	// 删除一条精灵记录
	void deletePokemon(std::string userName_, std::string pokemonName_) const;
	// 获取用户所有精灵列表
	std::vector<PokemonData> getAllPokemon(std::string userName_) const;
	// 获取所有已注册用户列表
	std::vector<std::string> getAllUserList() const;
};