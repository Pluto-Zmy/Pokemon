#include "LogIn.h"
#include "Pokemon_Stage_3_Client.h"
#include "SystemManager.h"

Pokemon_Stage_3_Client* MainWindow{nullptr};
extern SystemManager* System;

// 构造函数
LogIn::LogIn(QWidget* parent)
	: QWidget(parent) {
	ui.setupUi(this);
	// 当前窗口关闭时自动析构
	setAttribute(Qt::WA_DeleteOnClose);
	// 连接登录按钮信号
	connect(ui.pushButtonLogIn, &QPushButton::clicked, this, &LogIn::onPressLogIn);
	// 连接注册按钮信号
	connect(ui.pushButtonSignIn, &QPushButton::clicked, this, &LogIn::onPressSignIn);
	// 设置按钮快捷键
	ui.pushButtonLogIn->setShortcut(Qt::Key_Return);
	ui.pushButtonSignIn->setShortcut(Qt::Key_Return);
	// 限制用户名输入格式
	QRegExpValidator* regExpValidator = new QRegExpValidator(QRegExp("[a-zA-Z0-9]{25}"), this);
	ui.lineEditLogInUserName->setValidator(regExpValidator);
	ui.lineEditSignInUserName->setValidator(regExpValidator);
	// 光标焦点默认于用户名
	ui.lineEditLogInUserName->setFocus();
}

// 响应登录按钮点击事件
void LogIn::onPressLogIn() {
	// 用户名、密码
	std::string userName = ui.lineEditLogInUserName->text().toStdString();
	std::string passWord = ui.lineEditLogInPassWord->text().toStdString();
	// 检测输入框是否为空
	if (userName.empty() || passWord.empty()) {
		QApplication::beep();
		QMessageBox::warning(this,
			QString::fromLocal8Bit("登录"),
			QString::fromLocal8Bit("用户名或密码未输入。"),
			QString::fromLocal8Bit("确定"));
	}
	else {
		// 选择服务器回应的登录结果
		switch (System->logIn(userName, passWord)) {
			// 登录成功
			case LOG_IN_SUCCESS:
				// 设置当前用户名
				System->setUserName(userName);
				// 获取精灵列表
				System->pokemonList = System->getPokemonList(System->getUserName());
				// 关闭登录窗口、打开主界面
				this->close();
				MainWindow = new Pokemon_Stage_3_Client;
				MainWindow->show();
				break;
			// 登录失败-用户不存在
			case LOG_IN_FAIL_USER_NOT_EXIST:
				// 给出提示
				QApplication::beep();
				QMessageBox::warning(this,
					QString::fromLocal8Bit("登录"),
					QString::fromLocal8Bit("登录失败，该用户不存在。"),
					QString::fromLocal8Bit("确定"));
				break;
			// 登录失败-密码错误
			case LOG_IN_FAIL_PASSWORD_ERROR:
				// 给出提示
				QApplication::beep();
				QMessageBox::warning(this,
					QString::fromLocal8Bit("登录"),
					QString::fromLocal8Bit("登录失败，密码错误。"),
					QString::fromLocal8Bit("确定"));
				// 清除输入框中的旧密码
				ui.lineEditLogInPassWord->clear();
				break;
			// 登录失败-异地登录
			case LOG_IN_FAIL_USER_ONLINE:
				// 给出提示
				QApplication::beep();
				QMessageBox::warning(this,
					QString::fromLocal8Bit("登录"),
					QString::fromLocal8Bit("登录失败，该用户已经登录。"),
					QString::fromLocal8Bit("确定"));
				break;
			default:
				break;
		}
	}
}

// 响应注册按钮点击事件
void LogIn::onPressSignIn() {
	// 用户名、密码、重复密码
	std::string userName = ui.lineEditSignInUserName->text().toStdString();
	std::string passWord1 = ui.lineEditSignInPassWord1->text().toStdString();
	std::string passWord2 = ui.lineEditSignInPassWord2->text().toStdString();
	// 检测输入框是否为空
	if (userName.empty() || passWord1.empty() || passWord2.empty()) {
		QApplication::beep();
		QMessageBox::warning(this,
			QString::fromLocal8Bit("注册"),
			QString::fromLocal8Bit("用户名或密码未输入。"),
			QString::fromLocal8Bit("确定"));
	}
	else {
		// 检查两次输入的密码是否一致
		if (passWord1 != passWord2) {
			QApplication::beep();
			QMessageBox::warning(this,
				QString::fromLocal8Bit("注册"),
				QString::fromLocal8Bit("两次输入的密码不一致。"),
				QString::fromLocal8Bit("确定"));
			// 不一致则清空
			ui.lineEditSignInPassWord1->clear();
			ui.lineEditSignInPassWord2->clear();
		}
		else {
			// 选择服务器回应的注册结果
			switch (System->signIn(userName, passWord1)) {
				// 注册成功
				case SIGN_IN_SUCCESS:
					// 设置用户名
					System->setUserName(userName);
					// 随机生成 3 个精灵
					for (int i{1}; i <= 3; i++) {
						std::string name;
						name = userName.substr(0, 14) + std::to_string(i);
						System->createPokemon(System->generateRandPokemon(name));
					}
					QApplication::beep();
					QMessageBox::information(this,
						QString::fromLocal8Bit("注册"),
						QString::fromLocal8Bit("注册成功。"),
						QString::fromLocal8Bit("确定"));
					break;
					// 注册失败-用户名已被注册
				case SIGN_IN_FAIL:
					// 给出提示
					QApplication::beep();
					QMessageBox::warning(this,
						QString::fromLocal8Bit("注册"),
						QString::fromLocal8Bit("注册失败，该用户名已经被注册。"),
						QString::fromLocal8Bit("确定"));
					break;
				default:
					break;
			}
		}
	}
}