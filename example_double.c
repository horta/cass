#include "cass.h"

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
    cass_close(1.0, 1.0 + 1e-10);

    /* pass */
    cass_close(0.0, 0.0);
}

void test_fail(void)
{

    /* fail */
    cass_close(1.0, 1.0 + 1e-9);

    /* pass */
    cass_close2(1.0, 1.0 + 1e-9, 1e-10, 0.0);

    /* fail because of absolute difference */
    cass_close(0.0, 0.0 + 1e-10);

    /* pass */
    cass_close2(0.0, 0.0 + 1e-10, 1e-9, 1e-9);
}
