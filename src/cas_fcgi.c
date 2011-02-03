#include <fcgi_stdio.h>
#include <droplet.h>
#include <stdlib.h>

int count = 0;
dpl_ctx_t *dpl_ctx;

int main(void) {
  dpl_ctx = dpl_ctx_new(NULL, NULL);

  while (FCGI_Accept() >= 0)   {
    printf("Content-type: text/html\r\n"
           "\r\n"
           "<title>FastCGI Hello! (C, fcgi_stdio library)</title>"
           "<h1>FastCGI Hello! (C, fcgi_stdio library)</h1>"
           "Request number %d running, query <i>%s</i> %s\n",
	   ++count, getenv("QUERY_STRING"), getenv("REQUEST_URI"));
  }
}
