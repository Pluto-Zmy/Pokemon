#include "Pokemon_Stage_3_Client.h"
#include "LogIn.h"
#include "SystemManager.h"

extern SystemManager* System;

/***** Beginning of Class Pokemon_Stage_3_Client *****/
// ���캯��
Pokemon_Stage_3_Client::Pokemon_Stage_3_Client(QWidget* parent)
	: QMainWindow(parent), currentImg{nullptr} {
	ui.setupUi(this);
	// ��ǰ���ڹر�ʱ�Զ�����
	setAttribute(Qt::WA_DeleteOnClose);
	// Tab ��ǩ�л��ź�
	connect(ui.tabWidget, &QTabWidget::currentChanged, this, &Pokemon_Stage_3_Client::refreshUserList);
	connect(ui.tabWidget, &QTabWidget::currentChanged, this, &Pokemon_Stage_3_Client::refreshMyPokemonList);
	connect(ui.tabWidget, &QTabWidget::currentChanged, this, &Pokemon_Stage_3_Client::refreshSystemPokemonList);
	// ����/�����û��л��ź�
	connect(ui.comboBoxUser, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshUserList()));
	// ˢ���û��б��ź�
	connect(ui.pushButtonRefreshUserList, &QPushButton::clicked, this, &Pokemon_Stage_3_Client::refreshUserList);
	// �û��л��ź�
	connect(ui.listWidgetUser, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(refreshUserPokemonList()));
	// �û������л��ź�
	connect(ui.comboBoxPokemon, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshUserPokemonData()));
	// ϵͳ�����л��ź�
	connect(ui.listWidgetSystemPokemonList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(refreshSystemPokemonData()));
	// ��ս�ź�
	connect(ui.commandLinkButtonUpGradeFight, &QCommandLinkButton::clicked, this, &Pokemon_Stage_3_Client::onPressUpGradeFight);
	connect(ui.commandLinkButtonDuelFight, &QCommandLinkButton::clicked, this, &Pokemon_Stage_3_Client::onPressDuelFight);
	// �ҵľ����л��ź�
	connect(ui.listWidgetPokemonMy, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(refreshMyPokemonData()));
	// �˳���¼��ť����ź�
	connect(ui.commandLinkButtonLogOut, &QCommandLinkButton::clicked, this, &Pokemon_Stage_3_Client::onPressLogOut);
	// ���������ť�ź�
	connect(ui.pushButtonChangePokemonName, &QPushButton::clicked, this, &Pokemon_Stage_3_Client::onPressChangePokemonName);
	// ����ָ���ť�ź�
	connect(ui.pushButtonRecover, &QPushButton::clicked, this, &Pokemon_Stage_3_Client::onPressRecover);
	// ���ƾ����������ʽ
	QRegExpValidator* regExpValidator = new QRegExpValidator(QRegExp("[a-zA-Z0-9]{25}"), this);
	ui.lineEditNameMy->setValidator(regExpValidator);
	// ��ʾ�ҵ��û���
	ui.labelUserNameMy->setText(QString::fromLocal8Bit("��ӭ��") + QString::fromLocal8Bit(System->getUserName().data()));
	// ˢ���û��б�
	refreshUserList();
}

// ��������
Pokemon_Stage_3_Client::~Pokemon_Stage_3_Client() {
	// �ͷŵ�ǰ�����б�
	for (auto pokemon : currentPokemonList) {
		delete pokemon;
	}
	currentPokemonList.clear();
	// ��վ����б�
	int listSize = ui.comboBoxPokemon->count();
	for (int index{0}; index < listSize; index++) {
		QListWidgetItem* delItem = ui.listWidgetPokemonMy->takeItem(0);
		delete delItem;
	}
	// ����û��б�
	listSize = ui.listWidgetUser->count();
	for (int index{0}; index < listSize; index++) {
		QListWidgetItem* delItem = ui.listWidgetUser->takeItem(0);
		delete delItem;
	}
}

