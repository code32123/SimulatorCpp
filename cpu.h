#ifndef CPU_H
#define CPU_H

#include "QImage"

class cpu {
public:
	cpu();
	void tick(QImage& imageRef);
};

#endif // CPU_H
