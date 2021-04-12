#ifndef CASS_CASS_H
#define CASS_CASS_H

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define CASS_VERSION "1.0.4"

static int cass_errors = 0;

inline static int cass_close_impl(double actual, double desired, double rel_tol, double abs_tol);

inline static void cass_print_context(const char *filename, int line)
{
    fprintf(stderr, "Assertion error at %s:%d\n", filename, line);
}

inline static int cass_status(void)
{
    if (cass_errors == 1)
        printf("Attention: %d assertion has failed.\n", cass_errors);
    if (cass_errors > 1)
        printf("Attention: %d assertions have failed.\n", cass_errors);
    return cass_errors;
}

#define CASS_REL_TOL(x) _Generic((x), float : 5e-05, double : 1e-09)

#define cass_close(actual, desired)                                                                \
    cass_close2((double)(actual), (double)(desired), CASS_REL_TOL(x), 0.0)

#define cass_close2(actual, desired, rel_tol, abs_tol)                                             \
    __cass_close2((double)(actual), (double)(desired), (double)(rel_tol), (double)(abs_tol),       \
                  __FILE__, __LINE__)

void __cass_close2(double actual, double desired, double rel_tol, double abs_tol, char const *file,
                   unsigned line)
{
    double _a = actual;
    double _d = desired;
    if (cass_close_impl(_a, _d, rel_tol, abs_tol)) {
        cass_print_context(file, line);
        fprintf(stderr, " Items are not close:\n");
        fprintf(stderr, "  ACTUAL : %.10f\n", (double)_a);
        fprintf(stderr, "  DESIRED: %.10f\n\n", (double)_d);
        ++cass_errors;
    }
}

#define MAKE_CASS_EQUAL(S, T, F)                                                                   \
    static void __cass_equal_##S(T a, T d, char const *file, unsigned line)                        \
    {                                                                                              \
        if (!(a == d)) {                                                                           \
            cass_print_context(__FILE__, __LINE__);                                                \
            fprintf(stderr, " Items are not equal:\n");                                            \
            fprintf(stderr, "  ACTUAL : %" F "\n", a);                                             \
            fprintf(stderr, "  DESIRED: %" F "\n\n", d);                                           \
            ++cass_errors;                                                                         \
        }                                                                                          \
    }

MAKE_CASS_EQUAL(uint64, uint64_t, PRIu64)
MAKE_CASS_EQUAL(uint32, uint32_t, PRIu32)
MAKE_CASS_EQUAL(uint16, uint16_t, PRIu16)
MAKE_CASS_EQUAL(uint8, uint8_t, PRIu8)

MAKE_CASS_EQUAL(int64, int64_t, PRIi64)
MAKE_CASS_EQUAL(int32, int32_t, PRIi32)
MAKE_CASS_EQUAL(int16, int16_t, PRIi16)
MAKE_CASS_EQUAL(int8, int8_t, PRIi8)

#define cass_equal_uint64(actual, desired)                                                         \
    __cass_equal_uint64((uint64_t)(actual), (uint64_t)(desired), __FILE__, __LINE__)

#define cass_equal_uint32(actual, desired)                                                         \
    __cass_equal_uint32((uint32_t)(actual), (uint32_t)(desired), __FILE__, __LINE__)

#define cass_equal_uint16(actual, desired)                                                         \
    __cass_equal_uint16((uint16_t)(actual), (uint16_t)(desired), __FILE__, __LINE__)

#define cass_equal_uint8(actual, desired)                                                          \
    __cass_equal_uint8((uint8_t)(actual), (uint8_t)(desired), __FILE__, __LINE__)

#define cass_equal_int64(actual, desired)                                                          \
    __cass_equal_int64((int64_t)(actual), (int64_t)(desired), __FILE__, __LINE__)

#define cass_equal_int32(actual, desired)                                                          \
    __cass_equal_int32((int32_t)(actual), (int32_t)(desired), __FILE__, __LINE__)

