/**
* @file main.c
* @brief Description
* @date 2015-11-25
* @author vitor.carreira@ipleiria.pt
* @author eiYxxxxx@student.estg.ipleiria.pt
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#include "debug.h"
#include "memory.h"
#include "config.h"
#include "pgm.h"
#include "dic.h"
#include "utils.h"

// TODO: include other libraries


int main(int argc, char *argv[]) {
	struct gengetopt_args_info args;
	cmdline_parser(argc, argv, &args);   
    double timeofExecution;
    clock_t start, finish;

    start = clock();
    

    pgm_t pgm = pgm_init();
    dic_t dic = dic_init();

    install_signal_handler();
    
    // TODO: Measure execution time (use gettimeofday or clock_gettime)

    if (args.decode_given) {
        printf("[DO NOT IMPLEMENT OR FIX]\n");
    } else if (args.PSNR_given) {
        printf("[DO NOT IMPLEMENT OR FIX]\n");
    } else if (args.encode_given) {
        
        int status = pgm_parse_file(args.encode_arg, &pgm);
        if (status != 0) {
            fprintf(stderr, "FAILURE:file '%s' is not a valid PGM file\n", args.encode_arg);
            exit(1);
        }

        
        status = dic_parse_file(args.dict_arg, &dic);
        if (status != 0) {
            fprintf(stderr, "FAILURE:file '%s' is not a valid DIC file\n", args.dict_arg);
            return(1);
        }
        
         encodePGM(pgm, dic , args.encode_arg);
       

    } else if (args.parallel_encode_given) {

        int status = pgm_parse_file(args.parallel_encode_arg, &pgm);
        if (status != 0) {
            fprintf(stderr, "FAILURE:file '%s' is not a valid PGM file\n", args.encode_arg);
            return 1;
        }

        
        status = dic_parse_file(args.dict_arg, &dic);
        if (status != 0) {
            fprintf(stderr, "FAILURE:file '%s' is not a valid DIC file\n", args.dict_arg);
            return 1;
        }
        short numThreads;
        if (args.threads_given && args.threads_arg > 0)
        {
             numThreads = args.threads_arg;                        
        }
        else
        {
            numThreads = sysconf(_SC_NPROCESSORS_ONLN);            
        }
        DEBUG("NUMBER OF THREADS TO BY USED: %hd \n", numThreads);

        parallelEncode(pgm, dic, args.encode_arg, numThreads);



        // TODO: PARALLEL ENCODE IMAGE args.parallel_encode_arg WITH DICTIONARY args.dict_arg
        //       USE args.threads_arg threads if number of threads is positive; else use host number of CPUs
    } else if (args.decode_dir_given) {
        printf("[DO NOT IMPLEMENT OR FIX]\n");
    } else if (args.about_given) {
        printf("[DO NOT IMPLEMENT OR FIX]\n");
    } else if (args.dict_given) {
        printf("[DO NOT IMPLEMENT OR FIX]\n");
    } else {
        printf("Specify at least one operation\n");
        cmdline_parser_free(&args);
        exit(1);
    }

    // TODO: Check if operation was interrupted and print the "interrupted by user" message
    pgm_free(&pgm);
    dic_free(&dic);
    cmdline_parser_free(&args);

    finish = clock();
    timeofExecution = (double)(finish - start)/CLOCKS_PER_SEC;
    printf("Execution time: %.3fs\n", timeofExecution);
	return 0;
}


