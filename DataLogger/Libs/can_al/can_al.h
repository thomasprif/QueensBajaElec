#ifndef CAN_AL_H
#define CAN_AL_H

#include "main.h"

extern CAN_HandleTypeDef hcan;

typedef struct CAN_Connection {
    CAN_TxHeaderTypeDef     TxDataHeader;
    uint8_t                 TxData[8];
    uint32_t                TxMailbox;
    CAN_RxHeaderTypeDef     RxHeader;
    uint8_t                 RxData[8];
} CAN_ConnectionTypeDef;

extern int CAN_ISR_FLAG;
extern CAN_ConnectionTypeDef* Lib_CAN_Connection_ptr;

void CAN_Start(CAN_ConnectionTypeDef* CAN_Connection, uint16_t id);
void CAN_Send(CAN_ConnectionTypeDef* CAN_Connection, int length);
void CAN_Add_Filter_Discrete(CAN_ConnectionTypeDef* CAN_Connection, int length, uint16_t* ids);
void CAN_Pack_TX_2Byte(CAN_ConnectionTypeDef* CAN_Connection, int length, uint16_t* data);
void CAN_Unpack_RX_2Byte(CAN_ConnectionTypeDef* CAN_Connection, int length, uint16_t* data);

#endif

