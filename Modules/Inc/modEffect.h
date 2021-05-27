#include "modDelay.h"
#include "driverHWStatus.h"

// Public functions:
void modEffectInit(void);
void modEffectTask(void);
void modEffectChangeState(STATIDTypedef id, STATStateTypedef status);
void modEffectChangeStateError(STATIDTypedef id, STATStateTypedef status, uint8_t errorCode);

// Private functions:
STATStateTypedef modEffectTaskFlash(void);
STATStateTypedef modEffectTaskFlashFast(void);
STATStateTypedef modEffectTaskBlinkShort(uint32_t LEDPointer, uint32_t blinkTime);
STATStateTypedef modEffectTaskBlinkLong(uint32_t LEDPointer, uint32_t blinkTime);
STATStateTypedef modEffectTaskBlinkShortLong(uint32_t blinkTimeShort, uint32_t blinkRatio);
STATStateTypedef modEffectTaskError(uint32_t blinkTime, uint32_t blinkRatio, uint32_t LEDPointer);
