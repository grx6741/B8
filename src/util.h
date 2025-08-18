#pragma once

#include <stdio.h>

#ifndef LOG_PREFIX
    #define LOG_PREFIX "B8"
#endif // LOG_PREFIX

#if LOG_ENABLED
    #define LOG_INFO(...)	printf( "[ INFO :: " LOG_PREFIX " ] " __VA_ARGS__)
    #define LOG_WARN(...)	printf( "[ WARN :: " LOG_PREFIX " ] " __VA_ARGS__)
    #define LOG_ERROR(...)	printf( "[ ERR  :: " LOG_PREFIX " ] " __VA_ARGS__)
#else // LOG_ENABLED
    #define LOG_INFO(...)
    #define LOG_WARN(...)
    #define LOG_ERROR(...)
#endif // LOG_ENABLED