// ���ݾ�������ѡ��ͷ��
QMovie* Pokemon_Stage_3_Client::selectImg(PokemonKind kind_, QString sysStr_) {
	QMovie* pokemonImg{nullptr};
	// ѡ��������
	switch (kind_) {
		case HIGH_ATTACK:
			pokemonImg = new QMovie(":/Resources/HighAttackImg" + sysStr_ + ".gif");
			break;
		case HIGH_BLOOD:
			pokemonImg = new QMovie(":/Resources/HighBloodImg" + sysStr_ + ".gif");
			break;
		case HIGH_DEFENSE:
			pokemonImg = new QMovie(":/Resources/HighDefenseImg" + sysStr_ + ".gif");
			break;
		case HIGH_SPEED:
			pokemonImg = new QMovie(":/Resources/HighSpeedImg" + sysStr_ + ".gif");
			break;
		default:
			pokemonImg = new QMovie();
			break;
	}
	return pokemonImg;
}

// ѡ������������
QPixmap Pokemon_Stage_3_Client::selectPokemonNumMedal(int pokemonNum_) {
	if (pokemonNum_ <= 3) {
		return QPixmap(":/Resources/None.png");
	}
	else if (pokemonNum_ > 3 && pokemonNum_ <= 5) {
		return QPixmap(":/Resources/Bronze.png");
	}
	else if (pokemonNum_ > 5 && pokemonNum_ <= 10) {
		return QPixmap(":/Resources/Silver.png");
	}
	else {
		return QPixmap(":/Resources/Golden.png");
	}
}

// ѡ��߼��������
QPixmap Pokemon_Stage_3_Client::selectAdvancePokemonMedal(std::vector<Pokemon*> pokemonList_) {
	// ��ȡ�߼��������
	int advancePokemonNum{0};
	for (auto pokemon : pokemonList_) {
		if (pokemon->getLevel() == 15) {
			advancePokemonNum++;
		}
	}
	if (advancePokemonNum == 0) {
		return QPixmap(":/Resources/None.png");
	}
	else if (advancePokemonNum > 0 && advancePokemonNum <= 1) {
		return QPixmap(":/Resources/Bronze.png");
	}
	else if (advancePokemonNum > 1 && advancePokemonNum <= 5) {
		return QPixmap(":/Resources/Silver.png");
	}
	else {
		return QPixmap(":/Resources/Golden.png");
	}
}

// ˢ���û��б�
void Pokemon_Stage_3_Client::refreshUserList() {
	// �ж��Ƿ��ڵ�ǰ��ǩҳ�������̨ˢ��
	if (ui.tabWidget->currentIndex() != 0) {
		return;
	}
	// ɾ�����б�
	int listSize = ui.listWidgetUser->count();
	for (int index{0}; index < listSize; index++) {
		QListWidgetItem* delItem = ui.listWidgetUser->takeItem(0);
		delete delItem;
	}
	std::vector<std::string> userList;
	// ������ʾ����ȷ����ʾ�����û��������û�
	if (ui.comboBoxUser->currentIndex() == 0) {
		userList = System->getAllUserList();
	}
	else if (ui.comboBoxUser->currentIndex() == 1) {
		userList = System->getOnlineUserList();
	}
	// �����û��б�
	for (auto user : userList) {
		QListWidgetItem* newUser = new QListWidgetItem(QString::fromLocal8Bit(user.data()));
		newUser->setSizeHint(QSize(0, 24));
		ui.listWidgetUser->insertItem(ui.listWidgetUser->count(), newUser);
	}
	// �������ý���
	ui.listWidgetUser->setCurrentRow(0);
	// ˢ�¾����б�
	refreshUserPokemonList();
}

