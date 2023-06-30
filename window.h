#ifndef WINDOW_H
#define WINDOW_H

#include "cpu.h"
#include <QBoxLayout>
#include <QDateTime>
#include <QLabel>
#include <QTimer>
#include <QWidget>

class QLabel;
class QTimer;
class QDateTime;

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
	QLabel *fpsLabel;
	int t;
	int cacheTime;
	int cacheFrame;
	cpu *processor;
};

#endif // WINDOW_H
