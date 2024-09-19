#include "log.h"
#include "stdheader.h"

/*
 * Function prints name of function in which error ocurred and the correspoding
 * error message
 */
void logerror(const char *funcname, char *msg) {
  fprintf(stderr, "Function %s: %s\n", funcname, msg);
}