// ˢ���û������б�
void Pokemon_Stage_3_Client::refreshUserPokemonList() {
	// ��ȡ��ǰ��ѡ�û�
	currentUserName = ui.listWidgetUser->currentItem()->text().toStdString();
	// ��յ�ǰ�����б�
	ui.comboBoxPokemon->clear();
	for (auto pokemon : currentPokemonList) {
		delete pokemon;
	}
	currentPokemonList.clear();
	// ���ص�ǰ�����б�
	for (auto pokemon : System->getPokemonList(currentUserName)) {
		currentPokemonList.push_back(pokemon);
	}
	for (auto pokemon : currentPokemonList) {
		ui.comboBoxPokemon->addItem(QString::fromLocal8Bit(pokemon->getName().data()));
	}
	ui.labelPokemonNum->setPixmap(selectPokemonNumMedal(currentPokemonList.size()));
	ui.labelAdvancePokemon->setPixmap(selectAdvancePokemonMedal(currentPokemonList));
	UserFightData fightData = System->getFightData(currentUserName);
	ui.labelWinRate->setText(((fightData.fightAllNum == 0) ?
		"--" : (QString::number(100 * static_cast<double>(fightData.fightWinNum) / static_cast<double>(fightData.fightAllNum), 'f', 1) + "%")) + "\n" +
		QString::number(fightData.fightWinNum) + "/" + QString::number(fightData.fightAllNum));
}

// ˢ���û���������
void Pokemon_Stage_3_Client::refreshUserPokemonData() {
	// �жϾ����б��Ƿ�Ϊ�գ�������Чˢ��
	if (ui.comboBoxPokemon->count() == 0) {
		return;
	}
	// ��ȡ��ǰѡ�о���
	Pokemon* currentPokemon = this->currentPokemonList[ui.comboBoxPokemon->currentIndex()];
	// ˢ�¾�����Ϣ��ʾ
	ui.labelUserName->setText(QString::fromLocal8Bit(currentUserName.data()));
	ui.lineEditOwner->setText(QString::fromLocal8Bit(currentUserName.data()));
	ui.lineEditName->setText(QString::fromLocal8Bit(currentPokemon->getName().data()));
	ui.lineEditKind->setText(QString::fromLocal8Bit(System->pokemonKindStr[currentPokemon->getKind()].data()));
	ui.lineEditLevel->setText(QString::number(currentPokemon->getLevel()));
	ui.lineEditExp->setText(QString::number(currentPokemon->getExp()));
	ui.lineEditAttack->setText(QString::number(currentPokemon->getAttack()));
	ui.lineEditDefense->setText(QString::number(currentPokemon->getDefense()));
	ui.lineEditBlood->setText(QString::fromLocal8Bit((std::to_string(currentPokemon->getCurrentBlood()) +
		"/" + std::to_string(currentPokemon->getMaxBlood())).data()));
	ui.lineEditAttackSpeed->setText(QString::number(currentPokemon->getAttackSpeed()));
	ui.lineEditSkill->setText(QString::fromLocal8Bit(System->attackMethodStr[currentPokemon->getAttackMethod()].data()));
	// ɾ���ɾ���ͼƬ
	if (this->currentImg) {
		delete currentImg;
	}
	// ���ؾ���ͼƬ
	currentImg = selectImg(currentPokemon->getKind());
	ui.labelPokemonImg->setMovie(currentImg);
	currentImg->start();
}

// ˢ��ϵͳ�����б�
void Pokemon_Stage_3_Client::refreshSystemPokemonList() {
	// �ж��Ƿ��ڵ�ǰ��ǩҳ�������̨ˢ��
	if (ui.tabWidget->currentIndex() != 1) {
		return;
	}
	// ����ϵͳ�����б�
	if (ui.listWidgetSystemPokemonList->count() == 0) {
		for (auto pokemon : System->systemPokemonList) {
			QListWidgetItem* newPokemon = new QListWidgetItem(QString::fromLocal8Bit(pokemon->getName().data()));
			newPokemon->setSizeHint(QSize(0, 24));
			ui.listWidgetSystemPokemonList->insertItem(ui.listWidgetSystemPokemonList->count(), newPokemon);
		}
	}
	// �������ý���
	ui.listWidgetSystemPokemonList->setCurrentRow(0);
	// ˢ�¾�����Ϣ
	refreshSystemPokemonData();
}

