#ifndef I2C_H_
#define I2C_H_

void I2C1_INIT(void);
void I2C1_byteRead(char source_addr, char memory_addr, char* data);
void I2C1_burstRead(char source_addr, char memory_addr, int n, char* data);
void I2C1_burstWrite(char source_addr, char memory_addr, int n, char* data);

#endif
