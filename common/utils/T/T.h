#ifndef _T_T_T_
#define _T_T_T_

#if T_TRACER

#include <stdint.h>

#include "T_defs.h"

#ifdef T_SEND_TIME
#include <time.h>
#endif

/* T message IDs */
#include "T_IDs.h"

/* known type - this is where you add new types */

#define T_INT(x) int, (x)
#define T_FLOAT(x) float, (x)
#define T_BUFFER(x, len) buffer, ((T_buffer){addr:(x), length:(len)})
#define T_STRING(x) string, (x)
#define T_PRINTF(...) printf, (__VA_ARGS__)

/* for each known type a T_PUT_XX macro is defined */

#define T_PUT_int(argnum, val) \
  do { \
    int T_PUT_var = (val); \
    T_CHECK_SIZE(sizeof(int), argnum); \
    memcpy(T_LOCAL_buf + T_LOCAL_size, &T_PUT_var, sizeof(int)); \
    T_LOCAL_size += sizeof(int); \
  } while (0)

#define T_PUT_ulong(argnum, val) \
  do { \
    unsigned long T_PUT_var = (val); \
    T_CHECK_SIZE(sizeof(unsigned long), argnum); \
    memcpy(T_LOCAL_buf + T_LOCAL_size, &T_PUT_var, sizeof(unsigned long)); \
    T_LOCAL_size += sizeof(unsigned long); \
  } while (0)

#define T_PUT_float(argnum, val) \
  do { \
    float T_PUT_var = (val); \
    T_CHECK_SIZE(sizeof(float), argnum); \
    memcpy(T_LOCAL_buf + T_LOCAL_size, &T_PUT_var, sizeof(float)); \
    T_LOCAL_size += sizeof(float); \
  } while (0)

#define T_PUT_buffer(argnum, val) \
  do { \
    T_buffer T_PUT_buffer_var = (val); \
    T_PUT_int(argnum, T_PUT_buffer_var.length); \
    T_CHECK_SIZE(T_PUT_buffer_var.length, argnum); \
    memcpy(T_LOCAL_buf + T_LOCAL_size, T_PUT_buffer_var.addr, \
           T_PUT_buffer_var.length); \
    T_LOCAL_size += T_PUT_buffer_var.length; \
  } while (0)

#define T_PUT_string(argnum, val) \
  do { \
    char *T_PUT_var = (val); \
    int T_PUT_len = strlen(T_PUT_var) + 1; \
    T_CHECK_SIZE(T_PUT_len, argnum); \
    memcpy(T_LOCAL_buf + T_LOCAL_size, T_PUT_var, T_PUT_len); \
    T_LOCAL_size += T_PUT_len; \
  } while (0)

#define T_PUT_printf_deref(...) __VA_ARGS__

#define T_PUT_printf(argnum, x) \
  do { \
    int T_PUT_len = snprintf(T_LOCAL_buf + T_LOCAL_size, \
                             T_BUFFER_MAX - T_LOCAL_size, T_PUT_printf_deref x); \
    if (T_PUT_len < 0) { \
      printf("%s:%d:%s: you can't read this, or can you?", \
               __FILE__, __LINE__, __FUNCTION__); \
      abort(); \
    } \
    if (T_PUT_len >= T_BUFFER_MAX - T_LOCAL_size) { \
      printf("%s:%d:%s: cannot put argument %d in T macro, not enough space" \
               ", consider increasing T_BUFFER_MAX (%d)\n", \
               __FILE__, __LINE__, __FUNCTION__, argnum, T_BUFFER_MAX); \
      abort(); \
    } \
    T_LOCAL_size += T_PUT_len + 1; \
  } while (0)

/* structure type to detect that you pass a known type as first arg of T */
struct T_header;

/* to define message ID */
#define T_ID(x) ((struct T_header *)(uintptr_t)(x))

/* T macro tricks */

#define TN(...) TN_N(__VA_ARGS__,33,32,31,30,29,28,27,26,25,24,23,22,21,\
        20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)(__VA_ARGS__)
