/**
* @file main.c
* @brief Description
* @date 01-01-2006
* @author 2130628 - Paulo Penicheiro e
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
#include "exemplo.h"

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

	if (args_info.all_given)
	{
		printf("All Options detected\n" );
	}

	if (args_info.ficheiro_given)
	{
		printf("Ficheiro %s\n", args_info.ficheiro_arg);
	}


	

	/** free the memory allocated by gengetop */
	cmdline_parser_free (&args_info);


	return 0;
}

