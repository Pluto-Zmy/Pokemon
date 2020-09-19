#pragma once
#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "Sqlite3.h"

// 确认标志
constexpr char ACK = 0xFF;
// 缓冲区大小
constexpr int BUFFER_SIZE = 128;
// 各种数据段大小
constexpr int FLAG_SIZE = sizeof(char);
constexpr int STRING_SIZE = sizeof(char) * 16;
constexpr int POKEMON_DATA_SIZE = STRING_SIZE * 2 + sizeof(int) * 9;

class SystemManager;
class NetworkManager;
class DataBaseManager;
class ConnectedClient;

// 客户端请求类型
typedef enum {
	SIGN_IN = 0,					// 注册
	LOG_IN = 1,						// 登录
	LOG_OUT = 2,					// 退出登录
	CREATE_POKEMON = 3,				// 创建精灵
	GET_POKEMON = 4,				// 获取精灵
	UPDATE_POKEMON = 5,				// 更新精灵信息
	DELETE_POKEMON = 6,				// 送出精灵
	GET_FIGHT_DATA = 7,				// 获取战斗信息
	UPDATE_FIGHT_DATA = 8,			// 更新战斗数据
	GET_ONLINE_USER = 9,			// 获取在线用户列表
	GET_ALL_USER = 10				// 获取所有用户列表
} RequestType;

// 服务器回应类型
typedef enum {
	SIGN_IN_SUCCESS = 0,			// 注册成功
	SIGN_IN_FAIL = 1,				// 注册失败
	LOG_IN_SUCCESS = 2,				// 登录成功
	LOG_IN_FAIL_USER_NOT_EXIST = 3,	// 登录失败-用户不存在
	LOG_IN_FAIL_PASSWORD_ERROR = 4,	// 登录失败-密码错误
	LOG_IN_FAIL_USER_ONLINE = 5		// 登录失败-异地登录
} ReplyType;

// 战斗信息，传输时使用
typedef struct {
	int fightWinNum;
	int fightAllNum;
} UserFightData;

// 精灵属性，传输时使用
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