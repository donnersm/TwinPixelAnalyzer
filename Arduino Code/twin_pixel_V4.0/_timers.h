//***************************************************************
// This code comes from the FastLED library
// by Daniel Garcia and Mark Kriegsman.
// https://github.com/FastLED/FastLED
//
// By including this file all the EVERY_N_* timer functions from
// the FastLED library can be used without including the entire
// library.  Please see the original FastLED's lib8tion.h file
// for more details and comments.
// https://github.com/FastLED/FastLED/blob/master/lib8tion.h
//
//
// Marc Miller, May 2020
//***************************************************************


#define LIB8STATIC __attribute__ ((unused)) static inline
#define GET_MILLIS millis
LIB8STATIC uint16_t seconds16()
{
    uint32_t ms = GET_MILLIS();
    uint16_t s16;
    s16 = ms / 1000;
    return s16;
}

LIB8STATIC uint16_t minutes16()
{
    uint32_t ms = GET_MILLIS();
    uint16_t m16;
    m16 = (ms / (60000L)) & 0xFFFF;
    return m16;
}

LIB8STATIC uint8_t hours8()
{
    uint32_t ms = GET_MILLIS();
    uint8_t h8;
    h8 = (ms / (3600000L)) & 0xFF;
    return h8;
}

LIB8STATIC uint16_t div1024_32_16( uint32_t in32)
{
    uint16_t out16;
#if defined(__AVR__)
    asm volatile (
                  "  lsr %D[in]  \n\t"
                  "  ror %C[in]  \n\t"
                  "  ror %B[in]  \n\t"
                  "  lsr %D[in]  \n\t"
                  "  ror %C[in]  \n\t"
                  "  ror %B[in]  \n\t"
                  "  mov %B[out],%C[in] \n\t"
                  "  mov %A[out],%B[in] \n\t"
                  : [in] "+r" (in32),
                  [out] "=r" (out16)
                  );
#else
    out16 = (in32 >> 10) & 0xFFFF;
#endif
    return out16;
}

LIB8STATIC uint16_t bseconds16()
{
    uint32_t ms = GET_MILLIS();
    uint16_t s16;
    s16 = div1024_32_16( ms);
    return s16;
}

#if 1
#define INSTANTIATE_EVERY_N_TIME_PERIODS(NAME,TIMETYPE,TIMEGETTER) \
class NAME {    \
public: \
    TIMETYPE mPrevTrigger;  \
    TIMETYPE mPeriod;   \
    \
    NAME() { reset(); mPeriod = 1; }; \
    NAME(TIMETYPE period) { reset(); setPeriod(period); };    \
    void setPeriod( TIMETYPE period) { mPeriod = period; }; \
    TIMETYPE getTime() { return (TIMETYPE)(TIMEGETTER()); };    \
    TIMETYPE getPeriod() { return mPeriod; };   \
    TIMETYPE getElapsed() { return getTime() - mPrevTrigger; }  \
    TIMETYPE getRemaining() { return mPeriod - getElapsed(); }  \
    TIMETYPE getLastTriggerTime() { return mPrevTrigger; }  \
    bool ready() { \
        bool isReady = (getElapsed() >= mPeriod);   \
        if( isReady ) { reset(); }  \
        return isReady; \
    }   \
    void reset() { mPrevTrigger = getTime(); }; \
    void trigger() { mPrevTrigger = getTime() - mPeriod; }; \
        \
    operator bool() { return ready(); } \
};
INSTANTIATE_EVERY_N_TIME_PERIODS(CEveryNMillis,uint32_t,GET_MILLIS);
INSTANTIATE_EVERY_N_TIME_PERIODS(CEveryNSeconds,uint16_t,seconds16);
INSTANTIATE_EVERY_N_TIME_PERIODS(CEveryNBSeconds,uint16_t,bseconds16);
INSTANTIATE_EVERY_N_TIME_PERIODS(CEveryNMinutes,uint16_t,minutes16);
INSTANTIATE_EVERY_N_TIME_PERIODS(CEveryNHours,uint8_t,hours8);
#else

template<typename timeType,timeType (*timeGetter)()>
class CEveryNTimePeriods {
public:
    timeType mPrevTrigger;
    timeType mPeriod;

    CEveryNTimePeriods() { reset(); mPeriod = 1; };
    CEveryNTimePeriods(timeType period) { reset(); setPeriod(period); };
    void setPeriod( timeType period) { mPeriod = period; };
    timeType getTime() { return (timeType)(timeGetter()); };
    timeType getPeriod() { return mPeriod; };
    timeType getElapsed() { return getTime() - mPrevTrigger; }
    timeType getRemaining() { return mPeriod - getElapsed(); }
    timeType getLastTriggerTime() { return mPrevTrigger; }
    bool ready() {
        bool isReady = (getElapsed() >= mPeriod);
        if( isReady ) { reset(); }
        return isReady;
    }
    void reset() { mPrevTrigger = getTime(); };
    void trigger() { mPrevTrigger = getTime() - mPeriod; };

    operator bool() { return ready(); }
};
typedef CEveryNTimePeriods<uint16_t,seconds16> CEveryNSeconds;
typedef CEveryNTimePeriods<uint16_t,bseconds16> CEveryNBSeconds;
typedef CEveryNTimePeriods<uint32_t,millis> CEveryNMillis;
typedef CEveryNTimePeriods<uint16_t,minutes16> CEveryNMinutes;
typedef CEveryNTimePeriods<uint8_t,hours8> CEveryNHours;
#endif


#define CONCAT_HELPER( x, y ) x##y
#define CONCAT_MACRO( x, y ) CONCAT_HELPER( x, y )
#define EVERY_N_MILLIS(N) EVERY_N_MILLIS_I(CONCAT_MACRO(PER, __COUNTER__ ),N)
#define EVERY_N_MILLIS_I(NAME,N) static CEveryNMillis NAME(N); if( NAME )
#define EVERY_N_SECONDS(N) EVERY_N_SECONDS_I(CONCAT_MACRO(PER, __COUNTER__ ),N)
#define EVERY_N_SECONDS_I(NAME,N) static CEveryNSeconds NAME(N); if( NAME )
#define EVERY_N_BSECONDS(N) EVERY_N_BSECONDS_I(CONCAT_MACRO(PER, __COUNTER__ ),N)
#define EVERY_N_BSECONDS_I(NAME,N) static CEveryNBSeconds NAME(N); if( NAME )
#define EVERY_N_MINUTES(N) EVERY_N_MINUTES_I(CONCAT_MACRO(PER, __COUNTER__ ),N)
#define EVERY_N_MINUTES_I(NAME,N) static CEveryNMinutes NAME(N); if( NAME )
#define EVERY_N_HOURS(N) EVERY_N_HOURS_I(CONCAT_MACRO(PER, __COUNTER__ ),N)
#define EVERY_N_HOURS_I(NAME,N) static CEveryNHours NAME(N); if( NAME )

#define CEveryNMilliseconds CEveryNMillis
#define EVERY_N_MILLISECONDS(N) EVERY_N_MILLIS(N)
#define EVERY_N_MILLISECONDS_I(NAME,N) EVERY_N_MILLIS_I(NAME,N)
