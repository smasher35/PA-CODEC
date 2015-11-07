/**
* @file main.c
* @brief Description - main file of the paCodec application
* @november 2015
* @author Paulo Penicheiro nº 2130628 e Ruben Miguel nº 2130664
* @ version 1 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h> 

#include "debug.h"
#include "memory.h"
#include "utils.h"
#include "pagengetopt.h"
#include "filehandler.h"

int main(int argc, char *argv[])
{
	double time_spent;
	clock_t begin, end;
	begin = clock();
	
	install_signal_handler();

	struct gengetopt_args_info args_info;
	cod_t cod_struct;
	dict_t dict_struct;

	int parser_ret;
	parser_ret = cmdline_parser(argc,argv, &args_info);

	/** parse the user given parameters with gengetopt */
	if (parser_ret != 0) {
		fprintf(stderr, "[ERROR]  while calling cmdline_parser\n");
		exit(1);

	}


	/** MAIN CODE */

	/**Missing Arguments or incorrect arguments  ate least one argument must be provided*/
	if (argc < 2) {
	    printf("===================== HELP: Some Arguments Avaiable: =====================\n\n");
		printf("--encode --> use this to encode an image file, don't forget to supply the file to encode\n");
		printf("--decode --> use this to decode an image file, don't forget to supply the file to decode\n");
		printf("--decode-dir --> use this to decode images in a given directory, suply the directory path\n");
		printf("--PSNR --> use this to calculate .... between the original and decoded file, must supply both files as arguments\n");
		printf("--dict --> use this to suply the dictionary file while encoding or decoding images\n");
		printf("--about --> use this to know about the this app programmers\n");
		printf("--help --> to know all the arguments avaiable\n\n");
		printf("================================================================================\n");
		exit(1);
	}

	/** About the authors */
	 if (args_info.about_given){
	 	about();
	 }


	 if (args_info.decode_given){
	 	cod_struct = read_cod_file(args_info.decode_arg);
	 	dict_struct = read_dictionary (args_info.dict_arg);
	 	decode_pgm(cod_struct, dict_struct);
	 }

	 if(args_info.PSNR_given){


	 	    char *token;
	 		char delim[2] = ",";
	 		char *original_filename;
	 		char *decoded_filename;


	 		token = strtok (args_info.PSNR_arg,delim); //point to 1 filename (before delimiter)
	 		original_filename = token;
	 		token = strtok (NULL, delim); //point to 2 filename (after delimiter)
	 		decoded_filename = token;


	 		DEBUG("%s", original_filename);
	 		DEBUG("%s", decoded_filename);

	 		calculatePSNR (original_filename, decoded_filename);
	 }



	 /** -------------------  Project  Delivery II  ------------------------- */
	 /** --encode argument given*/
	 if(args_info.encode_given){

	 	printf("\n[TO BE DONE] option not implemented yet!!!\n");
	 }

	 /** --parallel-encode argument given*/
	 if(args_info.parallel_encode_given){
	 	/** --threads argument not given*/
	 	if(!args_info.threads_given) {
	 		fprintf(stderr, "[ERROR]  --threads <nthreads> parameter is mandatory with --parallel-enconde\n");
			exit(1);
	 	}else {
	 		if(args_info.threads_arg > 1) {
	 			printf("\n[TO BE DONE] option not implemented yet!!!\n");
	 		}else {
	 			fprintf(stderr, "[ERROR]  The threads number must be more than 1\n");
				exit(1);
	 		}
	 	}
	 }

	/** free the memory allocated by gengetop */
	cmdline_parser_free (&args_info);

	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf ("Excution Time: %.3f s\n", time_spent);

	return 0;
}

