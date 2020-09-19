#pragma once
#include "ProgramLib.h"
#include "ui_Pokemon_Stage_3_Client.h"
#include "ui_SelectPokemon.h"
#include "Pokemon.h"
#include "Fight.h"

// 游戏主窗口
class Pokemon_Stage_3_Client: public QMainWindow {
	Q_OBJECT
public:
	// 构造、析构函数
	Pokemon_Stage_3_Client(QWidget* parent = Q_NULLPTR);
	~Pokemon_Stage_3_Client();
	// 根据精灵类型选择头像
	static QMovie* selectImg(PokemonKind kind_, QString sysStr_ = "");
	// 选择徽章
	QPixmap selectPokemonNumMedal(int pokemonNum_);
	QPixmap selectAdvancePokemonMedal(std::vector<Pokemon*> pokemonList_);
private:
	Ui::Pokemon_Stage_3_ClientClass ui;
	// 当前用户名
	std::string currentUserName;
	// 当前精灵列表
	std::vector<Pokemon*> currentPokemonList;
	// 当前精灵头像
	QMovie* currentImg;
private slots:
	// 刷新用户列表
	void refreshUserList();
	// 刷新用户精灵列表
	void refreshUserPokemonList();
	// 刷新用户精灵数据
	void refreshUserPokemonData();
	// 刷新系统精灵列表
	void refreshSystemPokemonList();
	// 刷新系统精灵信息
	void refreshSystemPokemonData();
	// 响应升级赛按钮点击信号
	void onPressUpGradeFight();
	// 响应决斗赛按钮点击信号
	void onPressDuelFight();
	// 刷新我的精灵列表
	void refreshMyPokemonList();
	// 刷新我的精灵数据
	void refreshMyPokemonData();
	// 响应退出登录按钮点击事件
	void onPressLogOut();
	// 响应精灵改名按钮点击事件
	void onPressChangePokemonName();
	// 响应恢复按钮点击事件
	void onPressRecover();
};

// 选择出战精灵窗口
class SelectPokemon: public QWidget {
	Q_OBJECT
public:
	// 构造、析构函数
	SelectPokemon(FightType fightType_, Pokemon* systemPokemon_, QWidget* parent = Q_NULLPTR);
	~SelectPokemon();
private:
	Ui::SelectPokemon ui;
	// 当前精灵图片
	QMovie* currentImg;
	// 挑战模式
	FightType fightType;
	// 对方精灵
	Pokemon* systemPokemon;
private slots:
	// 刷新精灵信息
	void refreshPokemonInfo();
	// 响应开战按钮点击信号
	void onPressFight();
};