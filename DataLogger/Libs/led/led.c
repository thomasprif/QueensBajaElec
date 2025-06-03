#include "led.h"
/**
 * @brief This function controls a status LED based on a provided code.
 * The LED's blinking pattern is determined by the bits in the `code` parameter.
 * 
 * @param code A 8-bit integer where each bit represents a specific LED state.
 *        The most significant bit (MSB, 0x80) controls whether the LED should blink in a loop. Thus stopping all interupts and halting code execution
 *        The remaining bits (7 to 0) control the LED's on/off state at each step.
 * 
 * The function performs the following:
 * - If the MSB (bit 7) of the code is set (1), the LED will blink in a loop.
 * - If the MSB is not set (0), the LED will blink once according to the code.
 * - Each bit from 1 to 7 in the `code` determines the on/off state of the LED, with `1` meaning ON and `0` meaning OFF.
 * - The LED will change states every 200 milliseconds.
 */
void status_led(uint8_t code){
    int loop;
    if(code & 0x80){
        loop = 1;
    } else {
        loop = 0;
    }

    if(loop == 1){
        __disable_irq();
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    }

    do{
        for(int i=1; i<=7; i++){
            if((code << i) & 0x80){
                HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, GPIO_PIN_RESET);
            }
            HAL_Delay(200);
        }
    } while (loop);

}