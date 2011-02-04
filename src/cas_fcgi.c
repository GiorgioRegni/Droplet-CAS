#include <fcgi_stdio.h>
#include <droplet.h>
#include <stdlib.h>

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
      int got = 0;
      int offset = 0;
      char buf[DC_POST_READ_BLOCK_SIZE];
      
      while ( (c = FCGI_getchar()) != EOF) {
	buf[got] = c;
	
	offset++;
	got++;
	if (got == DC_POST_READ_BLOCK_SIZE) {
	  char* sig = dc_add_block(buf, got);

	  dc_log(DC_LOG_ERROR, "BLOCK %s %d", sig, offset);
	  free(sig);

	  got = 0;
	}
      }

      // Need to add the last block
      if (got > 0) {
	char* sig = dc_add_block(buf, got);
	dc_log(DC_LOG_ERROR, "BLOCK %s %d", sig, offset);
      }
      
      FCGI_printf("Content-type: text/plain\r\n"
		  "\r\n"
		  "<title>POST Answer</title>\n"
		  );
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
