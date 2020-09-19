#pragma once
#include "ProgramLib.h"
#include "ui_ConnectServer.h"

// 连接服务器窗口
class ConnectServer: public QWidget {
	Q_OBJECT
public:
	// 构造、析构函数
	ConnectServer(QWidget* parent = Q_NULLPTR);
	~ConnectServer() = default;
private:
	Ui::ConnectServer ui;
private slots:
	// 响应确定按钮点击事件
	void onPressOK();
	// 响应服务器按钮切换事件
	void setOtherIPAvailable();
};