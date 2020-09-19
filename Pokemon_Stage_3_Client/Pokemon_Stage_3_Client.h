#pragma once
#include "ProgramLib.h"
#include "ui_Pokemon_Stage_3_Client.h"
#include "ui_SelectPokemon.h"
#include "Pokemon.h"
#include "Fight.h"

// ��Ϸ������
class Pokemon_Stage_3_Client: public QMainWindow {
	Q_OBJECT
public:
	// ���졢��������
	Pokemon_Stage_3_Client(QWidget* parent = Q_NULLPTR);
	~Pokemon_Stage_3_Client();
	// ���ݾ�������ѡ��ͷ��
	static QMovie* selectImg(PokemonKind kind_, QString sysStr_ = "");
	// ѡ�����
	QPixmap selectPokemonNumMedal(int pokemonNum_);
	QPixmap selectAdvancePokemonMedal(std::vector<Pokemon*> pokemonList_);
private:
	Ui::Pokemon_Stage_3_ClientClass ui;
	// ��ǰ�û���
	std::string currentUserName;
	// ��ǰ�����б�
	std::vector<Pokemon*> currentPokemonList;
	// ��ǰ����ͷ��
	QMovie* currentImg;
private slots:
	// ˢ���û��б�
	void refreshUserList();
	// ˢ���û������б�
	void refreshUserPokemonList();
	// ˢ���û���������
	void refreshUserPokemonData();
	// ˢ��ϵͳ�����б�
	void refreshSystemPokemonList();
	// ˢ��ϵͳ������Ϣ
	void refreshSystemPokemonData();
	// ��Ӧ��������ť����ź�
	void onPressUpGradeFight();
	// ��Ӧ��������ť����ź�
	void onPressDuelFight();
	// ˢ���ҵľ����б�
	void refreshMyPokemonList();
	// ˢ���ҵľ�������
	void refreshMyPokemonData();
	// ��Ӧ�˳���¼��ť����¼�
	void onPressLogOut();
	// ��Ӧ���������ť����¼�
	void onPressChangePokemonName();
	// ��Ӧ�ָ���ť����¼�
	void onPressRecover();
};

// ѡ���ս���鴰��
class SelectPokemon: public QWidget {
	Q_OBJECT
public:
	// ���졢��������
	SelectPokemon(FightType fightType_, Pokemon* systemPokemon_, QWidget* parent = Q_NULLPTR);
	~SelectPokemon();
private:
	Ui::SelectPokemon ui;
	// ��ǰ����ͼƬ
	QMovie* currentImg;
	// ��սģʽ
	FightType fightType;
	// �Է�����
	Pokemon* systemPokemon;
private slots:
	// ˢ�¾�����Ϣ
	void refreshPokemonInfo();
	// ��Ӧ��ս��ť����ź�
	void onPressFight();
};