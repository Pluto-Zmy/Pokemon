#include "Fight.h"
#include "Pokemon_Stage_3_Client.h"
#include "SystemManager.h"

extern SystemManager* System;

Fight::Fight(FightType fightType_, Pokemon* systemPokemon_, Pokemon* myPokemon_, QWidget* parent)
	: QWidget(parent), fightType(fightType_), systemPokemon(systemPokemon_), myPokemon(myPokemon_),
	systemImg(Pokemon_Stage_3_Client::selectImg(systemPokemon->getKind(), "System")),
	myImg(Pokemon_Stage_3_Client::selectImg(myPokemon->getKind())),
	systemAttackTimer(new QTimer(this)), myAttackTimer(new QTimer(this)),
	fightBGM(":/Resources/FightBGM.wav"), attackTime(1) {
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	// 双方攻击定时信号
	connect(systemAttackTimer, &QTimer::timeout, this, &Fight::attackMe);
	connect(myAttackTimer, &QTimer::timeout, this, &Fight::attackSystem);
	// 停止战斗信号
	connect(this, &Fight::signalStopFight, this, &Fight::stopFight);
	// 显示双方精灵信息
	ui.labelSystemName->setText(QString::fromLocal8Bit(systemPokemon->getName().data()));
	ui.labelMyName->setText(QString::fromLocal8Bit(myPokemon->getName().data()));
	ui.labelSystemPokemonImg->setMovie(systemImg);
	ui.labelMyPokemonImg->setMovie(myImg);
	ui.progressBarSystemHP->setRange(0, systemPokemon->getMaxBlood());
	ui.progressBarMyHP->setRange(0, myPokemon->getMaxBlood());
	fightBGM.setLoops(QSound::Infinite);
	refreshHP();
	// 开始战斗
	startFight();
}

Fight::~Fight() {
	// 删除资源
	delete systemImg;
	delete myImg;
	delete systemAttackTimer;
	delete myAttackTimer;
}

// 开始战斗
void Fight::startFight() {
	// 双方精灵开始运动
	systemImg->start();
	myImg->start();
	// 开启攻击定时器
	systemAttackTimer->start(static_cast<int>(1e6) / systemPokemon->getAttackSpeed());
	myAttackTimer->start(static_cast<int>(1e6) / myPokemon->getAttackSpeed());
	fightBGM.play();
}

// 双方攻击函数
void Fight::attackMe() {
	// 获取攻击方式
	AttackMethod attackMethod = systemPokemon->attackPokemon(*myPokemon);
	QSound::play(":/Resources/Attack.wav");
	// 刷新血条
	refreshHP();
	// 刷新战斗信息
	if (attackMethod != INVALID) {
		ui.labelSystemAttackMethod->setText(QString::fromLocal8Bit(("第 " + std::to_string(attackTime++) + " 回合，" +
			System->attackMethodStr[attackMethod]).data()));
	}
	else {
		ui.labelMyAttackMethod->setText(QString::fromLocal8Bit(("第 " + std::to_string(attackTime++) + " 回合，" +
			System->attackMethodStr[attackMethod]).data()));
	}
	// 若对方战败则停止战斗
	if (myPokemon->getCurrentBlood() == 0) {
		signalStopFight(LOST);
	}
}

// 同上
void Fight::attackSystem() {
	AttackMethod attackMethod = myPokemon->attackPokemon(*systemPokemon);
	QSound::play(":/Resources/Attack.wav");
	refreshHP();
	if (attackMethod != INVALID) {
		ui.labelMyAttackMethod->setText(QString::fromLocal8Bit(("第 " + std::to_string(attackTime++) + " 回合，" +
			System->attackMethodStr[attackMethod]).data()));
	}
	else {
		ui.labelSystemAttackMethod->setText(QString::fromLocal8Bit(("第 " + std::to_string(attackTime++) + " 回合，" +
			System->attackMethodStr[attackMethod]).data()));
	}
	if (systemPokemon->getCurrentBlood() == 0) {
		signalStopFight(WIN);
	}
}

// 刷新双方生命值
void Fight::refreshHP() {
	// 刷新血条
	ui.progressBarSystemHP->setValue(systemPokemon->getCurrentBlood());
	ui.progressBarMyHP->setValue(myPokemon->getCurrentBlood());
	// 刷新文字
	ui.labelSystemHP->setText(QString::number(systemPokemon->getCurrentBlood()) +
		QString{"/"} + QString::number(systemPokemon->getMaxBlood()));
	ui.labelMyHP->setText(QString::number(myPokemon->getCurrentBlood()) +
		QString{"/"} + QString::number(myPokemon->getMaxBlood()));
}

