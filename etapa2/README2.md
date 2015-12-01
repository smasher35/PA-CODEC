# How to use the library

## Reading PGM files

```
#include "pgm.h"

...
pgm_t pgm = pgm_init();
int status = pgm_parse_file("example.pgm", &pgm);
if (status != 0) {
    // PGM is not valid
}
...
pgm_free(&pgm);

```

## Reading DIC files

```
#include "dic.h"

...
dic_t dic = dic_init();
int status = dic_parse_file("example.dic", &dic);
if (status != 0) {
    // DIC is not valid
}
...
// Iterate pixels at block "index"
 for (i = 0; i < dic.block_height; ++i) {
     for (j = 0; j < dic.block_width; ++j) {
        pixel_value = dic_get_pixel(&dic, index, j, i);
    }
}
...
dic_free(&dic);

```
