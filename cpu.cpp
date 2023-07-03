#include "cpu.h"
#include <bitset>
#include <iomanip>
#include <iostream>
#include <sstream>

cpu::cpu() {
	t = 0;
	PC = 0; // Program counter
	RD = 0; // Address register
	RV = 0; // Value register
	R0 = 0;
	R1 = 0;
	R2 = 0;
	R3 = 0;
	memset(MEMORY, 0, sizeof(MEMORY));
	halted = false;
	broken = false;
	jump = false;
	_shouldUpdate = false;
}
string cpu::debug() {
	return "t: " + to_string(t) + "\nPC: " + hexString(PC) + "\nR0: " + hexString(R0) + "\nR1: "
		   + hexString(R1) + "\nR2: " + hexString(R2) + "\nR3: " + hexString(R3) + "\nRV: "
		   + hexString(RV) + "\nRD: " + hexString(RD) + "\nSP1: " + hexString(MEMORY[0xe000])
		   + "\nSP2: " + hexString(MEMORY[0xf000]) + "\nJMP: " + boolString(jump)
		   + "\nBRK: " + boolString(broken) + "\nHLT: " + boolString(halted);
}
void cpu::processNext(QImage& imageRef) {
	if (halted || broken) {
		return;
	}
	//	for (int i = 0; i < 128 * 128; i++) {
	//		imageRef.setPixelColor(i % 128, i / 128, i * 16);
	//	}
	//	_shouldUpdate = true;
	instruction = MEMORY[PC];

	switch (instruction) {
	case 0x00:
		break;
	case 0x01:
		getValue();
		getAddress();
		setValue(RV, RD, imageRef);
		break;
	case 0x02:
		//		cout << "Update the screen, please?" << endl;
		//		cout << memorySectionDisplay(0xa000, 32) << endl;
		_shouldUpdate = true;
		break;
	case 0x04:
		getAddress();
		R0 = RV;
		break;
	case 0x05:
		getAddress();
		R1 = RV;
		break;
	case 0x06:
		getAddress();
		R2 = RV;
		break;
	case 0x07:
		getAddress();
		R3 = RV;
		break;
	case 0x08:
		getValue();
		R0 = RV;
		break;
	case 0x09:
		getValue();
		R1 = RV;
		break;
	case 0x0a:
		getValue();
		R2 = RV;
		break;
	case 0x0b:
		getValue();
		R3 = RV;
		break;
	case 0x0c:
		getAddress();
		setValue(RV, R0, imageRef);
		break;
	case 0x0d:
		getAddress();
		setValue(RV, R1, imageRef);
		break;
	case 0x0e:
		getAddress();
		setValue(RV, R2, imageRef);
		break;
	case 0x0f:
		getAddress();
		setValue(RV, R3, imageRef);
		break;
	case 0x12:
		getAtAddress();
		R2 = R2 + RV;
		break;
	case 0x15:
		getValue();
		R1 = R1 + RV;
		break;
	case 0x16:
		getValue();
		R2 = R2 + RV;
		break;
	case 0x1d:
		getValue();
		R1 = R1 - RV;
		break;
	case 0x2c:
		R0++;
		break;
	case 0x2d:
		R1++;
		break;
	case 0x2e:
		R2++;
		break;
	case 0x2f:
		R3++;
		break;
	case 0x36:
		R2 = R2 << 1;
		break;
	case 0x41:
		getValue();
		R1 = R1 & RV;
		break;
	case 0x43:
		getValue();
		R3 = R3 & RV;
		break;
	case 0x60:
		RD = R0;
		break;
	case 0x61:
		RD = R1;
		break;
	case 0x62:
		RD = R2;
		break;
	case 0x66:
		getValue();
		PC = RV - 1;
		break;
	case 0x68:
		jump = (R0 == 0);
		break;
	case 0x69:
		jump = R1 == 0;
		break;
	case 0x6a:
		jump = R2 == 0;
		break;
	case 0x6b:
		jump = R3 == 0;
		break;
	case 0x6e:
		getAtAddress();
		jump = R2 == RV;
		break;
	case 0x75:
		getAtAddress();
		jump = R1 < RV;
		break;
	case 0x7a:
		getValue();
		jump = R2 < RV;
		break;
	case 0x80:
		getValue();
		jump = R0 > RV;
		break;
	case 0x85: {
		getValue();
		pushToStack(0xe000, PC + 1);
		//		MEMORY[(unsigned short) (0xe000 + ++MEMORY[0xe000])] = PC + 1; // Put current PC in stack
		PC = RV - 1;												   // Update PC to Address
		//		cout << "0x85: " << MEMORY[0xe000] << endl;
		break;
	}
	case 0x86:
		//		PC = MEMORY[0xe000 + MEMORY[0xe000]] - 1; // Load PC from stack
		//		MEMORY[0xe000]--;						  // Decrease stack
		PC = popFromStack(0xe000) - 1;
		break;
	case 0x88:
		pushToStack(0xf000, R0);
		pushToStack(0xf000, R1);
		pushToStack(0xf000, R2);
		pushToStack(0xf000, R3);
		//		MEMORY[0xf000 + ++MEMORY[0xf000]] = R0;
		//		MEMORY[0xf000 + ++MEMORY[0xf000]] = R1;
		//		MEMORY[0xf000 + ++MEMORY[0xf000]] = R2;
		//		MEMORY[0xf000 + ++MEMORY[0xf000]] = R3;

		//		cout << "0x88: " << MEMORY[0xf000] << endl;
		break;
	case 0x89:
		R3 = popFromStack(0xf000);
		R2 = popFromStack(0xf000);
		R1 = popFromStack(0xf000);
		R0 = popFromStack(0xf000);
		//		R3 = MEMORY[0xf000 + MEMORY[0xf000]--];
		//		R2 = MEMORY[0xf000 + MEMORY[0xf000]--];
		//		R1 = MEMORY[0xf000 + MEMORY[0xf000]--];
		//		R0 = MEMORY[0xf000 + MEMORY[0xf000]--];

		//		cout << "0x89: " << MEMORY[0xf000] << endl;
		break;
	case 0x8b:
		getValue();
		setValue(RV, RD, imageRef);
		break;
	case 0x8c:
		pushToStack(0xf000, R0);
		//		MEMORY[0xf000 + ++MEMORY[0xf000]] = R0;
		//		cout << "0x8c: " << MEMORY[0xf000] << endl;
		break;
	case 0x8d:
		pushToStack(0xf000, R1);
		//		MEMORY[0xf000 + ++MEMORY[0xf000]] = R1;
		//		cout << "0x8d: " << MEMORY[0xf000] << endl;
		break;
	case 0x8e:
		pushToStack(0xf000, R2);
		//		MEMORY[0xf000 + ++MEMORY[0xf000]] = R2;
		//		cout << "0x8e: " << MEMORY[0xf000] << endl;
		break;
	case 0x8f:
		pushToStack(0xf000, R3);
		//		MEMORY[0xf000 + ++MEMORY[0xf000]] = R3;
		//		cout << "0x8f: " << MEMORY[0xf000] << endl;
		break;
	case 0x90:
		R0 = popFromStack(0xf000);
		//		R0 = MEMORY[0xf000 + MEMORY[0xf000]--]; // Update stack pointer
		//		cout << "0x90: " << MEMORY[0xf000] << endl;
		break;
	case 0x91:
		R1 = popFromStack(0xf000);
		//		R1 = MEMORY[0xf000 + MEMORY[0xf000]--]; // Update stack pointer
		//		cout << "0x91: " << MEMORY[0xf000] << endl;
		break;
	case 0x93:
		R3 = popFromStack(0xf000);
		//		R1 = MEMORY[0xf000 + MEMORY[0xf000]--]; // Update stack pointer
		//		cout << "0x91: " << MEMORY[0xf000] << endl;
		break;
	case 0x94:
		setValue(R0, RD, imageRef);
		break;
	case 0x95:
		setValue(R1, RD, imageRef);
		break;
	case 0x96:
		setValue(R2, RD, imageRef);
		break;
	case 0x97:
		setValue(R3, RD, imageRef);
		break;
	case 0x9c:
		R0 = MEMORY[RD];
		break;
	case 0x9d:
		R1 = MEMORY[RD];
		break;
	case 0x9e:
		R2 = MEMORY[RD];
		break;
	case 0x9f:
		R3 = MEMORY[RD];
		break;
	case 0xa0:
		getValue();
		R0 = R0 << RV;
		break;
	case 0xa1:
		getValue();
		R1 = R1 << RV;
		break;
	case 0xa2:
		getValue();
		R2 = R2 << RV;
		break;
	case 0xa4:
		getValue();
		R0 = R0 >> RV;
		break;
	case 0xa5:
		getValue();
		R1 = R1 >> RV;
		break;
	case 0xb9:
		R0 = R0 + R1;
		break;
	case 0xc5:
		R0 = R0 << R1;
		break;
	case 0xcd:
		jump = R1 != 0;
		break;
	case 0xd2:
		getAddress();
		if (jump) {
			PC = RD - 1;
		}
		break;
	case 0xe0:
		getValue();
		jump = (R0 != RV);
		break;
	case 0xe1:
		getValue();
		jump = (R1 != RV);
		break;
	case 0xe2:
		getValue();
		jump = (R2 != RV);
		break;
	case 0xe3:
		getValue();
		jump = (R3 != RV);
		break;
	case 0xff:
		halted = true;
		break;
	default:
		throw std::invalid_argument("Unknown instruction recieved: " + hexString(instruction, 2)
									+ "\n" + debug());
	}

	if (t < 2500) {
		cout << PC << ":" << instruction << endl;
		// cout << "\n" << debug() << endl;
	}
	PC++;
	t++;
}
void cpu::pushToStack(unsigned short stackAddress, unsigned short value) {
	unsigned short StackPointer = MEMORY[stackAddress] = MEMORY[stackAddress]
														 + 1; // Update stack pointer
	MEMORY[stackAddress + StackPointer] = value;
}
unsigned short cpu::popFromStack(unsigned short stackAddress) {
	unsigned short StackPointer = MEMORY[stackAddress];
	unsigned short retVal = MEMORY[stackAddress + StackPointer];
	MEMORY[stackAddress] = MEMORY[stackAddress] - 1; // Update stack pointer
	return retVal;
}
void cpu::tick(QImage& imageRef) {
	while (!_shouldUpdate && !halted && !broken) {
		processNext(imageRef);
	}
}
void cpu::setValue(unsigned short value, unsigned short address, QImage& imageRef) {
	MEMORY[address] = value;
	if (address >= 0xa000 && address < 0xe000) {
		//		cout << "Now I've poked the goat..." << endl;
		int position = address - 0xa000;

		QRgb* px1 = reinterpret_cast<QRgb*>(imageRef.bits());

		switch (displayMode) {
		case 0: {
			int x = position % 128; // Bytes per screen width
			int y = position / 128;
			for (int xShift = 0; xShift < 4; xShift++) {
				for (int yShift = 0; yShift < 4; yShift++) {
					// imageRef.setPixelColor(x * 4 + xShift, y * 4 + yShift, convertColor(value));

					QRgb& rgb = px1[x * 4 + xShift + (y * 4 + yShift) * 512];
					rgb = convertColor(value, displayMode);
				}
			}
			break;
		}
		case 1:
		case 2: {
			int x = position % 64; // Bytes per screen width
			int y = position / 64;
			for (int xShift = 0; xShift < 2; xShift++) {
				for (int yShift = 0; yShift < 2; yShift++) {
					for (int pxShift = 0; pxShift < 4; pxShift++) {
						QRgb& rgb = px1[x * 8 + xShift + (pxShift * 2) + (y * 2 + yShift) * 512];
						rgb = convertColor(value, displayMode, pxShift);
					}
				}
			}
			break;
		}
		case 3: {
			int x = position % 32; // Bytes per screen width
			int y = position / 32;
			for (int pxShift = 0; pxShift < 16; pxShift++) {
				QRgb& rgb = px1[(x * 16) + pxShift + (y * 512)];
				rgb = convertColor(value, displayMode, pxShift);
			}
			break;
		}
		}
		imageRef.bits();
	}
	if (address == 0x9f04) {
		displayMode = value & 0b11;
		std::cout << "Disp mode " << displayMode << std::endl;
	}
}
void cpu::getValue() {
	PC++;
	RV = MEMORY[PC];
}
void cpu::getAddress() {
	PC++;
	RD = MEMORY[PC];
}
void cpu::getAtAddress() {
	getAddress();
	RV = MEMORY[RD];
}
//void getValue(unsigned short address) {
//	RV = MEMORY[address];
//}

