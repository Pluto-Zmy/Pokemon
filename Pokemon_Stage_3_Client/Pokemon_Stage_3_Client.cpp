#include "Pokemon_Stage_3_Client.h"
#include "LogIn.h"
#include "SystemManager.h"

extern SystemManager* System;

/***** Beginning of Class Pokemon_Stage_3_Client *****/
// 构造函数
Pokemon_Stage_3_Client::Pokemon_Stage_3_Client(QWidget* parent)
	: QMainWindow(parent), currentImg{nullptr} {
	ui.setupUi(this);
	// 当前窗口关闭时自动析构
	setAttribute(Qt::WA_DeleteOnClose);
	// Tab 标签切换信号
	connect(ui.tabWidget, &QTabWidget::currentChanged, this, &Pokemon_Stage_3_Client::refreshUserList);
	connect(ui.tabWidget, &QTabWidget::currentChanged, this, &Pokemon_Stage_3_Client::refreshMyPokemonList);
	connect(ui.tabWidget, &QTabWidget::currentChanged, this, &Pokemon_Stage_3_Client::refreshSystemPokemonList);
	// 所有/在线用户切换信号
	connect(ui.comboBoxUser, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshUserList()));
	// 刷新用户列表信号
	connect(ui.pushButtonRefreshUserList, &QPushButton::clicked, this, &Pokemon_Stage_3_Client::refreshUserList);
	// 用户切换信号
	connect(ui.listWidgetUser, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(refreshUserPokemonList()));
	// 用户精灵切换信号
	connect(ui.comboBoxPokemon, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshUserPokemonData()));
	// 系统精灵切换信号
	connect(ui.listWidgetSystemPokemonList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(refreshSystemPokemonData()));
	// 对战信号
	connect(ui.commandLinkButtonUpGradeFight, &QCommandLinkButton::clicked, this, &Pokemon_Stage_3_Client::onPressUpGradeFight);
	connect(ui.commandLinkButtonDuelFight, &QCommandLinkButton::clicked, this, &Pokemon_Stage_3_Client::onPressDuelFight);
	// 我的精灵切换信号
	connect(ui.listWidgetPokemonMy, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(refreshMyPokemonData()));
	// 退出登录按钮点击信号
	connect(ui.commandLinkButtonLogOut, &QCommandLinkButton::clicked, this, &Pokemon_Stage_3_Client::onPressLogOut);
	// 精灵改名按钮信号
	connect(ui.pushButtonChangePokemonName, &QPushButton::clicked, this, &Pokemon_Stage_3_Client::onPressChangePokemonName);
	// 精灵恢复按钮信号
	connect(ui.pushButtonRecover, &QPushButton::clicked, this, &Pokemon_Stage_3_Client::onPressRecover);
	// 限制精灵名输入格式
	QRegExpValidator* regExpValidator = new QRegExpValidator(QRegExp("[a-zA-Z0-9]{25}"), this);
	ui.lineEditNameMy->setValidator(regExpValidator);
	// 显示我的用户名
	ui.labelUserNameMy->setText(QString::fromLocal8Bit("欢迎，") + QString::fromLocal8Bit(System->getUserName().data()));
	// 刷新用户列表
	refreshUserList();
}

// 析构函数
Pokemon_Stage_3_Client::~Pokemon_Stage_3_Client() {
	// 释放当前精灵列表
	for (auto pokemon : currentPokemonList) {
		delete pokemon;
	}
	currentPokemonList.clear();
	// 清空精灵列表
	int listSize = ui.comboBoxPokemon->count();
	for (int index{0}; index < listSize; index++) {
		QListWidgetItem* delItem = ui.listWidgetPokemonMy->takeItem(0);
		delete delItem;
	}
	// 清空用户列表
	listSize = ui.listWidgetUser->count();
	for (int index{0}; index < listSize; index++) {
		QListWidgetItem* delItem = ui.listWidgetUser->takeItem(0);
		delete delItem;
	}
}

