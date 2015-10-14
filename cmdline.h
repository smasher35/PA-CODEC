/** @file cmdline.h
 *  @brief The header file for the command line option parser
 *  generated by GNU Gengetopt version 2.22.6
 *  http://www.gnu.org/software/gengetopt.
 *  DO NOT modify this file, since it can be overwritten
 *  @author GNU Gengetopt by Lorenzo Bettini */

#ifndef CMDLINE_H
#define CMDLINE_H

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h> /* for FILE */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef CMDLINE_PARSER_PACKAGE
/** @brief the program name (used for printing errors) */
#define CMDLINE_PARSER_PACKAGE "paCodec"
#endif

#ifndef CMDLINE_PARSER_PACKAGE_NAME
/** @brief the complete program name (used for help and version) */
#define CMDLINE_PARSER_PACKAGE_NAME "paCodec"
#endif

#ifndef CMDLINE_PARSER_VERSION
/** @brief the program version */
#define CMDLINE_PARSER_VERSION "1.0"
#endif

/** @brief Where the command line options are stored */
struct gengetopt_args_info
{
  const char *help_help; /**< @brief Print help and exit help description.  */
  const char *version_help; /**< @brief Print version and exit help description.  */
  const char *about_help; /**< @brief About the Authores of this application help description.  */
  char * encode_arg;	/**< @brief encodes the file image.  */
  char * encode_orig;	/**< @brief encodes the file image original value given at command line.  */
  const char *encode_help; /**< @brief encodes the file image help description.  */
  char * decode_arg;	/**< @brief decodes the file image.  */
  char * decode_orig;	/**< @brief decodes the file image original value given at command line.  */
  const char *decode_help; /**< @brief decodes the file image help description.  */
  char * decode_dir_arg;	/**< @brief decodes all image files in the given directory.  */
  char * decode_dir_orig;	/**< @brief decodes all image files in the given directory original value given at command line.  */
  const char *decode_dir_help; /**< @brief decodes all image files in the given directory help description.  */
  char ** PSNR_arg;	/**< @brief calculates codec quality between original and decoded file.  */
  char ** PSNR_orig;	/**< @brief calculates codec quality between original and decoded file original value given at command line.  */
  unsigned int PSNR_min; /**< @brief calculates codec quality between original and decoded file's minimum occurreces */
  unsigned int PSNR_max; /**< @brief calculates codec quality between original and decoded file's maximum occurreces */
  const char *PSNR_help; /**< @brief calculates codec quality between original and decoded file help description.  */
  char * parallel_encode_arg;	/**< @brief Encoding using threads.  */
  char * parallel_encode_orig;	/**< @brief Encoding using threads original value given at command line.  */
  const char *parallel_encode_help; /**< @brief Encoding using threads help description.  */
  char * dict_arg;	/**< @brief supllies the dictonary for the encoding/decoding.  */
  char * dict_orig;	/**< @brief supllies the dictonary for the encoding/decoding original value given at command line.  */
  const char *dict_help; /**< @brief supllies the dictonary for the encoding/decoding help description.  */
  int threads_arg;	/**< @brief Number of threads used to encode the image.  */
  char * threads_orig;	/**< @brief Number of threads used to encode the image original value given at command line.  */
  const char *threads_help; /**< @brief Number of threads used to encode the image help description.  */
  
  unsigned int help_given ;	/**< @brief Whether help was given.  */
  unsigned int version_given ;	/**< @brief Whether version was given.  */
  unsigned int about_given ;	/**< @brief Whether about was given.  */
  unsigned int encode_given ;	/**< @brief Whether encode was given.  */
  unsigned int decode_given ;	/**< @brief Whether decode was given.  */
  unsigned int decode_dir_given ;	/**< @brief Whether decode-dir was given.  */
  unsigned int PSNR_given ;	/**< @brief Whether PSNR was given.  */
  int PSNR_group ; /**< @brief Whether PSNR's was updated.  */
  unsigned int parallel_encode_given ;	/**< @brief Whether parallel-encode was given.  */
  unsigned int dict_given ;	/**< @brief Whether dict was given.  */
  unsigned int threads_given ;	/**< @brief Whether threads was given.  */

  int group1_group_counter; /**< @brief Counter for group group1 */
} ;

/** @brief The additional parameters to pass to parser functions */
struct cmdline_parser_params
{
  int override; /**< @brief whether to override possibly already present options (default 0) */
  int initialize; /**< @brief whether to initialize the option structure gengetopt_args_info (default 1) */
  int check_required; /**< @brief whether to check that all required options were provided (default 1) */
  int check_ambiguity; /**< @brief whether to check for options already specified in the option structure gengetopt_args_info (default 0) */
  int print_errors; /**< @brief whether getopt_long should print an error message for a bad option (default 1) */
} ;

/** @brief the purpose string of the program */
extern const char *gengetopt_args_info_purpose;
/** @brief the usage string of the program */
extern const char *gengetopt_args_info_usage;
/** @brief the description string of the program */
extern const char *gengetopt_args_info_description;
/** @brief all the lines making the help output */
extern const char *gengetopt_args_info_help[];

/**
 * The command line parser
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser (int argc, char **argv,
  struct gengetopt_args_info *args_info);

/**
 * The command line parser (version with additional parameters - deprecated)
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @param override whether to override possibly already present options
 * @param initialize whether to initialize the option structure my_args_info
 * @param check_required whether to check that all required options were provided
 * @return 0 if everything went fine, NON 0 if an error took place
 * @deprecated use cmdline_parser_ext() instead
 */
int cmdline_parser2 (int argc, char **argv,
  struct gengetopt_args_info *args_info,
  int override, int initialize, int check_required);

/**
 * The command line parser (version with additional parameters)
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @param params additional parameters for the parser
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_ext (int argc, char **argv,
  struct gengetopt_args_info *args_info,
  struct cmdline_parser_params *params);

/**
 * Save the contents of the option struct into an already open FILE stream.
 * @param outfile the stream where to dump options
 * @param args_info the option struct to dump
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_dump(FILE *outfile,
  struct gengetopt_args_info *args_info);

/**
 * Save the contents of the option struct into a (text) file.
 * This file can be read by the config file parser (if generated by gengetopt)
 * @param filename the file where to save
 * @param args_info the option struct to save
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_file_save(const char *filename,
  struct gengetopt_args_info *args_info);

/**
 * Print the help
 */
void cmdline_parser_print_help(void);
/**
 * Print the version
 */
void cmdline_parser_print_version(void);

/**
 * Initializes all the fields a cmdline_parser_params structure 
 * to their default values
 * @param params the structure to initialize
 */
void cmdline_parser_params_init(struct cmdline_parser_params *params);

/**
 * Allocates dynamically a cmdline_parser_params structure and initializes
 * all its fields to their default values
 * @return the created and initialized cmdline_parser_params structure
 */
struct cmdline_parser_params *cmdline_parser_params_create(void);

/**
 * Initializes the passed gengetopt_args_info structure's fields
 * (also set default values for options that have a default)
 * @param args_info the structure to initialize
 */
void cmdline_parser_init (struct gengetopt_args_info *args_info);
/**
 * Deallocates the string fields of the gengetopt_args_info structure
 * (but does not deallocate the structure itself)
 * @param args_info the structure to deallocate
 */
void cmdline_parser_free (struct gengetopt_args_info *args_info);

/**
 * Checks that all the required options were specified
 * @param args_info the structure to check
 * @param prog_name the name of the program that will be used to print
 *   possible errors
 * @return
 */
int cmdline_parser_required (struct gengetopt_args_info *args_info,
  const char *prog_name);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CMDLINE_H */
