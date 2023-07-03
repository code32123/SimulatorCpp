/*
 * main.cpp
 * Hoping to set up the sim
 */

#include <QApplication>
#include <QSurfaceFormat>
#include <iostream>

#include "window.h"
using namespace std;

int main() {
	// https://stackoverflow.com/a/1519997
	char arg0[] = "SimulatorCpp";
	char* argv[] = {&arg0[0], NULL};
	int argc = (int) (sizeof(argv) / sizeof(argv[0])) - 1;
	//

	QApplication app(argc, &argv[0]);

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	QSurfaceFormat::setDefaultFormat(format);

	Window window;
	window.show();
	return app.exec();
}
