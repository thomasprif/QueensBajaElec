#include "debug.h"

char debug_buffer[DEBUG_BUFFER_SIZE];

void Debug_print(char *string)
{
	uint8_t len = strlen (string);
	HAL_UART_Transmit(&huart1, (uint8_t *) string, len, HAL_MAX_DELAY);  // transmit in blocking mode
}

void Debug_clear_buffer () {
	for (int i=0; i<DEBUG_BUFFER_SIZE; i++) debug_buffer[i] = '\0';
}

void Debug_HAL_function_assert(HAL_StatusTypeDef res)
{
	if (res != HAL_OK) {
		Debug_print("BA,SM,Error State Reached,JA\r\n");
		while (1) {
			HAL_Delay(1000);
		}
	}
}