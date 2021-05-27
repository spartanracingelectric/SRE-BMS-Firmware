/*
	Copyright 2017 - 2018 Danny Bokma	danny@diebie.nl
	Copyright 2019 - 2020 Kevin Dionne	kevin.dionne@ennoid.me

	This file is part of the DieBieMS/ENNOID-BMS firmware.

	The DieBieMS/ENNOID-BMS firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The DieBieMS/ENNOID-BMS firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "modCAN.h"

// Variables
CAN_HandleTypeDef      modCANHandle;
uint32_t               modCANErrorLastTick;
uint32_t               modCANSendStatusSimpleFastLastTisk;
uint32_t               modCANSendStatusSimpleSlowLastTisk;
uint32_t               modCANSafetyCANMessageTimeout;
uint32_t               modCANLastRXID;
uint32_t               modCANLastRXDifferLastTick;
static uint8_t         modCANRxBuffer[RX_CAN_BUFFER_SIZE];
static uint8_t         modCANRxBufferLastID;
static CanRxMsgTypeDef modCANRxFrames[RX_CAN_FRAMES_SIZE];
static uint8_t         modCANRxFrameRead;
static uint8_t         modCANRxFrameWrite;

uint32_t               modCANLastChargerHeartBeatTick;
uint32_t               modCANChargerTaskIntervalLastTick;
bool                   modCANChargerPresentOnBus;
uint8_t                modCANChargerCANOpenState;
uint8_t                modCANChargerChargingState;

ChargerStateTypedef chargerOpState = opInit;
ChargerStateTypedef chargerOpStateNew = opInit;

modPowerElectronicsPackStateTypedef *modCANPackStateHandle;
modConfigGeneralConfigStructTypedef *modCANGeneralConfigHandle;

void modCANInit(modPowerElectronicsPackStateTypedef *packState, modConfigGeneralConfigStructTypedef *generalConfigPointer){
  static CanTxMsgTypeDef        TxMessage;
  static CanRxMsgTypeDef        RxMessage;
	
	modCANPackStateHandle = packState;
	modCANGeneralConfigHandle = generalConfigPointer;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
  modCANHandle.Instance = CAN;
  modCANHandle.pTxMsg = &TxMessage;
  modCANHandle.pRxMsg = &RxMessage;
	
	switch(modCANGeneralConfigHandle->canBusSpeed) {
		case canSpeedBaud125k:
			modCANHandle.Init.Prescaler = 36;
			modCANHandle.Init.Mode = CAN_MODE_NORMAL;
			modCANHandle.Init.SJW = CAN_SJW_1TQ;
			modCANHandle.Init.BS1 = CAN_BS1_5TQ;
			modCANHandle.Init.BS2 = CAN_BS2_2TQ;
			break;
		case canSpeedBaud250k:
			modCANHandle.Init.Prescaler = 18;
			modCANHandle.Init.Mode = CAN_MODE_NORMAL;
			modCANHandle.Init.SJW = CAN_SJW_1TQ;
			modCANHandle.Init.BS1 = CAN_BS1_5TQ;
			modCANHandle.Init.BS2 = CAN_BS2_2TQ;
			break;
		case canSpeedBaud500k:
		default:
			modCANHandle.Init.Prescaler = 9;
			modCANHandle.Init.Mode = CAN_MODE_NORMAL;
			modCANHandle.Init.SJW = CAN_SJW_1TQ;
			modCANHandle.Init.BS1 = CAN_BS1_5TQ;
			modCANHandle.Init.BS2 = CAN_BS2_2TQ;
			break;
	}
	
	modCANHandle.Init.TTCM = DISABLE;
	modCANHandle.Init.ABOM = ENABLE; // Enable this for automatic recovery?
	modCANHandle.Init.AWUM = DISABLE;
	modCANHandle.Init.NART = DISABLE;
	modCANHandle.Init.RFLM = DISABLE;
	modCANHandle.Init.TXFP = DISABLE;
	
  if(HAL_CAN_Init(&modCANHandle) != HAL_OK)
    while(true){};
			
  CAN_FilterConfTypeDef canFilterConfig;
  canFilterConfig.FilterNumber = 0;
  canFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  canFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  canFilterConfig.FilterIdHigh = 0x0000;
  canFilterConfig.FilterIdLow = 0x0000;
  canFilterConfig.FilterMaskIdHigh = 0x0000 << 5;
  canFilterConfig.FilterMaskIdLow = 0x0000;
  canFilterConfig.FilterFIFOAssignment = CAN_FIFO0;
  canFilterConfig.FilterActivation = ENABLE;
  canFilterConfig.BankNumber = 0;
  HAL_CAN_ConfigFilter(&modCANHandle, &canFilterConfig);

  if(HAL_CAN_Receive_IT(&modCANHandle, CAN_FIFO0) != HAL_OK)
    while(true){};

	modCANRxFrameRead = 0;
	modCANRxFrameWrite = 0;
			
	modCANSendStatusSimpleFastLastTisk = HAL_GetTick();
	modCANSendStatusSimpleSlowLastTisk = HAL_GetTick();
	modCANSafetyCANMessageTimeout = HAL_GetTick();
	modCANErrorLastTick = HAL_GetTick();
}

void modCANTask(void){		
	// Manage HAL CAN driver's active state
	if((modCANHandle.State != HAL_CAN_STATE_BUSY_RX)) {
				//		if(modDelayTick1ms(&modCANErrorLastTick,1000))
	  HAL_CAN_Receive_IT(&modCANHandle, CAN_FIFO0);
	}else{
		modCANErrorLastTick = HAL_GetTick();
	}
	
	// if(modCANGeneralConfigHandle->emitStatusOverCAN) {
	// 	// Send status messages with interval
	// 	if(modDelayTick1ms(&modCANSendStatusSimpleFastLastTisk,200))                        // 5 Hz
	// 		modCANSendSimpleStatusFast();
		
	// 	// Send status messages with interval
	// 	if(modDelayTick1ms(&modCANSendStatusSimpleSlowLastTisk,500))                        // 2 Hz
	// 		modCANSendSimpleStatusSlow();
	// }
	
	if(modDelayTick1ms(&modCANSafetyCANMessageTimeout,5000))
		modCANPackStateHandle->safetyOverCANHCSafeNSafe = false;
		
	// Handle received CAN bus data
	modCANSubTaskHandleCommunication();
	modCANRXWatchDog();
	
	// Control the charger
	modCANHandleSubTaskCharger();
}

uint32_t modCANGetDestinationID(CanRxMsgTypeDef canMsg) {
	uint32_t destinationID;
	
	switch(modCANGeneralConfigHandle->CANIDStyle) {
		default:																																					// Default to VESC style ID
	  case CANIDStyleVESC:
			destinationID = canMsg.ExtId & 0xFF;
			break;
		case CANIDStyleFoiler:
			destinationID = (canMsg.ExtId >> 8) & 0xFF;
			break;
	}
	
	return destinationID;
}

CAN_PACKET_ID modCANGetPacketID(CanRxMsgTypeDef canMsg) {
	CAN_PACKET_ID packetID;

	switch(modCANGeneralConfigHandle->CANIDStyle) {
		default:																																					// Default to VESC style ID
	  case CANIDStyleVESC:
			packetID = (CAN_PACKET_ID)((canMsg.ExtId >> 8) & 0xFF);
			break;
		case CANIDStyleFoiler:
			packetID = (CAN_PACKET_ID)((canMsg.ExtId) & 0xFF);
			break;
	}
	
	return packetID;
}

uint32_t modCANGetCANID(uint32_t destinationID, CAN_PACKET_ID packetID) {
	uint32_t returnCANID;
	
	switch(modCANGeneralConfigHandle->CANIDStyle) {
		default:																																					// Default to VESC style ID
	  case CANIDStyleVESC:
			returnCANID = ((uint32_t) destinationID) | ((uint32_t)packetID << 8);
			break;
		case CANIDStyleFoiler:
			returnCANID = ((uint32_t) destinationID << 8) | ((uint32_t)packetID);
			break;
	}
	
  return returnCANID;
}

void modCANSendSimpleStatusFast(void) {
	int32_t sendIndex;
	uint8_t buffer[8];
	uint8_t flagHolder = 0;
	uint8_t disChargeDesiredMask;
	
	if(modCANGeneralConfigHandle->togglePowerModeDirectHCDelay || modCANGeneralConfigHandle->pulseToggleButton){
		disChargeDesiredMask = modCANPackStateHandle->disChargeDesired && modPowerElectronicsHCSafetyCANAndPowerButtonCheck();
	}else{
		disChargeDesiredMask = modCANPackStateHandle->disChargeDesired && modCANPackStateHandle->powerButtonActuated && modPowerElectronicsHCSafetyCANAndPowerButtonCheck();
	}
	
	flagHolder |= (modCANPackStateHandle->chargeAllowed          << 0);
	flagHolder |= (modCANPackStateHandle->chargeDesired          << 1);
	flagHolder |= (modCANPackStateHandle->disChargeLCAllowed     << 2);
	flagHolder |= (disChargeDesiredMask                          << 3);
	flagHolder |= (modCANPackStateHandle->balanceActive   			 << 4);
	flagHolder |= (modCANPackStateHandle->packInSOADischarge     << 5);
	flagHolder |= (modCANPackStateHandle->chargePFETDesired    	 << 6);
	flagHolder |= (modCANPackStateHandle->powerButtonActuated    << 7);
	
	// Send (dis)charge throttle and booleans.
	sendIndex = 0;
	libBufferAppend_float16(buffer, modCANPackStateHandle->loCurrentLoadVoltage,1e2,&sendIndex);
  libBufferAppend_float16(buffer, modCANPackStateHandle->SoCCapacityAh,1e2,&sendIndex);
  libBufferAppend_uint8(buffer, (uint8_t)modCANPackStateHandle->SoC,&sendIndex);
  libBufferAppend_uint8(buffer, modCANPackStateHandle->throttleDutyCharge/10,&sendIndex);
  libBufferAppend_uint8(buffer, modCANPackStateHandle->throttleDutyDischarge/10,&sendIndex);
	libBufferAppend_uint8(buffer,flagHolder,&sendIndex);
	modCANTransmitExtID(modCANGetCANID(modCANGeneralConfigHandle->CANID,CAN_PACKET_BMS_STATUS_THROTTLE_CH_DISCH_BOOL), buffer, sendIndex);
}

void modCANSendSimpleStatusSlow(void) {
	int32_t sendIndex = 0;
	uint8_t buffer[8];
	// Send voltage and current
	sendIndex = 0;
	libBufferAppend_float32(buffer, modCANPackStateHandle->packVoltage,1e5,&sendIndex);
	libBufferAppend_float32(buffer, modCANPackStateHandle->packCurrent,1e5,&sendIndex);
	modCANTransmitExtID(modCANGetCANID(modCANGeneralConfigHandle->CANID,CAN_PACKET_BMS_STATUS_MAIN_IV), buffer, sendIndex);
	
	// Send highest and lowest cell voltage
	sendIndex = 0;
	libBufferAppend_float32(buffer, modCANPackStateHandle->cellVoltageLow,1e5,&sendIndex);
	libBufferAppend_float32(buffer, modCANPackStateHandle->cellVoltageHigh,1e5,&sendIndex);
	modCANTransmitExtID(modCANGetCANID(modCANGeneralConfigHandle->CANID,CAN_PACKET_BMS_STATUS_CELLVOLTAGE), buffer, sendIndex);
}

void CAN_RX0_IRQHandler(void) {
  HAL_CAN_IRQHandler(&modCANHandle);
}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *CanHandle) {
	// Handle CAN message	
	if((*CanHandle->pRxMsg).IDE == CAN_ID_STD) {         // Standard ID
		modCANHandleCANOpenMessage(*CanHandle->pRxMsg);
	}else{                                               // Extended ID
		if((*CanHandle->pRxMsg).ExtId == 0x0A23){
			modCANHandleKeepAliveSafetyMessage(*CanHandle->pRxMsg);
		}else{
			uint8_t destinationID = modCANGetDestinationID(*CanHandle->pRxMsg);
			if(destinationID == modCANGeneralConfigHandle->CANID){
				modCANRxFrames[modCANRxFrameWrite++] = *CanHandle->pRxMsg;
				if(modCANRxFrameWrite >= RX_CAN_FRAMES_SIZE) {
					modCANRxFrameWrite = 0;
				}
			}
		}
	}
	
  HAL_CAN_Receive_IT(&modCANHandle, CAN_FIFO0);
}

void modCANSubTaskHandleCommunication(void) {
	static int32_t ind = 0;
	static unsigned int rxbuf_len;
	static unsigned int rxbuf_ind;
	static uint8_t crc_low;
	static uint8_t crc_high;
	static bool commands_send;

	while(modCANRxFrameRead != modCANRxFrameWrite) {
		CanRxMsgTypeDef rxmsg = modCANRxFrames[modCANRxFrameRead++];

		if(rxmsg.IDE == CAN_ID_EXT) {
			uint8_t destinationID = modCANGetDestinationID(rxmsg);
			CAN_PACKET_ID cmd = modCANGetPacketID(rxmsg);

			if(destinationID == modCANGeneralConfigHandle->CANID) {
				switch(cmd) {
					case CAN_PACKET_FILL_RX_BUFFER:
  					memcpy(modCANRxBuffer + rxmsg.Data[0], rxmsg.Data + 1, rxmsg.DLC - 1);
						break;

					case CAN_PACKET_FILL_RX_BUFFER_LONG:
						rxbuf_ind = (unsigned int)rxmsg.Data[0] << 8;
						rxbuf_ind |= rxmsg.Data[1];
						if(rxbuf_ind < RX_CAN_BUFFER_SIZE) {
							memcpy(modCANRxBuffer + rxbuf_ind, rxmsg.Data + 2, rxmsg.DLC - 2);
						}
						break;

					case CAN_PACKET_PROCESS_RX_BUFFER:
						ind = 0;
						modCANRxBufferLastID = rxmsg.Data[ind++];
						commands_send = rxmsg.Data[ind++];
						rxbuf_len = (unsigned int)rxmsg.Data[ind++] << 8;
						rxbuf_len |= (unsigned int)rxmsg.Data[ind++];

						if(rxbuf_len > RX_CAN_BUFFER_SIZE) {
							break;
						}

						crc_high = rxmsg.Data[ind++];
						crc_low = rxmsg.Data[ind++];

						if(libCRCCalcCRC16(modCANRxBuffer, rxbuf_len) == ((unsigned short) crc_high << 8 | (unsigned short) crc_low)) {

							if(commands_send) {
								modCommandsSendPacket(modCANRxBuffer, rxbuf_len);
							}else{
								modCommandsSetSendFunction(modCANSendPacketWrapper);
								modCommandsProcessPacket(modCANRxBuffer, rxbuf_len);
							}
						}
						break;

					case CAN_PACKET_PROCESS_SHORT_BUFFER:
						ind = 0;
						modCANRxBufferLastID = rxmsg.Data[ind++];
						commands_send = rxmsg.Data[ind++];

						if(commands_send) {
							modCommandsSendPacket(rxmsg.Data + ind, rxmsg.DLC - ind);
						}else{
							modCommandsSetSendFunction(modCANSendPacketWrapper);
							modCommandsProcessPacket(rxmsg.Data + ind, rxmsg.DLC - ind);
						}
						break;
					default:
						break;
					}
				}
		}

		if(modCANRxFrameRead >= RX_CAN_FRAMES_SIZE)
			modCANRxFrameRead = 0;
	}
}

void modCANTransmitExtID(uint32_t id, uint8_t *data, uint8_t len) {
	CanTxMsgTypeDef txmsg;
	txmsg.IDE = CAN_ID_EXT;
	txmsg.ExtId = id;
	txmsg.RTR = CAN_RTR_DATA;
	txmsg.DLC = len;
	memcpy(txmsg.Data, data, len);
	
	modCANHandle.pTxMsg = &txmsg;
	HAL_CAN_Transmit(&modCANHandle,1);
}

void modCANTransmitStandardID(uint32_t id, uint8_t *data, uint8_t len) {
	CanTxMsgTypeDef txmsg;
	txmsg.IDE = CAN_ID_STD;
	txmsg.StdId = id;
	txmsg.RTR = CAN_RTR_DATA;
	txmsg.DLC = len;
	memcpy(txmsg.Data, data, len);
	
	modCANHandle.pTxMsg = &txmsg;
	HAL_CAN_Transmit(&modCANHandle,1);
}

/**
 * Send a buffer up to RX_BUFFER_SIZE bytes as fragments. If the buffer is 6 bytes or less
 * it will be sent in a single CAN frame, otherwise it will be split into
 * several frames.
 *
 * @param controller_id
 * The controller id to send to.
 *
 * @param data
 * The payload.
 *
 * @param len
 * The payload length.
 *
 * @param send
 * If true, this packet will be passed to the send function of commands.
 * Otherwise, it will be passed to the process function.
 */
