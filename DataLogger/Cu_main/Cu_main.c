#include "main.h"
#include "can_al.h"
#include "fatfs.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

CAN_ConnectionTypeDef CAN;
extern UART_HandleTypeDef huart1;

#define BUFFER_SIZE 128

FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

char buffer[BUFFER_SIZE];  // to store strings..

int bufsize (char *buf) {
	int i=0;
	while (*buf++ != '\0') i++;
	return i;
}

void clear_buffer () {
	for (int i=0; i<BUFFER_SIZE; i++) buffer[i] = '\0';
}

void myprintf (char *string)
{
	uint8_t len = strlen (string);
	HAL_UART_Transmit(&huart1, (uint8_t *) string, len, HAL_MAX_DELAY);  // transmit in blocking mode
}

FRESULT open_append (
    FIL* fp,            /* [OUT] File object to create */
    const char* path    /* [IN]  File name to be opened */
)
{
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

void Cu_main(){
    myprintf("\r\n~ SD card demo by kiwih ~\r\n\r\n");

    HAL_Delay(2000); //a short delay is important to let the SD card settle

    fresult = f_mount(&fs, "/", 1);
    if (fresult != FR_OK) 
	myprintf("ERROR!!! in mounting SD CARD...\n\n");
    else myprintf("SD CARD mounted successfully...\n\n");
	sprintf(buffer, "%d\r\n", fresult);
	myprintf(buffer);
	clear_buffer();


    f_getfree("", &fre_clust, &pfs);

  	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
  	sprintf (buffer, "SD CARD Total Size: \t%lu\n",total);
  	myprintf(buffer);
  	clear_buffer();
  	free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
  	sprintf (buffer, "SD CARD Free Space: \t%lu\n\n",free_space);
  	myprintf(buffer);
  	clear_buffer();

    /************* The following operation is using PUTS and GETS *********************/

  	/* Open file to write/ create a file if it doesn't exist */
    fresult = open_append(&fil, "test.txt");
	sprintf(buffer, "%d\r\n", fresult);
	myprintf(buffer);
	clear_buffer();

  	/* Writing text */
  	f_puts("This data is from the FILE1.txt. And it was written using ...f_puts... ", &fil);

  	/* Close file */
  	fresult = f_close(&fil);

  	//if (fresult == FR_OK)myprintf ("File1.txt created and the data is written \n");

  	/* Open file to read */
  	//fresult = f_open(&fil, "file1.txt", FA_READ);

  	/* Read string from the file */
	/*
  	f_gets(buffer, f_size(&fil), &fil);

  	myprintf("File1.txt is opened and it contains the data as shown below\n");
  	myprintf(buffer);
  	myprintf("\n\n");
	*/
  	/* Close file */
  	//f_close(&fil);

  	//clear_buffer();

      	/**************** The following operation is using f_write and f_read **************************/

  	/* Create second file with read write access and open it */
  	fresult = f_open(&fil, "file2.txt", FA_CREATE_ALWAYS | FA_WRITE);
	sprintf(buffer, "%d\r\n", fresult);
	myprintf(buffer);
	clear_buffer();

  	/* Writing text */
  	strcpy (buffer, "This is File2.txt, written using ...f_write... and it says Hello from Controllerstech\n");

  	fresult = f_write(&fil, buffer, bufsize(buffer), &bw);

  	myprintf ("File2.txt created and data is written\n");

  	/* Close file */
  	f_close(&fil);



  	// clearing buffer to show that result obtained is from the file
  	clear_buffer();

  	/* Open second file to read */
  	fresult = f_open(&fil, "file2.txt", FA_READ);
  	if (fresult == FR_OK)myprintf ("file2.txt is open and the data is shown below\n");

  	/* Read data from the file
  	 * Please see the function details for the arguments */
  	f_read (&fil, buffer, f_size(&fil), &br);
  	myprintf(buffer);
  	myprintf("\n\n");

  	/* Close file */
  	f_close(&fil);

  	clear_buffer();

	uint16_t ids[] = {0x59D};
    CAN_Add_Filter_Discrete(&CAN, 1, ids);
    CAN_Start(&CAN, 0x40);
    uint16_t rpms[4] = {0};
	myprintf("Welcome\r\n");

	char filename[BUFFER_SIZE];
	int done = 0;
	for(int i=0; i<255; i++){
		sprintf(buffer, "cvt%d.txt", i);
		fresult = f_stat(buffer, &fno);
		switch (fresult){
		case FR_OK:
			break;
		
		case FR_NO_FILE:
		case FR_NO_PATH:
			strcpy(filename, buffer);
			done = 1;
			break;

		default:
			break;
		}
		clear_buffer();
		if(done) break;

	}

    while(1){
        if(CAN_ISR_FLAG){
            CAN_Unpack_RX_2Byte(&CAN, 4, rpms);
			fresult = open_append(&fil, filename);
			sprintf(buffer, "%d\t%d\t%d\t%d\r\n",rpms[0],rpms[1],rpms[2],rpms[3]);
			f_puts(buffer, &fil);
			clear_buffer();
			f_close(&fil);
            CAN_ISR_FLAG = 0;
        }
       //myprintf("Test");
       //HAL_Delay(1000);
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