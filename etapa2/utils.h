/**
 * @file utils.h
 * @brief Description - this files holds the functions prototypes and structures of the file utils.c
 * @author Paulo Penicheiro nº 2130628 Ruben Miguel nº 2130664
 * @date Dezembro - 2015
 * @version 1 
 */
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
#include <libgen.h>
#include <time.h>

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
	int block_index;   /**< block index > **/
	int distortion;		/**< distortion value > **/
} block_distortion_t;

typedef struct 
{    
	pthread_mutex_t mutex;		/**< mutex to access shared_task_ID **/
	int shared_task_ID;			/**< keeps the current task id **shared** **/
	int total_tasks;			/**< number of tasks **/
	int num_blocks_per_line;	/**< Nuymber of Blocks Per Line > **/
	pgm_t pgm_struct;			/**< PGM file structure > **/
	dic_t dic_struct;			/**< Dictionary file structure > **/
	pgmCod_t cod_struct;		/**< Encoded File Structure > **/
} PARAM_T;


/**
 * This function display information about the program paCodec
 *
 * @return Void
 */
void about(void);

/**
 * This function allows to install a signal handler
 *
 * @return 0 on success
 */
int install_signal_handler(void);
/**
 * This function allows to process the signal sent to program
 *
 * @param signum signal number to be processed
 * @return void
 */ 
void process_signal(int signum);

/**
 * This function allows encode the given .pgm file
 *
 * @param pgm_struct structure with the decoded file data
 * @param dict_struct structure with the dictionary file data
 * @param pgm_filename filename of decoded file
 * @param dict_filename filename of dictionary file
 * @return void
 */
void encodePGM(pgm_t pgm_struct, dic_t dic_struct, char *filename);

/**
 * This function allows encode the given .pgm file using threads
 * @param pgm_struct structure with the decoded file data
 * @param dict_struct structure with the dictionary file data
 * @param pgm_filename filename of decoded file
 * @param dict_filename filename of dictionary file
 * @param n_threads number of threads
 * @return void
 */
void parallelEncode(pgm_t pgm_struct, dic_t dic_struct, char *filename, int n_threads);

/**
 * This function calculates quadratic error between dictionary block and pgm block
 *
 * @param dict_struct structure with the dictionary file data
 * @param pgm_struct structure with the decoded file data
 * @param pX X coordenate of pixel
 * @param pY Y coordenat of pixel 
 * @return int returns the block index with lowest distortion value
 */
int  quadError (dic_t dic_struct, pgm_t pgm_struct, int pX, int pY);

/**
 * This function calculates the coordenates of the pixels to compare between Dictionary and pgm file
 * @param block_index the index of the current block
 * @param pgm_struct structure with the decoded file data
 * @param dic_struct structure with the dictionary file data
 * @param num_blocks_per_line gives the ammount of blocks per line of the pgm file
 * @return int returns the best matched block index
 */ 
int encodeBlockimgX(unsigned int block_index, pgm_t pgm_struct, dic_t dic_struct, int num_blocks_per_line);

/**
 * This function processes the image block and it's called whenener a new thread is created
 * @param arg params of the function
 * 
 * @return void
 */ 
void *process_block(void *arg);

/**
 * This function to attribute the block to be processed
 *
 * @param param_ptr 
 * @return int returns the new task ID
 */ 
int getNewTask(PARAM_T *param_ptr);



/**
 * This function builds the encoded file
 *
 * @param cod_struct structure to the encoded file
 * @param pgm_struct structure with the decoded file data
 * @param dict_struct structure with the dictionary file data
 * @param pgm_filename filename of decoded file
 * @return void
 */ 
void build_cod(pgmCod_t *cod_struct, pgm_t pgm_struct, dic_t dic_struct, char *filename);

/**
 * This function writes z2 cod file
 *
 * @param cod_struct structure to the encoded file
 * @param filename filename of encoded file
 * @return void
 */
void write_z2_cod_to_file(pgmCod_t cod_struct, char *filename);

/**
 * This function validates if the pgm image block size is multiple of  dictionary block size 
 *
 * @param pgm_struct structure with the decoded file data
 * @param dict_struct structure with the dictionary file data
 * @return int returns  -1 if the ditictionary block size is invalid, returns 0 the otherwise
 */ 
int validate_dic_pgm(pgm_t pgm_struct, dic_t dic_struct);

/**
 * This function writes z5 cod file
 *
 * @param cod_struct structure to the encoded file
 * @param filename filename of encoded file
 * @return void
 */ 
void write_z5_cod_to_file(pgmCod_t cod_struct, char *filename);






















#endif