void modCANSendBuffer(uint8_t controllerID, uint8_t *data, unsigned int len, bool send) {
	uint8_t send_buffer[8];

	if(len <= 6) {
		uint32_t ind = 0;
		send_buffer[ind++] = modCANGeneralConfigHandle->CANID;
		send_buffer[ind++] = send;
		memcpy(send_buffer + ind, data, len);
		ind += len;
		modCANTransmitExtID(modCANGetCANID(controllerID,CAN_PACKET_PROCESS_SHORT_BUFFER), send_buffer, ind);
	}else{
		unsigned int end_a = 0;
		for(unsigned int i = 0;i < len;i += 7) {
			if(i > 255) {
				break;
			}

			end_a = i + 7;

			uint8_t send_len = 7;
			send_buffer[0] = i;

			if((i + 7) <= len) {
				memcpy(send_buffer + 1, data + i, send_len);
			}else{
				send_len = len - i;
				memcpy(send_buffer + 1, data + i, send_len);
			}

			modCANTransmitExtID(modCANGetCANID(controllerID,CAN_PACKET_FILL_RX_BUFFER), send_buffer, send_len + 1);
		}

		for(unsigned int i = end_a;i < len;i += 6) {
			uint8_t send_len = 6;
			send_buffer[0] = i >> 8;
			send_buffer[1] = i & 0xFF;

			if((i + 6) <= len) {
				memcpy(send_buffer + 2, data + i, send_len);
			}else{
				send_len = len - i;
				memcpy(send_buffer + 2, data + i, send_len);
			}

			modCANTransmitExtID(modCANGetCANID(controllerID,CAN_PACKET_FILL_RX_BUFFER_LONG), send_buffer, send_len + 2);
		}

		uint32_t ind = 0;
		send_buffer[ind++] = modCANGeneralConfigHandle->CANID;
		send_buffer[ind++] = send;
		send_buffer[ind++] = len >> 8;
		send_buffer[ind++] = len & 0xFF;
		unsigned short crc = libCRCCalcCRC16(data, len);
		send_buffer[ind++] = (uint8_t)(crc >> 8);
		send_buffer[ind++] = (uint8_t)(crc & 0xFF);
    
		// Old ID method
		//modCANTransmitExtID(controllerID | ((uint32_t)CAN_PACKET_PROCESS_RX_BUFFER << 8), send_buffer, ind++);
		modCANTransmitExtID(modCANGetCANID(controllerID,CAN_PACKET_PROCESS_RX_BUFFER), send_buffer, ind++);
	}
}

