#pragma once
#include "ProgramLib.h"

// 精灵类型
typedef enum {
	HIGH_ATTACK = 0, HIGH_BLOOD = 1, HIGH_DEFENSE = 2, HIGH_SPEED = 3
} PokemonKind;

// 攻击方式
typedef enum {
	LIGHTNING = 0, FIRE = 1, ABSORB_BLOOD = 2, QUICK = 3, NORMAL = 4, SUPER = 5, INVALID = 6
} AttackMethod;

// 精灵基类
class Pokemon {
protected:
	std::string name;			// 名字
	PokemonKind kind;			// 类型
	int level;					// 精灵等级
	int exp;					// 经验值
	int attack;					// 攻击力
	int defense;				// 防御力
	int maxBlood;				// 最大生命值
	int currentBlood;			// 战斗时生命值
	int attackSpeed;			// 攻击速度
	AttackMethod attackMethod;	// 特有攻击方式
public:
	// 默认构造函数
	Pokemon(std::string name_);
	// 含参构造函数
	Pokemon(std::string name_, int level_, int exp_, int attack_, int defense_,
		int maxBlood, int currentBlood_, int attackSpeed);
	// 创建副本函数
	virtual Pokemon* getClone() const = 0;
	// 升级函数
	virtual void upGrade() = 0;
	// 攻击函数
	virtual AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) = 0;
	// 战后提高经验值函数
	void incExp(int incVal_);
	// 临时提高生命值函数
	void incCurrentBlood(int incVal_);
	// 被攻击，生命值降低函数
	void decBlood(int decVal_);
	// 获取单次攻击输出伤害值
	int getSingleAttackVal() const;
	// 获取单次伤害防御值
	int getSingleDefenseVal() const;
	// 获取单次普通攻击造成的实际伤害
	int getSingleNormalAttackRealDamage(Pokemon& beAttackedPokemon_) const;
	// 获取名字
	std::string getName() const;
	// 获取类型
	PokemonKind getKind() const;
	// 获取等级
	int getLevel() const;
	// 获取经验值
	int getExp() const;
	int getAttack() const;
	int getDefense() const;
	int getMaxBlood() const;
	// 获取当前生命值
	int getCurrentBlood() const;
	// 获取攻击速度
	int getAttackSpeed() const;
	AttackMethod getAttackMethod() const;
	// 获取描述信息
	std::string toString() const;
	// 改名
	void setName(std::string name_);
};

// 高攻击类型精灵类
class HighAttack: public Pokemon {
public:
	// 默认构造函数
	HighAttack(std::string name_);
	// 含参构造函数
	HighAttack(std::string name_, int level_, int exp_, int attack_, int defense_,
		int maxBlood_, int currentBlood_, int attackSpeed_);
	// 创建副本函数
	virtual Pokemon* getClone() const override;
	// 升级函数
	void upGrade() override;
	// 攻击函数：闪电攻击
	AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) override;
};

// 高生命值类型精灵类
class HighBlood: public Pokemon {
public:
	// 默认构造函数
	HighBlood(std::string name_);
	// 含参构造函数
	HighBlood(std::string name_, int level_, int exp_, int attack_, int defense_,
		int maxBlood_, int currentBlood_, int attackSpeed_);
	// 创建副本函数
	virtual Pokemon* getClone() const override;
	// 升级函数
	void upGrade() override;
	// 攻击函数：火焰攻击
	AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) override;
};

// 高防御类型精灵类
class HighDefense: public Pokemon {
public:
	// 默认构造函数
	HighDefense(std::string name_);
	// 含参构造函数
	HighDefense(std::string name_, int level_, int exp_, int attack_, int defense_,
		int maxBlood_, int currentBlood_, int attackSpeed_);
	// 创建副本函数
	virtual Pokemon* getClone() const override;
	// 升级函数
	void upGrade() override;
	// 攻击函数：火焰攻击
	AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) override;
};

// 低攻击间隔类型精灵类
class HighSpeed: public Pokemon {
public:
	// 默认构造函数
	HighSpeed(std::string name_);
	// 含参构造函数
	HighSpeed(std::string name_, int level_, int exp_, int attack_, int defense_,
		int maxBlood_, int currentBlood_, int attackSpeed_);
	// 创建副本函数
	virtual Pokemon* getClone() const override;
	// 升级函数
	void upGrade() override;
	// 攻击函数：火焰攻击
	AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) override;
};

// 细分：火爆猴继承于力量型
class HuoBaoHou: public HighAttack {
public:
	HuoBaoHou(std::string name_): HighAttack(name_) {
	}
	AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) override {
		return HighAttack::attackPokemon(beAttackedPokemon_);
	}
};

// 细分：沙瓦朗继承于力量型
class ShaWaLang: public HighAttack {
public:
	ShaWaLang(std::string name_): HighAttack(name_) {
	}
	AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) override {
		return HighAttack::attackPokemon(beAttackedPokemon_);
	}
};

// 细分：腕力继承于力量型
class WanLi: public HighAttack {
public:
	WanLi(std::string name_): HighAttack(name_) {
	}
	AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) override {
		return HighAttack::attackPokemon(beAttackedPokemon_);
	}
};