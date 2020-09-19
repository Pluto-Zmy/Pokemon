#pragma once
#include "ProgramLib.h"
#include "NetworkManager.h"
#include "Pokemon.h"

// 系统控制类/已登录用户类
class SystemManager {
private:
	// 网络对象
	NetworkManager Network;
	// 用户名
	std::string userName;
	// 战斗信息
	int fightWinNum;
	int fightAllNum;
public:
	// 精灵列表
	std::vector<Pokemon*> pokemonList;
	std::vector<Pokemon*> systemPokemonList;
	// 精灵文字描述信息
	std::vector<std::string> pokemonKindStr;
	std::vector<std::string> attackMethodStr;
	// 系统初始化
	SystemManager(std::string IP_);
	// 系统关闭
	~SystemManager();
	// 成员存取函数
	void setUserName(std::string userName_);
	void incFightWinNum();
	void incFightAllNum();
	std::string getUserName() const;
	int getFightWinNum() const;
	int getFightAllNum() const;
	// 注册请求函数
	ReplyType signIn(std::string userName_, std::string passWord_);
	// 登录请求函数
	ReplyType logIn(std::string userName_, std::string passWord_);
	// 退出登录请求函数
	void logOut();
	// 获取战斗信息函数
	UserFightData getFightData(std::string userName_);
	// 更新战斗信息函数
	void updateFightData();
	// 精灵随机生成函数
	Pokemon* generateRandPokemon(std::string name_) const;
	// 精灵创建函数
	void createPokemon(Pokemon* pokemon_);
	// 精灵信息更新函数
	void updatePokemon(std::string OldPokemonName_, Pokemon* pokemon_);
	// 精灵删除函数
	void deletePokemon(std::string pokemonName_);
	// 获取指定用户的精灵列表
	std::vector<Pokemon*> getPokemonList(std::string userName_);
	// 获取在线用户列表
	std::vector<std::string> getOnlineUserList();
	// 获取所有用户列表
	std::vector<std::string> getAllUserList();
};