void modCANSetESCDuty(uint8_t controllerID, float duty) {
	int32_t sendIndex = 0;
	uint8_t buffer[4];
	libBufferAppend_int32(buffer, (int32_t)(duty * 100000.0f), &sendIndex);
	modCANTransmitExtID(modCANGetCANID(controllerID,CAN_PACKET_ESC_SET_DUTY), buffer, sendIndex);
}

void modCANSetESCCurrent(uint8_t controllerID, float current) {
	int32_t sendIndex = 0;
	uint8_t buffer[4];
	libBufferAppend_int32(buffer, (int32_t)(current * 1000.0f), &sendIndex);
	modCANTransmitExtID(modCANGetCANID(controllerID,CAN_PACKET_ESC_SET_CURRENT), buffer, sendIndex);
}

void modCANSetESCBrakeCurrent(uint8_t controllerID, float current) {
	int32_t sendIndex = 0;
	uint8_t buffer[4];
	libBufferAppend_int32(buffer, (int32_t)(current * 1000.0f), &sendIndex);
	modCANTransmitExtID(modCANGetCANID(controllerID,CAN_PACKET_ESC_SET_CURRENT_BRAKE), buffer, sendIndex);
}

void modCANSetESCRPM(uint8_t controllerID, float rpm) {
	int32_t sendIndex = 0;
	uint8_t buffer[4];
	libBufferAppend_int32(buffer, (int32_t)rpm, &sendIndex);
	modCANTransmitExtID(modCANGetCANID(controllerID,CAN_PACKET_ESC_SET_RPM), buffer, sendIndex);
}

