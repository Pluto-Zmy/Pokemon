#pragma once
#include "ProgramLib.h"
#include "ui_ConnectServer.h"

// ���ӷ���������
class ConnectServer: public QWidget {
	Q_OBJECT
public:
	// ���졢��������
	ConnectServer(QWidget* parent = Q_NULLPTR);
	~ConnectServer() = default;
private:
	Ui::ConnectServer ui;
private slots:
	// ��Ӧȷ����ť����¼�
	void onPressOK();
	// ��Ӧ��������ť�л��¼�
	void setOtherIPAvailable();
};