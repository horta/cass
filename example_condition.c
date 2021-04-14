#include "cass/cass.h"
#include <math.h>
#include <string.h>

void test_pass(void);
void test_fail(void);

int main()
{
    test_pass();
    test_fail();
    return cass_status();
}

void test_pass(void)
{
    /* pass */
    cass_cond(1 == 1);

    /* pass */
    cass_cond(isinf(INFINITY));

    /* pass */
    cass_cond(isnan(NAN));

    /* pass */
    const char actual[] = "hello";
    cass_cond(strcmp(actual, "hello") == 0);
}

void test_fail(void)
{
    /* fail */
    cass_cond(1 == 2);

    /* fail */
    cass_cond(0);

    /* fail */
    const char actual[] = "hello";
    cass_cond(strcmp(actual, "world") == 0);
}