void modCANSetESCPosition(uint8_t controllerID, float pos) {
	int32_t sendIndex = 0;
	uint8_t buffer[4];
	libBufferAppend_int32(buffer, (int32_t)(pos * 1000000.0f), &sendIndex);
	modCANTransmitExtID(modCANGetCANID(controllerID,CAN_PACKET_ESC_SET_POS), buffer, sendIndex);
}

void modCANSetESCCurrentRelative(uint8_t controllerID, float currentRel) {
	int32_t sendIndex = 0;
	uint8_t buffer[4];
	libBufferAppend_float32(buffer, currentRel, 1e5, &sendIndex);
	modCANTransmitExtID(modCANGetCANID(controllerID,CAN_PACKET_ESC_SET_CURRENT_REL), buffer, sendIndex);
}

void modCANSetESCBrakeCurrentRelative(uint8_t controllerID, float currentRel) {
	int32_t sendIndex = 0;
	uint8_t buffer[4];
	libBufferAppend_float32(buffer, currentRel, 1e5, &sendIndex);
	modCANTransmitExtID(modCANGetCANID(controllerID,CAN_PACKET_ESC_SET_CURRENT_BRAKE_REL), buffer, sendIndex);
}

static void modCANSendPacketWrapper(unsigned char *data, unsigned int length) {
	modCANSendBuffer(modCANRxBufferLastID, data, length, true);
}

