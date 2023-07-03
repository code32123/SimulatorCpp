#ifndef WINDOW_H
#define WINDOW_H

#include "cpu.h"
#include <QBoxLayout>
#include <QDateTime>
#include <QKeyEvent>
#include <QLabel>
#include <QTimer>
#include <QWidget>
#include <fstream>
#include <iostream>

class QLabel;
class QTimer;
class QDateTime;
using namespace std;

class Window : public QWidget {
	Q_OBJECT
public:
	Window();
private slots:
	void animateOneStep();
	void fpsUpdate();

private:
	QImage *imageMap;
	QLabel *imgDisplayLabel;
	QLabel *debugLabel;
	int t;
	int cacheTime;
	int cacheFrame;
	cpu *processor;
	bool forceScreenUpdate;
	bool running;
	bool debugEveryStep;

protected:
	void keyPressEvent(QKeyEvent *event) override {
		if (event->key() == Qt::Key_Space) {
			cout << "Space" << endl;
			running = false;
			processor->broken = false;
			processor->processNext(*imageMap);
			if (processor->shouldUpdate() or forceScreenUpdate) {
				imgDisplayLabel->setPixmap(QPixmap::fromImage(*imageMap));
			}
			if (debugEveryStep) {
				cout << processor->debug() << endl;
			}
		}
		if (event->key() == Qt::Key_Q) {
			cout << "Q" << endl;
			forceScreenUpdate = !forceScreenUpdate;
			cout << "Force screen refresh: " << forceScreenUpdate << endl;
		}
		if (event->key() == Qt::Key_R) {
			cout << "R" << endl;
			running = true;
			processor->broken = false;
		}
		if (event->key() == Qt::Key_D) {
			cout << "D" << endl;
			cout << processor->debug() << endl;
		}
		if (event->key() == Qt::Key_W) {
			cout << "W" << endl;
			debugEveryStep = !debugEveryStep;
		}
	}
};

#endif // WINDOW_H
