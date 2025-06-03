#include "can_al.h"

int CAN_ISR_FLAG = 0;
CAN_ConnectionTypeDef* Lib_CAN_Connection_ptr;

/**
 * @brief  Defines the HAL Callback Function. 
 * 
 * In this case just sets the CAN_ISR_FLAG to 1
 * 
 * @param  canh The global CAN handle defined in can.h.
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* canh){
    HAL_CAN_GetRxMessage(canh, CAN_RX_FIFO0, &(Lib_CAN_Connection_ptr->RxHeader), Lib_CAN_Connection_ptr->RxData);
    CAN_ISR_FLAG = 1;
}



/**
 * @brief Starts the CAN bus
 *  
 * Set's the node's id
 * Defines important TX headers
 * Start the FIFO0 message Callback
 * Saves the CAN_ConnectionTypeDef's address for future use in the callback function
 * 
 * @param CAN_Connection Pointer the CAN_ConnectionTypeDef to start
 * @param id Node's id
 */
void CAN_Start(CAN_ConnectionTypeDef* CAN_Connection, uint16_t id){

    CAN_Connection->TxDataHeader.IDE = CAN_ID_STD;
    CAN_Connection->TxDataHeader.StdId = id;
    CAN_Connection->TxDataHeader.RTR = CAN_RTR_DATA;
  
    HAL_CAN_Start(&hcan);
  
    Lib_CAN_Connection_ptr = CAN_Connection;
    HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}


/**
 * @brief Sends a message onto the CANBUS
 * 
 * Send the first n bytes of CAN_ConnectionTypeDef::TxData to the Bus
 * 
 * @param CAN_Connection Pointer the CAN_ConnectionTypeDef to send from
 * @param length How many bytes of CAN_ConnectionTypeDef::TxData to send
 */
void CAN_Send(CAN_ConnectionTypeDef* CAN_Connection, int length){
    CAN_Connection->TxDataHeader.DLC = length;
    HAL_CAN_AddTxMessage(&hcan, &CAN_Connection->TxDataHeader, CAN_Connection->TxData, &CAN_Connection->TxMailbox);
}

/**
 * @brief Adds filters to a given CAN connection (discrete)
 * 
 * Takes an array on 11-bit integers to use as filters for the CAN Controller
 * 
 * @param CAN_Connection The CAN_ConnectionTypeDef to add filters too
 * @param length Length of ids array
 * @param ids Array of 11-bit CAN identifiers
 */
void CAN_Add_Filter_Discrete(CAN_ConnectionTypeDef* CAN_Connection, int length, uint16_t* ids){
    int ids_left = length;
  
    CAN_FilterTypeDef CAN_Filter_Config;
  
    CAN_Filter_Config.FilterActivation = CAN_FILTER_ENABLE;
    CAN_Filter_Config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    CAN_Filter_Config.FilterMode = CAN_FILTERMODE_IDLIST;
    CAN_Filter_Config.FilterScale = CAN_FILTERSCALE_16BIT;
  
    for(int i = 0; i < ((length-1)/4)+1; i++){
  
      CAN_Filter_Config.FilterBank = i;
  
      if (ids_left-- > 0)
        CAN_Filter_Config.FilterIdHigh = (ids[4*i] << 5);
  
      if (ids_left-- > 0)
        CAN_Filter_Config.FilterIdLow = (ids[4*i + 1] << 5);
  
      if (ids_left-- > 0)
        CAN_Filter_Config.FilterMaskIdHigh = (ids[4*i + 2] << 5);
    
      if (ids_left-- > 0)
        CAN_Filter_Config.FilterMaskIdLow = (ids[4*i + 3] << 5);
  
      HAL_CAN_ConfigFilter(&hcan, &CAN_Filter_Config);
    }
  }

void CAN_Pack_TX_2Byte(CAN_ConnectionTypeDef* CAN_Connection, int length, uint16_t* data){
  for(int i=0; i<length; i++){
    CAN_Connection->TxData[2*i]   = (data[i] >> 8) & 0xFF;
    CAN_Connection->TxData[2*i+1] = (data[i] >> 0) & 0xFF;
  }
}

void CAN_Unpack_RX_2Byte(CAN_ConnectionTypeDef* CAN_Connection, int length, uint16_t* data){
  for(int i=0; i<length; i++){
    data[i] = CAN_Connection->RxData[2*i] + (CAN_Connection->RxData[2*i+1] << 8);
  }
}