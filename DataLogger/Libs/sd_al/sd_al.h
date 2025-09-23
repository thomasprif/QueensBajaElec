#ifndef SD_AL_H
#define SD_AL_H

#include "main.h"
#include "fatfs.h"
#include <stdio.h>
#include <string.h>

#define FILENAME_BASE "data/data_%03d.csv"

extern FATFS fs;  // file system
extern FIL fil; // File
extern FRESULT fresult;  // result
extern FILINFO fno;
extern UINT br, bw;  // File read/write count

/**** capacity related *****/
extern FATFS *pfs;
extern DWORD fre_clust;
extern uint32_t total, free_space;

typedef struct SD_CANDataFrame{
    uint32_t timestamp;
    uint16_t id;
    uint8_t data[8];
} SD_CANDataFrameTypeDef;

extern SD_CANDataFrameTypeDef SD_CANDataFrame_buffer[500];
extern int SD_CANDataFrame_buffer_size;

extern int SD_SAVE_ISR_FLAG;

void mount_card ();
FRESULT open_append (FIL* fp, const char* path);
void SD_get_filename (char* filename);

#endif