#define TN_N(n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,\
        n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31,n32,n,...) T##n
#define T(...) TN(__VA_ARGS__)

/* type used to send arbitrary buffer data */
typedef struct {
  void *addr;
  int length;
} T_buffer;

extern volatile int *T_freelist_head;
extern T_cache_t *T_cache;

/* used at header of Tn, allocates buffer */
#define T_LOCAL_DATA \
  char *T_LOCAL_buf; \
  int T_LOCAL_size = 0; \
  int T_LOCAL_slot; \
  T_LOCAL_slot = __sync_fetch_and_add(T_freelist_head, 1) \
                 & (T_CACHE_SIZE - 1); \
  (void)__sync_fetch_and_and(T_freelist_head, T_CACHE_SIZE - 1); \
  if (T_cache[T_LOCAL_slot].busy) { \
    printf("%s:%d:%s: T cache is full - consider increasing its size\n", \
           __FILE__, __LINE__, __FUNCTION__); \
    abort(); \
  } \
  T_LOCAL_buf = T_cache[T_LOCAL_slot].buffer;

#define T_ACTIVE(x) T_active[(intptr_t)x]

#define T_COMMIT() \
  T_cache[T_LOCAL_slot].length = T_LOCAL_size; \
  __sync_synchronize(); \
  T_cache[T_LOCAL_slot].busy = 1; \

#define T_CHECK_SIZE(len, argnum) \
  if (T_LOCAL_size + (len) > T_BUFFER_MAX) { \
    printf("%s:%d:%s: cannot put argument %d in T macro, not enough space" \
             ", consider increasing T_BUFFER_MAX (%d)\n", \
             __FILE__, __LINE__, __FUNCTION__, argnum, T_BUFFER_MAX); \
    abort(); \
  }

#if 0
#define T_PUT(type, var, argnum) \
  do { \
    if (T_LOCAL_size + sizeof(var) > T_BUFFER_MAX) { \
      printf("%s:%d:%s: cannot put argument %d in T macro, not enough space" \
               ", consider increasing T_BUFFER_MAX (%d)\n", \
               __FILE__, __LINE__, __FUNCTION__, argnum, T_BUFFER_MAX); \
      abort(); \
    } \
    memcpy(T_LOCAL_buf + T_LOCAL_size, &var, sizeof(var)); \
    T_LOCAL_size += sizeof(var); \
  } while (0)
#endif

#if 0
#define T_PROCESS(x, argnum) \
  do { \
    T_PUT(typeof(x), x, argnum); \
  } while (0)
#endif

#if 0
#define T_PROCESS(x, argnum) \
  do { \
    if (__builtin_types_compatible_p(typeof(x), int)) \
      { T_PUT(int, (intptr_t)(x), argnum); printf("int\n"); } \
    else if (__builtin_types_compatible_p(typeof(x), short)) \
      { T_PUT(short, (intptr_t)(x), argnum); printf("short\n"); } \
    else if (__builtin_types_compatible_p(typeof(x), float)) \
      { T_PUT(float, (x), argnum); printf("float\n"); } \
    else if (__builtin_types_compatible_p(typeof(x), char *)) \
      { T_PUT(char *, (char *)(intptr_t)(x), argnum); printf("char *\n"); } \
    else if (__builtin_types_compatible_p(typeof(x), float *)) \
      { T_PUT(float *, (float *)(intptr_t)(x), argnum); printf("float *\n"); } \
    else if (__builtin_types_compatible_p(typeof(x), void *)) \
      { T_PUT(void *, (void *)(intptr_t)(x), argnum); printf("void *\n"); } \
    else { \
      printf("%s:%d:%s: unsupported type for argument %d in T macro\n", \
               __FILE__, __LINE__, __FUNCTION__, argnum); \
      abort(); \
    } \
  } while (0)
#endif

#ifdef T_SEND_TIME

