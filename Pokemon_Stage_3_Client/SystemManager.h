#pragma once
#include "ProgramLib.h"
#include "NetworkManager.h"
#include "Pokemon.h"

// ϵͳ������/�ѵ�¼�û���
class SystemManager {
private:
	// �������
	NetworkManager Network;
	// �û���
	std::string userName;
	// ս����Ϣ
	int fightWinNum;
	int fightAllNum;
public:
	// �����б�
	std::vector<Pokemon*> pokemonList;
	std::vector<Pokemon*> systemPokemonList;
	// ��������������Ϣ
	std::vector<std::string> pokemonKindStr;
	std::vector<std::string> attackMethodStr;
	// ϵͳ��ʼ��
	SystemManager(std::string IP_);
	// ϵͳ�ر�
	~SystemManager();
	// ��Ա��ȡ����
	void setUserName(std::string userName_);
	void incFightWinNum();
	void incFightAllNum();
	std::string getUserName() const;
	int getFightWinNum() const;
	int getFightAllNum() const;
	// ע��������
	ReplyType signIn(std::string userName_, std::string passWord_);
	// ��¼������
	ReplyType logIn(std::string userName_, std::string passWord_);
	// �˳���¼������
	void logOut();
	// ��ȡս����Ϣ����
	UserFightData getFightData(std::string userName_);
	// ����ս����Ϣ����
	void updateFightData();
	// ����������ɺ���
	Pokemon* generateRandPokemon(std::string name_) const;
	// ���鴴������
	void createPokemon(Pokemon* pokemon_);
	// ������Ϣ���º���
	void updatePokemon(std::string OldPokemonName_, Pokemon* pokemon_);
	// ����ɾ������
	void deletePokemon(std::string pokemonName_);
	// ��ȡָ���û��ľ����б�
	std::vector<Pokemon*> getPokemonList(std::string userName_);
	// ��ȡ�����û��б�
	std::vector<std::string> getOnlineUserList();
	// ��ȡ�����û��б�
	std::vector<std::string> getAllUserList();
};