void modCANHandleKeepAliveSafetyMessage(CanRxMsgTypeDef canMsg) {
	if(canMsg.DLC >= 1){
		if(canMsg.Data[0] & 0x01){
			modCANSafetyCANMessageTimeout = HAL_GetTick();
			modCANPackStateHandle->safetyOverCANHCSafeNSafe = (canMsg.Data[0] & 0x02) ? true : false;
		}
		
		if(canMsg.Data[0] & 0x04){
				modCANPackStateHandle->watchDogTime = (canMsg.Data[0] & 0x08) ? 255 : 0;
		}
	}
	
	if(canMsg.DLC >= 2){
		if(canMsg.Data[1] & 0x10){
			modCANPackStateHandle->chargeBalanceActive = modCANGeneralConfigHandle->allowChargingDuringDischarge;
			modPowerElectronicsResetBalanceModeActiveTimeout();
		}
	}
}

void modCANHandleCANOpenMessage(CanRxMsgTypeDef canMsg) {
  if(canMsg.StdId == 0x070A){
		modCANLastChargerHeartBeatTick = HAL_GetTick();
		modCANChargerCANOpenState = canMsg.Data[0];
	}else if(canMsg.StdId == 0x048A){
	  modCANChargerChargingState = canMsg.Data[5];
	}
}

