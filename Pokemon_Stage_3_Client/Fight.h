#pragma once
#include "ProgramLib.h"
#include "ui_Fight.h"
#include "ui_SendOutPokemon.h"
#include "Pokemon.h"

// 战斗场景
class Fight: public QWidget {
	Q_OBJECT
public:
	Fight(FightType fightType_, Pokemon* systemPokemon_, Pokemon* myPokemon_, QWidget* parent = Q_NULLPTR);
	~Fight();
private:
	Ui::Fight ui;
	// 战斗类型
	FightType fightType;
	// 双方精灵
	Pokemon* systemPokemon;
	Pokemon* myPokemon;
	// 双方精灵图片
	QMovie* systemImg;
	QMovie* myImg;
	// 双方攻击定时器
	QTimer* systemAttackTimer;
	QTimer* myAttackTimer;
	// 背景音效
	QSound fightBGM;
	// 回合
	int attackTime;
	// 开始战斗
	void startFight();
	// 双方攻击函数
	void attackMe();
	void attackSystem();
	// 刷新双方生命值
	void refreshHP();
	// 停止战斗
	void stopFight(FightResult fightResult_);
signals:
	// 停止战斗信号，一方精灵战败后发出
	void signalStopFight(FightResult);
};

class SendOutPokemon: public QWidget {
	Q_OBJECT
public:
	SendOutPokemon(Pokemon* myPokemon_, QWidget* parent = Q_NULLPTR);
	~SendOutPokemon();
private:
	Ui::SendOutPokemon ui;
	Pokemon* myPokemon;
	// 当前精灵图片
	QMovie* currentImg;
	// 生成三个不重复随机数
	std::set<int> getRandSet(int num_);
	std::vector<Pokemon*> sendOutList;
private slots:
	// 刷新精灵信息
	void refreshPokemonInfo();
	// 送出精灵
	void onPressSendOut();
};