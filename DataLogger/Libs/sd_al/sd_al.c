#include "sd_al.h"
#include "debug.h"

FATFS fs;
FIL fil; 
FRESULT fresult;
FILINFO fno;
UINT br, bw;

FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

SD_CANDataFrameTypeDef SD_CANDataFrame_buffer[500];
int SD_CANDataFrame_buffer_size = 0;

int SD_SAVE_ISR_FLAG = 0;

#define BUFFER_SIZE 128
static char buffer[BUFFER_SIZE];
static void clear_buffer () { 
	for (int i=0; i<BUFFER_SIZE; i++) buffer[i] = '\0';
}

FRESULT open_append (
    FIL* fp,            /* [OUT] File object to create */
    const char* path    /* [IN]  File name to be opened */
) {
    FRESULT fr;

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS);
    if (fr == FR_OK) {
        /* Seek to end of the file to append data */
        fr = f_lseek(fp, f_size(fp));
        if (fr != FR_OK)
            f_close(fp);
    }
    return fr;
}

void mount_card(){
	if(f_mount(&fs, "/", 1) != FR_OK) {
		NVIC_SystemReset();
	}
}

void SD_get_filename (char* filename) {
	for(int i=0; i<9999; i++){
		sprintf(buffer, FILENAME_BASE, i);
		fresult = f_stat(buffer, &fno);
		switch (fresult){
		case FR_OK:
			break;
		
		case FR_NO_FILE:
		case FR_NO_PATH:
			strcpy(filename, buffer);
			return;
            break;

		default:
			break;
		}
		clear_buffer();
	}
}
