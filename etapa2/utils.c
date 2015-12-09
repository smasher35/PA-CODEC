/**
 * @file utils.c
 * @brief Description - this files holds the functions related to the second project delivery, in program paCodec
 * @author Paulo Penicheiro nº 2130628 Ruben Miguel nº 2130664
 * @date Dezembro - 2015
 * @version 1 
 */
#define _GNU_SOURCE

#include "utils.h"

#define HEX(num) ((num) & 0x000000FF)

/**
 * This function display information about the program paCodec
 *
 * @return Void
 */
void about(void){
	printf("==================== paCodec Authors ====================\n");
	printf("* Paulo Penicheiro - 2130628                            *\n");
	printf("* Ruben Miguel - 2130664                                *\n");
	printf("* ---------------------------                           *\n");
	printf("* UC - Programacao Avancada                             *\n");
	printf("* 2015 - 2016                                           *\n");
	printf("=========================================================\n");
}

/**
 * This function allows to install a signal handler
 *
 * @return 0 on success
 */
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

	time_t rawtime;
  	struct tm * now_time;
  	if (signum == SIGINT)
  	{
	  	time ( &rawtime );
	  	now_time = gmtime ( &rawtime );
		fprintf(stderr, "\n[SIGINT] - Operation interrupted by user @%d-%02d-%02d %02dh%02d\n" ,now_time->tm_year+1900, now_time->tm_mon, now_time->tm_mday, now_time->tm_hour, now_time->tm_min);
		fcloseall();
		exit(0);
  	}
}


/**
 * This function allows encode the given .pgm file
 *
 * @param pgm_struct structure with the decoded file data
 * @param dict_struct structure with the dictionary file data
 * @param pgm_filename filename of decoded file
 * @param dict_filename filename of dictionary file
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
	
	for (block_index = 0; block_index < totBlocks; block_index++)
	{
		cod_struct.indexVector_ptr[block_index] = (int)encodeBlockimgX(block_index, pgm_struct, dic_struct, num_blocks_per_line);
		
	}
	cod_struct.num_blocks = totBlocks;
	build_cod(&cod_struct, pgm_struct, dic_struct, filename);
	if (cod_struct.pgm_type == 2)
	{
		write_z2_cod_to_file(cod_struct, filename);
	}
	else if (cod_struct.pgm_type == 5)
	{
		write_z5_cod_to_file(cod_struct, filename);
	}
	

	free(cod_struct.indexVector_ptr);

}

/**
 * This function allows encode the given .pgm file using threads
 * @param pgm_struct structure with the decoded file data
 * @param dict_struct structure with the dictionary file data
 * @param pgm_filename filename of decoded file
 * @param dict_filename filename of dictionary file
 * @param n_threads number of threads
 * @return void
 */
void parallelEncode(pgm_t pgm_struct, dic_t dic_struct, char *filename, int n_threads) {

	PARAM_T param;
	param.shared_task_ID = 0;

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

	
	/* Init Shared Data */
	memset(&param, 0, sizeof(param));
	param.shared_task_ID = 0;
	param.total_tasks = totBlocks;
	param.num_blocks_per_line = num_blocks_per_line;
	param.pgm_struct = pgm_struct;
	param.dic_struct = dic_struct;
	param.cod_struct.indexVector_ptr = (int*)MALLOC(totBlocks*sizeof(int));

	if ((errno = pthread_mutex_init(&param.mutex, NULL)) != 0)
		ERROR(C_ERRO_MUTEX_INIT, "pthread_mutex_init() failed!");

	/** Create all threads */
	int i;
	for (i = 0 ; i < n_threads ; i++)
	{
		/* Cria thread para executar o produtor */
		if ((errno = pthread_create(&working_threads[i], NULL, process_block, &param)) != 0)
			ERROR(C_ERRO_PTHREAD_CREATE, "pthread_create() failed for thread %d!", i);
	}


	param.cod_struct.num_blocks = totBlocks;
	//DEBUG("TOTBLOCKS %d", param.cod_struct.num_blocks);
	//DEBUG("FILENAME parallelEncode param %s", filename);
	


	/** Waint (join) for threads*/
	for (i = 0 ; i < n_threads ; i++)
	{
		
		if ((errno = pthread_join(working_threads[i], NULL)) != 0)
			ERROR(C_ERRO_PTHREAD_JOIN, "pthread_join() failed for thread %d!", i);
	}
	
	
	/* Destroi o mutex */
	if ((errno = pthread_mutex_destroy(&param.mutex)) != 0)
		ERROR(C_ERRO_MUTEX_DESTROY, "pthread_mutex_destroy() failed!");

	build_cod(&param.cod_struct, pgm_struct, dic_struct, filename);
	
	if (param.cod_struct.pgm_type == 2)
	{
		write_z2_cod_to_file(param.cod_struct, filename);
	}
	else if (param.cod_struct.pgm_type == 5)
	{
		write_z5_cod_to_file(param.cod_struct, filename);
	}
	
	free(param.cod_struct.indexVector_ptr);
	free (working_threads);

}

/**
 * This function calculates quadratic error between dictionary block and pgm block
 *
 * @param dict_struct structure with the dictionary file data
 * @param pgm_struct structure with the decoded file data
 * @param pX X coordenate of pixel
 * @param pY Y coordenat of pixel 
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

    	    	//DEBUG("uSleep here"); usleep(5000);

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
 * This function calculates the coordenates of the pixels to compare between Dictionary and pgm file
 * @param block_index the index of the current block
 * @param pgm_struct structure with the decoded file data
 * @param dic_struct structure with the dictionary file data
 * @param num_blocks_per_line gives the ammount of blocks per line of the pgm file
 * @return void
 */ 
 
