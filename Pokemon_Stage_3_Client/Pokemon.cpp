#include "SystemManager.h"
#include "Pokemon.h"

extern SystemManager* System;

/***** Beginning of Class Pokemon *****/
Pokemon::Pokemon(std::string name_)
	: name(name_), kind(), level(1), exp(100), attack(100), defense(100),
	maxBlood(100), currentBlood(maxBlood), attackSpeed(100), attackMethod() {
}

Pokemon::Pokemon(std::string name_, int level_, int exp_, int attack_, int defense_,
	int maxBlood_, int currentBlood_, int attackSpeed_)
	: name(name_), kind(), level(level_), exp(exp_), attack(attack_), defense(defense_),
	maxBlood(maxBlood_), currentBlood(currentBlood_), attackSpeed(attackSpeed_), attackMethod() {
}

void Pokemon::incExp(int incVal_) {
	// ���Ӿ���ֵ������Ϊ 100
	this->exp += min(incVal_, 100);
	// ����ֵ�ﵽ��������ֵ������
	if (this->exp / 100 > this->level && this->exp < 1600) {
		this->upGrade();
		this->currentBlood = this->maxBlood;
	}
}

void Pokemon::incCurrentBlood(int incVal_) {
	// ��ǰ����ֵ����������������ֵ
	this->currentBlood = min(this->currentBlood + incVal_, this->maxBlood);
}

void Pokemon::decBlood(int decVal_) {
	// ʣ������ֵ����Ӧ��һ���˺�����������ֵ����֮������ֵ��Ϊ 0��ս��
	this->currentBlood = max(this->currentBlood - decVal_, 0);
}

int Pokemon::getSingleAttackVal() const {
	// ��������˺�����Ϊ������/20
	return this->attack / 20;
}

int Pokemon::getSingleDefenseVal() const {
	// �����˺�����ֵ����Ϊ������/100
	return this->defense / 100;
}

int Pokemon::getSingleNormalAttackRealDamage(Pokemon& beAttackedPokemon_) const {
	return max(this->getSingleAttackVal() - beAttackedPokemon_.getSingleDefenseVal(), 0);
}

std::string Pokemon::getName() const {
	return this->name;
}

PokemonKind Pokemon::getKind() const {
	return this->kind;
}

int Pokemon::getLevel() const {
	return this->level;
}

int Pokemon::getExp() const {
	return this->exp;
}

int Pokemon::getAttack() const {
	return this->attack;
}

int Pokemon::getDefense() const {
	return this->defense;
}

int Pokemon::getMaxBlood() const {
	return this->maxBlood;
}

int Pokemon::getCurrentBlood() const {
	return this->currentBlood;
}

int Pokemon::getAttackSpeed() const {
	return this->attackSpeed;
}

AttackMethod Pokemon::getAttackMethod() const {
	return this->attackMethod;
}

std::string Pokemon::toString() const {
	std::string infoStr{};
	infoStr += ("����: " + this->name + "\n"
		+ "����: " + System->pokemonKindStr[this->kind] + "\n"
		+ "�ȼ�: " + std::to_string(this->level) + "\n"
		+ "����ֵ: " + std::to_string(this->exp) + "\n"
		+ "������: " + std::to_string(this->attack) + "\n"
		+ "������: " + std::to_string(this->defense) + "\n"
		+ "�������ֵ: " + std::to_string(this->maxBlood) + "\n"
		+ "��ǰ����ֵ: " + std::to_string(this->currentBlood) + "\n"
		+ "�����ٶ�: " + std::to_string(this->attackSpeed) + "\n"
		+ "���м���: " + System->attackMethodStr[this->attackMethod] + "\n"
		+ std::string(26, '-'));
	return infoStr;
}

void Pokemon::setName(std::string name_) {
	this->name = name_;
}
/***** Ending of Class Pokemon *****/

/***** Beginning of Class HighAttack *****/
HighAttack::HighAttack(std::string name_)
	: Pokemon(name_) {
	this->kind = HIGH_ATTACK;
	this->attackMethod = LIGHTNING;
}

HighAttack::HighAttack(std::string name_, int level_, int exp_, int attack_, int defense_,
	int maxBlood_, int currentBlood_, int attackSpeed_)
	: Pokemon(name_, level_, exp_, attack_, defense_, maxBlood_, currentBlood_, attackSpeed_) {
	this->kind = HIGH_ATTACK;
	this->attackMethod = LIGHTNING;
}

Pokemon* HighAttack::getClone() const {
	return new HighAttack(*this);
}

void HighAttack::upGrade() {
	// ���һ���ȼ�
	this->level++;
	// ��������� 150
	this->attack += 150;
	// ����������� 100
	this->defense += 100;
	this->maxBlood += 100;
	this->attackSpeed += 20;
}

