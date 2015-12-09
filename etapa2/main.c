/**
* @file main.c
* @brief Description - this files holds the calls to the functions related to the second project delivery, in program pacodec
* @date 2015-12-19
* @author vitor.carreira@ipleiria.pt
* @author eiYxxxxx@student.estg.ipleiria.pt
* @author Paulo Penicheiro nº 2130628 Ruben Miguel nº 2130664
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

    /** About the authors */
     if (args_info.about_given){
        about();
     }

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
            exit(1);
        }
        
        if (validate_dic_pgm(pgm, dic) == -1)
        {
            fprintf(stderr, "FAILURE:incompatible dimensions of file \'%s\' with dict \'%s\'\n", args.encode_arg, args.dict_arg);
            exit(1);
        }


        encodePGM(pgm, dic , args.encode_arg);
       

    } else if (args.parallel_encode_given) {

        int status = pgm_parse_file(args.parallel_encode_arg, &pgm);
        if (status != 0) {
            fprintf(stderr, "FAILURE:file '%s' is not a valid PGM file\n", args.parallel_encode_arg);
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
       
        parallelEncode(pgm, dic, args.parallel_encode_arg, numThreads);

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

    pgm_free(&pgm);
    dic_free(&dic);
    cmdline_parser_free(&args);

    finish = clock();
    timeofExecution = (double)(finish - start)/CLOCKS_PER_SEC;
    printf("Execution time: %.3fs\n", timeofExecution);
	return 0;
}


