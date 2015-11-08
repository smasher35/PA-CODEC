/**
 * @file utils.c
 * @brief Description - this files holds common functions related to the first project delivery, in program paCodec
 * @author Paulo Penicheiro nº 2130628 Ruben Miguel nº 2130664
 * @date Novembro - 2015
 * @version 1 
 */
 #define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <libgen.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

#include "utils.h"
#include "memory.h"
#include "debug.h"
#include "filehandler.h"

 
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
 * This function calculate Peak Signal-to-Noise Ratio (PSNR)
 *
 * @param originalFile Name of the original file to be processed
 * @param decodedFile Name of the decoded file to be processed
 * @return Void
 */
void calculatePSNR (char *originalFile, char *decodedFile){


	char status[10] = "";
	double MSE = 0;
	int maxIntensity=0;
	double MAXf =0;
	double PSNR_value = 0;
	pgm_t fileOriginal_struct;
	pgm_t fileDecoded_struct;
	int totLines = 0;
	int totCols = 0;



	char *basec, *basec2, *bname, *bname2;
	char *path = originalFile;
	char *path2 = decodedFile;


	/* time-consuming job */
	basec = strdup(path);
	bname = basename(basec);

	basec2 = strdup(path2);
	bname2 = basename(basec2);

	fileOriginal_struct = read_file(originalFile);
	fileDecoded_struct = read_file(decodedFile);


	//Acesssing formula data of the file given on his structure @ filahandler.h
	maxIntensity = fileOriginal_struct.max_gray_value;
	totLines = fileOriginal_struct.lines;
	totCols = fileOriginal_struct.columns;
	DEBUG("%d", maxIntensity);
	DEBUG("%d", totLines);
	DEBUG("%d", totCols);


	int i;
	int j;
	double sum = 0.0;
	double diff = 0.0;

	for (i = 0; i < totLines; i++)
	{
		for (j = 0; j < totCols; j++)
		{
			diff = 1.0*(fileOriginal_struct.matrix_ptr[i][j] - fileDecoded_struct.matrix_ptr[i][j]);
			diff = diff * diff;
			sum += diff;
		}
	}
	MSE = sum / (totLines * totCols);
	double MSE_sqrt = sqrt(MSE);
	double arg_log10 = (1.0*maxIntensity) / MSE_sqrt;
	PSNR_value = 20 * log10(arg_log10);

	if (MSE <= 0) {
		strcpy(status,"FAILURE");
		printf ("\n\nPSNR: %s:%s:%s\n", status, bname, bname2);
		printf("FAILURE: ");
		ERROR(31,"INVALID MSE - MEDIAN DEVIATION\n");
	}

	MAXf=( (double)maxIntensity);
	if (MAXf <= 0) {
		strcpy(status,"FAILURE");
		printf ("\n\nPSNR: %s %s %s", status, bname, bname2);
		printf("FAILURE: ");
		ERROR(32,"INVALID MAX INTENSITY\n");
	}
	else {
			strcpy(status,"OK");
	}

	/** PSNR FINAL VALUE */
	
	printf ("\n\nPSNR: %s:%s:%s: %.3f\n", status, bname, bname2, PSNR_value);
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
	fprintf(stderr, "[SIGINT=%d] Operation interrupt by: @user", signum);
	DEBUG("FIXME: free all resources");
	fcloseall();
	exit(0);
}
/**
 * This function allows decode the given .cod file
 *
 * @param cod_struct structure with the coded file data
 * @param dict_struct structure with the dictionary file data
 * @param cod_filename filename of codded file
 * @param dict_filename filename of dictionary file
 * @return void
 */