QRgb cpu::convertColor(unsigned short value, int mode, int selector) {
	int thisByte;
	switch (mode) {
	case 0: {
		return qRgba((value / 2048) * (255 / 31),
					 (value % 2048) / 32 * (255 / 63),
					 (value % 32) * (255 / 31),
					 255);
	}
	case 1: {
		thisByte = value / (1 << (16 - ((selector + 1) * 4))) % 0b10000;
		int r = (int) (bool) (thisByte & 0b1000);
		int g = (int) (bool) (thisByte & 0b0100);
		int b = (int) (bool) (thisByte & 0b0010);
		int light = (int) (bool) (thisByte & 0b0001);

		return qRgba((r + light) * 0x80, (g + light) * 0x80, (b + light) * 0x80, 255);
	}
	case 2: {
		thisByte = value / (1 << (16 - ((selector + 1) * 4))) % 0b10000;
		return MEMORY[0x9f10 + thisByte];
	}
	case 3: {
		thisByte = (value >> (15 - selector)) % 0b10;
		return MEMORY[0x9f10 + thisByte];
	}
	default:
		return qRgba(0, 0, 0, 255);
	}
}
bool cpu::shouldUpdate() {
	bool temp = _shouldUpdate;
	_shouldUpdate = false;
	return temp;
}