void modCANHandleSubTaskCharger(void) {
  //static uint8_t chargerOpState = opInit;
  //static uint8_t chargerOpStateNew = opInit;
	
  if(modDelayTick1ms(&modCANChargerTaskIntervalLastTick, 500)) {
		// Check charger present
		modCANOpenChargerCheckPresent();
		
		if(modCANChargerPresentOnBus) {
		  // Send HeartBeat from bms
			modCANOpenBMSSendHeartBeat();
		
			// Manage operational state and start network
			if(modCANChargerCANOpenState != 0x05)
				modCANOpenChargerStartNode();
			
			if(modCANChargerCANOpenState == 0x05) {
				switch(chargerOpState) {
					case opInit:
						if(modCANPackStateHandle->powerDownDesired) {
						  modCANOpenChargerSetCurrentVoltageReady(0.0f,0.0f,false);
						}else{
						  chargerOpStateNew = opChargerReset;
						}
						break;
					case opChargerReset:
						modCANOpenChargerSetCurrentVoltageReady(0.0f,0.0f,false);
					  chargerOpStateNew = opChargerSet;
						break;
					case opChargerSet:
						modCANOpenChargerSetCurrentVoltageReady(0.0f,0.0f,true);
					  chargerOpStateNew = opCharging;
						break;
					case opCharging:
						modCANOpenChargerSetCurrentVoltageReady(30.0f*modCANPackStateHandle->throttleDutyCharge/1000,modCANGeneralConfigHandle->noOfCellsSeries*modCANGeneralConfigHandle->cellSoftOverVoltage+0.6f,true);
					
					  if(modCANPackStateHandle->powerDownDesired)
					    chargerOpStateNew = opInit;

						break;
					default:
						chargerOpStateNew = opInit;
				}
				
				chargerOpState = chargerOpStateNew;
				
			  modCANPackStateHandle->chargeBalanceActive = modCANGeneralConfigHandle->allowChargingDuringDischarge;
			  modPowerElectronicsResetBalanceModeActiveTimeout();
		  }
			
	  }else{
		  chargerOpState = opInit;
		}
	}
}

