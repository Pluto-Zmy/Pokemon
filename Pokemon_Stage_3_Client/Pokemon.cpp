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
	// 增加经验值，上限为 100
	this->exp += min(incVal_, 100);
	// 经验值达到升级所需值则升级
	if (this->exp / 100 > this->level && this->exp < 1600) {
		this->upGrade();
		this->currentBlood = this->maxBlood;
	}
}

void Pokemon::incCurrentBlood(int incVal_) {
	// 当前生命值最多增加至最大生命值
	this->currentBlood = min(this->currentBlood + incVal_, this->maxBlood);
}

void Pokemon::decBlood(int decVal_) {
	// 剩余生命值可以应对一次伤害，降低生命值；反之，生命值减为 0，战败
	this->currentBlood = max(this->currentBlood - decVal_, 0);
}

int Pokemon::getSingleAttackVal() const {
	// 单次输出伤害定义为攻击力/20
	return this->attack / 20;
}

int Pokemon::getSingleDefenseVal() const {
	// 单次伤害防御值定义为防御力/100
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
	infoStr += ("名称: " + this->name + "\n"
		+ "类型: " + System->pokemonKindStr[this->kind] + "\n"
		+ "等级: " + std::to_string(this->level) + "\n"
		+ "经验值: " + std::to_string(this->exp) + "\n"
		+ "攻击力: " + std::to_string(this->attack) + "\n"
		+ "防御力: " + std::to_string(this->defense) + "\n"
		+ "最大生命值: " + std::to_string(this->maxBlood) + "\n"
		+ "当前生命值: " + std::to_string(this->currentBlood) + "\n"
		+ "攻击速度: " + std::to_string(this->attackSpeed) + "\n"
		+ "特有技能: " + System->attackMethodStr[this->attackMethod] + "\n"
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
	// 提高一个等级
	this->level++;
	// 主属性提高 150
	this->attack += 150;
	// 其它属性提高 100
	this->defense += 100;
	this->maxBlood += 100;
	this->attackSpeed += 20;
}

AttackMethod HighAttack::attackPokemon(Pokemon& beAttackedPokemon_) {
	int realDamage{0};
	int randVal = rand() % 100;
	AttackMethod attackMethod;
	// 60% 的概率使用普通技能，20% 的概率使用特有技能，10% 的概率使用暴击，10% 的概率被对手闪避
	if (randVal < 60) {
		// 普通攻击：被攻击者可以防御
		attackMethod = NORMAL;
		realDamage = getSingleNormalAttackRealDamage(beAttackedPokemon_);
		std::cout << "攻击者: " << this->name << ", 攻击方式: 普通攻击" << std::endl;
	}
	else if (randVal >= 60 && randVal < 80) {
		// 闪电攻击：被攻击者无法防御
		attackMethod = LIGHTNING;
		realDamage = this->getSingleAttackVal();
		std::cout << "攻击者: " << this->name << ", 攻击方式: 闪电攻击" << std::endl;
	}
	else if (randVal >= 80 && randVal < 90) {
		// 暴击：5 倍于技能攻击
		attackMethod = SUPER;
		realDamage = 5 * this->getSingleAttackVal();
	}
	else {
		// 闪避：无伤害
		attackMethod = INVALID;
		realDamage = 0;
	}
	// 被攻击者降低相应生命值
	beAttackedPokemon_.decBlood(realDamage);
	std::cout << "精灵 " << beAttackedPokemon_.getName() << " 受到的伤害为 " << realDamage << std::endl;
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
	// 提高一个等级
	this->level++;
	// 主属性提高 150
	this->maxBlood += 150;
	// 其它属性提高 100
	this->attack += 100;
	this->defense += 100;
	this->attackSpeed += 20;
}

AttackMethod HighBlood::attackPokemon(Pokemon& beAttackedPokemon_) {
	int realDamage{0};
	int randVal = rand() % 100;
	AttackMethod attackMethod;
	// 60% 的概率使用普通技能，20% 的概率使用特有技能，10% 的概率使用暴击，10% 的概率被对手闪避
	if (randVal < 60) {
		// 普通攻击：被攻击者可以防御
		attackMethod = NORMAL;
		realDamage = getSingleNormalAttackRealDamage(beAttackedPokemon_);
		std::cout << "攻击者: " << this->name << ", 攻击方式: 普通攻击" << std::endl;
	}
	else if (randVal >= 60 && randVal < 80) {
		// 火焰攻击：被攻击者生命值减半
		attackMethod = FIRE;
		realDamage = beAttackedPokemon_.getCurrentBlood() / 2;
		std::cout << "攻击者: " << this->name << ", 攻击方式: 火焰攻击" << std::endl;
	}
	else if (randVal >= 80 && randVal < 90) {
		// 暴击：5 倍于技能攻击
		attackMethod = SUPER;
		realDamage = 5 * this->getSingleAttackVal();
	}
	else {
		// 闪避：无伤害
		attackMethod = INVALID;
		realDamage = 0;
	}
	// 被攻击者降低相应生命值
	beAttackedPokemon_.decBlood(realDamage);
	std::cout << "精灵 " << beAttackedPokemon_.getName() << " 受到的伤害为 " << realDamage << std::endl;
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
	// 提高一个等级
	this->level++;
	// 主属性提高 150
	this->defense += 150;
	// 其它属性提高 100
	this->attack += 100;
	this->maxBlood += 100;
	this->attackSpeed += 20;
}

AttackMethod HighDefense::attackPokemon(Pokemon& beAttackedPokemon_) {
	int realDamage{0};
	int randVal = rand() % 100;
	AttackMethod attackMethod;
	// 60% 的概率使用普通技能，20% 的概率使用特有技能，10% 的概率使用暴击，10% 的概率被对手闪避
	if (randVal < 60) {
		// 普通攻击：被攻击者可以防御
		attackMethod = NORMAL;
		realDamage = getSingleNormalAttackRealDamage(beAttackedPokemon_);
		std::cout << "攻击者: " << this->name << ", 攻击方式: 普通攻击" << std::endl;
	}
	else if (randVal >= 60 && randVal < 80) {
		// 吸血攻击：被攻击者所降低的生命值的 50% 被攻击者吸收
		attackMethod = ABSORB_BLOOD;
		realDamage = getSingleNormalAttackRealDamage(beAttackedPokemon_);
		this->incCurrentBlood(realDamage / 2);
		std::cout << "攻击者: " << this->name << ", 攻击方式: 吸血攻击" << std::endl;
	}
	else if (randVal >= 80 && randVal < 90) {
		// 暴击：5 倍于技能攻击
		attackMethod = SUPER;
		realDamage = 5 * this->getSingleAttackVal();
	}
	else {
		// 闪避：无伤害
		attackMethod = INVALID;
		realDamage = 0;
	}
	// 被攻击者降低相应生命值
	beAttackedPokemon_.decBlood(realDamage);
	std::cout << "精灵 " << beAttackedPokemon_.getName() << " 受到的伤害为 " << realDamage << std::endl;
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
	// 提高一个等级
	this->level++;
	// 主属性提高 40
	this->attackSpeed += 40;
	// 其它属性提高 100
	this->attack += 100;
	this->defense += 100;
	this->maxBlood += 100;
}

AttackMethod HighSpeed::attackPokemon(Pokemon& beAttackedPokemon_) {
	int realDamage{0};
	int randVal = rand() % 100;
	AttackMethod attackMethod;
	// 60% 的概率使用普通技能，20% 的概率使用特有技能，10% 的概率使用暴击，10% 的概率被对手闪避
	if (randVal < 60) {
		// 普通攻击：被攻击者可以防御
		attackMethod = NORMAL;
		realDamage = getSingleNormalAttackRealDamage(beAttackedPokemon_);
		std::cout << "攻击者: " << this->name << ", 攻击方式: 普通攻击" << std::endl;
	}
	else if (randVal >= 60 && randVal < 80) {
		// 快速攻击：连续进行两次普通攻击
		attackMethod = QUICK;
		realDamage = 2 * getSingleNormalAttackRealDamage(beAttackedPokemon_);
		std::cout << "攻击者: " << this->name << ", 攻击方式: 快速攻击" << std::endl;
	}
	else if (randVal >= 80 && randVal < 90) {
		// 暴击：5 倍于技能攻击
		attackMethod = SUPER;
		realDamage = 5 * this->getSingleAttackVal();
	}
	else {
		// 闪避：无伤害
		attackMethod = INVALID;
		realDamage = 0;
	}
	// 被攻击者降低相应生命值
	beAttackedPokemon_.decBlood(realDamage);
	std::cout << "精灵 " << beAttackedPokemon_.getName() << " 受到的伤害为 " << realDamage << std::endl;
	return attackMethod;
}
/***** Ending of Class HighSpeed *****/