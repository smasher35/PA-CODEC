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

#include "debug.h"
#include "memory.h"
#include "utils.h"
#include "pagengetopt.h"

int main(int argc, char *argv[])
{
	struct gengetopt_args_info args_info;

	int parser_ret;
	parser_ret = cmdline_parser(argc,argv, &args_info);

	/** parse the user given parameters with gengetopt */
	if (parser_ret != 0) {
		fprintf(stderr, "ERROR while calling cmdline_parser\n");
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

	 

	/** free the memory allocated by gengetop */
	cmdline_parser_free (&args_info);


	return 0;
}

