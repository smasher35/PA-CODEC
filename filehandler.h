#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__

typedef short pixel_t;

void read_file(char *filename);
void skip_comments(FILE *fp);
pixel_t **allocate_matrix(int cols, int lines);
void dealloc_matrix(pixel_t **matrix, int lines);

#endif