void decode_pgm(cod_t cod_struct, dict_t dict_struct, const char *cod_filename, const char *dict_filename)
{
	pgm_t decoded_struct;

	decoded_struct.pgm_type = cod_struct.cod_type;
	decoded_struct.lines = cod_struct.lines;
	decoded_struct.columns = cod_struct.columns;


	decoded_struct.matrix_ptr = allocate_matrix(decoded_struct.columns, decoded_struct.lines);

	int img_size = cod_struct.lines * cod_struct.columns;
	int block_size = cod_struct.block_width * cod_struct.block_height;
	int len_of_coded_file = img_size / block_size;
	
	int index, j, k;
	int block_start_height;
	int block_start_width;
	int blocks_line_size = cod_struct.columns/cod_struct.block_width;
	int dict_index = 0;
	int	index_of_dict_block_elem;

	decoded_struct.max_gray_value = 0;

	for (index = 0; index < len_of_coded_file; index++) //for each array element
	{
		block_start_height = ( index / blocks_line_size) * cod_struct.block_height;
		if (index % blocks_line_size == 0)
		{
			block_start_width = 0;			
		}
		else
		{
			block_start_width = block_start_width + cod_struct.block_width;
		}

		dict_index = cod_struct.blocks_array[index];

		for (j=0 ; j < cod_struct.block_height; j++)
		{	
			for (k=0; k < cod_struct.block_width; k++)
			{				
				index_of_dict_block_elem = j+k+j;
				decoded_struct.matrix_ptr[block_start_height + j][block_start_width + k] = dict_struct.blocks_ptr[dict_index][index_of_dict_block_elem];
				if (dict_struct.blocks_ptr[dict_index][index_of_dict_block_elem] > decoded_struct.max_gray_value)
				{
					decoded_struct.max_gray_value = dict_struct.blocks_ptr[dict_index][index_of_dict_block_elem];
				}
			}
		}
	}

	write_pgm_file(decoded_struct, cod_filename);
	
	dealloc_dict_blocks(dict_struct.blocks_ptr, dict_struct.num_blocks);
	dealloc_matrix(decoded_struct.matrix_ptr, decoded_struct.lines);

}
/**
 * This function allows decode all the files in the given directory (recursivelly)
 *
 * @param dirname name of directory to decode
 * @param dict_file filename of dictionary file
 * @return 0 on success
 */
int decode_dir_recursive(const char *dirname, const char *dict_file)
{
	DIR *dir_p;
	dir_p = opendir(dirname);
	struct dirent my_entry;
	struct dirent *result_ptr;
	int ret_readdir;
	int done = 0;

	cod_t cod_struct;
	dict_t dict_struct;

	DEBUG("DIRNAME: %s", dirname);


	if (dir_p == NULL){
		fprintf(stderr, "Can't open dir %s:%s\n", dirname, strerror(errno));
		return -1;
	}	
	do{
		ret_readdir = readdir_r(dir_p, &my_entry, &result_ptr);
		if (ret_readdir == -1){
			fprintf(stderr, "error on readdir_r %s%s\n", dirname, strerror(errno));
			return -1;
		}
		if (result_ptr == NULL){
			//DEBUG("at end of dir %s", dirname);
			done = 1;
		}
		else{
			if (is_dot(my_entry.d_name) || is_dot_dot(my_entry.d_name)){
				continue;
			}
			if (my_entry.d_type == DT_DIR)
			{
				char new_dirame[PATH_MAX];
				sprintf(new_dirame, "%s/%s", dirname, my_entry.d_name);
				decode_dir_recursive(new_dirame, dict_file);
			}

			else if(my_entry.d_type == DT_LNK){
				//printf("SYM_LINK: %s\n", my_entry.d_name);
				continue;
			}else { //if diferent of DIR , SYM LINK, DOT and DOR_DOT (file)

				if (validate_extension(my_entry.d_name, ".cod"))
				{
					
					//DEBUG("ENCONTRADO .COD: %s/%s", dirname, my_entry.d_name);
					char filename_aux[PATH_MAX];
					strcpy(filename_aux, dirname);
					//DEBUG("AFTER SRTG COPY: %s", filename_aux);
					strcat(filename_aux, "/");
					strcat(filename_aux, my_entry.d_name);
					//DEBUG("AFTER SRTG CAT: %s", filename_aux);



					if (dict_file != NULL)
					{
						//DEBUG("ENTROU NO IFFFFFF");
						cod_struct = read_cod_file(filename_aux);
						//DEBUG("PASSOU");
						dict_struct = read_dictionary (dict_file);
						decode_pgm(cod_struct, dict_struct, filename_aux, dict_file);
					}
				}

			}
			
		}
	}while(done == 0);
	

	closedir(dir_p);
	return 0;
}

/**
 * This function allows decide if the directory is "." (itself)
 *
 * @param dirname name of directory decide
 * @return 0 on success
 */
int is_dot(const char *dirname){
	return (strcmp(".", dirname) == 0);
}

/**
 * This function allows decide if the directory is ".." (parent)
 *
 * @param dirname name of directory decide
 * @return 0 on success
 */
int is_dot_dot(const char *dirname){
	return (strcmp("..", dirname) == 0);
}

/**
 * This function allows validate the extension of the given file
 *
 * @param file_name name of file to validate
 * @param extension extension to validate
 * @return 1 on success (filename equal to extension)
 */
int validate_extension(char* file_name, char* extention) {
	char *ptr;

	if(file_name == NULL || extention == NULL){
		return 0;
	}

	ptr = strrchr(file_name, '.');
	if(ptr == NULL){
		return 0;
	}

	return (strcmp(ptr, extention) == 0);
}
