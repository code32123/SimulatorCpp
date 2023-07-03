#ifndef CPU_H
#define CPU_H

#include <QImage>
#include <iostream>
using namespace std;

class cpu {
public:
	cpu();
	void tick(QImage& imageRef);
	void processNext(QImage& imageRef);
	bool shouldUpdate();
	void loadToMemory(string bytesToLoad);

	void writeIO(unsigned char address, unsigned short value);
	void interrupt(unsigned char address);

	vector<string> stringSplitter(string s);
	string vectorDisplay(vector<string> strVec);
	string memorySectionDisplay(int start, int length);
	string hexString(unsigned short value, int length = 4);
	string binString4(unsigned short value);
	string boolString(bool value);
	bool halted;
	bool broken;
	string debug();
	int T() { return t; };

	bool lock;
	bool handlingAnInterrupt;

private:
	int t;
	unsigned short PC;
	unsigned short DD;
	unsigned short R0;
	unsigned short R1;
	unsigned short R2;
	unsigned short R3;
	unsigned short MEMORY[0x10000];
	bool _shouldUpdate;
	bool jump;
	unsigned short instruction;
	int displayMode;

	unsigned short getFromMemory(unsigned short address);
	unsigned int getValue();
	unsigned int getAtAddress();
	void pushToStack(unsigned short stackAddress, unsigned short value);
	unsigned short popFromStack(unsigned short stackAddress);

	void setValue(unsigned short value, unsigned short address, QImage& imageRef);

	QRgb convertColor(unsigned short rgb565, int mode, int selector = 0);

	const unsigned short functionStack = 0xe000;
	const unsigned short registerStack = 0xf000;
};
#endif // CPU_H
