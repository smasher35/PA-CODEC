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
	void calculatePSN (FILE *originalFile, FILE *decodedFile);
	long calc_sum_matrix(pgm_t pgm_struct);

 #endif