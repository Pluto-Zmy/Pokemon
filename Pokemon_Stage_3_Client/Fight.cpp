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
	// ˫��������ʱ�ź�
	connect(systemAttackTimer, &QTimer::timeout, this, &Fight::attackMe);
	connect(myAttackTimer, &QTimer::timeout, this, &Fight::attackSystem);
	// ֹͣս���ź�
	connect(this, &Fight::signalStopFight, this, &Fight::stopFight);
	// ��ʾ˫��������Ϣ
	ui.labelSystemName->setText(QString::fromLocal8Bit(systemPokemon->getName().data()));
	ui.labelMyName->setText(QString::fromLocal8Bit(myPokemon->getName().data()));
	ui.labelSystemPokemonImg->setMovie(systemImg);
	ui.labelMyPokemonImg->setMovie(myImg);
	ui.progressBarSystemHP->setRange(0, systemPokemon->getMaxBlood());
	ui.progressBarMyHP->setRange(0, myPokemon->getMaxBlood());
	fightBGM.setLoops(QSound::Infinite);
	refreshHP();
	// ��ʼս��
	startFight();
}

Fight::~Fight() {
	// ɾ����Դ
	delete systemImg;
	delete myImg;
	delete systemAttackTimer;
	delete myAttackTimer;
}

// ��ʼս��
void Fight::startFight() {
	// ˫�����鿪ʼ�˶�
	systemImg->start();
	myImg->start();
	// ����������ʱ��
	systemAttackTimer->start(static_cast<int>(1e6) / systemPokemon->getAttackSpeed());
	myAttackTimer->start(static_cast<int>(1e6) / myPokemon->getAttackSpeed());
	fightBGM.play();
}

// ˫����������
void Fight::attackMe() {
	// ��ȡ������ʽ
	AttackMethod attackMethod = systemPokemon->attackPokemon(*myPokemon);
	QSound::play(":/Resources/Attack.wav");
	// ˢ��Ѫ��
	refreshHP();
	// ˢ��ս����Ϣ
	if (attackMethod != INVALID) {
		ui.labelSystemAttackMethod->setText(QString::fromLocal8Bit(("�� " + std::to_string(attackTime++) + " �غϣ�" +
			System->attackMethodStr[attackMethod]).data()));
	}
	else {
		ui.labelMyAttackMethod->setText(QString::fromLocal8Bit(("�� " + std::to_string(attackTime++) + " �غϣ�" +
			System->attackMethodStr[attackMethod]).data()));
	}
	// ���Է�ս����ֹͣս��
	if (myPokemon->getCurrentBlood() == 0) {
		signalStopFight(LOST);
	}
}

// ͬ��
void Fight::attackSystem() {
	AttackMethod attackMethod = myPokemon->attackPokemon(*systemPokemon);
	QSound::play(":/Resources/Attack.wav");
	refreshHP();
	if (attackMethod != INVALID) {
		ui.labelMyAttackMethod->setText(QString::fromLocal8Bit(("�� " + std::to_string(attackTime++) + " �غϣ�" +
			System->attackMethodStr[attackMethod]).data()));
	}
	else {
		ui.labelSystemAttackMethod->setText(QString::fromLocal8Bit(("�� " + std::to_string(attackTime++) + " �غϣ�" +
			System->attackMethodStr[attackMethod]).data()));
	}
	if (systemPokemon->getCurrentBlood() == 0) {
		signalStopFight(WIN);
	}
}

// ˢ��˫������ֵ
void Fight::refreshHP() {
	// ˢ��Ѫ��
	ui.progressBarSystemHP->setValue(systemPokemon->getCurrentBlood());
	ui.progressBarMyHP->setValue(myPokemon->getCurrentBlood());
	// ˢ������
	ui.labelSystemHP->setText(QString::number(systemPokemon->getCurrentBlood()) +
		QString{"/"} + QString::number(systemPokemon->getMaxBlood()));
	ui.labelMyHP->setText(QString::number(myPokemon->getCurrentBlood()) +
		QString{"/"} + QString::number(myPokemon->getMaxBlood()));
}

