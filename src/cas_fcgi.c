#include <fcgi_stdio.h>
#include <droplet.h>
#include <stdlib.h>

#include "cas.h"
#include "cas_fcgi.h"

int count = 0;
dpl_ctx_t *dpl_ctx;

int main(void) {

  dc_log_init(DC_LOG_NORM);
  dc_log(DC_LOG_ERROR, "Started");

  dpl_ctx = dpl_ctx_new(NULL, NULL);

  while (FCGI_Accept() >= 0)   {
    char *method = getenv("REQUEST_METHOD");
    char *uri = getenv("REQUEST_URI");
    /*
     * Dispatch methods
     */
    if (strcasecmp(method, HTTP_METHOD_GET) == 0) {
	FCGI_printf("Content-type: text/html\r\n"
	       "\r\n"
	       "<title>FastCGI Hello! (C, fcgi_stdio library)</title>"
	       "<h1>FastCGI Hello! (C, fcgi_stdio library)</h1>"
	       "Request number %d running, query <i>%s</i> %s\n",
	       ++count, getenv("QUERY_STRING"), getenv("REQUEST_URI"));
      }
    else if (strcasecmp(method, HTTP_METHOD_POST) == 0) {

      int len = atoi(getenv("CONTENT_LENGTH"));

      dc_log(DC_LOG_ERROR, "POST %s %d", uri, len);

      // READ input by blocks 
      int c;
      int offset = 0;
      int old_offset = 0;
      char window[DC_ROLLING_WINDOW] = {0};
      int window_in = 0; // number of bytes in the window
      int window_c = 0; // cursor in the window

      trollsum rsum;
      rollsumInit(&rsum);

      tdc_stream* s = dc_write_start();

      while ( (c = FCGI_getchar()) != EOF) {
	dc_write_add(s, c);
	/*
	// Update rolling checksum
	rollsumRotate(&rsum, window[window_c], c); 
	window[window_c++] = c;
	window_in++;
	offset++;

	if (window_c == DC_ROLLING_WINDOW)
	  window_c = 0;
	
	//dc_log(DC_LOG_ERROR, "BLOCK %d ADLER %x", offset, (&rsum)->s1 & ROLLSUM_CHECK_MASK);
	
	if (window_in >= DC_ROLLING_WINDOW &&
	    rollsumMinima(&rsum)) {
	  //char* sig = dc_add_block(window, got);

	  dc_log(DC_LOG_ERROR, "BLOCK %d ADLER %x", offset - old_offset, rollsumDigest(&rsum));
	  
	  old_offset = offset;
	  //free(sig);

	  // Reset rolling checksum
	  rollsumInit(&rsum);
	  bzero(window, sizeof(window));
	  window_in = 0;
	}
	*/
      }
      
      // Need to add the last block
      /*
	if (got > 0) {
	char* sig = dc_add_block(buf, got);
	dc_log(DC_LOG_ERROR, "BLOCK %s %d", sig, offset);
      }
      */
      char *sig = dc_write_end(s);
      
      FCGI_printf("Content-type: text/plain\r\n"
		  "\r\n"
		  "<title>%s</title>\n",
		  sig
		  );
      free(sig);
    }
    else {
      	FCGI_printf("Content-type: text/html\r\n"
	       "\r\n"
	       "<title>Unsupported Method</title>"
	       "<h1>Unsupported Method</h1>"
	       "Request number %d running, method <i>%s</i> %s\n",
	       ++count, method, uri);
    }
  }

  dc_log_close();
  exit(0);
}