AttackMethod HighAttack::attackPokemon(Pokemon& beAttackedPokemon_) {
	int realDamage{0};
	int randVal = rand() % 100;
	AttackMethod attackMethod;
	// 60% �ĸ���ʹ����ͨ���ܣ�20% �ĸ���ʹ�����м��ܣ�10% �ĸ���ʹ�ñ�����10% �ĸ��ʱ���������
	if (randVal < 60) {
		// ��ͨ�������������߿��Է���
		attackMethod = NORMAL;
		realDamage = getSingleNormalAttackRealDamage(beAttackedPokemon_);
		std::cout << "������: " << this->name << ", ������ʽ: ��ͨ����" << std::endl;
	}
	else if (randVal >= 60 && randVal < 80) {
		// ���繥�������������޷�����
		attackMethod = LIGHTNING;
		realDamage = this->getSingleAttackVal();
		std::cout << "������: " << this->name << ", ������ʽ: ���繥��" << std::endl;
	}
	else if (randVal >= 80 && randVal < 90) {
		// ������5 ���ڼ��ܹ���
		attackMethod = SUPER;
		realDamage = 5 * this->getSingleAttackVal();
	}
	else {
		// ���ܣ����˺�
		attackMethod = INVALID;
		realDamage = 0;
	}
	// �������߽�����Ӧ����ֵ
	beAttackedPokemon_.decBlood(realDamage);
	std::cout << "���� " << beAttackedPokemon_.getName() << " �ܵ����˺�Ϊ " << realDamage << std::endl;
	return attackMethod;
}
/***** Ending of Class HighAttack *****/

/***** Beginning of Class HighBlood *****/
HighBlood::HighBlood(std::string name_)
	: Pokemon(name_) {
	this->kind = HIGH_BLOOD;
	this->attackMethod = FIRE;
}

HighBlood::HighBlood(std::string name_, int level_, int exp_, int attack_, int defense_,
	int maxBlood_, int currentBlood_, int attackSpeed_)
	: Pokemon(name_, level_, exp_, attack_, defense_, maxBlood_, currentBlood_, attackSpeed_) {
	this->kind = HIGH_BLOOD;
	this->attackMethod = FIRE;
}

Pokemon* HighBlood::getClone() const {
	return new HighBlood(*this);
}

void HighBlood::upGrade() {
	// ���һ���ȼ�
	this->level++;
	// ��������� 150
	this->maxBlood += 150;
	// ����������� 100
	this->attack += 100;
	this->defense += 100;
	this->attackSpeed += 20;
}

AttackMethod HighBlood::attackPokemon(Pokemon& beAttackedPokemon_) {
	int realDamage{0};
	int randVal = rand() % 100;
	AttackMethod attackMethod;
	// 60% �ĸ���ʹ����ͨ���ܣ�20% �ĸ���ʹ�����м��ܣ�10% �ĸ���ʹ�ñ�����10% �ĸ��ʱ���������
	if (randVal < 60) {
		// ��ͨ�������������߿��Է���
		attackMethod = NORMAL;
		realDamage = getSingleNormalAttackRealDamage(beAttackedPokemon_);
		std::cout << "������: " << this->name << ", ������ʽ: ��ͨ����" << std::endl;
	}
	else if (randVal >= 60 && randVal < 80) {
		// ���湥����������������ֵ����
		attackMethod = FIRE;
		realDamage = beAttackedPokemon_.getCurrentBlood() / 2;
		std::cout << "������: " << this->name << ", ������ʽ: ���湥��" << std::endl;
	}
	else if (randVal >= 80 && randVal < 90) {
		// ������5 ���ڼ��ܹ���
		attackMethod = SUPER;
		realDamage = 5 * this->getSingleAttackVal();
	}
	else {
		// ���ܣ����˺�
		attackMethod = INVALID;
		realDamage = 0;
	}
	// �������߽�����Ӧ����ֵ
	beAttackedPokemon_.decBlood(realDamage);
	std::cout << "���� " << beAttackedPokemon_.getName() << " �ܵ����˺�Ϊ " << realDamage << std::endl;
	return attackMethod;
}
/***** Ending of Class HighBlood *****/

/***** Beginning of Class HighDefense *****/
HighDefense::HighDefense(std::string name_)
	: Pokemon(name_) {
	this->kind = HIGH_DEFENSE;
	this->attackMethod = ABSORB_BLOOD;
}

HighDefense::HighDefense(std::string name_, int level_, int exp_, int attack_, int defense_,
	int maxBlood_, int currentBlood_, int attackSpeed_)
	: Pokemon(name_, level_, exp_, attack_, defense_, maxBlood_, currentBlood_, attackSpeed_) {
	this->kind = HIGH_DEFENSE;
	this->attackMethod = ABSORB_BLOOD;
}

