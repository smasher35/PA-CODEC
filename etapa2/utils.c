#define _GNU_SOURCE




#include "utils.h"



int install_signal_handler(void){
	struct sigaction act;
	int exit_code = 1;
	act.sa_handler = process_signal;
	sigemptyset(&act.sa_mask);

	act.sa_flags = 0;/*fidedigno*/
	act.sa_flags |= SA_RESTART; /*recupera chamadas bloqueantes*/

	if (sigaction(SIGINT, &act, NULL))
	{
		ERROR(exit_code, "Can't install signal handler (sigaction)");
	}
	return 0;
} 

/**
 * This function allows to process the signal sent to program
 *
 * @param signum signal number to be processed
 * @return void
 */ 

void process_signal(int signum){
	fprintf(stderr, "[SIGINT=%d] Operation interrupt by: @user", signum);
	DEBUG("FIXME: free all resources");
	fcloseall();
	exit(0);
}


/**
 * This function allows to process the signal sent to program
 *
 * @param signum signal number to be processed
 * @return void
 */ 


void encodePGM(pgm_t pgm_struct, dic_t dic_struct, char *filename) {

	int num_blocks_per_line; /* gives the amount of blocks per liner of the image */
	unsigned int totBlocks; /* total blocks of the pgm image file */
	unsigned int block_index;
	int blockSize;
	unsigned int imageSize; /*pgm image size */

	pgmCod_t cod_struct;

    blockSize=dic_struct.block_width * dic_struct.block_height;
	imageSize = pgm_struct.header.width * pgm_struct.header.height;
	totBlocks = imageSize/blockSize;
	num_blocks_per_line = pgm_struct.header.width/dic_struct.block_width;

	cod_struct.indexVector_ptr = (int*)MALLOC(totBlocks*sizeof(int));

    

	for (block_index = 0; block_index < totBlocks-1; block_index++)
	{
		cod_struct.indexVector_ptr[block_index] = encodeBlockimgX(block_index, pgm_struct, dic_struct, num_blocks_per_line);
	}

	build_cod(cod_struct, pgm_struct, dic_struct, filename);	

}


/**
 * This function allows to process the signal sent to program
 *
 * @param signum signal number to be processed
 * @return void
 */ 
void parallelEncode(pgm_t pgm_struct, dic_t dic_struct, char *filename, int n_threads) {

	PARAM_T param;
	param.shared_task_ID = 0;
	pgmCod_t cod_Struct;



	pthread_t *working_threads;
	working_threads = (pthread_t*)MALLOC(n_threads*sizeof(pthread_t));

	/*Block related calculations*/
	int num_blocks_per_line; /* gives the amount of blocks per liner of the image */
	unsigned int totBlocks; /* total blocks of the pgm image file */
	
	int blockSize;
	unsigned int imageSize; /*pgm image size */	



    blockSize=dic_struct.block_width * dic_struct.block_height;
	imageSize = pgm_struct.header.width * pgm_struct.header.height;
	totBlocks = imageSize/blockSize;
	num_blocks_per_line = pgm_struct.header.width/dic_struct.block_width;

	cod_Struct.indexVector_ptr = (int*)MALLOC(totBlocks*sizeof(int));

	*param.cod_Struct.indexVector_ptr = *cod_Struct.indexVector_ptr;

	

	/* Init Shared Data */
	memset(&param, 0, sizeof(param));
	param.shared_task_ID = 0;
	param.total_tasks = totBlocks;
	param.num_blocks_per_line = num_blocks_per_line;
	param.pgm_struct = pgm_struct;
	param.dic_struct = dic_struct;

	if ((errno = pthread_mutex_init(&param.mutex, NULL)) != 0)
		ERROR(C_ERRO_MUTEX_INIT, "pthread_mutex_init() failed!");

	/** Create all threads */
	int i;
	for (i = 0 ; i < n_threads ; i++)
	{
		/* Cria thread para executar o produtor */
		if ((errno = pthread_create(&working_threads[i], NULL, processa_bloco, &param)) != 0)
			ERROR(C_ERRO_PTHREAD_CREATE, "pthread_create() failed for thread %d!", i);
	}

	/** Waint (join) for threads*/
	for (i = 0 ; i < n_threads ; i++)
	{
		
		if ((errno = pthread_join(working_threads[i], NULL)) != 0)
			ERROR(C_ERRO_PTHREAD_JOIN, "pthread_join() failed for thread %d!", i);
	}
	



	/* Destroi o mutex */
	if ((errno = pthread_mutex_destroy(&param.mutex)) != 0)
		ERROR(C_ERRO_MUTEX_DESTROY, "pthread_mutex_destroy() failed!");

}

/**
 * This function allows to process the signal sent to program
 *
 * @param signum signal number to be processed
 * @return void
 */ 
 
