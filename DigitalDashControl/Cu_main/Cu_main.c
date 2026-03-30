#include "Cu_main.h"
#include "max7219.h"

extern SPI_HandleTypeDef hspi2;

uint8_t FISH[4] = {
    MAX7219_SYMBOL_F,
    MAX7219_SYMBOL_I,
    MAX7219_SYMBOL_S,
    MAX7219_SYMBOL_H
};

uint8_t CAR[3] = {
    MAX7219_SYMBOL_C,
    MAX7219_SYMBOL_A,
    MAX7219_SYMBOL_r
};

uint8_t FISH_CAR[13] = {
    MAX7219_SYMBOL_F,
    MAX7219_SYMBOL_I,
    MAX7219_SYMBOL_S,
    MAX7219_SYMBOL_H,
    MAX7219_SYMBOL_BLANK,
    MAX7219_SYMBOL_C,
    MAX7219_SYMBOL_A,
    MAX7219_SYMBOL_r,
    MAX7219_SYMBOL_BLANK,
    MAX7219_SYMBOL_BLANK,
    MAX7219_SYMBOL_F,
    MAX7219_SYMBOL_I,
    MAX7219_SYMBOL_S,
    MAX7219_SYMBOL_H
};


uint8_t Smiley[4] = {
    0x8C,
    0xEB,
    0xEB,
    0x18
};

uint8_t Smile_left[4] = {
    0x8C,
    0xAA,
    0x18,
    MAX7219_SYMBOL_BLANK
};

uint8_t Smile_right[4] = {
    MAX7219_SYMBOL_BLANK,
    0x8C,
    0xAA,
    0x18,
};


uint8_t Penis[4] = {
    MAX7219_SYMBOL_8,
    0x1,
    0x1,
    0x1
};

void Cu_main()
{

    max7219_t dash;
    max7219_config_t config = {
        .brightness = MAX7219_BRIGHTNESS_MAX,
        .decode_mode = MAX7219_DECODE_OFF,
        .enable_pin = GPIO_PIN_4,
        .enable_pin_port = GPIOB,
        .hspi = &hspi2,
        .num_of_7segs = 4
    };

    max7219_init(&config, &dash);

    while(1)
    {
        for(int j = 0; j < 10; j++)
        {

            for(int i = 0; i<10; i++)
            {

                max7219_write_symbols(&dash, &FISH_CAR[i], 4);
                HAL_Delay(250);

            }

        }
        
        max7219_write_symbols(&dash, FISH, 4);
        HAL_Delay(500);
        max7219_write_symbols(&dash, CAR, 3);
        HAL_Delay(500);

        max7219_write_symbols(&dash, FISH, 4);
        HAL_Delay(500);
        max7219_write_symbols(&dash, CAR, 3);
        HAL_Delay(500);

        max7219_write_symbols(&dash, Smiley, 4);
        HAL_Delay(1500);

        max7219_write_symbols(&dash, Penis, 4);
        HAL_Delay(1500);

        for(int i = 0; i<10; i++)
        {

            max7219_write_symbols(&dash, Smile_left, 4);
            HAL_Delay(200);
            max7219_write_symbols(&dash, Smile_right, 4);
            HAL_Delay(200);
            
        }

    }

}