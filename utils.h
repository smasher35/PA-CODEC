/**
 * @file utils.c
 * @brief Description - this files holds the functions prototypes of the utils.c file
 * @author Paulo Penicheiro nº 2130628 Ruben Miguel nº 2130664
 * @date Novembro - 2015
 * @version 1 
 */

#ifndef _UTILS_H_
#define _UTILS_H_

 	#include "filehandler.h"

	void about(void);
	void calculatePSNR (char *originalFile, char *decodedFile);
	long calc_sum_matrix(pgm_t pgm_struct);
	void process_signal(int signum);
	int install_signal_handler(void);
	void decode_pgm(cod_t cod_Struct, dict_t dict_Struct);

 #endif