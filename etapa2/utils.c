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

	//Allocate memory for the elements in a block (16 elements)
    pgm_image_block = (int *) MALLOC(dic_struct.width * dic_struct.height sizeof (int));


	int lineBlocks; /* gives the amount of blocks per liner of the image */
	int totBlocks; /* total blocks of the pgm image file */
	int blockIndex; /* index number */
	unsigned int imageSize; /*pgm image size */
	short blockSize; /*dictionary block size */
	int bx, by;
	unsigned int px,py;
	/* auxiliar variables */
	int bi,di,i,j;

	
		blockSize=dic_struct.width*dic_struct.height;
		imageSize = pgm_struct.width*pgm_struct.height;
		totBlocks = imageSize/blockSize;
		lineBlocks = pgm_struct.width/dic_struct.block_width;

		for (bi = 0; bi < totBlocks; bi++)
		{
			bx=bi/lineBlocks;
			by=bi%lineBlocks;
			px = bx*dic_struct.width;
			py = by*dic_struct.height;
			for (di = 0; di < dic_struct.size; di++)
			{
				for (i = px	; i < dic_struct.block_width; i++)
				{
					for (j = 0; j < dic_struct.block_height; j++)
					{
						
					}
					
				}
			}


		}

	
}


/**
 * This function allows to process the signal sent to program
 *
 * @param signum signal number to be processed
 * @return void
 */ 
void parallelEncode(void) {


}


/**
 * This function allows to process the signal sent to program
 *
 * @param signum signal number to be processed
 * @return void
 */ 
 
void getnewTask(void) {



}


/**
 * This function allows to process the signal sent to program
 *
 * @param signum signal number to be processed
 * @return void
 */ 
 
void quadError (void) {




}


/**
 * This function allows to process the signal sent to program
 *
 * @param signum signal number to be processed
 * @return void
 */ 
 
void singleEncode(void) {




}













