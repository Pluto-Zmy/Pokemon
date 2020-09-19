#include "ConnectServer.h"
#include "LogIn.h"
#include "SystemManager.h"

SystemManager* System{nullptr};
LogIn* LogInWindow{nullptr};

// ���캯��
ConnectServer::ConnectServer(QWidget* parent)
	: QWidget(parent) {
	ui.setupUi(this);
	// ����ȷ����ť�ź�
	connect(ui.pushButtonOK, &QPushButton::clicked, this, &ConnectServer::onPressOK);
	// ���ӷ������л���ť�ź�
	connect(ui.radioButtonOther, &QRadioButton::toggled, this, &ConnectServer::setOtherIPAvailable);
	// ����ȷ����ť�Ŀ�ݼ�-�س�
	ui.pushButtonOK->setShortcut(Qt::Key_Return);
	// Ĭ��ѡ�йٷ���������ť
	ui.radioButtonBeijing3->setChecked(true);
	// �����Զ�������������
	ui.lineEditIP->setEnabled(false);
	// �������ӽ�������ֵ��Χ�ͳ�ʼֵ
	ui.progressBar->setRange(0, 100);
	ui.progressBar->setValue(0);
}

// ��Ӧȷ����ť����¼�
void ConnectServer::onPressOK() {
	// ���ݷ������л���ťѡ�� IP ��ֵַ
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
	// ���� IP ��ַ��ʼ��ϵͳ
	System = new SystemManager(IP);
	// ���� 0.1 ���ƽ���ʾ������������Ѹ�����ӵ����������������ʵ��
	for (int i{0}; i <= 100; i++) {
		ui.progressBar->setValue(i);
		Sleep(1);
	}
	// �رյ�ǰ���ڣ���ʾ��¼/ע�ᴰ��
	this->close();
	LogInWindow = new LogIn;
	LogInWindow->show();
}

// ��Ӧ��������ť�л��¼�
void ConnectServer::setOtherIPAvailable() {
	// �Զ����������ťѡ��ʱ�������Զ������������򣬷������
	ui.lineEditIP->setEnabled(ui.radioButtonOther->isChecked());
}