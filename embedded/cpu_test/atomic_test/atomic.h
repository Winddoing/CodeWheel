#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#include <stdint.h>

#define ANDROID_ATOMIC_INLINE inline __attribute__((always_inline))

extern ANDROID_ATOMIC_INLINE void android_compiler_barrier(void)
{
    __asm__ __volatile__ ("" : : : "memory");
}

extern ANDROID_ATOMIC_INLINE void android_memory_barrier(void)
{
    __asm__ __volatile__ ("sync" : : : "memory");
}

extern ANDROID_ATOMIC_INLINE int32_t
android_atomic_acquire_load(volatile const int32_t *ptr)
{
    int32_t value = *ptr;
    android_memory_barrier();
    return value;
}

extern ANDROID_ATOMIC_INLINE int32_t
android_atomic_release_load(volatile const int32_t *ptr)
{
    android_memory_barrier();
    return *ptr;
}

extern ANDROID_ATOMIC_INLINE void
android_atomic_acquire_store(int32_t value, volatile int32_t *ptr)
{
    *ptr = value;
    android_memory_barrier();
}

extern ANDROID_ATOMIC_INLINE void
android_atomic_release_store(int32_t value, volatile int32_t *ptr)
{
    android_memory_barrier();
    *ptr = value;
}

extern ANDROID_ATOMIC_INLINE int
android_atomic_cas(int32_t old_value, int32_t new_value, volatile int32_t *ptr)
{
    int32_t prev, status;
    do {
        __asm__ __volatile__ (
            "    ll     %[prev], (%[ptr])\n"
            "    li     %[status], 1\n"
            "    bne    %[prev], %[old], 9f\n"
            "    move   %[status], %[new_value]\n"
            "    sc     %[status], (%[ptr])\n"
            "9:\n"
            : [prev] "=&r" (prev), [status] "=&r" (status)
            : [ptr] "r" (ptr), [old] "r" (old_value), [new_value] "r" (new_value)
            );
    } while (__builtin_expect(status == 0, 0));
    return prev != old_value;
}

extern ANDROID_ATOMIC_INLINE int
android_atomic_acquire_cas(int32_t old_value,
                           int32_t new_value,
                           volatile int32_t *ptr)
{
    int status = android_atomic_cas(old_value, new_value, ptr);
    android_memory_barrier();
    return status;
}

extern ANDROID_ATOMIC_INLINE int
android_atomic_release_cas(int32_t old_value,
                           int32_t new_value,
                           volatile int32_t *ptr)
{
    android_memory_barrier();
    return android_atomic_cas(old_value, new_value, ptr);
}


extern ANDROID_ATOMIC_INLINE int32_t
android_atomic_add(int32_t increment, volatile int32_t *ptr)
{
    int32_t prev, status;
    android_memory_barrier();
    do {
        __asm__ __volatile__ (
			"    ll    %[prev], (%[ptr])\n"
			"    addu  %[status], %[prev], %[inc]\n"
			"    sc    %[status], (%[ptr])\n"
			:  [status] "=&r" (status), [prev] "=&r" (prev)
			:  [ptr] "r" (ptr), [inc] "Ir" (increment)
			);
    } while (__builtin_expect(status == 0, 0));
    return prev;
}

extern ANDROID_ATOMIC_INLINE int32_t
android_atomic_inc(volatile int32_t *addr)
{
    return android_atomic_add(1, addr);
}

extern ANDROID_ATOMIC_INLINE int32_t
android_atomic_dec(volatile int32_t *addr)
{
    return android_atomic_add(-1, addr);
}

extern ANDROID_ATOMIC_INLINE int32_t
android_atomic_and(int32_t value, volatile int32_t *ptr)
{
    int32_t prev, status;
    android_memory_barrier();
    do {
        __asm__ __volatile__ (
			"    ll    %[prev], (%[ptr])\n"
			"    and   %[status], %[prev], %[value]\n"
			"    sc    %[status], (%[ptr])\n"
			: [prev] "=&r" (prev), [status] "=&r" (status)
			: [ptr] "r" (ptr), [value] "Ir" (value)
            );
    } while (__builtin_expect(status == 0, 0));
    return prev;
}

extern ANDROID_ATOMIC_INLINE int32_t
android_atomic_or(int32_t value, volatile int32_t *ptr)
{
    int32_t prev, status;
    android_memory_barrier();
    do {
        __asm__ __volatile__ (
			"    ll    %[prev], (%[ptr])\n"
			"    or    %[status], %[prev], %[value]\n"
			"    sc    %[status], (%[ptr])\n"
			: [prev] "=&r" (prev), [status] "=&r" (status)
			: [ptr] "r" (ptr), [value] "Ir" (value)
            );
    } while (__builtin_expect(status == 0, 0));
    return prev;
}

#endif /* _ATOMIC_H_ */
