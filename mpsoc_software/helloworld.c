#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xbasic_types.h"
#include "xparameters.h"

#include "time.h"
#include <xil_types.h>
#include "ff.h"
#include "xil_cache.h"
#include <stdlib.h>
#include <time.h>
#include "xil_types.h"
#include "xilfpga.h"
#include <xstatus.h>

Xuint32 *baseaddr_p = (Xuint32 *)XPAR_STATUS_V1_0_0_BASEADDR;
#define FULL_BITSTREAM_SIZE 19311210
//#define PR_BIT_STREAM_SIZE	409011		/* Size in bytes*/
#define PR_BIT_STREAM_SIZE	408880
#define XFGA_PARTIAL_EN 0x1

char *partial_bitstream = {"subber.bin"};
unsigned int partial_size;
unsigned char *partial_buffer;

XFpga XFpgaInstance = {0U};

int read_bitstreams(void){
    static FIL fil;
    static FATFS fatfs;
    static char *SD_File;
    FRESULT RES;
    UINT NumBytesRead;

    TCHAR *PATH = "0:/";

    RES = f_mount(&fatfs, PATH, 0);
    if(RES !=  FR_OK){
        return XST_FAILURE;
    }

    printf("sd card mount success\n\r");
    SD_File = (char *)partial_bitstream;
    printf("SD_file : %s\n\r", SD_File);
    RES = f_open(&fil, SD_File, FA_READ);
    if (RES){
        printf("error opening the file\n\r");
        return XST_FAILURE;
    }
    printf("Finish Opening\n\r");
    RES = f_lseek(&fil,0);
    printf("Finish Lseek\n\r");
    RES = f_read(&fil, partial_buffer,PR_BIT_STREAM_SIZE, &NumBytesRead);
    if (RES) {
        printf("error reading the file %s\n\r",SD_File);
        return XST_FAILURE;
    }
    printf("Success reading file %s total %d bytes\n\r",SD_File, NumBytesRead);

    partial_size = NumBytesRead;
    printf("done reading bitstream file\n\r");
    return 0;
}

void read_axilite(void){
	*(baseaddr_p+0)= 5 ;
	*(baseaddr_p+1)= 1 ;
	printf("Write first: %ld\n\r",*(baseaddr_p+0));
	printf("Write second: %ld\n\r",*(baseaddr_p+1));
	printf("Result: %ld\n\r",*(baseaddr_p+2));
}

void do_partial_reconfig(void){
	int Status;
	Status = XFpga_PL_BitStream_Load(&XFpgaInstance, partial_buffer,
			PR_BIT_STREAM_SIZE, XFPGA_PARTIAL_EN);
	if (Status == XFPGA_SUCCESS)
		xil_printf("Partial Reconfiguration Bitstream loaded into the PL successfully\n\r");
	else
		xil_printf("Partial Reconfiguration Bitstream loading into the PL failed\n\r");
}
int main(){
    init_platform();

    printf("\n\n\n\nHello World\n\r");
    printf("Successfully ran Hello World application\n\r");
    printf("Successfully ran Hello World application\n\r");
    int Status;
    Status = XFpga_Initialize(&XFpgaInstance);
    if (Status != XST_SUCCESS) {
		printf("XFPGA_Init failed\n\r");
	}
    partial_buffer = (unsigned char *) malloc(PR_BIT_STREAM_SIZE);
    printf("Size allocated to buffer\n\r");
    Status = read_bitstreams();
    printf("Finish bitstream\n\r");
    read_axilite();
    do_partial_reconfig();
    sleep(3);
    read_axilite();
    cleanup_platform();
    return 0;
}
