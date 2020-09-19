#include "LogIn.h"
#include "Pokemon_Stage_3_Client.h"
#include "SystemManager.h"

Pokemon_Stage_3_Client* MainWindow{nullptr};
extern SystemManager* System;

// ���캯��
LogIn::LogIn(QWidget* parent)
	: QWidget(parent) {
	ui.setupUi(this);
	// ��ǰ���ڹر�ʱ�Զ�����
	setAttribute(Qt::WA_DeleteOnClose);
	// ���ӵ�¼��ť�ź�
	connect(ui.pushButtonLogIn, &QPushButton::clicked, this, &LogIn::onPressLogIn);
	// ����ע�ᰴť�ź�
	connect(ui.pushButtonSignIn, &QPushButton::clicked, this, &LogIn::onPressSignIn);
	// ���ð�ť��ݼ�
	ui.pushButtonLogIn->setShortcut(Qt::Key_Return);
	ui.pushButtonSignIn->setShortcut(Qt::Key_Return);
	// �����û��������ʽ
	QRegExpValidator* regExpValidator = new QRegExpValidator(QRegExp("[a-zA-Z0-9]{25}"), this);
	ui.lineEditLogInUserName->setValidator(regExpValidator);
	ui.lineEditSignInUserName->setValidator(regExpValidator);
	// ��꽹��Ĭ�����û���
	ui.lineEditLogInUserName->setFocus();
}

// ��Ӧ��¼��ť����¼�
void LogIn::onPressLogIn() {
	// �û���������
	std::string userName = ui.lineEditLogInUserName->text().toStdString();
	std::string passWord = ui.lineEditLogInPassWord->text().toStdString();
	// ���������Ƿ�Ϊ��
	if (userName.empty() || passWord.empty()) {
		QApplication::beep();
		QMessageBox::warning(this,
			QString::fromLocal8Bit("��¼"),
			QString::fromLocal8Bit("�û���������δ���롣"),
			QString::fromLocal8Bit("ȷ��"));
	}
	else {
		// ѡ���������Ӧ�ĵ�¼���
		switch (System->logIn(userName, passWord)) {
			// ��¼�ɹ�
			case LOG_IN_SUCCESS:
				// ���õ�ǰ�û���
				System->setUserName(userName);
				// ��ȡ�����б�
				System->pokemonList = System->getPokemonList(System->getUserName());
				// �رյ�¼���ڡ���������
				this->close();
				MainWindow = new Pokemon_Stage_3_Client;
				MainWindow->show();
				break;
			// ��¼ʧ��-�û�������
			case LOG_IN_FAIL_USER_NOT_EXIST:
				// ������ʾ
				QApplication::beep();
				QMessageBox::warning(this,
					QString::fromLocal8Bit("��¼"),
					QString::fromLocal8Bit("��¼ʧ�ܣ����û������ڡ�"),
					QString::fromLocal8Bit("ȷ��"));
				break;
			// ��¼ʧ��-�������
			case LOG_IN_FAIL_PASSWORD_ERROR:
				// ������ʾ
				QApplication::beep();
				QMessageBox::warning(this,
					QString::fromLocal8Bit("��¼"),
					QString::fromLocal8Bit("��¼ʧ�ܣ��������"),
					QString::fromLocal8Bit("ȷ��"));
				// ���������еľ�����
				ui.lineEditLogInPassWord->clear();
				break;
			// ��¼ʧ��-��ص�¼
			case LOG_IN_FAIL_USER_ONLINE:
				// ������ʾ
				QApplication::beep();
				QMessageBox::warning(this,
					QString::fromLocal8Bit("��¼"),
					QString::fromLocal8Bit("��¼ʧ�ܣ����û��Ѿ���¼��"),
					QString::fromLocal8Bit("ȷ��"));
				break;
			default:
				break;
		}
	}
}

// ��Ӧע�ᰴť����¼�
void LogIn::onPressSignIn() {
	// �û��������롢�ظ�����
	std::string userName = ui.lineEditSignInUserName->text().toStdString();
	std::string passWord1 = ui.lineEditSignInPassWord1->text().toStdString();
	std::string passWord2 = ui.lineEditSignInPassWord2->text().toStdString();
	// ���������Ƿ�Ϊ��
	if (userName.empty() || passWord1.empty() || passWord2.empty()) {
		QApplication::beep();
		QMessageBox::warning(this,
			QString::fromLocal8Bit("ע��"),
			QString::fromLocal8Bit("�û���������δ���롣"),
			QString::fromLocal8Bit("ȷ��"));
	}
	else {
		// �����������������Ƿ�һ��
		if (passWord1 != passWord2) {
			QApplication::beep();
			QMessageBox::warning(this,
				QString::fromLocal8Bit("ע��"),
				QString::fromLocal8Bit("������������벻һ�¡�"),
				QString::fromLocal8Bit("ȷ��"));
			// ��һ�������
			ui.lineEditSignInPassWord1->clear();
			ui.lineEditSignInPassWord2->clear();
		}
		else {
			// ѡ���������Ӧ��ע����
			switch (System->signIn(userName, passWord1)) {
				// ע��ɹ�
				case SIGN_IN_SUCCESS:
					// �����û���
					System->setUserName(userName);
					// ������� 3 ������
					for (int i{1}; i <= 3; i++) {
						std::string name;
						name = userName.substr(0, 14) + std::to_string(i);
						System->createPokemon(System->generateRandPokemon(name));
					}
					QApplication::beep();
					QMessageBox::information(this,
						QString::fromLocal8Bit("ע��"),
						QString::fromLocal8Bit("ע��ɹ���"),
						QString::fromLocal8Bit("ȷ��"));
					break;
					// ע��ʧ��-�û����ѱ�ע��
				case SIGN_IN_FAIL:
					// ������ʾ
					QApplication::beep();
					QMessageBox::warning(this,
						QString::fromLocal8Bit("ע��"),
						QString::fromLocal8Bit("ע��ʧ�ܣ����û����Ѿ���ע�ᡣ"),
						QString::fromLocal8Bit("ȷ��"));
					break;
				default:
					break;
			}
		}
	}
}