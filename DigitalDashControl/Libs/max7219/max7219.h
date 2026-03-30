#pragma once
#include "spi.h"
#include "stdint.h"
#include "math.h"

#define MAX7219_MAX_NUM_SEG 8

typedef struct
{

    SPI_HandleTypeDef* hspi;
    uint8_t num_of_7segs;
    uint8_t decode_mode;
    GPIO_TypeDef* enable_pin_port;
    uint16_t enable_pin;

}max7219_t;

typedef struct
{
    
    uint8_t decode_mode;
    uint8_t brightness;
    uint8_t num_of_7segs;
    SPI_HandleTypeDef* hspi;
    GPIO_TypeDef* enable_pin_port;
    uint16_t enable_pin;

}max7219_config_t;

int max7219_write_symbol(max7219_t* max7219, uint8_t disp, uint8_t symbol);

int max7219_write_float(max7219_t* max7219, float number, int dec);

int max7219_write_symbols(max7219_t* max7219, uint8_t* symbols, int len);

int max7219_write_int(max7219_t* max7219, int num);

int max7219_init(max7219_config_t* config, max7219_t* max7219);

int max7219_set_brightness(max7219_t* max7219, uint8_t brightness);

int max7219_send(max7219_t* max7219, uint8_t address, uint8_t data);

int max7219_enable(max7219_t* max7219);  

int max7219_disable(max7219_t* max7219);

uint8_t max7219_num_to_symbol(uint8_t num);

uint8_t max7219_letter_to_symbol(uint8_t letter);

#define MAX7219_SYMBOL_BLANK 0x00

#define MAX7219_SYMBOL_A 0x77
#define MAX7219_SYMBOL_b 0x1F
#define MAX7219_SYMBOL_C 0x4E
#define MAX7219_SYMBOL_d 0x3D
#define MAX7219_SYMBOL_E 0x4F
#define MAX7219_SYMBOL_F 0x47
#define MAX7219_SYMBOL_g 0x7B
#define MAX7219_SYMBOL_H 0x37
#define MAX7219_SYMBOL_I 0x06
#define MAX7219_SYMBOL_J 0x38
#define MAX7219_SYMBOL_L 0x0E
#define MAX7219_SYMBOL_n 0x15
#define MAX7219_SYMBOL_o 0x1D
#define MAX7219_SYMBOL_P 0x67
#define MAX7219_SYMBOL_q 0x73
#define MAX7219_SYMBOL_r 0x66
#define MAX7219_SYMBOL_S 0x5B
#define MAX7219_SYMBOL_t 0x0F
#define MAX7219_SYMBOL_u 0x1C
#define MAX7219_SYMBOL_y 0x3B
#define MAX7219_SYMBOL_Z 0x5B

#define MAX7219_SYMBOL_0 0x7E
#define MAX7219_SYMBOL_1 0x30
#define MAX7219_SYMBOL_2 0x6D
#define MAX7219_SYMBOL_3 0x79
#define MAX7219_SYMBOL_4 0x33
#define MAX7219_SYMBOL_5 0x5B
#define MAX7219_SYMBOL_6 0x5F
#define MAX7219_SYMBOL_7 0x70
#define MAX7219_SYMBOL_8 0x7F
#define MAX7219_SYMBOL_9 0x73

#define MAX7219_BRIGHTNESS_MIN 0x00
#define MAX7219_BRIGHTNESS_ONE_QUARTER 0x03
#define MAX7219_BRIGHTNESS_HALF 0x07
#define MAX7219_BRIGHTNESS_THREE_QUARTER 0x0B
#define MAX7219_BRIGHTNESS_MAX 0x0F

#define MAX7219_DECIMAL_POINT_MASK 0x80

#define MAX7219_DECODE_OFF 0x00

#define MAX7219_DECODE_ON 0x0F