// 停止战斗
void Fight::stopFight(FightResult fightResult_) {
	fightBGM.stop();
	// 停止攻击定时器
	systemAttackTimer->stop();
	myAttackTimer->stop();
	// 选择战斗结果
	if (fightResult_ == WIN) {
		QSound::play(":/Resources/Win.wav");
		// 胜利
		systemImg->stop();
		// 更新战斗信息、精灵信息
		myPokemon->incExp(50);
		System->updatePokemon(myPokemon->getName(), myPokemon);
		System->incFightWinNum();
		System->incFightAllNum();
		System->updateFightData();
		std::string resultStr{"恭喜你，挑战成功！"};
		// 决斗赛可以获取精灵
		if (this->fightType == DUEL) {
			// 拷贝精灵副本存储到背包
			srand(time(nullptr));
			Pokemon* newPokemon = systemPokemon->getClone();
			newPokemon->setName(System->getUserName().substr(0, 10) + std::to_string(rand()).substr(0, 5));
			newPokemon->incCurrentBlood(newPokemon->getMaxBlood());
			System->createPokemon(newPokemon);
			resultStr += ("\n您已获得对方的精灵，新名字为" + newPokemon->getName() + "。");
		}
		QMessageBox::information(this,
			QString::fromLocal8Bit("对战"),
			QString::fromLocal8Bit(resultStr.data()),
			QString::fromLocal8Bit("确定"));
	}
	else {
		// 失败
		QSound::play(":/Resources/Lost.wav");
		myImg->stop();
		if (this->fightType == UPGRADE) {
			// 更新战斗信息、精灵信息
			myPokemon->incExp(30);
			System->updatePokemon(myPokemon->getName(), myPokemon);
			System->incFightAllNum();
			System->updateFightData();
			QMessageBox::information(this,
				QString::fromLocal8Bit("对战"),
				QString::fromLocal8Bit("挑战失败，再接再厉！"),
				QString::fromLocal8Bit("确定"));
		}
		// 决斗赛送出精灵
		else {
			System->updatePokemon(myPokemon->getName(), myPokemon);
			SendOutPokemon* SendOutWindow = new SendOutPokemon(myPokemon);
			SendOutWindow->setWindowFlags(SendOutWindow->windowFlags() & ~Qt::WindowMinMaxButtonsHint);
			SendOutWindow->setWindowModality(Qt::ApplicationModal);
			SendOutWindow->show();
		}
	}
	this->close();
}

SendOutPokemon::SendOutPokemon(Pokemon* myPokemon_, QWidget* parent)
	: QWidget(parent), myPokemon(myPokemon_), currentImg(nullptr) {
	ui.setupUi(this);
	// 当前窗口关闭时自动析构
	setAttribute(Qt::WA_DeleteOnClose);
	// 精灵切换信号
	connect(ui.comboBoxPokemon, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshPokemonInfo()));
	connect(ui.commandLinkButtonSendOut, &QCommandLinkButton::clicked, this, &SendOutPokemon::onPressSendOut);
	// 随机加载三只精灵
	std::set<int> randSet = getRandSet(System->pokemonList.size());
	for (auto randVal : randSet) {
		sendOutList.push_back(System->pokemonList[randVal]);
	}
	for (auto pokemon : sendOutList) {
		ui.comboBoxPokemon->addItem(QString::fromLocal8Bit(pokemon->getName().data()));
	}
	// 设置焦点
	ui.comboBoxPokemon->setCurrentIndex(0);
	// 刷新精灵信息
	refreshPokemonInfo();
}

SendOutPokemon::~SendOutPokemon() {
	// 删除精灵图片
	if (this->currentImg) {
		delete currentImg;
	}
}

// 刷新精灵信息
void SendOutPokemon::refreshPokemonInfo() {
	// 获取当前选中精灵
	Pokemon* currentPokemon = System->pokemonList[ui.comboBoxPokemon->currentIndex()];
	// 刷新精灵信息
	ui.labelPokemonInfo->setText(QString::fromLocal8Bit(currentPokemon->toString().data()));
	// 删除旧图片，加载新图片
	if (this->currentImg) {
		delete currentImg;
	}
	currentImg = Pokemon_Stage_3_Client::selectImg(currentPokemon->getKind());
	ui.labelPokemonImg->setMovie(currentImg);
	currentImg->start();
}

std::set<int> SendOutPokemon::getRandSet(int num_) {
	std::set<int> randSet;
	srand(time(nullptr));
	while (randSet.size() < min(3, num_)) {
		int randVal = rand() % num_;
		if (randSet.find(randVal) == randSet.end()) {
			randSet.insert(randVal);
		}
	}
	return randSet;
}

// 送出精灵
void SendOutPokemon::onPressSendOut() {
	// 防作弊，点击按钮后再更新战斗信息、精灵信息
	// 若强制退出则本场战斗失效，不进行记录，但作为惩罚，精灵受到的伤害无法恢复
	myPokemon->incExp(30);
	System->updatePokemon(myPokemon->getName(), myPokemon);
	System->incFightAllNum();
	System->updateFightData();
	// 获取当前选中精灵
	Pokemon* currentPokemon = sendOutList[ui.comboBoxPokemon->currentIndex()];
	// 送出精灵
	System->deletePokemon(currentPokemon->getName());
	// 若精灵全部送出，则发放一个初级精灵
	if (System->pokemonList.empty()) {
		System->createPokemon(System->generateRandPokemon(System->getUserName().substr(0, 10) + std::to_string(rand()).substr(0, 5)));
		QApplication::beep();
		QMessageBox::information(this,
			QString::fromLocal8Bit("发放精灵"),
			QString::fromLocal8Bit("您的精灵已全部送出，系统已为你发放一只初级精灵。"),
			QString::fromLocal8Bit("确定"));
	}
	this->close();
}