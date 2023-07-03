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
	void keyReleaseEvent(QKeyEvent *event) override {
		if (event->isAutoRepeat()) {
			return;
		}
		if (!processor->handlingAnInterrupt) {
			if (event->key() == Qt::Key_Left) {
				cout << "Left^" << endl;
				processor->writeIO(0x70, 0x25);
				processor->writeIO(0x71, 0);
				processor->interrupt(0x00);
			}
			if (event->key() == Qt::Key_Up) {
				cout << "Up^" << endl;
				processor->writeIO(0x70, 0x26);
				processor->writeIO(0x71, 0);
				processor->interrupt(0x00);
			}
			if (event->key() == Qt::Key_Right) {
				cout << "Right^" << endl;
				processor->writeIO(0x70, 0x27);
				processor->writeIO(0x71, 0);
				processor->interrupt(0x00);
			}
			if (event->key() == Qt::Key_Down) {
				cout << "Down^" << endl;
				processor->writeIO(0x70, 0x28);
				processor->writeIO(0x71, 0);
				processor->interrupt(0x00);
			}
		}
	}
	void keyPressEvent(QKeyEvent *event) override {
		if (event->isAutoRepeat()) {
			return;
		}
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
		//		if (event->key() == Qt::Key_Q) {
		//			cout << "Q" << endl;
		//			forceScreenUpdate = !forceScreenUpdate;
		//			cout << "Force screen refresh: " << forceScreenUpdate << endl;
		//		}
		if (event->key() == Qt::Key_R) {
			cout << "R" << endl;
			running = true;
			processor->broken = false;
		}
		if (event->key() == Qt::Key_D) {
			cout << "D" << endl;
			cout << processor->debug() << endl;
		}
		//		if (event->key() == Qt::Key_W) {
		//			cout << "W" << endl;
		//			debugEveryStep = !debugEveryStep;
		//		}
		if (!processor->handlingAnInterrupt) {
			if (event->key() == Qt::Key_Left) {
				cout << "Left" << processor->lock << endl;
				processor->writeIO(0x70, 0x25);
				processor->writeIO(0x71, 1);
				processor->interrupt(0x00);
			}
			if (event->key() == Qt::Key_Up) {
				cout << "Up" << processor->lock << endl;
				processor->writeIO(0x70, 0x26);
				processor->writeIO(0x71, 1);
				processor->interrupt(0x00);
			}
			if (event->key() == Qt::Key_Right) {
				cout << "Right" << processor->lock << endl;
				processor->writeIO(0x70, 0x27);
				processor->writeIO(0x71, 1);
				processor->interrupt(0x00);
			}
			if (event->key() == Qt::Key_Down) {
				cout << "Down" << processor->lock << endl;
				processor->writeIO(0x70, 0x28);
				processor->writeIO(0x71, 1);
				processor->interrupt(0x00);
			}
		}
	}
};

#endif // WINDOW_H
