/**
 * @file utils.c
 * @brief Description - this files holds the functions prototypes of the utils.c file
 * @author Paulo Penicheiro nº 2130628 Ruben Miguel nº 2130664
 * @date Novembro - 2015
 * @version 1 
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include "filehandler.h"
/**
 * This function display information about the program paCodec
 *
 * @return Void
 */
void about(void);
/**
 * This function calculate Peak Signal-to-Noise Ratio (PSNR)
 *
 * @param originalFile Name of the original file to be processed
 * @param decodedFile Name of the decoded file to be processed
 * @return Void
 */
void calculatePSNR (char *originalFile, char *decodedFile);
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
 * This function allows decode the given .cod file
 *
 * @param cod_struct structure with the coded file data
 * @param dict_struct structure with the dictionary file data
 * @param cod_filename filename of codded file
 * @param dict_filename filename of dictionary file
 * @return void
 */
void decode_pgm(cod_t cod_struct, dict_t dict_struct, const char *cod_filename);
/**
 * This function allows decode all the files in the given directory (recursivelly)
 *
 * @param dirname name of directory to decode
 * @param dict_file filename of dictionary file
 * @return 0 on success
 */
 int decode_dir_recursive(const char *dirname, const char *dict);
/**
 * This function allows decide if the directory is "." (itself)
 *
 * @param dirname name of directory decide
 * @return 0 on success
 */
 int is_dot(const char *dirname);
/**
 * This function allows decide if the directory is ".." (parent)
 *
 * @param dirname name of directory decide
 * @return 0 on success
 */
 int is_dot_dot(const char *dirname);
 /**
 * This function allows validate the extension of the given file
 *
 * @param file_name name of file to validate
 * @param extension extension to validate
 * @return 1 on success (filename equal to extension)
 */
int validate_extension(char* file_name, char* extention);



 #endif