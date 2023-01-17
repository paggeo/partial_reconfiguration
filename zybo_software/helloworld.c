#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xbasic_types.h"
#include "xparameters.h"
//#include "sdCard.h"
//#include "pcap.h"
#include "time.h"
#include <xil_types.h>
#include "ff.h"
#include "xil_cache.h"
#include <stdlib.h>
#include <time.h>
#include "xdevcfg.h"
#include "xil_types.h"
#include <xstatus.h>

Xuint32 *baseaddr_p = (Xuint32 *)XPAR_STATUS_V1_0_0_BASEADDR;

#define FULL_BITSTREAM_SIZE 2084864
#define PR_BIT_STREAM_SIZE	90112		/* Size in bytes*/
#define XFGA_PARTIAL_EN 0x1

char bitStreamBuffer[FULL_BITSTREAM_SIZE];

//XFpga XFpgaInstance = {0U};

unsigned int reset = 0;

char *partial_bitstream = {"subber.bin"};
unsigned int partial_size;
unsigned char *partial_buffer;



#define SLCR_PCAP_CLK_CTRL XPAR_PS7_SLCR_0_S_AXI_BASEADDR + 0x168 /**< SLCR
					* PCAP clock control register address
					*/
#define MAX_COUNT 1000000000
#define SLCR_PCAP_CLK_CTRL_EN_MASK 0x1

#define SLCR_LOCK	0xF8000004
#define SLCR_LOCK_VAL	0x767B
#define SLCR_UNLOCK_VAL	0xDF0D
#define SLCR_UNLOCK	0xF8000008 /**< SLCR Write Protection Unlock */
#define SLCR_LVL_SHFTR_EN 0xF8000900 /**< SLCR Level Shifters Enable */

int initPCAP(u32 DEVICE_ID,XDcfg *DcfgInstance){
	int Status;
	XDcfg_Config *ConfigPtr;
	ConfigPtr = XDcfg_LookupConfig(DEVICE_ID);
	Status = XDcfg_CfgInitialize(DcfgInstance, ConfigPtr,ConfigPtr->BaseAddr);
	//XDcfg_SetLockRegister(&DcfgInstance, 0x757BDF0D);
	if (Status != XST_SUCCESS) {
		xil_printf("PCAP init failed\n\r");
		return XST_FAILURE;
	}
	Status = Xil_In32(SLCR_PCAP_CLK_CTRL);
	if (!(Status & SLCR_PCAP_CLK_CTRL_EN_MASK)) {
		Xil_Out32(SLCR_UNLOCK, SLCR_UNLOCK_VAL);
		Xil_Out32(SLCR_PCAP_CLK_CTRL,(Status | SLCR_PCAP_CLK_CTRL_EN_MASK));
		Xil_Out32(SLCR_UNLOCK, SLCR_LOCK_VAL);
	}
	return XST_SUCCESS;
}

int partialReconfigure(XDcfg *DcfgInstance,char *bitStreamBuffer, u32 bitStreamSize){
	int Status;
	printf("Before init\n\r");

	XDcfg_IntrClear(DcfgInstance, XDCFG_IXR_D_P_DONE_MASK|XDCFG_IXR_DMA_DONE_MASK);
	printf("After init\n\r");
	printf("Start transfer\n\r");
	Status = XDcfg_Transfer(DcfgInstance, (u8 *)bitStreamBuffer,bitStreamSize/4,(u8 *)XDCFG_DMA_INVALID_ADDRESS,0, XDCFG_NON_SECURE_PCAP_WRITE);
	if (Status != XST_SUCCESS) {
		xil_printf("PR failed\n\r");
		return XST_FAILURE;
	}
	printf("Finish transfer\n\r");
	Status = XDcfg_IntrGetStatus(DcfgInstance);
	while ((Status & XDCFG_IXR_DMA_DONE_MASK) != XDCFG_IXR_DMA_DONE_MASK)
	{
		Status = XDcfg_IntrGetStatus(DcfgInstance);
	}
	Status = XDcfg_IntrGetStatus(DcfgInstance);
	while ((Status & XDCFG_IXR_D_P_DONE_MASK) != XDCFG_IXR_D_P_DONE_MASK)
	{
		Status = XDcfg_IntrGetStatus(DcfgInstance);
	}
	return XST_SUCCESS;
}

