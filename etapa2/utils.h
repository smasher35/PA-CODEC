#ifndef	__UTILS_H__
#define __UTILS_H__

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "pgm.h"
#include "dic.h"
#include "memory.h"
#include "debug.h"
#include "pgmCod.h"


#define C_ERRO_PTHREAD_CREATE           1
#define C_ERRO_PTHREAD_JOIN             2
#define C_ERRO_MUTEX_INIT               3
#define C_ERRO_MUTEX_DESTROY            4
#define C_ERRO_CONDITION_INIT           5
#define C_ERRO_CONDITION_DESTROY        6

typedef struct {
	int block_index;
	int distortion;
} block_distortion_t;

typedef struct 
{    
	pthread_mutex_t mutex;		/**< mutex to access shared_task_ID **/
	int shared_task_ID;			/**< keeps the current task id **shared** **/
	int total_tasks;			/**< number of tasks **/
	int num_blocks_per_line;
	pgm_t pgm_struct;
	dic_t dic_struct;
	pgmCod_t cod_struct;
} PARAM_T;


void encodePGM(pgm_t pgm_struct, dic_t dic_struct, char *filename);
void process_signal(int signum);

int install_signal_handler(void);
void parallelEncode(pgm_t pgm_struct, dic_t dic_struct, char *filename, int n_threads);
int getnewTask(PARAM_T *param_ptr);
void *processa_bloco(void *arg);
int getNewTask(PARAM_T *param_ptr);
int  quadError (dic_t dic_struct, pgm_t pgm_struct, int pX, int pY);
int encodeBlockimgX(unsigned int block_index, pgm_t pgm_struct, dic_t dic_struct, int num_blocks_per_line);
void build_cod(pgmCod_t *cod_struct, pgm_t pgm_struct, dic_t dic_struct, char *filename);
void write_pgm_to_file(pgmCod_t cod_struct);
/* -------------------------------------------- unfinished functions --------------------------------------- */



















#endif