#define T_HEADER(x) \
  do { \
    if (!__builtin_types_compatible_p(typeof(x), struct T_header *)) { \
      printf("%s:%d:%s: " \
             "bad use of T, pass a message ID as first parameter\n", \
             __FILE__, __LINE__, __FUNCTION__); \
      abort(); \
    } \
    struct timespec T_HEADER_time; \
    if (clock_gettime(CLOCK_REALTIME, &T_HEADER_time)) abort(); \
    memcpy(T_LOCAL_buf, &T_HEADER_time, sizeof(struct timespec)); \
    T_LOCAL_size += sizeof(struct timespec); \
    T_PUT_int(1, (int)(uintptr_t)(x)); \
  } while (0)

#else /* #ifdef T_SEND_TIME */

#define T_HEADER(x) \
  do { \
    if (!__builtin_types_compatible_p(typeof(x), struct T_header *)) { \
      printf("%s:%d:%s: " \
             "bad use of T, pass a message ID as first parameter\n", \
             __FILE__, __LINE__, __FUNCTION__); \
      abort(); \
    } \
    T_PUT_int(1, (int)(uintptr_t)(x)); \
  } while (0)

#endif /* #ifdef T_SEND_TIME */

#define T1(t) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_COMMIT(); \
    } \
  } while (0)

#define T3(t,t0,x0) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_COMMIT(); \
    } \
  } while (0)

#define T5(t,t0,x0,t1,x1) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_COMMIT(); \
    } \
  } while (0)

#define T7(t,t0,x0,t1,x1,t2,x2) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_PUT_##t2(4, x2); \
      T_COMMIT(); \
    } \
  } while (0)

#define T9(t,t0,x0,t1,x1,t2,x2,t3,x3) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_PUT_##t2(4, x2); \
      T_PUT_##t3(5, x3); \
      T_COMMIT(); \
    } \
  } while (0)

#define T11(t,t0,x0,t1,x1,t2,x2,t3,x3,t4,x4) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_PUT_##t2(4, x2); \
      T_PUT_##t3(5, x3); \
      T_PUT_##t4(6, x4); \
      T_COMMIT(); \
    } \
  } while (0)

#define T13(t,t0,x0,t1,x1,t2,x2,t3,x3,t4,x4,t5,x5) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_PUT_##t2(4, x2); \
      T_PUT_##t3(5, x3); \
      T_PUT_##t4(6, x4); \
      T_PUT_##t5(7, x5); \
      T_COMMIT(); \
    } \
  } while (0)

#define T15(t,t0,x0,t1,x1,t2,x2,t3,x3,t4,x4,t5,x5,t6,x6) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_PUT_##t2(4, x2); \
      T_PUT_##t3(5, x3); \
      T_PUT_##t4(6, x4); \
      T_PUT_##t5(7, x5); \
      T_PUT_##t6(8, x6); \
      T_COMMIT(); \
    } \
  } while (0)

#define T17(t,t0,x0,t1,x1,t2,x2,t3,x3,t4,x4,t5,x5,t6,x6,t7,x7) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_PUT_##t2(4, x2); \
      T_PUT_##t3(5, x3); \
      T_PUT_##t4(6, x4); \
      T_PUT_##t5(7, x5); \
      T_PUT_##t6(8, x6); \
      T_PUT_##t7(9, x7); \
      T_COMMIT(); \
    } \
  } while (0)

#define T19(t,t0,x0,t1,x1,t2,x2,t3,x3,t4,x4,t5,x5,t6,x6,t7,x7,t8,x8) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_PUT_##t2(4, x2); \
      T_PUT_##t3(5, x3); \
      T_PUT_##t4(6, x4); \
      T_PUT_##t5(7, x5); \
      T_PUT_##t6(8, x6); \
      T_PUT_##t7(9, x7); \
      T_PUT_##t8(10, x8); \
      T_COMMIT(); \
    } \
  } while (0)