void modCANRXWatchDog(void){
  if(modCANHandle.pRxMsg->ExtId != modCANLastRXID){
	  modCANLastRXID = modCANHandle.pRxMsg->ExtId;
		modCANLastRXDifferLastTick = HAL_GetTick();
	}
	
	if(modDelayTick1ms(&modCANLastRXDifferLastTick,1000)){
		modCANInit(modCANPackStateHandle,modCANGeneralConfigHandle);
	}
}

void modCANOpenChargerCheckPresent(void) {
	if((HAL_GetTick() - modCANLastChargerHeartBeatTick) < 2000)
		modCANChargerPresentOnBus = true;
	else
		modCANChargerPresentOnBus = false;
}

void modCANOpenBMSSendHeartBeat(void) {
  // Send the canopen heartbeat from the BMS
	int32_t sendIndex = 0;
	uint8_t operationalState = 5;
	uint8_t buffer[1];
	libBufferAppend_uint8(buffer, operationalState, &sendIndex);
	modCANTransmitStandardID(0x0701, buffer, sendIndex);
}

void modCANOpenChargerStartNode(void) {
  // Send the canopen heartbeat from the BMS
	int32_t sendIndex = 0;
	uint8_t buffer[2];
	libBufferAppend_uint8(buffer, 0x01, &sendIndex);
	libBufferAppend_uint8(buffer, 0x0A, &sendIndex);	
	modCANTransmitStandardID(0x0000, buffer, sendIndex);
}

void modCANOpenChargerSetCurrentVoltageReady(float current,float voltage,bool ready) {
	uint32_t modCANChargerRequestVoltageInt = voltage * 1024;
	uint16_t modCANChargerRequestCurrentInt = current * 16;
	
	int32_t sendIndex = 0;
	uint8_t buffer[8];
	libBufferAppend_uint16_LSBFirst(buffer, modCANChargerRequestCurrentInt, &sendIndex);
	libBufferAppend_uint8(buffer, ready, &sendIndex);	
	libBufferAppend_uint32_LSBFirst(buffer, modCANChargerRequestVoltageInt, &sendIndex);		
	modCANTransmitStandardID(0x040A, buffer, sendIndex);
}




