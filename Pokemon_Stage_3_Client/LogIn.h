#pragma once
#include "ProgramLib.h"
#include "ui_LogIn.h"

// 登录/注册窗口
class LogIn: public QWidget {
	Q_OBJECT
public:
	// 构造、析构函数
	LogIn(QWidget* parent = Q_NULLPTR);
	~LogIn() = default;
private:
	Ui::LogIn ui;
private slots:
	// 响应登录按钮点击事件
	void onPressLogIn();
	// 响应注册按钮点击事件
	void onPressSignIn();
};