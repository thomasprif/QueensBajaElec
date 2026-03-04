#include "max7219.h"

int max7219_init(max7219_config_t* config, max7219_t* max7219)
{

    max7219->hspi = config->hspi;
    max7219->num_of_7segs = config->num_of_7segs;
    max7219->decode_mode = config->decode_mode;
    max7219->enable_pin_port = config->enable_pin_port;
    max7219->enable_pin = config->enable_pin;

    HAL_SPI_Init(max7219->hspi);
    
    max7219_disable(max7219);
    max7219_send(max7219, 0x09, max7219->decode_mode); //Decode Mode Decode Dig 7-0
    max7219_send(max7219, 0x0B, max7219->num_of_7segs-1); //Scan Limit Dig 0-3
    max7219_send(max7219, 0x0A, config->brightness); //Brightness Set Max
    max7219_enable(max7219);

    return 0;

}

int max7219_send(max7219_t* max7219, uint8_t address, uint8_t data)
{

    uint8_t val[2] = {address, data};

    HAL_GPIO_WritePin(max7219->enable_pin_port, max7219->enable_pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef ret = HAL_SPI_Transmit(max7219->hspi, (uint8_t*)val, 2, 100);
    if(ret != HAL_OK){
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    }
    HAL_GPIO_WritePin(max7219->enable_pin_port, max7219->enable_pin, GPIO_PIN_SET);

    return 0;

}

int max7219_enable(max7219_t* max7219)
{

    max7219_send(max7219, 0x0C, 0x01);

    return 0;

}

int max7219_disable(max7219_t* max7219)
{

    max7219_send(max7219, 0x0C, 0x00);

    return 0;

}

int max7219_write_symbol(max7219_t* max7219, uint8_t disp, uint8_t symbol)
{

    if(max7219->decode_mode == MAX7219_DECODE_ON)
        return 2;

    max7219_send(max7219, disp, symbol);

    return 0;

}

int max7219_set_brightness(max7219_t* max7219, uint8_t brightness)
{

    if(brightness > 16)
        return 1;

    max7219_disable(max7219);
    max7219_send(max7219, 0x0A, brightness);
    max7219_enable(max7219);

    return 0;

}

int max7219_write_symbols(max7219_t* max7219, uint8_t* symbols, int len)
{

    if(max7219->decode_mode == MAX7219_DECODE_ON)
        return 2;

    if(len > max7219->num_of_7segs)
        return 1;

    for(int i = 0; i < max7219->num_of_7segs; i++)
    {

        if(i < max7219->num_of_7segs - len)
            max7219_write_symbol(max7219, i + 1, MAX7219_SYMBOL_BLANK);
            
        else
            max7219_send(max7219, i + 1, symbols[i - (max7219->num_of_7segs - len)]);

    }

    return 0;

}

int max7219_write_float(max7219_t* max7219, float number, int dec)
{

    int num = (number * pow(10, dec));

    if(num > pow(10, max7219->num_of_7segs) - 1)
        return 3;
    if(num < 0 && num < -1 * pow(10, max7219->num_of_7segs - 1) + 1)
        return 3;

    if(max7219->decode_mode == MAX7219_DECODE_OFF)
    {

        uint8_t num_array[MAX7219_MAX_NUM_SEG];

        int negative = 0;
        if(num < 0)
        {
            num *= -1;
            negative = 1; 
        }


        for(int i = 0; i < max7219->num_of_7segs; i++)
        {

            num_array[max7219->num_of_7segs - i - 1] = max7219_num_to_symbol(num % 10);
            num /= 10;

        }

        for(int i = 0; i < max7219->num_of_7segs; i++)
        {

            if(num_array[i] == MAX7219_SYMBOL_0){
                if(i == max7219->num_of_7segs - 1)
                    break;
                num_array[i] = MAX7219_SYMBOL_BLANK;
            }
                
            else{
                if(negative)
                    num_array[i-1] = 0x01;
                break;
            }
                

        }

        num_array[max7219->num_of_7segs - dec - 1] += MAX7219_DECIMAL_POINT_MASK;

        max7219_write_symbols(max7219, num_array, max7219->num_of_7segs);

    }

    return 0;

}

int max7219_write_int(max7219_t* max7219, int num)
{

    if(num > pow(10, max7219->num_of_7segs) - 1)
        return 3;
    if(num < 0 && num < -1 * pow(10, max7219->num_of_7segs - 1) + 1)
        return 3;

    if(max7219->decode_mode == MAX7219_DECODE_OFF)
    {

        uint8_t num_array[MAX7219_MAX_NUM_SEG];

        int negative = 0;
        if(num < 0)
        {
            num *= -1;
            negative = 1; 
        }


        for(int i = 0; i < max7219->num_of_7segs; i++)
        {

            num_array[max7219->num_of_7segs - i - 1] = max7219_num_to_symbol(num % 10);
            num /= 10;

        }

        for(int i = 0; i < max7219->num_of_7segs; i++)
        {

            if(num_array[i] == MAX7219_SYMBOL_0){
                if(i == max7219->num_of_7segs - 1)
                    break;
                num_array[i] = MAX7219_SYMBOL_BLANK;
            }
                
            else{
                if(negative)
                    num_array[i-1] = 0x01;
                break;
            }
                

        }

        max7219_write_symbols(max7219, num_array, max7219->num_of_7segs);

    }

    return 0;

}

uint8_t max7219_num_to_symbol(uint8_t num)
{

    switch (num)
    {
    case 0:
        return MAX7219_SYMBOL_0;
    
    case 1:
        return MAX7219_SYMBOL_1;

    case 2:
        return MAX7219_SYMBOL_2;   

    case 3:
        return MAX7219_SYMBOL_3;

    case 4:
        return MAX7219_SYMBOL_4;
    
    case 5:
        return MAX7219_SYMBOL_5;

    case 6:
        return MAX7219_SYMBOL_6;

    case 7:
        return MAX7219_SYMBOL_7;

    case 8:
        return MAX7219_SYMBOL_8;

    case 9:
        return MAX7219_SYMBOL_9;
    }

}

uint8_t max7219_letter_to_symbol(uint8_t letter)
{

    switch (letter)
    {
    case 'a':
        return MAX7219_SYMBOL_A;
    
    case 'b':
        return MAX7219_SYMBOL_b;
    
    case 'c':
        return MAX7219_SYMBOL_C;
    
    case 'd':
        return MAX7219_SYMBOL_d;
    
    case 'e':
        return MAX7219_SYMBOL_E;
    
    case 'f':
        return MAX7219_SYMBOL_F;
    
    case 'g':
        return MAX7219_SYMBOL_g;
    
    case 'h':
        return MAX7219_SYMBOL_H;
    
    case 'i':
        return MAX7219_SYMBOL_I;
    
    case 'j':
        return MAX7219_SYMBOL_J;
    
    case 'k':
        break;
    
    case 'l':
        return MAX7219_SYMBOL_L;
    
    case 'm':
        break;
    
    case 'n':
        return MAX7219_SYMBOL_n;
    
    case 'o':
        return MAX7219_SYMBOL_o;
    
    case 'p':
        return MAX7219_SYMBOL_P;
    
    case 'q':
        return MAX7219_SYMBOL_q;
    
    case 'r':
        return MAX7219_SYMBOL_r;
    
    case 's':
        return MAX7219_SYMBOL_S;
    
    case 't':
        return MAX7219_SYMBOL_t;
    
    case 'u':
        return MAX7219_SYMBOL_u;
    
    case 'v':
        break;
    
    case 'w':
        break;
    
    case 'x':
        break;
    
    case 'y':
        return MAX7219_SYMBOL_y;
    
    case 'z':
        return MAX7219_SYMBOL_Z;
    }
    
}