#include "main.h"
#include "can_al.h"
#include "rpm.h"

CAN_ConnectionTypeDef CAN;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

uint16_t rpm_data[4];

void Cu_main(){
    uint16_t ids[] = {0x40};
    CAN_Add_Filter_Discrete(&CAN, 1, ids);
    CAN_Start(&CAN, 0x59D);
    RPM_Init();
    CAN_Send(&CAN, 8);
    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_Base_Start(&htim2);
    HAL_TIM_Base_Start_IT(&htim3);

    while(1){
        if(RPM_EXTI_FLAG){
            RPM_Update();
            RPM_EXTI_FLAG = 0;
        }
    }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){

    if(htim != &htim3) return;

    rpm_data[0] = rpms[0].current_rpm;
    rpm_data[1] = rpms[1].current_rpm;
    rpm_data[2] = rpms[2].current_rpm;
    rpm_data[3] = rpms[3].current_rpm;

    CAN_Pack_TX_2Byte(&CAN, 4, rpm_data);

    CAN_Send(&CAN, 8);
}

/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    switch (GPIO_Pin)
    {
    case RPM0_Pin:
        CAN.TxData[0] += 1;
        break;

    case RPM1_Pin:
        CAN.TxData[1] += 1;
        break;

    case RPM2_Pin:
        CAN.TxData[2] += 1;
        break;

    case RPM3_Pin:
        CAN.TxData[3] += 1;
        break;

    default:
        break;
    }

}
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