#include "ConnectServer.h"

int main(int argc, char* argv[]) {
	QApplication App(argc, argv);
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	ConnectServer ConnectWindow;
	ConnectWindow.show();
	return App.exec();
}