#include "ProgramLib.h"
#include "SystemManager.h"
#include "ConnectedClient.h"

SystemManager* System{nullptr};

int main(int argc, char** argv) {
	system("color 1E");
	System = new SystemManager;
	System->Network.acceptConnect();
}