/*void do_partial_reconfig(void){
    int xStatus;
    printf("performing partial reconfig of %d bytes ... \n\r", partial_size[0]);

    xStatus = XFpga_PL_BitStream_Load(&XFpgaInstance,partial_buffer[0],partial_size[0], XFGA_PARTIAL_EN);
    if (xStatus = XFPGA_SUCCESS)
    printf("ok with partial reconfiguration\n\r");
    else 
    printf("partial_reconfiguration failed\n\r");
}*/

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
    RES = f_lseek(&fil,0);
    RES = f_read(&fil, partial_buffer,PR_BIT_STREAM_SIZE, &NumBytesRead);
    if (RES) {
        printf("error reading the file %s\n\r",SD_File);
        return XST_FAILURE;
    }
    printf("Success reading file %s total %d bytes\n\r",SD_File, NumBytesRead);

    partial_size = NumBytesRead;
    printf("dones reading bitstream file\n\r");
    return 0;
}



int main()
{
    init_platform();

    printf("\n\n\nHello World\n\r");
    printf("Successfully ran Hello World application\n");
    printf("Successfully ran Hello World application\n");

    int Status;
    XDcfg DcfgInstance;
	Status = initPCAP(XPAR_XDCFG_0_DEVICE_ID,&DcfgInstance);
	if (Status != XST_SUCCESS) {
		print("PCAP init failed!!\n\r");
		return XST_FAILURE;
	}
    /*Status = XFpga_Initialize(&XFpgaInstance);
    if (Status != XST_SUCCESS) {
        printf("XFPGA init failed!!\n\r");
        return XST_FAILURE;
    }*/
    partial_buffer = (unsigned char *) malloc(PR_BIT_STREAM_SIZE);
    printf("Size allocated to buffer\n\r");
    Status = read_bitstreams();
    if(Status == 1)  printf("Problem\n\r");
    *(baseaddr_p+0)= 5 ;
    *(baseaddr_p+1)= 1 ;
    printf("Write first: %ld\n\r",*(baseaddr_p+0));
    printf("Write second: %ld\n\r",*(baseaddr_p+1));
    printf("Result: %ld\n\r",*(baseaddr_p+2));
    sleep(3);

    partialReconfigure(&DcfgInstance,(char *)partial_buffer,(u32)partial_size);
    printf("Completed the partial reconfiguration\n\r");
//	XDcfg DcfgInstance;
//	Status = initPCAP(XPAR_XDCFG_0_DEVICE_ID,&DcfgInstance);
//	if (Status != XST_SUCCESS) {
//		printf("PCAP init failed!!\n\r");
//		return XST_FAILURE;
//	}
//	printf("PCAP complete\n");
//	Status = SD_Init();
//	if (Status != XST_SUCCESS) {
//	   printf("file system init failed!!\n\r");
//	   return XST_FAILURE;
//	}
//	printf("SD complete\n");
//	fullReconfigure("full.bin",&DcfgInstance,bitStreamBuffer,(u32) FULL_BITSTREAM_SIZE);
//	printf("full.bin complete\n");
////    *(baseaddr_p+0)= 5 ;
////    *(baseaddr_p+1)= 1 ;
////    printf("Write first: %d\n\r",*(baseaddr_p+0));
////    printf("Write second: %d\n\r",*(baseaddr_p+1));
////    printf("Result: %d\n\r",*(baseaddr_p+2));
//    printf("\nStart to change reconfigure\n");
////    sleep(1);
//    partialReconfigure("subber.bin",&DcfgInstance,bitStreamBuffer,(u32)PR_BIT_STREAM_SIZE);
//    printf("subber.bin complete\n");

    sleep(3);
        *(baseaddr_p+0)= 5 ;
        *(baseaddr_p+1)= 1 ;
        printf("Write first: %ld\n\r",*(baseaddr_p+0));
        printf("Write second: %ld\n\r",*(baseaddr_p+1));
        printf("Result: %ld\n\r",*(baseaddr_p+2));
    cleanup_platform();
    return 0;
}