// 根据精灵类型选择头像
QMovie* Pokemon_Stage_3_Client::selectImg(PokemonKind kind_, QString sysStr_) {
	QMovie* pokemonImg{nullptr};
	// 选择精灵类型
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

// 选择宠物个数徽章
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

// 选择高级宠物徽章
QPixmap Pokemon_Stage_3_Client::selectAdvancePokemonMedal(std::vector<Pokemon*> pokemonList_) {
	// 获取高级宠物个数
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

// 刷新用户列表
void Pokemon_Stage_3_Client::refreshUserList() {
	// 判断是否在当前标签页，避免后台刷新
	if (ui.tabWidget->currentIndex() != 0) {
		return;
	}
	// 删除旧列表
	int listSize = ui.listWidgetUser->count();
	for (int index{0}; index < listSize; index++) {
		QListWidgetItem* delItem = ui.listWidgetUser->takeItem(0);
		delete delItem;
	}
	std::vector<std::string> userList;
	// 根据显示条件确定显示所有用户或在线用户
	if (ui.comboBoxUser->currentIndex() == 0) {
		userList = System->getAllUserList();
	}
	else if (ui.comboBoxUser->currentIndex() == 1) {
		userList = System->getOnlineUserList();
	}
	// 创建用户列表
	for (auto user : userList) {
		QListWidgetItem* newUser = new QListWidgetItem(QString::fromLocal8Bit(user.data()));
		newUser->setSizeHint(QSize(0, 24));
		ui.listWidgetUser->insertItem(ui.listWidgetUser->count(), newUser);
	}
	// 重新设置焦点
	ui.listWidgetUser->setCurrentRow(0);
	// 刷新精灵列表
	refreshUserPokemonList();
}

// 刷新用户精灵列表
void Pokemon_Stage_3_Client::refreshUserPokemonList() {
	// 获取当前所选用户
	currentUserName = ui.listWidgetUser->currentItem()->text().toStdString();
	// 清空当前精灵列表
	ui.comboBoxPokemon->clear();
	for (auto pokemon : currentPokemonList) {
		delete pokemon;
	}
	currentPokemonList.clear();
	// 加载当前精灵列表
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

// 刷新用户精灵数据
void Pokemon_Stage_3_Client::refreshUserPokemonData() {
	// 判断精灵列表是否为空，避免无效刷新
	if (ui.comboBoxPokemon->count() == 0) {
		return;
	}
	// 获取当前选中精灵
	Pokemon* currentPokemon = this->currentPokemonList[ui.comboBoxPokemon->currentIndex()];
	// 刷新精灵信息显示
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
	// 删除旧精灵图片
	if (this->currentImg) {
		delete currentImg;
	}
	// 加载精灵图片
	currentImg = selectImg(currentPokemon->getKind());
	ui.labelPokemonImg->setMovie(currentImg);
	currentImg->start();
}

// 刷新系统精灵列表
void Pokemon_Stage_3_Client::refreshSystemPokemonList() {
	// 判断是否在当前标签页，避免后台刷新
	if (ui.tabWidget->currentIndex() != 1) {
		return;
	}
	// 加载系统精灵列表
	if (ui.listWidgetSystemPokemonList->count() == 0) {
		for (auto pokemon : System->systemPokemonList) {
			QListWidgetItem* newPokemon = new QListWidgetItem(QString::fromLocal8Bit(pokemon->getName().data()));
			newPokemon->setSizeHint(QSize(0, 24));
			ui.listWidgetSystemPokemonList->insertItem(ui.listWidgetSystemPokemonList->count(), newPokemon);
		}
	}
	// 重新设置焦点
	ui.listWidgetSystemPokemonList->setCurrentRow(0);
	// 刷新精灵信息
	refreshSystemPokemonData();
}

// 刷新系统精灵信息
void Pokemon_Stage_3_Client::refreshSystemPokemonData() {
	// 获取当前选中精灵
	Pokemon* currentPokemon = System->systemPokemonList[ui.listWidgetSystemPokemonList->currentIndex().row()];
	// 刷新精灵信息显示
	ui.lineEditKindSystem->setText(QString::fromLocal8Bit(System->pokemonKindStr[currentPokemon->getKind()].data()));
	ui.lineEditLevelSystem->setText(QString::number(currentPokemon->getLevel()));
	ui.lineEditExpSystem->setText(QString::number(currentPokemon->getExp()));
	ui.lineEditAttackSystem->setText(QString::number(currentPokemon->getAttack()));
	ui.lineEditDefenseSystem->setText(QString::number(currentPokemon->getDefense()));
	ui.lineEditBloodSystem->setText(QString::fromLocal8Bit((std::to_string(currentPokemon->getCurrentBlood()) +
		"/" + std::to_string(currentPokemon->getMaxBlood())).data()));
	ui.lineEditAttackSpeedSystem->setText(QString::number(currentPokemon->getAttackSpeed()));
	ui.lineEditSkillSystem->setText(QString::fromLocal8Bit(System->attackMethodStr[currentPokemon->getAttackMethod()].data()));
	// 删除旧精灵图片
	if (this->currentImg) {
		delete currentImg;
	}
	// 加载精灵图片
	currentImg = selectImg(currentPokemon->getKind());
	ui.labelPokemonImgSystem->setMovie(currentImg);
	currentImg->start();
}

// 响应升级赛按钮点击信号
void Pokemon_Stage_3_Client::onPressUpGradeFight() {
	SelectPokemon* SelectWindow = new SelectPokemon(UPGRADE, System->systemPokemonList[ui.listWidgetSystemPokemonList->currentIndex().row()]);
	SelectWindow->setWindowFlags(SelectWindow->windowFlags() & ~Qt::WindowMinMaxButtonsHint);
	SelectWindow->setWindowModality(Qt::ApplicationModal);
	SelectWindow->show();
}

// 响应决斗赛按钮点击信号
void Pokemon_Stage_3_Client::onPressDuelFight() {
	SelectPokemon* SelectWindow = new SelectPokemon(DUEL, System->systemPokemonList[ui.listWidgetSystemPokemonList->currentIndex().row()]);
	SelectWindow->setWindowFlags(SelectWindow->windowFlags() & ~Qt::WindowMinMaxButtonsHint);
	SelectWindow->setWindowModality(Qt::ApplicationModal);
	SelectWindow->show();
}

// 刷新我的精灵列表
void Pokemon_Stage_3_Client::refreshMyPokemonList() {
	// 判断是否在当前标签页，避免后台刷新
	if (ui.tabWidget->currentIndex() != 2) {
		return;
	}
	// 删除旧列表
	int listSize = ui.listWidgetPokemonMy->count();
	for (int index{0}; index < listSize; index++) {
		QListWidgetItem* delItem = ui.listWidgetPokemonMy->takeItem(0);
		delete delItem;
	}
	for (auto pokemon : System->pokemonList) {
		delete pokemon;
	}
	System->pokemonList.clear();
	// 加载我的精灵列表
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
	// 重新设置焦点
	ui.listWidgetPokemonMy->setCurrentRow(0);
	// 刷新精灵信息
	refreshMyPokemonData();
}

// 刷新我的精灵数据
void Pokemon_Stage_3_Client::refreshMyPokemonData() {
	// 获取当前选中精灵
	Pokemon* currentPokemon = System->pokemonList[ui.listWidgetPokemonMy->currentIndex().row()];
	// 刷新精灵信息显示
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
	// 删除旧精灵图片
	if (this->currentImg) {
		delete currentImg;
	}
	// 加载精灵图片
	currentImg = selectImg(currentPokemon->getKind());
	ui.labelPokemonImgMy->setMovie(currentImg);
	currentImg->start();
}

// 响应退出登录按钮点击事件
void Pokemon_Stage_3_Client::onPressLogOut() {
	// 注销登录
	System->logOut();
	// 关闭游戏主窗口，显示登录/注册窗口
	this->close();
	LogIn* LogInWindow = new LogIn;
	LogInWindow->show();
}

// 响应精灵改名按钮点击事件
void Pokemon_Stage_3_Client::onPressChangePokemonName() {
	// 获取当前选中精灵
	int currentPokemonListRow = ui.listWidgetPokemonMy->currentRow();
	Pokemon* pokemon = System->pokemonList[currentPokemonListRow];
	// 获取精灵原名字
	std::string oldName = pokemon->getName();
	// 获取新名字
	std::string newName = ui.lineEditNameMy->text().toStdString();
	// 检查是否有重名
	bool nameExist{false};
	for (auto pokemon : System->pokemonList) {
		if (pokemon->getName() == newName) {
			nameExist = true;
		}
	}
	if (nameExist) {
		QApplication::beep();
		QMessageBox::warning(this,
			QString::fromLocal8Bit("改名"),
			QString::fromLocal8Bit("您输入的新名字与您已有精灵的名字重复，请重新取名字。"),
			QString::fromLocal8Bit("确定"));
	}
	else {
		// 修改精灵名字
		pokemon->setName(newName);
		// 更新精灵信息
		System->updatePokemon(oldName, pokemon);
		// 刷新精灵列表中的精灵名字
		ui.listWidgetPokemonMy->currentItem()->setText(QString::fromLocal8Bit(newName.data()));
	}
}

// 响应恢复按钮点击事件
void Pokemon_Stage_3_Client::onPressRecover() {
	// 获取当前选中精灵
	int currentPokemonListRow = ui.listWidgetPokemonMy->currentRow();
	Pokemon* pokemon = System->pokemonList[currentPokemonListRow];
	// 获取精灵名字
	std::string name = pokemon->getName();
	// 修改精灵生命值
	pokemon->incCurrentBlood(0.2 * pokemon->getMaxBlood());
	ui.lineEditBloodMy->setText(QString::fromLocal8Bit((std::to_string(pokemon->getCurrentBlood()) +
		"/" + std::to_string(pokemon->getMaxBlood())).data()));
	// 更新精灵信息
	System->updatePokemon(name, pokemon);
}
/***** Ending of Class Pokemon_Stage_3_Client *****/

/***** Beginning of Class SelectPokemon *****/
// 构造函数
SelectPokemon::SelectPokemon(FightType fightType_, Pokemon* systemPokemon_, QWidget* parent)
	: QWidget(parent), fightType(fightType_), systemPokemon(systemPokemon_), currentImg(nullptr) {
	ui.setupUi(this);
	// 当前窗口关闭时自动析构
	setAttribute(Qt::WA_DeleteOnClose);
	// 精灵切换信号
	connect(ui.comboBoxPokemon, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshPokemonInfo()));
	// 开战信号
	connect(ui.commandLinkButtonFight, &QCommandLinkButton::clicked, this, &SelectPokemon::onPressFight);
	// 加载我的精灵列表
	for (auto pokemon : System->pokemonList) {
		ui.comboBoxPokemon->addItem(QString::fromLocal8Bit(pokemon->getName().data()));
	}
	// 设置焦点
	ui.comboBoxPokemon->setCurrentIndex(0);
	// 刷新精灵信息
	refreshPokemonInfo();
}

// 析构函数
SelectPokemon::~SelectPokemon() {
	// 删除精灵图片
	if (this->currentImg) {
		delete currentImg;
	}
}

// 刷新精灵信息
void SelectPokemon::refreshPokemonInfo() {
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

// 响应开战按钮点击信号
void SelectPokemon::onPressFight() {
	Pokemon* currentPokemon = System->pokemonList[ui.comboBoxPokemon->currentIndex()];
	if (currentPokemon->getCurrentBlood() == 0) {
		QApplication::beep();
		QMessageBox::warning(this,
			QString::fromLocal8Bit("对战"),
			QString::fromLocal8Bit("您选择的出战精灵生命值不足，无法参加战斗。"),
			QString::fromLocal8Bit("确定"));
		return;
	}
	// 打开战斗界面
	Fight* FightWindow = new Fight(fightType, systemPokemon, System->pokemonList[ui.comboBoxPokemon->currentIndex()]);
	FightWindow->setWindowFlags(FightWindow->windowFlags() & ~Qt::WindowMinMaxButtonsHint);
	FightWindow->setWindowModality(Qt::ApplicationModal);
	FightWindow->show();
	// 关闭当前窗口
	this->close();
}
/***** Ending of Class SelectPokemon *****/