#define T21(t,t0,x0,t1,x1,t2,x2,t3,x3,t4,x4,t5,x5,t6,x6,t7,x7,t8,x8,t9,x9) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_PUT_##t2(4, x2); \
      T_PUT_##t3(5, x3); \
      T_PUT_##t4(6, x4); \
      T_PUT_##t5(7, x5); \
      T_PUT_##t6(8, x6); \
      T_PUT_##t7(9, x7); \
      T_PUT_##t8(10, x8); \
      T_PUT_##t9(11, x9); \
      T_COMMIT(); \
    } \
  } while (0)

#define T23(t,t0,x0,t1,x1,t2,x2,t3,x3,t4,x4,t5,x5,t6,x6,t7,x7,t8,x8,t9,x9,t10,x10) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_PUT_##t2(4, x2); \
      T_PUT_##t3(5, x3); \
      T_PUT_##t4(6, x4); \
      T_PUT_##t5(7, x5); \
      T_PUT_##t6(8, x6); \
      T_PUT_##t7(9, x7); \
      T_PUT_##t8(10, x8); \
      T_PUT_##t9(11, x9); \
      T_PUT_##t10(12, x10); \
      T_COMMIT(); \
    } \
  } while (0)

#define T25(t,t0,x0,t1,x1,t2,x2,t3,x3,t4,x4,t5,x5,t6,x6,t7,x7,t8,x8,t9,x9,t10,x10,t11,x11) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_PUT_##t2(4, x2); \
      T_PUT_##t3(5, x3); \
      T_PUT_##t4(6, x4); \
      T_PUT_##t5(7, x5); \
      T_PUT_##t6(8, x6); \
      T_PUT_##t7(9, x7); \
      T_PUT_##t8(10, x8); \
      T_PUT_##t9(11, x9); \
      T_PUT_##t10(12, x10); \
      T_PUT_##t11(13, x11); \
      T_COMMIT(); \
    } \
  } while (0)

#define T27(t,t0,x0,t1,x1,t2,x2,t3,x3,t4,x4,t5,x5,t6,x6,t7,x7,t8,x8,t9,x9,t10,x10,t11,x11,t12,x12) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_PUT_##t2(4, x2); \
      T_PUT_##t3(5, x3); \
      T_PUT_##t4(6, x4); \
      T_PUT_##t5(7, x5); \
      T_PUT_##t6(8, x6); \
      T_PUT_##t7(9, x7); \
      T_PUT_##t8(10, x8); \
      T_PUT_##t9(11, x9); \
      T_PUT_##t10(12, x10); \
      T_PUT_##t11(13, x11); \
      T_PUT_##t12(14, x12); \
      T_COMMIT(); \
    } \
  } while (0)

#define T29(t,t0,x0,t1,x1,t2,x2,t3,x3,t4,x4,t5,x5,t6,x6,t7,x7,t8,x8,t9,x9,t10,x10,t11,x11,t12,x12,t13,x13) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_PUT_##t2(4, x2); \
      T_PUT_##t3(5, x3); \
      T_PUT_##t4(6, x4); \
      T_PUT_##t5(7, x5); \
      T_PUT_##t6(8, x6); \
      T_PUT_##t7(9, x7); \
      T_PUT_##t8(10, x8); \
      T_PUT_##t9(11, x9); \
      T_PUT_##t10(12, x10); \
      T_PUT_##t11(13, x11); \
      T_PUT_##t12(14, x12); \
      T_PUT_##t13(15, x13); \
      T_COMMIT(); \
    } \
  } while (0)

#define T31(t,t0,x0,t1,x1,t2,x2,t3,x3,t4,x4,t5,x5,t6,x6,t7,x7,t8,x8,t9,x9,t10,x10,t11,x11,t12,x12,t13,x13,t14,x14) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_PUT_##t2(4, x2); \
      T_PUT_##t3(5, x3); \
      T_PUT_##t4(6, x4); \
      T_PUT_##t5(7, x5); \
      T_PUT_##t6(8, x6); \
      T_PUT_##t7(9, x7); \
      T_PUT_##t8(10, x8); \
      T_PUT_##t9(11, x9); \
      T_PUT_##t10(12, x10); \
      T_PUT_##t11(13, x11); \
      T_PUT_##t12(14, x12); \
      T_PUT_##t13(15, x13); \
      T_PUT_##t14(16, x14); \
      T_COMMIT(); \
    } \
  } while (0)

