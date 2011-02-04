#include <droplet.h>
#include <stdlib.h>

#include "cas_fcgi.h"
#include <openssl/sha.h>

/*
 * Add a content block to the storage, return it's SHA1 checksum (as BCH) on success,
 * NULL on error
 * Caller has to free the char* when done
 */
char* dc_add_block(const char *buf, int len) {
  SHA_CTX sc;
  unsigned char md[SHA_DIGEST_LENGTH] = {0};
  int i;
  
  SHA1_Init(&sc);
  SHA1_Update(&sc, (const void*) buf,
	      len);
  SHA1_Final(md, &sc);

  char *ret = malloc(SHA_DIGEST_LENGTH*2);

  for(i = 0; i < SHA_DIGEST_LENGTH; i++) 
    sprintf(ret+i*2, "%02X", md[i]);

  return ret;
}

