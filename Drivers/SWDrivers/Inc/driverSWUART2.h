<<<<<<< HEAD
#include "stm32f3xx_hal.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include "driverHWUART2.h"
#include "libRingbuffer.h"

#define RINGBUFFERSIZE									1024

void driverSWUART2Init(uint32_t baudRate);
char driverSWUART2PutCharInOutputBuffer(char character, FILE *stream);
bool driverSWUART2Task(void);
=======
#include "stm32f3xx_hal.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include "driverHWUART2.h"
#include "libRingbuffer.h"

#define RINGBUFFERSIZE									1024

void driverSWUART2Init(uint32_t baudRate);
char driverSWUART2PutCharInOutputBuffer(char character, FILE *stream);
bool driverSWUART2Task(void);
>>>>>>> 8f5bc7a7b1425c6768c1e60b7c3e4e2256d31cb5