// ˢ��ϵͳ������Ϣ
void Pokemon_Stage_3_Client::refreshSystemPokemonData() {
	// ��ȡ��ǰѡ�о���
	Pokemon* currentPokemon = System->systemPokemonList[ui.listWidgetSystemPokemonList->currentIndex().row()];
	// ˢ�¾�����Ϣ��ʾ
	ui.lineEditKindSystem->setText(QString::fromLocal8Bit(System->pokemonKindStr[currentPokemon->getKind()].data()));
	ui.lineEditLevelSystem->setText(QString::number(currentPokemon->getLevel()));
	ui.lineEditExpSystem->setText(QString::number(currentPokemon->getExp()));
	ui.lineEditAttackSystem->setText(QString::number(currentPokemon->getAttack()));
	ui.lineEditDefenseSystem->setText(QString::number(currentPokemon->getDefense()));
	ui.lineEditBloodSystem->setText(QString::fromLocal8Bit((std::to_string(currentPokemon->getCurrentBlood()) +
		"/" + std::to_string(currentPokemon->getMaxBlood())).data()));
	ui.lineEditAttackSpeedSystem->setText(QString::number(currentPokemon->getAttackSpeed()));
	ui.lineEditSkillSystem->setText(QString::fromLocal8Bit(System->attackMethodStr[currentPokemon->getAttackMethod()].data()));
	// ɾ���ɾ���ͼƬ
	if (this->currentImg) {
		delete currentImg;
	}
	// ���ؾ���ͼƬ
	currentImg = selectImg(currentPokemon->getKind());
	ui.labelPokemonImgSystem->setMovie(currentImg);
	currentImg->start();
}

// ��Ӧ��������ť����ź�
void Pokemon_Stage_3_Client::onPressUpGradeFight() {
	SelectPokemon* SelectWindow = new SelectPokemon(UPGRADE, System->systemPokemonList[ui.listWidgetSystemPokemonList->currentIndex().row()]);
	SelectWindow->setWindowFlags(SelectWindow->windowFlags() & ~Qt::WindowMinMaxButtonsHint);
	SelectWindow->setWindowModality(Qt::ApplicationModal);
	SelectWindow->show();
}

// ��Ӧ��������ť����ź�
void Pokemon_Stage_3_Client::onPressDuelFight() {
	SelectPokemon* SelectWindow = new SelectPokemon(DUEL, System->systemPokemonList[ui.listWidgetSystemPokemonList->currentIndex().row()]);
	SelectWindow->setWindowFlags(SelectWindow->windowFlags() & ~Qt::WindowMinMaxButtonsHint);
	SelectWindow->setWindowModality(Qt::ApplicationModal);
	SelectWindow->show();
}

