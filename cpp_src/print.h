#ifndef PRINT_H_
#define PRINT_H_

#include <cinttypes>

#ifdef WIN32
    #define LOG_FILE_PATH "../"
#else
    #define LOG_FILE_PATH "/home/kijada/genetic/"
#endif

#define LOG_FILE_NAME "evolution"
#define LOG_FILE_EXTENSION ".log"

#define LOG_SIZE (4096L*4096L)

#define PRINT_LEVEL_ALWAYS      -1
#define PRINT_LEVEL_ERROR       0
#define PRINT_LEVEL_WARNING     1
#define PRINT_LEVEL_NOTICE      2
#define PRINT_LEVEL_INFO 	    3
#define PRINT_LEVEL_DEBUG       4
#define PRINT_LEVEL_TRASH       5
#define PRINT_LEVEL_JUNK        6


#define pr_always(__x...)  print(PRINT_LEVEL_ALWAYS, __x)
#define pr_error(__format, __x...)   print(PRINT_LEVEL_ERROR,"Error: " __format " <%s:%d>" , ##__x, __FUNCTION__, __LINE__)
#define pr_warning(__x...) print(PRINT_LEVEL_WARNING, __x)
#define pr_notice(__x...)  print(PRINT_LEVEL_NOTICE, __x)
#define pr_info(__x...)    print(PRINT_LEVEL_INFO, __x)
#define pr_debug(__format, __x...)   print(PRINT_LEVEL_DEBUG,"Debug: " __format " <%s:%d>" , ##__x, __FUNCTION__, __LINE__)
#define pr_trash(__format, __x...)   print(PRINT_LEVEL_TRASH, "Trash: " __format, ##__x)
#define pr_junk(__format, __x...)   print(PRINT_LEVEL_JUNK, "Junk: " __format, ##__x)

#define pr_malloc(__format, __x...)   print(PRINT_LEVEL_DEBUG,"malloc: " __format " <%s:%d>" , ##__x, __FUNCTION__, __LINE__)
#define pr_free(__format, __x...)   print(PRINT_LEVEL_DEBUG,"free: " __format " <%s:%d>" , ##__x, __FUNCTION__, __LINE__)

void print(int level, char const *format, ...);

#endif
