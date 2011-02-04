#include "log.h"

static int cur_log_lvl = DC_LOG_NORM;

void dc_log_init(int lvl) {
  openlog(DC_LOG_IDENT,
	  LOG_PID,
	  LOG_DAEMON);
  cur_log_lvl = lvl;
}

void dc_log_close(void) {  
}

/***/

void dc_log(int lvl, const char *msg, ...) {
  if (lvl <= cur_log_lvl) {
    va_list ap;
    
    va_start(ap, msg);
    vsyslog(lvl, msg, ap);
    va_end(ap);
  }
}
