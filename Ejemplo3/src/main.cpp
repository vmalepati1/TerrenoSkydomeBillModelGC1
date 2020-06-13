#include <iostream>

#include "platform/Application.h"

int main() {	
	Application app("Barco del Agua", {1280, 720, false, true, 0.1f, 1000.0f});

	app.Start();

	return 0;
}