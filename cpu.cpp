#include "cpu.h"
#include <bitset>
#include <iomanip>
#include <iostream>
#include <sstream>

cpu::cpu() {
	t = 0;
	PC = 0; // Program counter
	DD = 0; // Address Register
	R0 = 0;
	R1 = 0;
	R2 = 0;
	R3 = 0;
	memset(MEMORY, 0, sizeof(MEMORY));
	halted = false;
	broken = false;
	jump = false;
	_shouldUpdate = false;
	srand((unsigned) time(NULL));
	lock = false;
	handlingAnInterrupt = false;
}
string cpu::debug() {
	return "t: " + to_string(t) + "\nPC: " + hexString(PC) + "\nR0: " + hexString(R0)
		   + "\nR1: " + hexString(R1) + "\nR2: " + hexString(R2) + "\nR3: " + hexString(R3)
		   + "\nDD: " + hexString(DD) + "\nSP1: " + hexString(MEMORY[registerStack]) + ":"
		   + memorySectionDisplay(registerStack, MEMORY[registerStack] + 1)
		   + "\nSP2: " + hexString(MEMORY[functionStack]) + ":"
		   + memorySectionDisplay(functionStack, MEMORY[functionStack] + 1) + "\nJMP: "
		   + boolString(jump) + "\nBRK: " + boolString(broken) + "\nHLT: " + boolString(halted);
}
void cpu::processNext(QImage& imageRef) {
	if (halted || broken) {
		return;
	}
	lock = true;
	instruction = MEMORY[PC];

	//		cout << PC << ":" << instruction << endl;

	switch (instruction) {
	case 0x00:
		break;
	case 0x01: {
		unsigned short val = getValue();
		unsigned short add = getValue();
		setValue(val, add, imageRef);
		break;
	}
	case 0x02:
		_shouldUpdate = true;
		break;
	case 0x03:
		setValue(getAtAddress(), getValue(), imageRef);
		break;
	case 0x04:
		R0 = getAtAddress();
		break;
	case 0x05:
		R1 = getAtAddress();
		break;
	case 0x06:
		R2 = getAtAddress();
		break;
	case 0x07:
		R3 = getAtAddress();
		break;
	case 0x08:
		R0 = getValue();
		break;
	case 0x09:
		R1 = getValue();
		break;
	case 0x0a:
		R2 = getValue();
		break;
	case 0x0b:
		R3 = getValue();
		break;
	case 0x0c:
		setValue(R0, getValue(), imageRef);
		break;
	case 0x0d:
		setValue(R1, getValue(), imageRef);
		break;
	case 0x0e:
		setValue(R2, getValue(), imageRef);
		break;
	case 0x0f:
		setValue(R3, getValue(), imageRef);
		break;
	case 0x10:
		R0 = R0 + getAtAddress();
		break;
	case 0x11:
		R1 = R1 + getAtAddress();
		break;
	case 0x12:
		R2 = R2 + getAtAddress();
		break;
	case 0x13:
		R3 = R3 + getAtAddress();
		break;
	case 0x14:
		R0 = R0 + getValue();
		break;
	case 0x15:
		R1 = R1 + getValue();
		break;
	case 0x16:
		R2 = R2 + getValue();
		break;
	case 0x17:
		R3 = R3 + getValue();
		break;
	case 0x18:
		R0 = R0 - getAtAddress();
		break;
	case 0x19:
		R1 = R1 - getAtAddress();
		break;
	case 0x1a:
		R2 = R2 - getAtAddress();
		break;
	case 0x1b:
		R3 = R3 - getAtAddress();
		break;
	case 0x1c:
		R0 = R0 - getValue();
		break;
	case 0x1d:
		R1 = R1 - getValue();
		break;
	case 0x1e:
		R2 = R2 - getValue();
		break;
	case 0x1f:
		R3 = R3 - getValue();
		break;
	case 0x20:
		R0 = getAtAddress() - R0;
		break;
	case 0x21:
		R1 = getAtAddress() - R1;
		break;
	case 0x22:
		R2 = getAtAddress() - R2;
		break;
	case 0x23:
		R3 = getAtAddress() - R3;
		break;
	case 0x24:
		R0 = getValue() - R0;
		break;
	case 0x25:
		R1 = getValue() - R1;
		break;
	case 0x26:
		R2 = getValue() - R2;
		break;
	case 0x27:
		R3 = getValue() - R3;
		break;
	case 0x28:
		R0 = -(unsigned short) R0;
		break;
	case 0x29:
		R1 = -(unsigned short) R1;
		break;
	case 0x2a:
		R2 = -(unsigned short) R2;
		break;
	case 0x2b:
		R3 = -(unsigned short) R3;
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
	case 0x30:
		R0--;
		break;
	case 0x31:
		R1--;
		break;
	case 0x32:
		R2--;
		break;
	case 0x33:
		R3--;
		break;
	case 0x34:
		R0 = R0 << 1;
		break;
	case 0x35:
		R1 = R1 << 1;
		break;
	case 0x36:
		R2 = R2 << 1;
		break;
	case 0x37:
		R3 = R3 << 1;
		break;
	case 0x38:
		R0 = R0 >> 1;
		break;
	case 0x39:
		R1 = R1 >> 1;
		break;
	case 0x3a:
		R2 = R2 >> 1;
		break;
	case 0x3b:
		R3 = R3 >> 1;
		break;
	case 0x3c:
		R0 = R0 & getAtAddress();
		break;
	case 0x3d:
		R1 = R1 & getAtAddress();
		break;
	case 0x3e:
		R2 = R2 & getAtAddress();
		break;
	case 0x3f:
		R3 = R3 & getAtAddress();
		break;
	case 0x40:
		R0 = R0 & getValue();
		break;
	case 0x41:
		R1 = R1 & getValue();
		break;
	case 0x42:
		R2 = R2 & getValue();
		break;
	case 0x43:
		R3 = R3 & getValue();
		break;
	case 0x44:
		R0 = ~(R0 & getAtAddress());
		break;
	case 0x45:
		R1 = ~(R1 & getAtAddress());
		break;
	case 0x46:
		R2 = ~(R2 & getAtAddress());
		break;
	case 0x47:
		R3 = ~(R3 & getAtAddress());
		break;
	case 0x48:
		R0 = ~(R0 & getValue());
		break;
	case 0x49:
		R1 = ~(R1 & getValue());
		break;
	case 0x4a:
		R2 = ~(R2 & getValue());
		break;
	case 0x4b:
		R3 = ~(R3 & getValue());
		break;
	case 0x4c:
		R0 = R0 | getAtAddress();
		break;
	case 0x4d:
		R1 = R1 | getAtAddress();
		break;
	case 0x4e:
		R2 = R2 | getAtAddress();
		break;
	case 0x4f:
		R3 = R3 | getAtAddress();
		break;
	case 0x50:
		R0 = R0 | getValue();
		break;
	case 0x51:
		R1 = R1 | getValue();
		break;
	case 0x52:
		R2 = R2 | getValue();
		break;
	case 0x53:
		R3 = R3 | getValue();
		break;
	case 0x54:
		R0 = R0 ^ getAtAddress();
		break;
	case 0x55:
		R1 = R1 ^ getAtAddress();
		break;
	case 0x56:
		R2 = R2 ^ getAtAddress();
		break;
	case 0x57:
		R3 = R3 ^ getAtAddress();
		break;
	case 0x58:
		R0 = R0 ^ getValue();
		break;
	case 0x59:
		R1 = R1 ^ getValue();
		break;
	case 0x5a:
		R2 = R2 ^ getValue();
		break;
	case 0x5b:
		R3 = R3 ^ getValue();
		break;
	case 0x5c:
		R0 = !R0;
		break;
	case 0x5d:
		R1 = !R1;
		break;
	case 0x5e:
		R2 = !R2;
		break;
	case 0x5f:
		R3 = !R3;
		break;
	case 0x60:
		DD = R0;
		break;
	case 0x61:
		DD = R1;
		break;
	case 0x62:
		DD = R2;
		break;
	case 0x63:
		DD = R3;
		break;
	case 0x64:
		DD = getAtAddress();
		break;
	case 0x65:
		DD = getValue();
		break;
	case 0x66:
		PC = getValue() - 1;
		break;
	case 0x67:
		PC = DD - 1;
		break;
	case 0x68:
		jump = R0 == 0;
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
	case 0x6c:
		jump = R0 == getAtAddress();
		break;
	case 0x6d:
		jump = R1 == getAtAddress();
		break;
	case 0x6e:
		jump = R2 == getAtAddress();
		break;
	case 0x6f:
		jump = R3 == getAtAddress();
		break;
	case 0x70:
		jump = R0 == getValue();
		break;
	case 0x71:
		jump = R1 == getValue();
		break;
	case 0x72:
		jump = R2 == getValue();
		break;
	case 0x73:
		jump = R3 == getValue();
		break;
	case 0x74:
		jump = R0 < getAtAddress();
		break;
	case 0x75:
		jump = R1 < getAtAddress();
		break;
	case 0x76:
		jump = R2 < getAtAddress();
		break;
	case 0x77:
		jump = R3 < getAtAddress();
		break;
	case 0x78:
		jump = R0 < getValue();
		break;
	case 0x79:
		jump = R1 < getValue();
		break;
	case 0x7a:
		jump = R2 < getValue();
		break;
	case 0x7b:
		jump = R3 < getValue();
		break;
	case 0x7c:
		jump = R0 > getAtAddress();
		break;
	case 0x7d:
		jump = R1 > getAtAddress();
		break;
	case 0x7e:
		jump = R2 > getAtAddress();
		break;
	case 0x7f:
		jump = R3 > getAtAddress();
		break;
	case 0x80:
		jump = R0 > getValue();
		break;
	case 0x81:
		jump = R1 > getValue();
		break;
	case 0x82:
		jump = R2 > getValue();
		break;
	case 0x83:
		jump = R3 > getValue();
		break;
	case 0x84:
		// Routine at DD
		pushToStack(functionStack, PC + 1);
		PC = DD - 1;
		break;
	case 0x85:
		// Routine at Address
		pushToStack(functionStack, PC + 1);
		PC = getValue() - 1;
		break;
	case 0x86:
		handlingAnInterrupt = false;
		PC = popFromStack(functionStack);
		break;
		//case 0x87
	case 0x88:
		pushToStack(registerStack, R0);
		pushToStack(registerStack, R1);
		pushToStack(registerStack, R2);
		pushToStack(registerStack, R3);
		break;
	case 0x89:
		R3 = popFromStack(registerStack);
		R2 = popFromStack(registerStack);
		R1 = popFromStack(registerStack);
		R0 = popFromStack(registerStack);
		break;
	case 0x8a:
		if (jump) {
			handlingAnInterrupt = false;
			PC = popFromStack(functionStack);
		}
		break;
	case 0x8b:
		setValue(getValue(), DD, imageRef);
		break;
	case 0x8c:
		pushToStack(registerStack, R0);
		break;
	case 0x8d:
		pushToStack(registerStack, R1);
		break;
	case 0x8e:
		pushToStack(registerStack, R2);
		break;
	case 0x8f:
		pushToStack(registerStack, R3);
		break;
	case 0x90:
		R0 = popFromStack(registerStack);
		break;
	case 0x91:
		R1 = popFromStack(registerStack);
		break;
	case 0x92:
		R2 = popFromStack(registerStack);
		break;
	case 0x93:
		R3 = popFromStack(registerStack);
		break;
	case 0x94:
		setValue(R0, DD, imageRef);
		break;
	case 0x95:
		setValue(R1, DD, imageRef);
		break;
	case 0x96:
		setValue(R2, DD, imageRef);
		break;
	case 0x97:
		setValue(R3, DD, imageRef);
		break;
	case 0x98:
		setValue(R0, getValue(), imageRef);
		break;
	case 0x99:
		setValue(R1, getValue(), imageRef);
		break;
	case 0x9a:
		setValue(R2, getValue(), imageRef);
		break;
	case 0x9b:
		setValue(R3, getValue(), imageRef);
		break;
	case 0x9c:
		R0 = getFromMemory(DD);
		break;
	case 0x9d:
		R1 = getFromMemory(DD);
		break;
	case 0x9e:
		R2 = getFromMemory(DD);
		break;
	case 0x9f:
		R3 = getFromMemory(DD);
		break;
	case 0xa0:
		R0 = R0 << getValue();
		break;
	case 0xa1:
		R1 = R1 << getValue();
		break;
	case 0xa2:
		R2 = R2 << getValue();
		break;
	case 0xa3:
		R3 = R3 << getValue();
		break;
	case 0xa4:
		R0 = R0 >> getValue();
		break;
	case 0xa5:
		R1 = R1 >> getValue();
		break;
	case 0xa6:
		R2 = R2 >> getValue();
		break;
	case 0xa7:
		R3 = R3 >> getValue();
		break;
	case 0xa8:
		R0 = R0 << getAtAddress();
		break;
	case 0xa9:
		R1 = R1 << getAtAddress();
		break;
	case 0xaa:
		R2 = R2 << getAtAddress();
		break;
	case 0xab:
		R3 = R3 << getAtAddress();
		break;
	case 0xac:
		R0 = R0 >> getAtAddress();
		break;
	case 0xad:
		R1 = R1 >> getAtAddress();
		break;
	case 0xae:
		R2 = R2 >> getAtAddress();
		break;
	case 0xaf:
		R3 = R3 >> getAtAddress();
		break;
		//	case 0xb0
		//	case 0xb1
		//	case 0xb2
		//	case 0xb3
		//	case 0xb4
		//	case 0xb5
		//	case 0xb6
		//	case 0xb7
	case 0xb8:
		R0 = R0 + R0;
		break;
	case 0xb9:
		R0 = R0 + R1;
		break;
	case 0xba:
		R0 = R0 + R2;
		break;
	case 0xbb:
		R0 = R0 + R3;
		break;
	case 0xbc:
		R0 = R0 - R0;
		break;
	case 0xbd:
		R0 = R0 - R1;
		break;
	case 0xbe:
		R0 = R0 - R2;
		break;
	case 0xbf:
		R0 = R0 - R3;
		break;
	case 0xc0:
		R0 = R0 - R0;
		break;
	case 0xc1:
		R0 = R1 - R0;
		break;
	case 0xc2:
		R0 = R2 - R0;
		break;
	case 0xc3:
		R0 = R3 - R0;
		break;
	case 0xc4:
		R0 = R0 << R0;
		break;
	case 0xc5:
		R0 = R0 << R1;
		break;
	case 0xc6:
		R0 = R0 << R2;
		break;
	case 0xc7:
		R0 = R0 << R3;
		break;
	case 0xc8:
		R0 = R0 >> R0;
		break;
	case 0xc9:
		R0 = R0 >> R1;
		break;
	case 0xca:
		R0 = R0 >> R2;
		break;
	case 0xcb:
		R0 = R0 >> R3;
		break;
	case 0xcc:
		jump = R0 != 0;
		break;
	case 0xcd:
		jump = R1 != 0;
		break;
	case 0xce:
		jump = R2 != 0;
		break;
	case 0xcf:
		jump = R3 != 0;
		break;
	case 0xd0:
		jump = getAtAddress() == 0;
		break;
	case 0xd1:
		jump = getAtAddress() != 0;
		break;
	case 0xd2: {
		unsigned short val = getValue();
		if (jump)
			PC = val - 1;
		break;
	}
	case 0xd3:
		if (jump)
			PC = DD - 1;
		break;
	case 0xd4: {
		unsigned int val = getValue();
		if (jump) {
			// Routine at Address
			pushToStack(functionStack, PC + 1);
			PC = val - 1;
		}
		break;
	}
	case 0xd5:
		if (jump) {
			// Routine at DD
			pushToStack(functionStack, PC + 1);
			PC = DD - 1;
		}
		break;
		//	case 0xd6
		//	case 0xd7
		//	case 0xd8
		//	case 0xd9
		//	case 0xda
		//	case 0xdb
	case 0xdc:
		jump = R0 != getAtAddress();
		break;
	case 0xdd:
		jump = R1 != getAtAddress();
		break;
	case 0xde:
		jump = R2 != getAtAddress();
		break;
	case 0xdf:
		jump = R3 != getAtAddress();
		break;
	case 0xe0:
		jump = R0 != getValue();
		break;
	case 0xe1:
		jump = R1 != getValue();
		break;
	case 0xe2:
		jump = R2 != getValue();
		break;
	case 0xe3:
		jump = R3 != getValue();
		break;
		//	case 0xe4
		//	case 0xe5
		//	case 0xe6
		//	case 0xe7
		//	case 0xe8
		//	case 0xe9
		//	case 0xea
		//	case 0xeb
		//	case 0xec
		//	case 0xed
		//	case 0xee
		//	case 0xef
		//	case 0xf0
		//	case 0xf1
		//	case 0xf2
		//	case 0xf3
		//	case 0xf4
		//	case 0xf5
		//	case 0xf6
		//	case 0xf7
		//	case 0xf8
		//	case 0xf9
		//	case 0xfa
		//	case 0xfb
		//	case 0xfc
	case 0xfe:
		broken = true;
		break;
	case 0xff:
		halted = true;
		break;
	default:
		throw std::invalid_argument("Unknown instruction recieved: " + hexString(instruction, 2)
									+ "\n" + debug());
	}

	//	if (PC != 4 && PC != 5)
	//		cout << PC << "I'm here..." << endl;
	PC++;
	//	if (PC != 5 && PC != 6)
	//		cout << PC << "Still here..." << endl;
	t++;
	lock = false;
}
void cpu::interrupt(unsigned char address) {
	if (!halted && !broken) {
		//		cout << "Called to do an interrupt: " << (int) (address)
		//			 << " + 0x9f00 = " << hexString(0x9f00 + address) << " -> " << MEMORY[0x9f00 + address]
		//			 << endl;
		handlingAnInterrupt = true;
		// Routine at Address
		//		cout << debug() << endl;
		pushToStack(functionStack, PC - 1);
		//		cout << debug() << endl;
		PC = MEMORY[0x9f00 + address];
	}
}

