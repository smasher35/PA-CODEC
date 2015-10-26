/**
 * @file utils.c
 * @brief Description - this files holds common functions related to the first project delivery, in program paCodec
 * @author Paulo Penicheiro nº 2130628 Ruben Miguel nº 2130664
 * @date Novembro - 2015
 * @version 1 
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h> 
#include "utils.h"
#include "memory.h"
#include "debug.h"







 
/** This function proveides information about the programmers of this application */
void about(void){
	printf("==================== paCodec Authors ====================\n");
	printf("* Paulo Penicheiro - 2130628                            *\n");
	printf("* Ruben Miguel - 2130664                                *\n");
	printf("* ---------------------------                           *\n");
	printf("* UC - Programacao Avancada                             *\n");
	printf("* 2015 - 2016                                           *\n");
	printf("=========================================================\n");
}



/** Peak Signal-to-Noise Ratio (PSNR) */

void calculatePSN (File *originalFile, FILE *decodedFile, struct timeval start_time, struct timeval end_time){

	/** Under implementation */

	char[] psnrOutput = "";
	cgar[] error="";
	int result=0;
	long meanDeviation= 0;
	time executionTime;
	long long func_start_time, func_end_time;
	long psnr =0;


	 /* determine running time */
    func_start_time = start_time.tv_sec * 1000 + start_time.tv_usec / 1000;
    func_end_time = end_time.tv_sec * 1000 + end_time.tv_usec / 1000;
    executionTime = func_end_time - func_start_time;


	meanDeviation = (1/lines*cols);
	


	psnr = 20*log10(maxIntensity / meanDeviation);


	/** Output the PSNR calculation */
	if(result==0){
		char[] status = "OK";
	}else {
		char[] status = "FAILURE";
	}

	psnrOutput =("PSNR:" + status + ":" + *originalFile + ":" + *decodedFile + psnr + "\n"+"Excution Time: " + executionTime + "\n");
	if (result == 1){
		psnrOutput=("FAILURE: " + error + "\n");
	}

}



