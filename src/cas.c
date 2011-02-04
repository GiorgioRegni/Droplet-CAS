#include <droplet.h>
#include <stdlib.h>

#include "cas.h"

/* Create a new stream, for writing */
tdc_stream* dc_write_start(void) {
  tdc_stream *s = malloc(sizeof(tdc_stream));
  bzero(s, sizeof(*s));

  SHA1_Init(&s->sc);
  rollsumInit(&s->rsum);

  return s;
}

void dc_write_add(tdc_stream *s, char c) {
  int reached_boundary = 0;

  s->b[s->b_c++] = c;
  s->offset++;


  if (s->b_c == DC_MAX_CHUNK_SIZE) {
    reached_boundary = 1;
  } else {
    // Update rolling checksum
    rollsumRotate(&s->rsum, s->w[s->w_c], c); 
    s->w[s->w_c++] = c;
    s->w_in++;

    if (s->w_c == DC_ROLLING_WINDOW)
      s->w_c = 0;
	
    if (s->w_in >= DC_ROLLING_WINDOW &&
	rollsumMinima(&s->rsum)) {
      reached_boundary = 1;
    }
  }

  if (reached_boundary) {
    //char* sig = dc_add_block(window, got);
    //free(sig);

    SHA1_Update(&s->sc, (const void*) &s->b,
		s->b_c);

    dc_log(DC_LOG_ERROR, "BLOCK %d LEN %d", s->offset, s->b_c);
	  
    // reset window and buffer
    bzero(s->b, sizeof(s->b));
    s->b_c = 0;
    rollsumInit(&s->rsum);
    bzero(s->w, sizeof(s->w));
    s->w_in = 0;
    s->w_c = 0;
  }

}

/*
 * Finish writing file, returns its unique ID,
 * must be stored for later read or delete operations
 */
char *dc_write_end(tdc_stream *s) {
  unsigned char md[SHA_DIGEST_LENGTH] = {0};
  int i;

  SHA1_Final(md, &s->sc);

  char *ret = malloc(SHA_DIGEST_LENGTH*2);

  for(i = 0; i < SHA_DIGEST_LENGTH; i++) 
    sprintf(ret+i*2, "%02X", md[i]);

  return ret;
}
