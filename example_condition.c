#include "cass.h"
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
    cass_condition(1 == 1);

    /* pass */
    cass_condition(isinf(INFINITY));

    /* pass */
    cass_condition(isnan(NAN));

    /* pass */
    const char actual[] = "hello";
    cass_condition(strcmp(actual, "hello") == 0);
}

void test_fail(void)
{
    /* fail */
    cass_condition(1 == 2);

    /* fail */
    cass_condition(0);

    /* fail */
    const char actual[] = "hello";
    cass_condition(strcmp(actual, "world") == 0);
}
