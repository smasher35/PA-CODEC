/**
 * @file memory.h
 * @brief Macros para gest�o de mem�ria
 *
 * Macros que ser�o usadas nos programas que requerem o uso das fun��es malloc e free
 * @version 1
 */
#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdlib.h>

void *eipa_malloc(size_t size, const int line, const char *file);
void eipa_free(void **ptr, const int line, const char *file);


/**
 * Macro para alocar mem�ria.
 * 
 * @return retorna o bloco de mem�ria alocado
 */
#define MALLOC(size) eipa_malloc((size), __LINE__, __FILE__)

/**
 * Macro para libertar mem�ria. Coloca o ponteiro a NULL.
 * 
 * @return N�o retorna nada
 */
#define FREE(ptr) eipa_free((void**)(&(ptr)), __LINE__, __FILE__)



#endif				/* _MEMORY_H_ */