// ˢ���ҵľ����б�
void Pokemon_Stage_3_Client::refreshMyPokemonList() {
	// �ж��Ƿ��ڵ�ǰ��ǩҳ�������̨ˢ��
	if (ui.tabWidget->currentIndex() != 2) {
		return;
	}
	// ɾ�����б�
	int listSize = ui.listWidgetPokemonMy->count();
	for (int index{0}; index < listSize; index++) {
		QListWidgetItem* delItem = ui.listWidgetPokemonMy->takeItem(0);
		delete delItem;
	}
	for (auto pokemon : System->pokemonList) {
		delete pokemon;
	}
	System->pokemonList.clear();
	// �����ҵľ����б�
	for (auto pokemon : System->getPokemonList(System->getUserName())) {
		System->pokemonList.push_back(pokemon);
	}
	for (auto pokemon : System->pokemonList) {
		QListWidgetItem* newPokemon = new QListWidgetItem(QString::fromLocal8Bit(pokemon->getName().data()));
		newPokemon->setSizeHint(QSize(0, 24));
		ui.listWidgetPokemonMy->insertItem(ui.listWidgetPokemonMy->count(), newPokemon);
	}
	ui.labelPokemonNumMy->setPixmap(selectPokemonNumMedal(System->pokemonList.size()));
	ui.labelAdvancePokemonMy->setPixmap(selectAdvancePokemonMedal(System->pokemonList));
	ui.labelWinRateMy->setText(((System->getFightAllNum() == 0) ?
		"--" : (QString::number(100 * static_cast<double>(System->getFightWinNum()) / static_cast<double>(System->getFightAllNum()), 'f', 1) + "%")) + "\n" +
		QString::number(System->getFightWinNum()) + "/" + QString::number(System->getFightAllNum()));
	// �������ý���
	ui.listWidgetPokemonMy->setCurrentRow(0);
	// ˢ�¾�����Ϣ
	refreshMyPokemonData();
}

// ˢ���ҵľ�������
void Pokemon_Stage_3_Client::refreshMyPokemonData() {
	// ��ȡ��ǰѡ�о���
	Pokemon* currentPokemon = System->pokemonList[ui.listWidgetPokemonMy->currentIndex().row()];
	// ˢ�¾�����Ϣ��ʾ
	ui.lineEditOwnerMy->setText(QString::fromLocal8Bit(System->getUserName().data()));
	ui.lineEditNameMy->setText(QString::fromLocal8Bit(currentPokemon->getName().data()));
	ui.lineEditKindMy->setText(QString::fromLocal8Bit(System->pokemonKindStr[currentPokemon->getKind()].data()));
	ui.lineEditLevelMy->setText(QString::number(currentPokemon->getLevel()));
	ui.lineEditExpMy->setText(QString::number(currentPokemon->getExp()));
	ui.lineEditAttackMy->setText(QString::number(currentPokemon->getAttack()));
	ui.lineEditDefenseMy->setText(QString::number(currentPokemon->getDefense()));
	ui.lineEditBloodMy->setText(QString::fromLocal8Bit((std::to_string(currentPokemon->getCurrentBlood()) +
		"/" + std::to_string(currentPokemon->getMaxBlood())).data()));
	ui.lineEditAttackSpeedMy->setText(QString::number(currentPokemon->getAttackSpeed()));
	ui.lineEditSkillMy->setText(QString::fromLocal8Bit(System->attackMethodStr[currentPokemon->getAttackMethod()].data()));
	// ɾ���ɾ���ͼƬ
	if (this->currentImg) {
		delete currentImg;
	}
	// ���ؾ���ͼƬ
	currentImg = selectImg(currentPokemon->getKind());
	ui.labelPokemonImgMy->setMovie(currentImg);
	currentImg->start();
}

// ��Ӧ�˳���¼��ť����¼�
void Pokemon_Stage_3_Client::onPressLogOut() {
	// ע����¼
	System->logOut();
	// �ر���Ϸ�����ڣ���ʾ��¼/ע�ᴰ��
	this->close();
	LogIn* LogInWindow = new LogIn;
	LogInWindow->show();
}

// ��Ӧ���������ť����¼�
void Pokemon_Stage_3_Client::onPressChangePokemonName() {
	// ��ȡ��ǰѡ�о���
	int currentPokemonListRow = ui.listWidgetPokemonMy->currentRow();
	Pokemon* pokemon = System->pokemonList[currentPokemonListRow];
	// ��ȡ����ԭ����
	std::string oldName = pokemon->getName();
	// ��ȡ������
	std::string newName = ui.lineEditNameMy->text().toStdString();
	// ����Ƿ�������
	bool nameExist{false};
	for (auto pokemon : System->pokemonList) {
		if (pokemon->getName() == newName) {
			nameExist = true;
		}
	}
	if (nameExist) {
		QApplication::beep();
		QMessageBox::warning(this,
			QString::fromLocal8Bit("����"),
			QString::fromLocal8Bit("��������������������о���������ظ���������ȡ���֡�"),
			QString::fromLocal8Bit("ȷ��"));
	}
	else {
		// �޸ľ�������
		pokemon->setName(newName);
		// ���¾�����Ϣ
		System->updatePokemon(oldName, pokemon);
		// ˢ�¾����б��еľ�������
		ui.listWidgetPokemonMy->currentItem()->setText(QString::fromLocal8Bit(newName.data()));
	}
}

