#include "main.h"
#include "can_al.h"
#include "led.h"
#include "rpm.h"

CAN_ConnectionTypeDef CAN;


void Cu_main(){
    status_led(STATUS_OK);

    uint16_t ids[] = {0x447, 0x449, 0x1, 0x2, 0x3, 0x445};
    CAN_Add_Filter_Discrete(&CAN, 6, ids);

    CAN_Start(&CAN, 0x447);

    




    while(1){
        if(CAN_ISR_FLAG){
            for(int i=0; i < CAN.RxData[0] + CAN.RxData[1]; i++){
                HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
                HAL_Delay(200);
                HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
                HAL_Delay(200);
            }
            CAN_ISR_FLAG = 0;
        }
    }
}

/*

*/

/*
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

    uint16_t ids[] = {0x447, 0x446, 0x1, 0x2, 0x3, 0x448};
    CAN_Add_Filter_Discrete(&CAN, 6, ids);

    CAN_Start(&CAN, 0x446);

    while(1){
        HAL_Delay(3000);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        CAN.TxData[0] = 1;
        CAN.TxData[1] = 2;
        CAN_Send(&CAN, 2);

        HAL_Delay(3000);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        CAN.TxData[0] = 2;
        CAN.TxData[1] = 3;
        CAN_Send(&CAN, 2);

    }
*/