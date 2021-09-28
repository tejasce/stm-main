#ifndef CUTILS_TIME_H
#define CUTILS_TIME_H

#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief  Get current timestamp with nanosecond granularity.
 *         Uses CLOCK_MONOTNOIC_RAW. Good for testing short durations.
 *
 * @return  64bit current timestamp value in nanoseconds
 */
static inline uint64_t
gettsc(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

/*
 * @brief  Get current timestamp with microsecond granularity.
 *         Uses CLOCK_MONOTNOIC_RAW. Good for testing short durations.
 *
 * @return  64bit current timestamp value in microseconds
 */
static inline uint64_t
gettsc_us(void)
{
    return gettsc() / 1000;
}

/*
 * @brief  Get current timestamp with millisecond granularity.
 *         Uses CLOCK_MONOTNOIC_RAW. Good for testing short durations.
 *
 * @return  64bit current timestamp value in milliseconds
 */
static inline uint64_t
gettsc_ms(void)
{
    return gettsc() / 1000000;
}

/*
 * @brief  Get timestamp from given duration (in milliseconds) from now.
 *
 * @param[in] ms  Duration in milliseconds
 *
 * @return  64bit timestamp value [in future or past] in milliseconds from
 *          current value
 */
static inline uint64_t
get_timestamp_ms(int64_t ms)
{
    return gettsc_ms() + ms;
}

#ifdef __cplusplus
}
#endif

#endif // CUTILS_TIME_H