// ��Ӧ�ָ���ť����¼�
void Pokemon_Stage_3_Client::onPressRecover() {
	// ��ȡ��ǰѡ�о���
	int currentPokemonListRow = ui.listWidgetPokemonMy->currentRow();
	Pokemon* pokemon = System->pokemonList[currentPokemonListRow];
	// ��ȡ��������
	std::string name = pokemon->getName();
	// �޸ľ�������ֵ
	pokemon->incCurrentBlood(0.2 * pokemon->getMaxBlood());
	ui.lineEditBloodMy->setText(QString::fromLocal8Bit((std::to_string(pokemon->getCurrentBlood()) +
		"/" + std::to_string(pokemon->getMaxBlood())).data()));
	// ���¾�����Ϣ
	System->updatePokemon(name, pokemon);
}
/***** Ending of Class Pokemon_Stage_3_Client *****/

/***** Beginning of Class SelectPokemon *****/
// ���캯��
SelectPokemon::SelectPokemon(FightType fightType_, Pokemon* systemPokemon_, QWidget* parent)
	: QWidget(parent), fightType(fightType_), systemPokemon(systemPokemon_), currentImg(nullptr) {
	ui.setupUi(this);
	// ��ǰ���ڹر�ʱ�Զ�����
	setAttribute(Qt::WA_DeleteOnClose);
	// �����л��ź�
	connect(ui.comboBoxPokemon, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshPokemonInfo()));
	// ��ս�ź�
	connect(ui.commandLinkButtonFight, &QCommandLinkButton::clicked, this, &SelectPokemon::onPressFight);
	// �����ҵľ����б�
	for (auto pokemon : System->pokemonList) {
		ui.comboBoxPokemon->addItem(QString::fromLocal8Bit(pokemon->getName().data()));
	}
	// ���ý���
	ui.comboBoxPokemon->setCurrentIndex(0);
	// ˢ�¾�����Ϣ
	refreshPokemonInfo();
}

// ��������
SelectPokemon::~SelectPokemon() {
	// ɾ������ͼƬ
	if (this->currentImg) {
		delete currentImg;
	}
}

// ˢ�¾�����Ϣ
void SelectPokemon::refreshPokemonInfo() {
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

// ��Ӧ��ս��ť����ź�
void SelectPokemon::onPressFight() {
	Pokemon* currentPokemon = System->pokemonList[ui.comboBoxPokemon->currentIndex()];
	if (currentPokemon->getCurrentBlood() == 0) {
		QApplication::beep();
		QMessageBox::warning(this,
			QString::fromLocal8Bit("��ս"),
			QString::fromLocal8Bit("��ѡ��ĳ�ս��������ֵ���㣬�޷��μ�ս����"),
			QString::fromLocal8Bit("ȷ��"));
		return;
	}
	// ��ս������
	Fight* FightWindow = new Fight(fightType, systemPokemon, System->pokemonList[ui.comboBoxPokemon->currentIndex()]);
	FightWindow->setWindowFlags(FightWindow->windowFlags() & ~Qt::WindowMinMaxButtonsHint);
	FightWindow->setWindowModality(Qt::ApplicationModal);
	FightWindow->show();
	// �رյ�ǰ����
	this->close();
}
/***** Ending of Class SelectPokemon *****/