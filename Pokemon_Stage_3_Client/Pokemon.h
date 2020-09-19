#pragma once
#include "ProgramLib.h"

// ��������
typedef enum {
	HIGH_ATTACK = 0, HIGH_BLOOD = 1, HIGH_DEFENSE = 2, HIGH_SPEED = 3
} PokemonKind;

// ������ʽ
typedef enum {
	LIGHTNING = 0, FIRE = 1, ABSORB_BLOOD = 2, QUICK = 3, NORMAL = 4, SUPER = 5, INVALID = 6
} AttackMethod;

// �������
class Pokemon {
protected:
	std::string name;			// ����
	PokemonKind kind;			// ����
	int level;					// ����ȼ�
	int exp;					// ����ֵ
	int attack;					// ������
	int defense;				// ������
	int maxBlood;				// �������ֵ
	int currentBlood;			// ս��ʱ����ֵ
	int attackSpeed;			// �����ٶ�
	AttackMethod attackMethod;	// ���й�����ʽ
public:
	// Ĭ�Ϲ��캯��
	Pokemon(std::string name_);
	// ���ι��캯��
	Pokemon(std::string name_, int level_, int exp_, int attack_, int defense_,
		int maxBlood, int currentBlood_, int attackSpeed);
	// ������������
	virtual Pokemon* getClone() const = 0;
	// ��������
	virtual void upGrade() = 0;
	// ��������
	virtual AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) = 0;
	// ս����߾���ֵ����
	void incExp(int incVal_);
	// ��ʱ�������ֵ����
	void incCurrentBlood(int incVal_);
	// ������������ֵ���ͺ���
	void decBlood(int decVal_);
	// ��ȡ���ι�������˺�ֵ
	int getSingleAttackVal() const;
	// ��ȡ�����˺�����ֵ
	int getSingleDefenseVal() const;
	// ��ȡ������ͨ������ɵ�ʵ���˺�
	int getSingleNormalAttackRealDamage(Pokemon& beAttackedPokemon_) const;
	// ��ȡ����
	std::string getName() const;
	// ��ȡ����
	PokemonKind getKind() const;
	// ��ȡ�ȼ�
	int getLevel() const;
	// ��ȡ����ֵ
	int getExp() const;
	int getAttack() const;
	int getDefense() const;
	int getMaxBlood() const;
	// ��ȡ��ǰ����ֵ
	int getCurrentBlood() const;
	// ��ȡ�����ٶ�
	int getAttackSpeed() const;
	AttackMethod getAttackMethod() const;
	// ��ȡ������Ϣ
	std::string toString() const;
	// ����
	void setName(std::string name_);
};

// �߹������;�����
class HighAttack: public Pokemon {
public:
	// Ĭ�Ϲ��캯��
	HighAttack(std::string name_);
	// ���ι��캯��
	HighAttack(std::string name_, int level_, int exp_, int attack_, int defense_,
		int maxBlood_, int currentBlood_, int attackSpeed_);
	// ������������
	virtual Pokemon* getClone() const override;
	// ��������
	void upGrade() override;
	// �������������繥��
	AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) override;
};

// ������ֵ���;�����
class HighBlood: public Pokemon {
public:
	// Ĭ�Ϲ��캯��
	HighBlood(std::string name_);
	// ���ι��캯��
	HighBlood(std::string name_, int level_, int exp_, int attack_, int defense_,
		int maxBlood_, int currentBlood_, int attackSpeed_);
	// ������������
	virtual Pokemon* getClone() const override;
	// ��������
	void upGrade() override;
	// �������������湥��
	AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) override;
};

// �߷������;�����
class HighDefense: public Pokemon {
public:
	// Ĭ�Ϲ��캯��
	HighDefense(std::string name_);
	// ���ι��캯��
	HighDefense(std::string name_, int level_, int exp_, int attack_, int defense_,
		int maxBlood_, int currentBlood_, int attackSpeed_);
	// ������������
	virtual Pokemon* getClone() const override;
	// ��������
	void upGrade() override;
	// �������������湥��
	AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) override;
};

// �͹���������;�����
class HighSpeed: public Pokemon {
public:
	// Ĭ�Ϲ��캯��
	HighSpeed(std::string name_);
	// ���ι��캯��
	HighSpeed(std::string name_, int level_, int exp_, int attack_, int defense_,
		int maxBlood_, int currentBlood_, int attackSpeed_);
	// ������������
	virtual Pokemon* getClone() const override;
	// ��������
	void upGrade() override;
	// �������������湥��
	AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) override;
};

// ϸ�֣��𱬺�̳���������
class HuoBaoHou: public HighAttack {
public:
	HuoBaoHou(std::string name_): HighAttack(name_) {
	}
	AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) override {
		return HighAttack::attackPokemon(beAttackedPokemon_);
	}
};

// ϸ�֣�ɳ���ʼ̳���������
class ShaWaLang: public HighAttack {
public:
	ShaWaLang(std::string name_): HighAttack(name_) {
	}
	AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) override {
		return HighAttack::attackPokemon(beAttackedPokemon_);
	}
};

// ϸ�֣������̳���������
class WanLi: public HighAttack {
public:
	WanLi(std::string name_): HighAttack(name_) {
	}
	AttackMethod attackPokemon(Pokemon& beAttackedPokemon_) override {
		return HighAttack::attackPokemon(beAttackedPokemon_);
	}
};