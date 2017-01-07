#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Includes.h"

typedef struct RingBuf RingBuf;

class Process;



class Scheduler
{

public:
    Scheduler();
    ~Scheduler();

/*************** Methods to Perform Actions on Processes ****************/
// NOTE: These can also be called directly on the Process object
// example process.add()

    /**
    * Add a new process to the scheduler chain
    * If it is already added, do nothing
    * This will trigger the Processes setup() method
    *
    * @return: True on success
    */
    bool add(Process &process, bool enableIfNot = false);


    /**
    * Disable a process
    * If it is not part of chain or already disabled, do nothing
    * This will trigger the Processes onDisable() method
    *
    * @return: True on success
    */
    bool disable(Process &process);


    /**
    * Enable a process
    * If it is not part of chain or already enabled, do nothing
    * This will trigger the Processes onEnable() method
    *
    * @return: True on success
    */
    bool enable(Process &process);


    /**
    * Remove a process from the scheduling chain
    * If it is not part of chain, do nothing
    * This will trigger the Processes cleanup() method
    * NOTE: If it is currently enabled, disable() will automatically be called first
    *
    * @return: True on success
    */
    bool destroy(Process &process);

    /**
    * Same as calling destroy() and add() with less overhead
    *
    * @return: True on success
    */
    bool restart(Process &process);


    /**
    * Get the id of process
    *
    * @return: the unique id, otherwise 0 if not added to a scheduler
    */
    uint8_t getID(Process &process);


    /**
    * Determine if the process is currently enabled
    *
    * @return: bool
    */
    bool isEnabled(Process &process);


    /**
    * Determine if the scheduler is currently in the middle of servicing this process
    *
    * @return: bool
    */
    bool isRunningProcess(Process &process);


    /**
    * Determine if the process is currently destroyed (not part of the scheduler chain)
    *
    * @return: bool
    */
    bool isNotDestroyed(Process &process);

/*********************** End Processes Methods ********************/


    /**
    * Destroy all processes then put the processor into a low power sleep state
    *
    * @return: True on success
    */
    bool halt();


    /**
    * Get the currently running process
    *
    * @return: a pointer to the process, NULL on no process currently being run
    */
    static Process *getActive();

    /**
    * Get a pointer to the process with id
    *
    * @return: a pointer to the process, NULL on no process with that id
    */
    Process *findProcById(uint8_t id);

    /**
    * Get the number of processes in the scheduler chain at priority levels
    * If priority = ALL_PRIORITY_LEVELS, count at all priority levels
    * If enabledOnly = true, only the enabled processes will be counted
    *
    * @return: a pointer to the process, NULL on no process with that id
    */
    uint8_t countProcesses(int priority = ALL_PRIORITY_LEVELS, bool enabledOnly = true);

    /**
    * Get the internal timestamp the scheduler is using to track time
    * Either the same as millis() or micros() depending on _MICROS_PRECISION
    * @return: uint32_t
    */
    static uint32_t getCurrTS();

    /**
    * Run one pass through the scheduler, call this repeatedly in your void loop()
    *
    * @return: The number of processes serviced in that pass
    */
    int run();


// Enable this option in config.h to track time statistics on processes
#ifdef _PROCESS_STATISTICS
    /**
    * This will update the Process.getLoadPercent() method
    * It will estimate the % CPU time for all processes
    * NOTE: This will simply add this task to the scheduler job queue
    * The update will not happen until the scheduler gets a chance to process the request
    *
    * @return: True on success
    */
    bool updateStats();

#endif

// Enable this option to allow processes to raise and catch custom exceptions
// Behind the scenes this is using setjmp and longjmp
#ifdef _PROCESS_EXCEPTION_HANDLING
    /**
    * Raise Exception with code e inside a Process service routine
    * Execution will stop immediatley, and the processes handleException() will be called
    * NOTE: DO NOT CALL THIS FROM OUTSIDE A PROCESS SERVICE ROUTINE
    * @return: True on success
    */
    void raiseException(int e);

    static jmp_buf _env; // have to do this to access it from ISR
#endif

////////// YOU CAN IGNORE ALL THE PROTECTED/PRIVATE METHODS BELOW HERE /////////

protected:
#ifdef _PROCESS_EXCEPTION_HANDLING
    /*
    * Handle uncaught Process exceptions from Process process with Exception code e
    */
    virtual void handleException(Process &process, int e) { };
#endif
    // Inner queue object class to queue scheduler jobs
    class QueableOperation
    {
    public:
        enum OperationType
        {
            NONE = 0,
            ADD_SERVICE,
            DESTROY_SERVICE,
            DISABLE_SERVICE,
            ENABLE_SERVICE,
            RESTART_SERVICE,
            HALT,
#ifdef _PROCESS_STATISTICS
            UPDATE_STATS,
#endif
        };

        QueableOperation();
        QueableOperation(OperationType op);
        QueableOperation(Process *serv, OperationType op);

        Process *getProcess();
        OperationType getOperation();
        bool queue(RingBuf *queue);

    private:
        Process *_process;
        const uint8_t _operation;
    };


#ifdef _PROCESS_EXCEPTION_HANDLING
    // handle the return from setjmp()
    bool jmpHandler(int e);
#endif

#ifdef _PROCESS_STATISTICS
    // Actually do the update that was requested in updateStats()
    void procUpdateStats();
    // This will handle overflow condiitions on the statistics
    void handleHistOverFlow(uint8_t div);
#endif

    // Methods that process queued operations
    void procDisable(Process &process);
    void procEnable(Process &process);
    void procDestroy(Process &process);
    void procAdd(Process &process);
    void procRestart(Process &process);
    void procHalt();

    // Process the scheduler job queue
    void processQueue();

    // Linked list methods
    bool appendNode(Process &node); // true on success
    bool removeNode(Process &node); // true on success
    bool findNode(Process &node); // True if node exists in list
    Process *findPrevNode(Process &node);


    static Process *_active; // needs to be static for access in ISR
    uint8_t _lastID;
    RingBuf *_queue;

    struct SchedulerPriorityLevel
    {
        Process *head;
        Process *next;
    };
    struct SchedulerPriorityLevel _pLevels[NUM_PRIORITY_LEVELS];


/* CUSTOM COMPILE OPTIONS*/
/*
    bool swapNode(Process &n1, Process &n2);
    void reOrderProcs(ProcPriority level);
*/

};

#endif
