#pragma once
#include "ProgramLib.h"
#include "ui_LogIn.h"

// ��¼/ע�ᴰ��
class LogIn: public QWidget {
	Q_OBJECT
public:
	// ���졢��������
	LogIn(QWidget* parent = Q_NULLPTR);
	~LogIn() = default;
private:
	Ui::LogIn ui;
private slots:
	// ��Ӧ��¼��ť����¼�
	void onPressLogIn();
	// ��Ӧע�ᰴť����¼�
	void onPressSignIn();
};