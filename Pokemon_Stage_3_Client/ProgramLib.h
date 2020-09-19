#pragma once
#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <thread>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QStyleFactory>
#include <QMessagebox>
#include <QWidget>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QTranslator>
#include <QMovie>
#include <QtMultimedia/QSound>

// ȷ�ϱ�־
constexpr char ACK = 0xFF;
// ��������С
constexpr int BUFFER_SIZE = 128;
// �������ݶδ�С
constexpr int FLAG_SIZE = sizeof(char);
constexpr int STRING_SIZE = sizeof(char) * 16;
constexpr int POKEMON_DATA_SIZE = STRING_SIZE * 2 + sizeof(int) * 9;

// �ͻ�����������
typedef enum {
	SIGN_IN = 0,					// ע��
	LOG_IN = 1,						// ��¼
	LOG_OUT = 2,					// �˳���¼
	CREATE_POKEMON = 3,				// ��������
	GET_POKEMON = 4,				// ��ȡ����
	UPDATE_POKEMON = 5,				// ���¾�����Ϣ
	DELETE_POKEMON = 6,				// �ͳ�����
	GET_FIGHT_DATA = 7,				// ��ȡս����Ϣ
	UPDATE_FIGHT_DATA = 8,			// ����ս������
	GET_ONLINE_USER = 9,			// ��ȡ�����û��б�
	GET_ALL_USER = 10				// ��ȡ�����û��б�
} RequestType;

// ��������Ӧ����
typedef enum {
	SIGN_IN_SUCCESS = 0,			// ע��ɹ�
	SIGN_IN_FAIL = 1,				// ע��ʧ��
	LOG_IN_SUCCESS = 2,				// ��¼�ɹ�
	LOG_IN_FAIL_USER_NOT_EXIST = 3,	// ��¼ʧ��-�û�������
	LOG_IN_FAIL_PASSWORD_ERROR = 4,	// ��¼ʧ��-�������
	LOG_IN_FAIL_USER_ONLINE = 5		// ��¼ʧ��-��ص�¼
} ReplyType;

// ս������
typedef enum {
	UPGRADE = 0,
	DUEL = 1
} FightType;

// ս�����
typedef enum {
	LOST = 0,
	WIN = 1
} FightResult;

// ս����Ϣ������ʱʹ��
typedef struct {
	int fightWinNum;
	int fightAllNum;
} UserFightData;

// �������ԣ�����ʱʹ��
typedef struct {
	std::string ownerName;
	std::string name;
	int kind;
	int level;
	int exp;
	int attack;
	int defense;
	int maxBlood;
	int currentBlood;
	int attackSpeed;
	int attackMethod;
} PokemonData;