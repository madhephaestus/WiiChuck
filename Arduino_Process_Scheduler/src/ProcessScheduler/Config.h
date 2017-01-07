#ifndef PS_CONFIG_H
#define PS_CONFIG_H

#include <Arduino.h>

/* Uncomment this to allow Exception Handling functionality */
//#define _PROCESS_EXCEPTION_HANDLING

/* Uncomment this to allow the scheduler to interrupt long running processes */
// This requires _PROCESS_EXCEPTION_HANDLING to also be enabled
//#define _PROCESS_TIMEOUT_INTERRUPTS

/* Uncomment this to allow Process timing statistics functionality */
//#define _PROCESS_STATISTICS

/* Uncomment this to use microseconds instead of milliseconds for timestamp unit (more precise) */
//#define _MICROS_PRECISION


/* The size of the scheduler job queue, */
//increase if add(), destroy(), enable(), disable(), or updateStats() is returning false*/
#define SCHEDULER_JOB_QUEUE_SIZE 20

typedef enum ProcPriority
{
    // Feel free to add custom prority levels in here
    ////////////// BEGIN //////////////////
    HIGH_PRIORITY = 0,
    MEDIUM_PRIORITY,
    LOW_PRIORITY,

    ////////////// END //////////////////

    NUM_PRIORITY_LEVELS
} ProcPriority;


#ifdef _PROCESS_STATISTICS
/*** The larger the following two types are, the more accurate the statistics will be ****/
    // Type used to track process iterations run count
    typedef uint32_t hIterCount_t;
    // Type used to track process total runtime
    typedef uint32_t hTimeCount_t;
/**************************************/

    // What to divide the two vars above when overflow is about to happen
    #define HISTORY_DIV_FACTOR 2
#endif

#endif
