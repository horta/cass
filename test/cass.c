#include "cass/cass.h"
#include <math.h>

int main()
{
    char a = 'f';
    cass_cond(3 == 3);
    cass_not_null(&a);
    cass_null(NULL);
    cass_strncmp("afdw", "afdwf", 3);
    cass_equal(2, 2);
    cass_cond(isinf(INFINITY));
    cass_cond(isnan(NAN));
    const char actual[] = "hello";
    cass_cond(strcmp(actual, "hello") == 0);
    cass_close(1.0, 1.0 + 1e-10);
    cass_close(0.0, 0.0);
    cass_close2(0.0, 0.0 + 1e-10, 1e-9, 1e-9);
    return cass_status();
}
