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
#include "filehandler.h"

 
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

void calculatePSNR (FILE *originalFile, FILE *decodedFile){

	/** Under implementation */

	char psnrOutput[] = "";
	char error[] = "";
	int result=0;
	double MSE = 0;
	int maxIntensity =0;
	long executionTime =0;
	long long func_start_time, func_end_time;
	double psnr = 0;
	pgm_t fileOriginal_struct;
	pgm_t fileDecoded_struct;
	int totLines = 0;
	int totCols = 0;
	int status;
	double maxF;

	fileOriginal_struct = read_file(originalFile);
	fileDecoded_struct = read_file(decodedFile);






	//Acesssing formula data of the file given on his structure @ filahandler.h
	maxIntensity = fileOriginal_struct.max_gray_value;
	totLines = fileOriginal_struct.lines;
	totCols = fileOriginal_struct.columns;
	DEBUG("%d", maxIntensity);
	DEBUG("%d", totLines);
	DEBUG("%d", totCols);


	

	 /** determine running time */
   // func_start_time = start_time.tv_sec * 1000 + start_time.tv_usec / 1000;
   // func_end_time = end_time.tv_sec * 1000 + end_time.tv_usec / 1000;
   // executionTime = func_end_time - func_start_time;


//MSE = (1/(m*n))*sum(sum((f-g).^2))
	long f = calc_sum_matrix(fileOriginal_struct);
	long g = calc_sum_matrix(fileDecoded_struct);
	DEBUG("SUM ORIGINAL %ld", f);
	DEBUG("SUM DECODED %ld", g);

    /** Stil lacking the second part of the formula MEAN DEVIATION */
	//MSE = (1 / ((double)totLines * (double)totCols) * sum(sum()));
	/** PSNR FINAL VALUE */
	//maxF=( (double)maxIntensity / (double)MSE);
	//psnr = 20*log10(maxF / sqrt(MSE));

	//DEBUG("PSNR temp: %e", psnr);


	/** Output the PSNR calculation */
	/*if(result==0){
		status = 0;
	}else {
		status = -1;
	}*/



	//psnrOutput =("PSNR:" + status[0] + ":" + *originalFile + ":" + *decodedFile + psnr + "\n"+"Excution Time: " + executionTime + "\n");
	/*if (result == 1){
		psnrOutput=("FAILURE: " + error + "\n");
	}*/

}

long calc_sum_matrix(pgm_t pgm_struct)
{
	int i;
	int j;
	long sum = 0;

	for (i = 0; i < pgm_struct.lines; i++)
	{
		for (j = 0; j < pgm_struct.columns; j++)
		{
			sum += pgm_struct.matrix_ptr[i][j];
		}
	}
	return sum;

}