vector<string> cpu::stringSplitter(string s) {
	// I'm used to this being a standard build-in, not having to implement it from scratch!
	// https://stackoverflow.com/a/68397097
	vector<string> result = {""};
	for (char ch : s) {
		if (isspace(ch) == 0) {
			result.back() += ch;
		} else {
			if (result.back() != "") {
				result.push_back("");
			}
		}
	}
	if (result.back() == "") {
		result.pop_back();
	}
	return result;
}
string cpu::boolString(bool value) {
	return value ? "True" : "False";
}
string cpu::hexString(unsigned short value, int length) {
	// https://stackoverflow.com/a/5100745
	stringstream stream;
	stream << "0x" << setfill('0') << setw(length) << hex << value;
	return stream.str();
}
string cpu::binString4(unsigned short value) {
	// https://stackoverflow.com/a/5100745
	stringstream stream;
	stream << "0b" << std::bitset<4>(value);
	return stream.str();
}
string cpu::memorySectionDisplay(int start, int length) {
	string result = "[";
	for (int i = start; i < start + length; i++) {
		result += hexString(MEMORY[i], 4) + ", ";
		// result += to_string(MEMORY[i]) + ", ";
	}
	return result.substr(0, result.length() - 2) + "]";
}

string cpu::vectorDisplay(vector<string> strVec) {
	string result = "{";
	for (string element : strVec) {
		result += element + ", ";
	}
	return result.substr(0, result.length() - 2) + "}";
}

void cpu::loadToMemory(string bytesToLoad) {
	vector<string> strMemoryBytes = stringSplitter(bytesToLoad);
	cout << vectorDisplay(strMemoryBytes) << endl;
	vector<unsigned short> memoryBytes;
	for (int i = 0; i < strMemoryBytes.size(); i++) {
		MEMORY[i] = std::stoul(strMemoryBytes[i], nullptr, 16);
	}
	//	cout << memorySectionDisplay(0, 12) << endl;
}
