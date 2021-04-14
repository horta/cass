#ifndef CASS_CASS_H
#define CASS_CASS_H

#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CASS_VERSION "1.0.7"

static int cass_errors = 0;

inline static int __cass_close(double actual, double desired, double rel_tol, double abs_tol);

inline static void __cass_print_context(const char* filename, int line)
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

#define __CASS_REL_TOL(x) _Generic((x), float : 5e-05, double : 1e-09)

#define cass_close(actual, desired) cass_close2((double)(actual), (double)(desired), __CASS_REL_TOL(actual), 0.0)

#define cass_close2(actual, desired, rel_tol, abs_tol)                                                                 \
    __cass_close2((double)(actual), (double)(desired), (double)(rel_tol), (double)(abs_tol), __FILE__, __LINE__)

static void __cass_close2(double actual, double desired, double rel_tol, double abs_tol, char const* file, int line)
{
    double _a = actual;
    double _d = desired;
    if (__cass_close(_a, _d, rel_tol, abs_tol)) {
        __cass_print_context(file, line);
        fprintf(stderr, " Items are not close:\n");
        fprintf(stderr, "  ACTUAL : %.10f\n", (double)_a);
        fprintf(stderr, "  DESIRED: %.10f\n\n", (double)_d);
        ++cass_errors;
    }
}

#define __MAKE_CLASS_EQUAL(S, T, F)                                                                                    \
    static void __cass_equal_##S(T a, T d, char const* file, int line)                                                 \
    {                                                                                                                  \
        if (!(a == d)) {                                                                                               \
            __cass_print_context(file, line);                                                                          \
            fprintf(stderr, " Items are not equal:\n");                                                                \
            fprintf(stderr, "  ACTUAL : %" F "\n", a);                                                                 \
            fprintf(stderr, "  DESIRED: %" F "\n\n", d);                                                               \
            ++cass_errors;                                                                                             \
        }                                                                                                              \
    }

__MAKE_CLASS_EQUAL(uint64, uint64_t, PRIu64)
__MAKE_CLASS_EQUAL(uint32, uint32_t, PRIu32)
__MAKE_CLASS_EQUAL(uint16, uint16_t, PRIu16)
__MAKE_CLASS_EQUAL(uint8, uint8_t, PRIu8)

__MAKE_CLASS_EQUAL(int64, int64_t, PRIi64)
__MAKE_CLASS_EQUAL(int32, int32_t, PRIi32)
__MAKE_CLASS_EQUAL(int16, int16_t, PRIi16)
__MAKE_CLASS_EQUAL(int8, int8_t, PRIi8)

#define cass_equal_uint64(actual, desired)                                                                             \
    __cass_equal_uint64((uint64_t)(actual), (uint64_t)(desired), __FILE__, __LINE__)

#define cass_equal_uint32(actual, desired)                                                                             \
    __cass_equal_uint32((uint32_t)(actual), (uint32_t)(desired), __FILE__, __LINE__)

#define cass_equal_uint16(actual, desired)                                                                             \
    __cass_equal_uint16((uint16_t)(actual), (uint16_t)(desired), __FILE__, __LINE__)

#define cass_equal_uint8(actual, desired) __cass_equal_uint8((uint8_t)(actual), (uint8_t)(desired), __FILE__, __LINE__)

#define cass_equal_int64(actual, desired) __cass_equal_int64((int64_t)(actual), (int64_t)(desired), __FILE__, __LINE__)

#define cass_equal_int32(actual, desired) __cass_equal_int32((int32_t)(actual), (int32_t)(desired), __FILE__, __LINE__)

#define cass_equal_int16(actual, desired) __cass_equal_int16((int16_t)(actual), (int16_t)(desired), __FILE__, __LINE__)

#define cass_equal_int8(actual, desired) __cass_equal_int8((int8_t)(actual), (int8_t)(desired), __FILE__, __LINE__)

#define __cass_equal(actual, desired, file, line)                                                                      \
    _Generic((actual), uint64_t                                                                                        \
             : __cass_equal_uint64, uint32_t                                                                           \
             : __cass_equal_uint32, uint16_t                                                                           \
             : __cass_equal_uint16, uint8_t                                                                            \
             : __cass_equal_uint8, int64_t                                                                             \
             : __cass_equal_int64, int32_t                                                                             \
             : __cass_equal_int32, int16_t                                                                             \
             : __cass_equal_int16, int8_t                                                                              \
             : __cass_equal_int8)((actual), (desired), file, line)

#define cass_equal(actual, desired) __cass_equal(actual, desired, __FILE__, __LINE__)

#define cass_null(a) __cass_null((void const*)(a), (a) != NULL, #a, __FILE__, __LINE__)

static void __cass_null(void const* addr, int cond, char const* expr, char const* file, int line)
{
    if (cond) {
        __cass_print_context(file, line);
        fprintf(stderr, " Address should be NULL:\n");
        fprintf(stderr, "  ADDRESS   : %p\n", addr);
        fprintf(stderr, "  EXPRESSION: %s\n\n", expr);
        ++cass_errors;
    }
}

#define cass_not_null(a) __cass_not_null((void const*)(a), (a) == NULL, #a, __FILE__, __LINE__)

static void __cass_not_null(void const* addr, int cond, char const* expr, char const* file, int line)
{
    if (cond) {
        __cass_print_context(file, line);
        fprintf(stderr, " Address should not be NULL:\n");
        fprintf(stderr, "  ADDRESS   : %p\n", addr);
        fprintf(stderr, "  EXPRESSION: %s\n\n", expr);
        ++cass_errors;
    }
}

#define cass_strncmp(a, d, len) __cass_strncmp((a), (d), (int)(len), strncmp((a), (d), len), __FILE__, __LINE__)

static void __cass_strncmp(char const* a, char const* d, int len, int cond, char const* file, int line)
{
    if (cond) {
        __cass_print_context(file, line);
        fprintf(stderr, " Strings are not equal:\n");
        fprintf(stderr, "  ACTUAL : %.*s\n", len, a);
        fprintf(stderr, "  DESIRED: %.*s\n\n", len, d);
        ++cass_errors;
    }
}

#define cass_cond(a) __cass_cond(#a, !(a), __FILE__, __LINE__)

static void __cass_cond(char const* expr, int cond, char const* file, int line)
{
    if (cond) {
        __cass_print_context(file, line);
        fprintf(stderr, " Condition evaluates to false:\n");
        fprintf(stderr, "  EXPRESSION: %s\n\n", expr);
        ++cass_errors;
    }
}

inline static int __cass_close(double actual, double desired, double rel_tol, double abs_tol)
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

    return !(((diff <= fabs(rel_tol * desired)) || (diff <= fabs(rel_tol * actual))) || (diff <= abs_tol));
}

#endif
