#include "log.h"
#include "stdheader.h"

/*
 * Function prints name of function in which error ocurred and the correspoding error message
 */
void logerror(const char *funcname, char *msg) {
    printf("Error in function %s: %s\n", funcname, msg);
}
