#pragma once
#include "ProgramLib.h"
#include "ui_Fight.h"
#include "ui_SendOutPokemon.h"
#include "Pokemon.h"

// ս������
class Fight: public QWidget {
	Q_OBJECT
public:
	Fight(FightType fightType_, Pokemon* systemPokemon_, Pokemon* myPokemon_, QWidget* parent = Q_NULLPTR);
	~Fight();
private:
	Ui::Fight ui;
	// ս������
	FightType fightType;
	// ˫������
	Pokemon* systemPokemon;
	Pokemon* myPokemon;
	// ˫������ͼƬ
	QMovie* systemImg;
	QMovie* myImg;
	// ˫��������ʱ��
	QTimer* systemAttackTimer;
	QTimer* myAttackTimer;
	// ������Ч
	QSound fightBGM;
	// �غ�
	int attackTime;
	// ��ʼս��
	void startFight();
	// ˫����������
	void attackMe();
	void attackSystem();
	// ˢ��˫������ֵ
	void refreshHP();
	// ֹͣս��
	void stopFight(FightResult fightResult_);
signals:
	// ֹͣս���źţ�һ������ս�ܺ󷢳�
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
	// ��ǰ����ͼƬ
	QMovie* currentImg;
	// �����������ظ������
	std::set<int> getRandSet(int num_);
	std::vector<Pokemon*> sendOutList;
private slots:
	// ˢ�¾�����Ϣ
	void refreshPokemonInfo();
	// �ͳ�����
	void onPressSendOut();
};