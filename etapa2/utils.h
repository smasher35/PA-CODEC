#ifndef	__UTILS_H__
#define __UTILS_H__



#include "pgm.h"
#include "dic.h"


void encodePGM(pgm_t pgm_struct, dic_t dic_struct, char *filename);
void process_signal(int signum);
int install_signal_handler(void);



















#endif