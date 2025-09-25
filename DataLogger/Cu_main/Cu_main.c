#include "main.h"
#include "can_al.h"
#include "fatfs.h"
#include "sd_al.h"
#include "debug.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 128

CAN_ConnectionTypeDef CAN;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1;

char buffer[BUFFER_SIZE];
uint8_t rx_data[128];
void clear_buffer () { 
	for (int i=0; i<BUFFER_SIZE; i++) buffer[i] = '\0';
}

char filename[128];
int SD_REQUEST_DATA_ISR_FLAG = 0;

void Cu_main(){
    Debug_print("BA,SM,Welcome,JA\r\n");
    HAL_Delay(500);

	mount_card();
	Debug_print("BA,SM,SD Card Mounted,JA\r\n");

	uint16_t ids[] = {0x59D};
    Debug_HAL_function_assert(CAN_Add_Filter_Discrete(&CAN, 1, ids));
	Debug_print("BA,SM,CAN Filters Configured,JA\r\n");

    Debug_HAL_function_assert(CAN_Start(&CAN, 0x40));
	Debug_print("BA,SM,CAN Started,JA\r\n");

	Debug_HAL_function_assert(HAL_UART_Receive_IT(&huart1, rx_data, 10));
	Debug_print("BA,SM,UARTRx Intterupt Started,JA\r\n");

	
	fresult = f_mkdir("data");
	if (fresult != FR_OK && fresult != FR_EXIST) {
		Debug_print("BA,SM,Error Creating data Directory,JA\r\n");
	} else if (fresult == FR_OK) {
		Debug_print("BA,SM,data Directory Created,JA\r\n");
	} else if (fresult == FR_EXIST) {
		Debug_print("BA,SM,data Directory Already Exists,JA\r\n");
	}

	//SD_get_filename(filename);
	sprintf(filename, "test.txt");


	HAL_TIM_Base_Start_IT(&htim1);

	open_append(&fil, filename);

	SD_CANDataFrameTypeDef RxFrame;

	Debug_print("BA,SM,Ready,JA\r\n");

    while(1){
        if(CAN_ISR_FLAG){
			RxFrame.timestamp = HAL_GetTick();
			RxFrame.id = CAN.RxHeader.StdId;
			memcpy(RxFrame.data, CAN.RxData, 8);
			sprintf(buffer, "%lu,0x%03X,%d,%d,%d,%d\n", RxFrame.timestamp, RxFrame.id, 
					RxFrame.data[0]*256 + RxFrame.data[1], RxFrame.data[2]*256 + RxFrame.data[3],
					RxFrame.data[4]*256 + RxFrame.data[5], RxFrame.data[6]*256 + RxFrame.data[7]);
			f_puts(buffer, &fil);
			Debug_print(buffer);
			clear_buffer();
            CAN_ISR_FLAG = 0;
        }

		if(SD_SAVE_ISR_FLAG){
			f_sync(&fil);
			//Debug_print("BA,SM,Data Saved,JA\r\n");
			SD_SAVE_ISR_FLAG = 0;
		}

		if(SD_REQUEST_DATA_ISR_FLAG){
			HAL_TIM_Base_Stop_IT(&htim1);
			SD_SAVE_ISR_FLAG = 0;
			uint8_t file_buffer[8192];
			UINT bytesRead;
			f_close(&fil);
			f_open(&fil, filename, FA_READ);
			DWORD file_size = f_size(&fil);
			HAL_UART_Transmit(&huart1, (uint8_t*)&file_size, sizeof(DWORD), HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart1, (uint8_t *)"START\n", 6, HAL_MAX_DELAY);
			do {
				f_read(&fil, file_buffer, sizeof(file_buffer), &bytesRead);
				if (bytesRead > 0) {	
					HAL_UART_Transmit(&huart1, file_buffer, bytesRead, HAL_MAX_DELAY);
				}
			} while (bytesRead == sizeof(file_buffer));
			HAL_UART_Transmit(&huart1, (uint8_t *)"EOF\n", 4, HAL_MAX_DELAY);
			f_close(&fil);
			NVIC_SystemReset();
		}
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	HAL_UART_Transmit(huart, rx_data, 10, HAL_MAX_DELAY);
	if (strncmp((char*)rx_data, "BA,RE,JA\r\n", 10) == 0) {
		Debug_print("BA,SM,Resetting System,JA\r\n");
		NVIC_SystemReset();
	} else if (strncmp((char*)rx_data, "BA,RD,JA\r\n", 10) == 0) {
		Debug_print("BA,SM,Sending Data,JA\r\n");
		SD_REQUEST_DATA_ISR_FLAG = 1;
	}
	HAL_UART_Receive_IT(huart, rx_data, 10);  // Re-enable reception
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM1){  // Assuming TIM2 is used for periodic tasks
		SD_SAVE_ISR_FLAG = 1;
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