// ֹͣս��
void Fight::stopFight(FightResult fightResult_) {
	fightBGM.stop();
	// ֹͣ������ʱ��
	systemAttackTimer->stop();
	myAttackTimer->stop();
	// ѡ��ս�����
	if (fightResult_ == WIN) {
		QSound::play(":/Resources/Win.wav");
		// ʤ��
		systemImg->stop();
		// ����ս����Ϣ��������Ϣ
		myPokemon->incExp(50);
		System->updatePokemon(myPokemon->getName(), myPokemon);
		System->incFightWinNum();
		System->incFightAllNum();
		System->updateFightData();
		std::string resultStr{"��ϲ�㣬��ս�ɹ���"};
		// ���������Ի�ȡ����
		if (this->fightType == DUEL) {
			// �������鸱���洢������
			srand(time(nullptr));
			Pokemon* newPokemon = systemPokemon->getClone();
			newPokemon->setName(System->getUserName().substr(0, 10) + std::to_string(rand()).substr(0, 5));
			newPokemon->incCurrentBlood(newPokemon->getMaxBlood());
			System->createPokemon(newPokemon);
			resultStr += ("\n���ѻ�öԷ��ľ��飬������Ϊ" + newPokemon->getName() + "��");
		}
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ս"),
			QString::fromLocal8Bit(resultStr.data()),
			QString::fromLocal8Bit("ȷ��"));
	}
	else {
		// ʧ��
		QSound::play(":/Resources/Lost.wav");
		myImg->stop();
		if (this->fightType == UPGRADE) {
			// ����ս����Ϣ��������Ϣ
			myPokemon->incExp(30);
			System->updatePokemon(myPokemon->getName(), myPokemon);
			System->incFightAllNum();
			System->updateFightData();
			QMessageBox::information(this,
				QString::fromLocal8Bit("��ս"),
				QString::fromLocal8Bit("��սʧ�ܣ��ٽ�������"),
				QString::fromLocal8Bit("ȷ��"));
		}
		// �������ͳ�����
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
	// ��ǰ���ڹر�ʱ�Զ�����
	setAttribute(Qt::WA_DeleteOnClose);
	// �����л��ź�
	connect(ui.comboBoxPokemon, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshPokemonInfo()));
	connect(ui.commandLinkButtonSendOut, &QCommandLinkButton::clicked, this, &SendOutPokemon::onPressSendOut);
	// ���������ֻ����
	std::set<int> randSet = getRandSet(System->pokemonList.size());
	for (auto randVal : randSet) {
		sendOutList.push_back(System->pokemonList[randVal]);
	}
	for (auto pokemon : sendOutList) {
		ui.comboBoxPokemon->addItem(QString::fromLocal8Bit(pokemon->getName().data()));
	}
	// ���ý���
	ui.comboBoxPokemon->setCurrentIndex(0);
	// ˢ�¾�����Ϣ
	refreshPokemonInfo();
}

SendOutPokemon::~SendOutPokemon() {
	// ɾ������ͼƬ
	if (this->currentImg) {
		delete currentImg;
	}
}

// ˢ�¾�����Ϣ
void SendOutPokemon::refreshPokemonInfo() {
	// ��ȡ��ǰѡ�о���
	Pokemon* currentPokemon = System->pokemonList[ui.comboBoxPokemon->currentIndex()];
	// ˢ�¾�����Ϣ
	ui.labelPokemonInfo->setText(QString::fromLocal8Bit(currentPokemon->toString().data()));
	// ɾ����ͼƬ��������ͼƬ
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

// �ͳ�����
void SendOutPokemon::onPressSendOut() {
	// �����ף������ť���ٸ���ս����Ϣ��������Ϣ
	// ��ǿ���˳��򱾳�ս��ʧЧ�������м�¼������Ϊ�ͷ��������ܵ����˺��޷��ָ�
	myPokemon->incExp(30);
	System->updatePokemon(myPokemon->getName(), myPokemon);
	System->incFightAllNum();
	System->updateFightData();
	// ��ȡ��ǰѡ�о���
	Pokemon* currentPokemon = sendOutList[ui.comboBoxPokemon->currentIndex()];
	// �ͳ�����
	System->deletePokemon(currentPokemon->getName());
	// ������ȫ���ͳ����򷢷�һ����������
	if (System->pokemonList.empty()) {
		System->createPokemon(System->generateRandPokemon(System->getUserName().substr(0, 10) + std::to_string(rand()).substr(0, 5)));
		QApplication::beep();
		QMessageBox::information(this,
			QString::fromLocal8Bit("���ž���"),
			QString::fromLocal8Bit("���ľ�����ȫ���ͳ���ϵͳ��Ϊ�㷢��һֻ�������顣"),
			QString::fromLocal8Bit("ȷ��"));
	}
	this->close();
}