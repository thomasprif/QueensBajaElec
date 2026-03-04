#ifndef DEBUG_H
#define DEBUG_H

#include "main.h"
#include <string.h>

#define DEBUG_BUFFER_SIZE 128

extern UART_HandleTypeDef huart1;
extern char debug_buffer[DEBUG_BUFFER_SIZE];

void Debug_print(char *string);
void Debug_clear_buffer ();
void Debug_HAL_function_assert(HAL_StatusTypeDef res);

#endif