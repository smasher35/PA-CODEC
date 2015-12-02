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
    struct timeval startTime;
    struct timeval endTime;
    float timeofExecution;
    unsigned int i,j;
    unsigned short pixel_value;

    pgm_t pgm = pgm_init();
    dic_t dic = dic_init();
    gettimeofday(&startTime,0);

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
            exit(1);
        }
        
        // Iterate pixels at block "index"
         for (i = 0; i < dic.block_height; ++i) {
             for (j = 0; j < dic.block_width; ++j) {
                pixel_value = dic_get_pixel(&dic, index, j, i);
            }
        }

    } else if (args.parallel_encode_given) {
            short numCores = sysconf(_SC_NPROCESSORS_ONLN);
            printf ("NUMBER OF CORES: %hd",numCores);


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
    gettimeofday(&endTime,0);

    timeofExecution = (endTime.tv_sec+endTime.tv_usec) - (startTime.tv_sec+startTime.tv_usec);
    printf ("TIME OF EXCUTION: %.4f\n", timeofExecution );
    pgm_free(&pgm);
    dic_free(&dic);
    cmdline_parser_free(&args);
	return 0;
}


