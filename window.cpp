#include "window.h"

Window::Window() {
	QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::TopToBottom);

	imageMap = new QImage(QSize(128, 128), QImage::Format_RGB32);
	imageMap->fill(8349);
	imgDisplayLabel = new QLabel("");
	imgDisplayLabel->setPixmap(QPixmap::fromImage(*imageMap));
	imgDisplayLabel->adjustSize();

	//	fpsLabel = new QLabel(this); //("FPS: ");
	//	fpsLabel->setText("FPS: ");
	fpsLabel = new QLabel("FPS: ");
	fpsLabel->move(0, 0);

	mainLayout->addWidget(fpsLabel);
	mainLayout->addWidget(imgDisplayLabel);

	setLayout(mainLayout);

	QTimer *timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &Window::animateOneStep);
	timer->start();

	QTimer *fpstimer = new QTimer(this);
	connect(fpstimer, &QTimer::timeout, this, &Window::fpsUpdate);
	fpstimer->start(1000);

	setWindowTitle(tr("Textures"));
	t = 0;
	cacheTime = QDateTime::currentSecsSinceEpoch();
	cacheFrame = 0;

	processor = new cpu();
}
void Window::animateOneStep() {
	t++;
	processor->tick(*imageMap);
	imgDisplayLabel->setPixmap(QPixmap::fromImage(*imageMap));
}
void Window::fpsUpdate() {
	//	fpsLabel->setText("FPS: " + QString::number(t));
	int currentTime = QDateTime::currentSecsSinceEpoch();
	if (currentTime - cacheTime == 0) {
		return;
	}
	fpsLabel->setText("FPS: "
					  + QString::number((t - cacheFrame) / (currentTime - cacheTime))); // Δf/Δs
	cacheTime = currentTime;
	cacheFrame = t;
}