Pokemon* HighDefense::getClone() const {
	return new HighDefense(*this);
}

void HighDefense::upGrade() {
	// ���һ���ȼ�
	this->level++;
	// ��������� 150
	this->defense += 150;
	// ����������� 100
	this->attack += 100;
	this->maxBlood += 100;
	this->attackSpeed += 20;
}

AttackMethod HighDefense::attackPokemon(Pokemon& beAttackedPokemon_) {
	int realDamage{0};
	int randVal = rand() % 100;
	AttackMethod attackMethod;
	// 60% �ĸ���ʹ����ͨ���ܣ�20% �ĸ���ʹ�����м��ܣ�10% �ĸ���ʹ�ñ�����10% �ĸ��ʱ���������
	if (randVal < 60) {
		// ��ͨ�������������߿��Է���
		attackMethod = NORMAL;
		realDamage = getSingleNormalAttackRealDamage(beAttackedPokemon_);
		std::cout << "������: " << this->name << ", ������ʽ: ��ͨ����" << std::endl;
	}
	else if (randVal >= 60 && randVal < 80) {
		// ��Ѫ�������������������͵�����ֵ�� 50% ������������
		attackMethod = ABSORB_BLOOD;
		realDamage = getSingleNormalAttackRealDamage(beAttackedPokemon_);
		this->incCurrentBlood(realDamage / 2);
		std::cout << "������: " << this->name << ", ������ʽ: ��Ѫ����" << std::endl;
	}
	else if (randVal >= 80 && randVal < 90) {
		// ������5 ���ڼ��ܹ���
		attackMethod = SUPER;
		realDamage = 5 * this->getSingleAttackVal();
	}
	else {
		// ���ܣ����˺�
		attackMethod = INVALID;
		realDamage = 0;
	}
	// �������߽�����Ӧ����ֵ
	beAttackedPokemon_.decBlood(realDamage);
	std::cout << "���� " << beAttackedPokemon_.getName() << " �ܵ����˺�Ϊ " << realDamage << std::endl;
	return attackMethod;
}
/***** Ending of Class HighDefense *****/

/***** Beginning of Class HighSpeed *****/
HighSpeed::HighSpeed(std::string name_)
	: Pokemon(name_) {
	this->kind = HIGH_SPEED;
	this->attackMethod = QUICK;
}

HighSpeed::HighSpeed(std::string name_, int level_, int exp_, int attack_, int defense_,
	int maxBlood_, int currentBlood_, int attackSpeed_)
	: Pokemon(name_, level_, exp_, attack_, defense_, maxBlood_, currentBlood_, attackSpeed_) {
	this->kind = HIGH_SPEED;
	this->attackMethod = QUICK;
}

Pokemon* HighSpeed::getClone() const {
	return new HighSpeed(*this);
}

void HighSpeed::upGrade() {
	// ���һ���ȼ�
	this->level++;
	// ��������� 40
	this->attackSpeed += 40;
	// ����������� 100
	this->attack += 100;
	this->defense += 100;
	this->maxBlood += 100;
}

AttackMethod HighSpeed::attackPokemon(Pokemon& beAttackedPokemon_) {
	int realDamage{0};
	int randVal = rand() % 100;
	AttackMethod attackMethod;
	// 60% �ĸ���ʹ����ͨ���ܣ�20% �ĸ���ʹ�����м��ܣ�10% �ĸ���ʹ�ñ�����10% �ĸ��ʱ���������
	if (randVal < 60) {
		// ��ͨ�������������߿��Է���
		attackMethod = NORMAL;
		realDamage = getSingleNormalAttackRealDamage(beAttackedPokemon_);
		std::cout << "������: " << this->name << ", ������ʽ: ��ͨ����" << std::endl;
	}
	else if (randVal >= 60 && randVal < 80) {
		// ���ٹ�������������������ͨ����
		attackMethod = QUICK;
		realDamage = 2 * getSingleNormalAttackRealDamage(beAttackedPokemon_);
		std::cout << "������: " << this->name << ", ������ʽ: ���ٹ���" << std::endl;
	}
	else if (randVal >= 80 && randVal < 90) {
		// ������5 ���ڼ��ܹ���
		attackMethod = SUPER;
		realDamage = 5 * this->getSingleAttackVal();
	}
	else {
		// ���ܣ����˺�
		attackMethod = INVALID;
		realDamage = 0;
	}
	// �������߽�����Ӧ����ֵ
	beAttackedPokemon_.decBlood(realDamage);
	std::cout << "���� " << beAttackedPokemon_.getName() << " �ܵ����˺�Ϊ " << realDamage << std::endl;
	return attackMethod;
}
/***** Ending of Class HighSpeed *****/