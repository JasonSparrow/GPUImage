//
//  CGDrawLog.h
//  CGDraw
//
//  Created by Jason on 2021/6/23.
//

#ifndef CGDrawLog_h
#define CGDrawLog_h

#define CG_LOG_ALL     1
#define CG_LOG_VERBOSE 2
#define CG_LOG_DEBUG   3
#define CG_LOG_INFO    4
#define CG_LOG_WARN    5
#define CG_LOG_ERROR   6
#define CG_LOG_FATAL   7

extern int CG_LOG_LEVEL;

namespace CGDraw {

#ifndef MODULE_NAME
#define MODULE_NAME CGDraw
#endif
#ifndef MODULE_VERSION
#define MODULE_VERSION dev
#endif

#define _CG_DRAW_MAKE_TAG(...) #__VA_ARGS__
#define CG_DRAW_MAKE_TAG(...) _CG_DRAW_MAKE_TAG(__VA_ARGS__)

#ifndef CG_DRAW_TAG
#define CG_DRAW_TAG CG_DRAW_MAKE_TAG(SlidePlayer # MODULE_NAME-MODULE_VERSION)
#endif

#ifndef CG_DRAW_LOG_LEVEL
#  ifndef CG_DEBUG
#    define CG_DRAW_LOG_LEVEL CG_LOG_ALL
#  else
#    define CG_DRAW_LOG_LEVEL CG_LOG_INFO
#  endif
#endif


template<char c, int k = 0>
struct ShortName {
    template<int L>
    static constexpr
    inline const char *_(char const (&val)[L]) {
        return k >= L ? val : val[L - k - 1] == c ? (&val[L - k]) : ShortName<c, k + 1>::_(val);
    }
};

template<char c>
struct ShortName<c, 32> {
    template<int L>
    static constexpr
    inline const char *_(char const (&val)[L]) {
        return val;
    }
};

#if defined(__ANDROID__)

#define CG_DRAW_LOG_IMPL(LEVEL, fmt, ...) \
do { \
    if(LEVEL >= CG_LOG_LEVEL){\
        constexpr const char* sname = CGDraw::ShortName<'/'>::_(__FILE__); \
        __android_log_print((int)LEVEL, CG_DRAW_TAG, "[%s %d] " fmt, sname, __LINE__, ##__VA_ARGS__); \
    }\
} while (false)

#define CG_DRAW_BREAK_IMPL(fmt, ...) \
do { \
    constexpr const char* sname = CGDraw::ShortName<'/'>::_(__FILE__); \
    __android_log_print((int)CG_LOG_FATAL, CG_DRAW_TAG, "[%s %d] " fmt, sname, __LINE__, ##__VA_ARGS__); \
    __builtin_trap(); \
} while (false)


#else

#define CG_DRAW_LOG_IMPL(LEVEL, fmt, ...) \
do { \
    if(LEVEL >= CG_LOG_LEVEL){\
        constexpr const char* sname = CGDraw::ShortName<'/'>::_(__FILE__); \
        printf("@L%d %s - [%s %d] " fmt "\n", (int)LEVEL, CG_DRAW_TAG, sname, __LINE__, ##__VA_ARGS__); \
    }\
} while (false)

#define CG_DRAW_BREAK_IMPL(fmt, ...) \
do { \
    constexpr const char* sname = CGDraw::ShortName<'/'>::_(__FILE__); \
    printf("%s [%s %d] " fmt "\n", CG_DRAW_TAG, sname, __LINE__, ##__VA_ARGS__); \
    void* callstack[128]; \
    int frames = backtrace(callstack, 128); \
    char** strs = backtrace_symbols(callstack, frames); \
    for (int i = 0; i < frames; ++i) { \
        fprintf(stderr, "%s\n", strs[i]); \
    } \
    free(strs); \
    __builtin_trap(); \
} while (false)

#endif
}

#ifndef CG_DEBUG
#define CG_DRAW_LOGT(fmt, ...)   CG_DRAW_LOG_IMPL(CG_LOG_VERBOSE, fmt, ##__VA_ARGS__)
#else
#define CG_DRAW_LOGT(fmt, ...)
#endif

#if(CG_LOG_VERBOSE > CG_DRAW_LOG_LEVEL)
#define CG_DRAW_LOGV(fmt, ...)   CG_DRAW_LOG_IMPL(CG_LOG_VERBOSE, fmt, ##__VA_ARGS__)
#else
#define CG_DRAW_LOGV(fmt, ...)
#endif

#if(CG_LOG_DEBUG > CG_DRAW_LOG_LEVEL)
#define CG_DRAW_LOGD(fmt, ...)   CG_DRAW_LOG_IMPL(CG_LOG_DEBUG, fmt, ##__VA_ARGS__)
#else
#define CG_DRAW_LOGD(fmt, ...)
#endif

#if(CG_LOG_INFO > CG_DRAW_LOG_LEVEL)
#define CG_DRAW_LOGI(fmt, ...)   CG_DRAW_LOG_IMPL(CG_LOG_INFO, fmt, ##__VA_ARGS__)
#else
#define CG_DRAW_LOGI(fmt, ...)
#endif

#if(CG_LOG_WARN > CG_DRAW_LOG_LEVEL)
#define CG_DRAW_LOGW(fmt, ...)   CG_DRAW_LOG_IMPL(CG_LOG_WARN, fmt, ##__VA_ARGS__)
#else
#define CG_DRAW_LOGW(fmt, ...)
#endif

#if(CG_LOG_ERROR > CG_DRAW_LOG_LEVEL)
#define CG_DRAW_LOGE(fmt, ...)   CG_DRAW_LOG_IMPL(CG_LOG_ERROR, fmt, ##__VA_ARGS__)
#else
#define CG_DRAW_LOGE(fmt, ...)
#endif

#ifndef CG_DEBUG
#define CG_DRAW_LOGF(fmt, ...)   CG_DRAW_BREAK_IMPL(fmt, ##__VA_ARGS__)
#else
#define CGLOGF(fmt, ...)
#endif


#define CGASSERT_RELEASE(e) { \
    if(!(e)) { \
        CG_DRAW_LOGE("Assert failed: File: %s, Function: %s, Line: %d", __FILE__, __FUNCTION__, __LINE__); \
    } \
}


#endif /* CGDrawLog_h */