void cpu::writeIO(unsigned char address, unsigned short value) {
	MEMORY[0x9f00 + address] = value;
}
void cpu::pushToStack(unsigned short stackAddress, unsigned short value) {
	unsigned short addr = ++MEMORY[stackAddress];
	MEMORY[stackAddress + addr] = value;
}
unsigned short cpu::popFromStack(unsigned short stackAddress) {
	//	cout << debug() << endl;
	unsigned short addr = MEMORY[stackAddress]--;
	//	cout << "Just popped and got " << MEMORY[stackAddress + addr] << endl;
	return MEMORY[stackAddress + addr];
}
void cpu::tick(QImage& imageRef) {
	while (!_shouldUpdate && !halted && !broken) {
		processNext(imageRef);
	}
}
unsigned short cpu::getFromMemory(unsigned short address) {
	if (address == 0x9fff) {
		return (unsigned short) rand();
	}
	return MEMORY[address];
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
	if (address == 0x9ffe) {
		displayMode = value & 0b11;
		std::cout << "Disp mode " << displayMode << std::endl;
	}
}
unsigned int cpu::getValue() {
	PC++;
	return getFromMemory(PC);
}
unsigned int cpu::getAtAddress() {
	unsigned short add = getValue();
	return getFromMemory(add);
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
		int r = (int) ((thisByte & 0b1000) != 0);
		int g = (int) ((thisByte & 0b0100) != 0);
		int b = (int) ((thisByte & 0b0010) != 0);
		int light = (int) ((thisByte & 0b0001) != 0);
		r = r * 0x80 + light * 0x7f;
		g = g * 0x80 + light * 0x7f;
		b = b * 0x80 + light * 0x7f;
		//		cout << "(" << r << ", " << g << ", " << b << ")" << endl;

		return qRgba(r, g, b, 255);
	}
	case 2: {
		thisByte = value / (1 << (16 - ((selector + 1) * 4))) % 0b10000;
		return convertColor(MEMORY[0x9f80 + thisByte], 0, 0);
	}
	case 3: {
		thisByte = (value >> (15 - selector)) % 0b10;
		//		cout << thisByte << endl;
		return convertColor(MEMORY[0x9f80 + thisByte], 0, 0);
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