int quadError (dic_t dic_struct, pgm_t pgm_struct, int pX, int pY) {

 	unsigned int i,j;
    unsigned short pixel_value_at_dict;
    unsigned short pixel_value_at_img;
    

    block_distortion_t distort_struct;    
    distort_struct.distortion = INT_MAX;
    
   
    int dict_index;
    for (dict_index = 0; dict_index < dic_struct.size; dict_index++)
    {
    	
    	int result = 0;
    	int quadResult = 0;
    	int distortion = 0;
    	unsigned int match_pixels = 0;
    	 for (i = 0; i < dic_struct.block_height; ++i) {
    	    for (j = 0; j < dic_struct.block_width; ++j) {

	    	    pixel_value_at_dict = dic_get_pixel(&dic_struct, dict_index, j, i);

	    	    pixel_value_at_img = pgm_struct.pixels[pY+i][pX+j];
	    	    result = pixel_value_at_dict - pixel_value_at_img;
	    	    quadResult = result * result;
	    	    distortion += quadResult;

	    	    if(pixel_value_at_img == pixel_value_at_dict)
	    	    {
	    	    	match_pixels++;
	    	    }
    	    }
    	}
    	
    	if (match_pixels == dic_struct.block_height*dic_struct.block_width)
    	{
    		distort_struct.block_index = dict_index;
    		distort_struct.distortion = distortion;
    		break;
    	}

    	if (distortion < distort_struct.distortion)
    	{
    		distort_struct.block_index = dict_index;
    		distort_struct.distortion = distortion;
    	}

    
    }

    return distort_struct.block_index;
     

}


/**
 * This function allows to process the signal sent to program
 *
 * @param signum signal number to be processed
 * @return void
 */ 
 
int encodeBlockimgX(unsigned int block_index, pgm_t pgm_struct, dic_t dic_struct, int num_blocks_per_line) {

	int blockX = block_index % num_blocks_per_line;
	int blockY = block_index / num_blocks_per_line;

	//DEBUG("blockX: %d,,,,,blockY: %d", blockX,blockY);

	int pY = blockY * dic_struct.block_height;
	int pX = blockX * dic_struct.block_width;

	int match_block = quadError (dic_struct, pgm_struct, pX, pY);	

	return match_block;
}



void *processa_bloco(void *arg)
{
	PARAM_T *param_ptr = (PARAM_T *) arg; 
	//int i;
	int new_task;
	int match_block;

	int finito = 0;
	while (finito == 0)
	{
		new_task = getNewTask(param_ptr);
		if (new_task == -1)
		{
			finito = 1;
			break;
		}
		match_block = encodeBlockimgX(new_task, param_ptr->pgm_struct, param_ptr->dic_struct, param_ptr->num_blocks_per_line);
		param_ptr->cod_Struct.indexVector_ptr[new_task] = match_block;
		
	}
	return NULL;
}


/**
 * This function allows to process the signal sent to program
 *
 * @param signum signal number to be processed
 * @return void
 */ 
 
int getNewTask(PARAM_T *param_ptr) {
	int my_taskID;

	pthread_mutex_lock(&param_ptr->mutex);

		my_taskID = param_ptr->shared_task_ID;
		param_ptr->shared_task_ID++;

	pthread_mutex_unlock(&param_ptr->mutex);

	if(my_taskID >= param_ptr->total_tasks)
	{
		my_taskID = -1; /**< no more tasks **/
	}
	
	return my_taskID;
}


void build_cod(pgmCod_t cod_struct, pgm_t pgm_struct, dic_t dic_struct, char *filename)
{
	strcpy(cod_struct.filename, filename);
	//todo pgmType
	cod_struct.columns = pgm_struct.header.width;
	cod_struct.rows = pgm_struct.header.height;
	//todo max_value
	cod_struct.block_width = dic_struct.block_width;
	cod_struct.block_height = dic_struct.block_height;
	cod_struct.num_blocks = sizeof(cod_struct.indexVector_ptr)/sizeof(int*);

	DEBUG("FILENAME: %s", cod_struct.filename);
	DEBUG("COLUMNS: %d", cod_struct.columns);
	DEBUG("ROWS: %d", cod_struct.rows);
	DEBUG("BLOCK_WIDTH: %d", cod_struct.block_width);
	DEBUG("BLOCK_HEIGHT: %d", cod_struct.block_height);
	DEBUG("NUM_BLOCKS: %d", cod_struct.num_blocks);




	//char filename[MAX_FNAME];	/**< COD filename */
	//char pgm_type[MAX_FNAME];	/**< COD file type in string format */
	//int columns;				/**< original PGM width */
	//int rows;					/**< original PGM height */
	//int max_value;				/**< COD max value */
	//int block_width;			/**< used DIC file block width */
	//int block_height;			/**< used DIC file block height */
	//int *indexVector_ptr;		/**< array to hold COD file indexes */
	//int num_blocks;	
}



