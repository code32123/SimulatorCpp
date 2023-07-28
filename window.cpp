#include "window.h"

Window::Window() {
	forceScreenUpdate = false;
	running = true;
	debugEveryStep = false;
	QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::TopToBottom);

	imageMap = new QImage(QSize(512, 512), QImage::Format_RGB32);
	imageMap->fill(Qt::black);
	imgDisplayLabel = new QLabel("");
	imgDisplayLabel->setPixmap(QPixmap::fromImage(*imageMap));
	imgDisplayLabel->adjustSize();

	//	fpsLabel = new QLabel(this); //("FPS: ");
	//	fpsLabel->setText("FPS: ");
	debugLabel = new QLabel("FPS: ");
	debugLabel->move(0, 0);

	mainLayout->addWidget(debugLabel);
	mainLayout->addWidget(imgDisplayLabel);

	setLayout(mainLayout);

	QTimer *timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &Window::animateOneStep);
	timer->start(200); // Change to 0 for full speeeeed!

	QTimer *fpstimer = new QTimer(this);
	connect(fpstimer, &QTimer::timeout, this, &Window::fpsUpdate);
	fpstimer->start(1000);

	setWindowTitle(tr("GHOST Simulator - Interrupts!"));
	t = 0;
	cacheTime = QDateTime::currentSecsSinceEpoch();
	cacheFrame = 0;

	processor = new cpu();

	loadFile();
	//	run();
}
void Window::loadFile() {
	string cwd = "C:/Users/jimmy/Desktop/Coding/8Bit CPU/Ghost/programs/";
	string fileName = cwd + "Snake.hex";
	//	string fileName = cwd + "keypress.hex";
	//	string fileName = cwd + "stringPrint/StringPrin.hex";
	//	string fileName = cwd + "ColorModeTest.hex";
	//	string fileName = QFileDialog::getOpenFileName(this,
	//												   tr("Open hex file"),
	//												   "",
	//												   tr("Hex File (*.hex)"))
	//						  .toStdString();
	std::ifstream hexFile;
	hexFile.open(fileName);
	std::string fileContents;
	if (hexFile.is_open()) {
		char curr_char;
		while (hexFile) {
			curr_char = hexFile.get();
			if (curr_char == EOF)
				break;
			fileContents += curr_char;
			// std::cout << curr_char;
		}
	}
	processor->loadToMemory(fileContents);
}
void Window::run() {
	QTimer *timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &Window::animateOneStep);
	timer->start(); // Change to 0 for full speeeeed!

	QTimer *fpstimer = new QTimer(this);
	connect(fpstimer, &QTimer::timeout, this, &Window::fpsUpdate);
	fpstimer->start(1000);
}
void Window::animateOneStep() {
	if (running) {
		t++;

		processor->tick(*imageMap);
		if (processor->shouldUpdate() or forceScreenUpdate) {
			imgDisplayLabel->setPixmap(QPixmap::fromImage(*imageMap));
		}
	}
}
void Window::fpsUpdate() {
	int currentTime = QDateTime::currentSecsSinceEpoch();
	if (currentTime - cacheTime == 0) {
		return;
	}
	QString flags = (running ? "RUN | " : "STP | ") + QString(debugEveryStep ? "DBG | " : "")
					+ (processor->halted ? "HLT | " : "")
					+ QString(processor->broken ? "BRK | " : "");
	debugLabel->setText(flags
						+ "FPS: " + QString::number((t - cacheFrame) / (currentTime - cacheTime))
						+ "                   Proc t: " + QString::number(processor->T()));
	// Δf/Δs
	cacheTime = currentTime;
	cacheFrame = t;
}
