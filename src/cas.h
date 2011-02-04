#ifndef _CAS_H_
#define _CAS_H_

#include <openssl/sha.h>

#include "log.h"
#include "blocks.h"
#include "rollsum.h"

#define DC_ROLLING_WINDOW       48
#define DC_MIN_CHUNK_SIZE       512
#define DC_MAX_CHUNK_SIZE       (ROLLSUM_CHECK_AVG_CHUNK_SIZE*2)

typedef struct dc_stream {
  SHA_CTX sc;
  int len;
  int offset; // current position in entire file
  char b[DC_MAX_CHUNK_SIZE]; // Current buffer
  int b_c; // Offset in current buffer
  char w[DC_ROLLING_WINDOW]; // Rolling window for adler checksum
  int w_in;  // number of bytes in the window
  int w_c;   // cursor in the window
  trollsum rsum; // current window rollsum
} tdc_stream;

tdc_stream* dc_write_start(void);
void dc_write_add(tdc_stream *s, char c);
char *dc_write_end(tdc_stream *s);

#endif
