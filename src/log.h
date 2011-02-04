#ifndef DC_LOG_H
#define DC_LOG_H

#include <syslog.h>
#include <stdarg.h>

#define DC_LOG_IDENT "DPL_CAS"

#define DC_LOG_VERB LOG_DEBUG 
#define DC_LOG_NORM LOG_INFO
#define DC_LOG_ERROR LOG_ERR 

void dc_log_init(int lvl);
void dc_log_close(void);

/***/

void dc_log(int lvl, const char *msg, ...);

#endif
