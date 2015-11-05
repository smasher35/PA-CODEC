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
#include <signal.h>
#include <libgen.h>

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

void calculatePSNR (char *originalFile, char *decodedFile){

	/** Under implementation */

	char status[10] = "";
	double MSE = 0;
	int maxIntensity=0;
	double MAXf =0;
	double PSNR_value = 0;
	pgm_t fileOriginal_struct;
	pgm_t fileDecoded_struct;
	int totLines = 0;
	int totCols = 0;



	char *basec, *basec2, *bname, *bname2;
	char *path = originalFile;
	char *path2 = decodedFile;


	/* time-consuming job */
	basec = strdup(path);
	bname = basename(basec);

	basec2 = strdup(path2);
	bname2 = basename(basec2);

	fileOriginal_struct = read_file(originalFile);
	fileDecoded_struct = read_file(decodedFile);


	//Acesssing formula data of the file given on his structure @ filahandler.h
	maxIntensity = fileOriginal_struct.max_gray_value;
	totLines = fileOriginal_struct.lines;
	totCols = fileOriginal_struct.columns;
	DEBUG("%d", maxIntensity);
	DEBUG("%d", totLines);
	DEBUG("%d", totCols);


	int i;
	int j;
	double sum = 0.0;
	double diff = 0.0;

	for (i = 0; i < totLines; i++)
	{
		for (j = 0; j < totCols; j++)
		{
			diff = 1.0*(fileOriginal_struct.matrix_ptr[i][j] - fileDecoded_struct.matrix_ptr[i][j]);
			diff = diff * diff;
			sum += diff;
		}
	}
	MSE = sum / (totLines * totCols);
	double MSE_sqrt = sqrt(MSE);
	double arg_log10 = (1.0*maxIntensity) / MSE_sqrt;
	PSNR_value = 20 * log10(arg_log10);

	if (MSE <= 0) {
		strcpy(status,"FAILURE");
		printf ("\n\nPSNR: %s:%s:%s\n", status, bname, bname2);
		printf("FAILURE: ");
		ERROR(31,"INVALID MSE - MEDIAN DEVIATION\n");
	}

	MAXf=( (double)maxIntensity);
	if (MAXf <= 0) {
		strcpy(status,"FAILURE");
		printf ("\n\nPSNR: %s %s %s", status, bname, bname2);
		printf("FAILURE: ");
		ERROR(32,"INVALID MAX INTENSITY\n");
	}
	else {
			strcpy(status,"OK");
	}

	/** PSNR FINAL VALUE */
	
	printf ("\n\nPSNR: %s:%s:%s: %.3f\n", status, bname, bname2, PSNR_value);
}

int install_signal_handler(void){
	struct sigaction act;
	int exit_code = 1;
	act.sa_handler = process_signal;
	sigemptyset(&act.sa_mask);

	act.sa_flags = 0;/*fidedigno*/
	act.sa_flags |= SA_RESTART; /*recupera chamadas bloqueantes*/

	if (sigaction(SIGINT, &act, NULL))
	{
		ERROR(exit_code, "Can't install signal handler (sigaction)");
	}
	DEBUG("install_signal_handler done!");

	return 0;
} 

void process_signal(int signum){
	fprintf(stderr, "[SIGINT=%d] Operation interrupt by: @user", signum);
	DEBUG("FIXME: free all resources");
	fcloseall();
	exit(0);
}


