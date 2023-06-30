#include "cpu.h"

cpu::cpu()
{

}
void cpu::tick(QImage& imageRef) {
	for (int i = 0; i < 128 * 128; i++) {
		imageRef.setPixelColor(i % 128, i / 128, i * 16);
	}
}