#define cass_equal_int16(actual, desired)                                                          \
    __cass_equal_int16((int16_t)(actual), (int16_t)(desired), __FILE__, __LINE__)

#define cass_equal_int8(actual, desired)                                                           \
    __cass_equal_int8((int8_t)(actual), (int8_t)(desired), __FILE__, __LINE__)

#define cass_equal(actual, desired)                                                                \
    _Generic((actual), uint64_t                                                                    \
             : __cass_equal_uint64((actual), (desired), __FILE__, __LINE__), uint32_t              \
             : __cass_equal_uint32((actual), (desired), __FILE__, __LINE__), uint16_t              \
             : __cass_equal_uint16((actual), (desired), __FILE__, __LINE__), uint8_t               \
             : __cass_equal_uint8((actual), (desired), __FILE__, __LINE__), int64_t                \
             : __cass_equal_int64((actual), (desired), __FILE__, __LINE__), int32_t                \
             : __cass_equal_int32((actual), (desired), __FILE__, __LINE__), int16_t                \
             : __cass_equal_int16((actual), (desired), __FILE__, __LINE__), int8_t                 \
             : __cass_equal_int8((actual), (desired), __FILE__, __LINE__))

#define cass_null(a)                                                                               \
    if ((a) != NULL) {                                                                             \
        cass_print_context(__FILE__, __LINE__);                                                    \
        fprintf(stderr, " Address should not be NULL:\n");                                         \
        fprintf(stderr, "  EXPRESSION: " #a "\n\n");                                               \
        ++cass_errors;                                                                             \
    }

#define cass_not_null(a)                                                                           \
    if ((a) == NULL) {                                                                             \
        cass_print_context(__FILE__, __LINE__);                                                    \
        fprintf(stderr, " Address should not be NULL:\n");                                         \
        fprintf(stderr, "  EXPRESSION: " #a "\n\n");                                               \
        ++cass_errors;                                                                             \
    }

#define cass_strncmp(a, d, len)                                                                    \
    if (strncmp(a, d, len) != 0) {                                                                 \
        cass_print_context(__FILE__, __LINE__);                                                    \
        fprintf(stderr, " Items are not equal:\n");                                                \
        fprintf(stderr, "  ACTUAL : %.*s\n", len, a);                                              \
        fprintf(stderr, "  DESIRED: %.*s\n\n", len, d);                                            \
        ++cass_errors;                                                                             \
    }

#define cass_cond(a)                                                                               \
    if (!(a)) {                                                                                    \
        cass_print_context(__FILE__, __LINE__);                                                    \
        fprintf(stderr, " Condition evaluates to false:\n");                                       \
        fprintf(stderr, "  EXPRESSION: " #a "\n\n");                                               \
        ++cass_errors;                                                                             \
    }

inline static int cass_close_impl(double actual, double desired, double rel_tol, double abs_tol)
{
    /* This implementation is basically a copy of the `math.isclose`
     * implementation of the Python library plus returning 0 in case
     * both values are NaN.
     */
    if (actual == desired) {
        /* short circuit exact equality -- needed to catch two infinities of
         * the same sign. And perhaps speeds things up a bit sometimes.
         */
        return 0;
    }

    if (isnan(actual) && isnan(desired)) {
        return 0;
    }

    /* This catches the case of two infinities of opposite sign, or
     * one infinity and one finite number. Two infinities of opposite
     * sign would otherwise have an infinite relative tolerance.
     * Two infinities of the same sign are caught by the equality check
     * above.
     */

    if (isinf(actual) || isinf(desired)) {
        return 1;
    }

    /* now do the regular computation
     * this is essentially the "weak" test from the Boost library
     */

    double diff = fabs(desired - actual);

    return !(((diff <= fabs(rel_tol * desired)) || (diff <= fabs(rel_tol * actual))) ||
             (diff <= abs_tol));
}

#endif
