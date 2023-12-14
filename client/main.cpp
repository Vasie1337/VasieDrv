#include "driver.h"

int main() 
{
	c_driver::initComm(L"explorer.exe");
	c_driver::getBase();

	printf("base: 0x%p\n", (void*)c_driver::targetBase);

	c_driver::writeMem<int>(0x14DA2BC0, 1338);
	int value = c_driver::readMem<int>(0x14DA2BC0);
	printf("value: %i\n", value);
}