#define T33(t,t0,x0,t1,x1,t2,x2,t3,x3,t4,x4,t5,x5,t6,x6,t7,x7,t8,x8,t9,x9,t10,x10,t11,x11,t12,x12,t13,x13,t14,x14,t15,x15) \
  do { \
    if (T_ACTIVE(t)) { \
      T_LOCAL_DATA \
      T_HEADER(t); \
      T_PUT_##t0(2, x0); \
      T_PUT_##t1(3, x1); \
      T_PUT_##t2(4, x2); \
      T_PUT_##t3(5, x3); \
      T_PUT_##t4(6, x4); \
      T_PUT_##t5(7, x5); \
      T_PUT_##t6(8, x6); \
      T_PUT_##t7(9, x7); \
      T_PUT_##t8(10, x8); \
      T_PUT_##t9(11, x9); \
      T_PUT_##t10(12, x10); \
      T_PUT_##t11(13, x11); \
      T_PUT_##t12(14, x12); \
      T_PUT_##t13(15, x13); \
      T_PUT_##t14(16, x14); \
      T_PUT_##t15(17, x15); \
      T_COMMIT(); \
    } \
  } while (0)

#define T_CALL_ERROR \
  do { \
    printf("%s:%d:%s: error calling T, you have to use T_INT() or T_XX()\n", \
           __FILE__, __LINE__, __FUNCTION__); \
  } while (0)

#define T2(...) T_CALL_ERROR
#define T4(...) T_CALL_ERROR
#define T6(...) T_CALL_ERROR
#define T8(...) T_CALL_ERROR
#define T10(...) T_CALL_ERROR
#define T12(...) T_CALL_ERROR
#define T14(...) T_CALL_ERROR
#define T16(...) T_CALL_ERROR
#define T18(...) T_CALL_ERROR
#define T20(...) T_CALL_ERROR
#define T22(...) T_CALL_ERROR
#define T24(...) T_CALL_ERROR
#define T26(...) T_CALL_ERROR
#define T28(...) T_CALL_ERROR
#define T30(...) T_CALL_ERROR
#define T32(...) T_CALL_ERROR

/* special cases for VCD logs */

#define T_VCD_VARIABLE(var, val) \
  do { \
    if (T_ACTIVE(((var) + VCD_FIRST_VARIABLE))) { \
      if ((var) > VCD_NUM_VARIABLES) { \
        printf("%s:%d:%s: VCD data out of synch for the T, contact" \
               " the authors!\n", __FILE__, __LINE__, __FUNCTION__); \
        abort(); \
      } \
      T_LOCAL_DATA \
      T_HEADER(T_ID((var) + VCD_FIRST_VARIABLE)); \
      T_PUT_ulong(1, (val)); \
      T_COMMIT(); \
    } \
  } while (0)

#define T_VCD_FUNCTION(fun, val) \
  do { \
    if (T_ACTIVE(((fun) + VCD_FIRST_FUNCTION))) { \
      if ((fun) > VCD_NUM_FUNCTIONS) { \
        printf("%s:%d:%s: VCD data out of synch for the T, contact" \
               " the authors!\n", __FILE__, __LINE__, __FUNCTION__); \
        abort(); \
      } \
      T_LOCAL_DATA \
      T_HEADER(T_ID((fun) + VCD_FIRST_FUNCTION)); \
      T_PUT_int(1, (val)); \
      T_COMMIT(); \
    } \
  } while (0)

extern int *T_active;

void T_init(int remote_port, int wait_for_tracer, int dont_fork);

#else /* T_TRACER */

/* if T_TRACER is not defined or is 0, the T is deactivated */
#define T(...) /**/

#endif /* T_TRACER */

#endif /* _T_T_T_ */