int encodeBlockimgX(unsigned int block_index, pgm_t pgm_struct, dic_t dic_struct, int num_blocks_per_line) {

	int blockX = block_index % num_blocks_per_line;
	int blockY = block_index / num_blocks_per_line;

	//DEBUG("blockX: %d,,,,,blockY: %d", blockX,blockY);

	int pY = blockY * dic_struct.block_height;
	int pX = blockX * dic_struct.block_width;

	int match_block = quadError (dic_struct, pgm_struct, pX, pY);

		//DEBUG("match_block: %d", match_block);
	

	return match_block;
}



/**
 * This function calculates the coordenates of the pixels to compare between Dictionary and pgm file
 * @param block_index the index of the current block
 * @param pgm_struct structure with the decoded file data
 * @param dic_struct structure with the dictionary file data
 * @param num_blocks_per_line gives the ammount of blocks per line of the pgm file
 * @return void
 */ 
void *process_block(void *arg)
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
		param_ptr->cod_struct.indexVector_ptr[new_task] = match_block;
		
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


void build_cod(pgmCod_t *cod_struct, pgm_t pgm_struct, dic_t dic_struct, char *filename)
{

	strcpy(cod_struct->filename, basename(filename));
	strcat(cod_struct->filename, ".cod");
	cod_struct->pgm_type = pgm_struct.header.format;
	cod_struct->columns = pgm_struct.header.width;
	cod_struct->rows = pgm_struct.header.height;
	cod_struct->block_width = dic_struct.block_width;
	cod_struct->block_height = dic_struct.block_height;

	/*Calculate Max value*/
	int max_value_aux = 0;
	int i;
	for (i=0; i<cod_struct->num_blocks; i++)
	{
		
		if (cod_struct->indexVector_ptr[i] > max_value_aux)
		{
			cod_struct->max_value = cod_struct->indexVector_ptr[i];
			max_value_aux = cod_struct->max_value;
		}
	}

	
}


void write_z2_cod_to_file(pgmCod_t cod_struct, char *filename)
{	
	//DEBUG("FILENAME: %s", cod_struct.filename);

	
	char dname[MAX_FNAME];
	strcpy(dname, dirname(filename));
	char complete_filename[MAX_FNAME];
	strcpy(complete_filename, dname);
	strcat(complete_filename, "/");
	strcat(complete_filename, cod_struct.filename);

	//DEBUG("COMPLETE FILENAME: %s", complete_filename);


	FILE *file;
	file = fopen(complete_filename, "w");

	fprintf(file, "Z%d\n", cod_struct.pgm_type);
	fprintf(file, "%d %d\n", cod_struct.columns, cod_struct.rows);
	fprintf(file, "%d\n", cod_struct.max_value);
	fprintf(file, "%d %d\n", cod_struct.block_width, cod_struct.block_height);

	int i;
	for (i=0; i<cod_struct.num_blocks; i++)
	{
		
		fprintf(file, "%d\n", cod_struct.indexVector_ptr[i]);
	}

	fclose(file);
}

int validate_dic_pgm(pgm_t pgm_struct, dic_t dic_struct)
{
	/*DEBUG("PGM WIDTH: %d", pgm_struct.header.width);
	DEBUG("PGM WIDTH: %d", pgm_struct.header.height);
	DEBUG("DIC BLOCK_WIDTH: %d", dic_struct.block_width);
	DEBUG("DIC BLOCK_HEIGHT: %d", dic_struct.block_height);*/
	if (pgm_struct.header.width % dic_struct.block_width != 0 || pgm_struct.header.height % dic_struct.block_height != 0)
	{

		return -1;
	}
	else
	{
		return 0;
	}


	//test case: t01-p2-2x2-02.pgm; PGM width is not a multiple of dictionary block width; FAILURE

}

void write_z5_cod_to_file(pgmCod_t cod_struct, char *filename)
{
	//DEBUG("FILENAME: %s", cod_struct.filename);
	
	char dname[MAX_FNAME];
	strcpy(dname, dirname(filename));
	char complete_filename[MAX_FNAME];
	strcpy(complete_filename, dname);
	strcat(complete_filename, "/");
	strcat(complete_filename, cod_struct.filename);

	//DEBUG("COMPLETE FILENAME: %s", complete_filename);


	FILE *file;
	file = fopen(complete_filename, "w");

	fprintf(file, "Z%d\n", cod_struct.pgm_type);
	fprintf(file, "%d %d\n", cod_struct.columns, cod_struct.rows);
	fprintf(file, "%d\n", cod_struct.max_value);
	fprintf(file, "%d %d\n", cod_struct.block_width, cod_struct.block_height);

	int bytesToWrite = 1;
	int i;
	//int hex;
	if(cod_struct.max_value >=256 ) {

		bytesToWrite = 2;

	}
	for (i=0; i<cod_struct.num_blocks; i++)
	{
		unsigned short aux;
		aux = (unsigned short)cod_struct.indexVector_ptr[i];

		fwrite(&aux, bytesToWrite, 1, file);	
		//hex = HEX(cod_struct.indexVector_ptr[i]);
        //        fputc(hex, file);
	}
	//fwrite(cod_struct.indexVector_ptr,sizeof(cod_struct.indexVector_ptr),1,file);
	

	fclose(file);
}