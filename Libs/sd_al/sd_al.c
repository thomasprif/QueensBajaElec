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

	DIR dir;
	FILINFO fno;
	FRESULT res;

	f_opendir(&dir, "data");

	uint16_t max_file_number = 0;
	uint16_t file_number = 0;

	for(;;){
		res = f_readdir(&dir, &fno);

		if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
		if (fno.fname[0] == '.') continue;	

		file_number = (fno.fname[5]-'0') * 100 + (fno.fname[6]-'0') * 10 + (fno.fname[7]-'0');

		if (file_number > max_file_number){
			max_file_number = file_number;
		}
	}

	sprintf(filename, FILENAME_BASE, max_file_number+1);
}
