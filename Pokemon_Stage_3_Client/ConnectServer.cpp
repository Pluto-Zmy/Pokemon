#include "ConnectServer.h"
#include "LogIn.h"
#include "SystemManager.h"

SystemManager* System{nullptr};
LogIn* LogInWindow{nullptr};

// 构造函数
ConnectServer::ConnectServer(QWidget* parent)
	: QWidget(parent) {
	ui.setupUi(this);
	// 连接确定按钮信号
	connect(ui.pushButtonOK, &QPushButton::clicked, this, &ConnectServer::onPressOK);
	// 连接服务器切换按钮信号
	connect(ui.radioButtonOther, &QRadioButton::toggled, this, &ConnectServer::setOtherIPAvailable);
	// 设置确定按钮的快捷键-回车
	ui.pushButtonOK->setShortcut(Qt::Key_Return);
	// 默认选中官方服务器按钮
	ui.radioButtonBeijing3->setChecked(true);
	// 禁用自定义服务器输入框
	ui.lineEditIP->setEnabled(false);
	// 设置连接进度条数值范围和初始值
	ui.progressBar->setRange(0, 100);
	ui.progressBar->setValue(0);
}

// 响应确定按钮点击事件
void ConnectServer::onPressOK() {
	// 根据服务器切换按钮选择 IP 地址值
	std::string IP;
	if (ui.radioButtonBeijing1->isChecked()) {
		IP = "118.89.246.223";
	}
	else if (ui.radioButtonBeijing3->isChecked()) {
		IP = "62.234.27.75";
	}
	else if (ui.radioButtonLocal->isChecked()) {
		IP = "127.0.0.1";
	}
	else {
		IP = ui.lineEditIP->text().toStdString();
	}
	// 根据 IP 地址初始化系统
	System = new SystemManager(IP);
	// 休眠 0.1 秒推进显示进度条，避免迅速连接到服务器，以提高真实性
	for (int i{0}; i <= 100; i++) {
		ui.progressBar->setValue(i);
		Sleep(1);
	}
	// 关闭当前窗口，显示登录/注册窗口
	this->close();
	LogInWindow = new LogIn;
	LogInWindow->show();
}

// 响应服务器按钮切换事件
void ConnectServer::setOtherIPAvailable() {
	// 自定义服务器按钮选中时才启用自定义服务器输入框，否则禁用
	ui.lineEditIP->setEnabled(ui.radioButtonOther->isChecked());
}