#ifndef PROCESS_H
#define PROCESS_H

#include "Includes.h"
#include "Scheduler.h"

class Scheduler;

class Process
{
    friend class Scheduler;
public:
    /*
    * @param manager: The scheduler overseeing this Process
    * @param priority: The prority of this Process defined in config.h
    * @param period: The period this process should be serviced at (SERVICE_CONSTANTLY = As often as possible)
    * @param iterations: Number of iterations this process should be serviced before being disabled (RUNTIME_FOREVER = infinite)
    * @param overSchedThresh: The periods behind this process can get, before a WARNING_PROC_OVERSCHEDULED is triggered
    * (OVERSCHEDULED_NO_WARNING = a warning will never be triggered)
    */
    Process(Scheduler &manager, ProcPriority priority, uint32_t period,
            int iterations=RUNTIME_FOREVER,
            uint16_t overSchedThresh = OVERSCHEDULED_NO_WARNING);

    ///////////////////// PROCESS OPERATIONS /////////////////////////
    // These are all the same as calling scheduler.method(process)
    // Ex: scheduler.add(process), see Scheduler.h for documentation
    bool add(bool enableIfNot=false);
    bool disable();
    bool enable();
    bool destroy();
    bool restart();

    ///////////////////// GETTERS /////////////////////////

    // These methods are also the same as calling calling scheduler.method(process)
    // See the documentation in Scheduler.h
    inline uint8_t getID() { return _sid; };
    inline bool isEnabled() { return _enabled; }
    inline bool isNotDestroyed() { return _scheduler.isNotDestroyed(*this); }


    /*
    * Get the scheduler that is overseeing this process
    *
    * @return: Refrence to Scheduler
    */
    inline Scheduler &scheduler() { return _scheduler; }

    /*
    * Get the remaining iterations this Process will be serviced
    *
    * @return: Number of iterations or RUNTIME_FOREVER
    */
    inline int getIterations() { return _iterations; }

    /*
    * Get the period this Process is scheduled to run
    *
    * @return: period or SERVICE_CONSTANTLY
    */
    inline unsigned int getPeriod() { return _period; }

    /*
    * Get the priority for this process
    *
    * @return: ProcPriority level defined in Config.h
    */
    inline ProcPriority getPriority() { return _pLevel; }


    /*
    * Get the time before this process is scheduled to be serviced again
    * A negative number means the scheduler is behind
    *
    * @return: int32_t time offset
    */
    inline int32_t timeToNextRun() { return (_scheduledTS + _period) - _scheduler.getCurrTS(); }


    /*
    * Get the timestamp the most recent iteration actually started at
    *
    * @return: uint32_t timestamp
    */
    inline uint32_t getActualRunTS() { return _actualTS; }


    /*
    * Get the time stamp the most recent iteration was scheduled to run at
    *
    * @return: uint32_t timestamp
    */
    inline uint32_t getScheduledTS() { return _scheduledTS; }


    /*
    * The number of additional periods behind since resetOverSchedWarning() was called
    * this Process can be before an WARNING_PROC_OVERSCHEDULED is triggered
    *
    * @return: uint16_t threshhold
    */
    inline uint16_t getOverSchedThresh() { return _overSchedThresh; }


    /*
    * The number of additional periods behind since resetOverSchedWarning() was called
    *
    * @return: uint16_t count
    */
    inline uint16_t getCurrPBehind() { return _pBehind; }


    ///////////////////// SETTERS /////////////////////////

    /*
    * Set the number of iterations for this process to run
    * NOTE: Setting iterations to RUNTIME_FOREVER runs the process forever
    */
    void setIterations(int iterations);

    /*
    * Set the period between when this process is serviced
    * NOTE: Setting period to SERVICE_CONSTANTLY, will have the Scheduler service it as often as possible
    */
    void setPeriod(uint32_t period);

    /*
    * Force the scheduler to service this on the next pass (if enabled)
    * NOTE: This service will not count twoards an iteration
    */
    inline void force() { _force = true; }

    /*
    * Reset the number of period behind count back to zero
    * ie. Reset the warning
    * NOTE: This warning will trigger again soon unless the scheduler can catch up
    */
    inline void resetOverSchedWarning() { _pBehind = 0; }

    /*
    * Similar to resetOverSchedWarning(), except it also resets how far behind the scheduler is
    * Ex: If this the next iteration should of happened 20 ms ago, it will now be zero
    */
    void resetTimeStamps();

// Enable this option in config.h to track time statistics on processes
#ifdef _PROCESS_STATISTICS
    /*
    * Returns the average run time for this Process' service routine
    *
    * @return: uin32_t time
    */
    uint32_t getAvgRunTime();

    /*
    * Returns the computed CPU load % for this process
    * NOTE: You must call updateStats() on the scheduler to update this value
    *
    * @return: uint8_t percent
    */
    inline uint8_t getLoadPercent() { return _histLoadPercent; }

#endif


// Enable this option in config.h to allow the Scheduler to interrupt processes that are not returning for their service routine
#ifdef _PROCESS_TIMEOUT_INTERRUPTS
    /*
    * Returns the max runtime for this process before the Scheduler interrupts it
    *
    * @return: uint32_t timespan or PROCESS_NO_TIMEOUT
    */
    inline uint32_t getTimeout() {return _timeout;}


    /*
    * Set the max runtime for this process before the Scheduler interrupts it
    */
    void setTimeout(uint32_t timeout);
#endif

protected:

#ifdef _PROCESS_EXCEPTION_HANDLING

    /*
    * Yield. This will immediatley transfer control back to the Scheduler
    * NOTE: that nothing below this call will ever be executed
    * NOTE: ONLY CALL THIS FROM WITHIN YOUR SERVICE ROUTINE
    */
    inline void yield() { _scheduler.raiseException(LONGJMP_YIELD_CODE); }

#endif

    ///////////////////// GETTERS /////////////////////////
#ifdef _PROCESS_TIMEOUT_INTERRUPTS
    /*
    * Returns the time remaining this process can run before it will be interrupted
    * NOTE: This method is undefined if the timeout is set to PROCESS_NO_TIMEOUT
    *
    * @return: int32_t time remaining, a negative value means it will happen any time now
    */
    inline int32_t timeToTimeout() { return _timeout - (_scheduler.getCurrTS() - getActualRunTS()); }
#endif

    /*
    * Get the delay from when the Scheduler scheduled it to run, to when it actualy was serviced
    * ie. Reset the warning
    */
    inline uint32_t getStartDelay() { return _actualTS - _scheduledTS; }


    ///////////////////// VIRTUAL FUNCTIONS /////////////////////////
    /*
    * This is the heart of your Process, this is the Scheduler's entry point into your Process' service routine
    * This is the method that will be called by the Scheduler
    * This method should run your service routine
    */
    virtual void service() = 0;


    /////// THESE ARE ALL METHODS FOR YOU TO OVVERRIDE AS NEEDED ////////

    /*
    * This will be called by the scheduler when your process is added
    * Use it to setup everything this process uses
    */
    virtual void setup();


    /*
    * This is the opposite of setup(), it should undo everything it did
    * It will be called when this process is being destroyed
    */
    virtual void cleanup();


    /*
    * This will be called be the scheduler when this process is enabled
    */
    virtual void onEnable();


    /*
    * This is the opposite of onEnable(), it should undo everything it did
    * It will be called when this process is being disabled
    */
    virtual void onDisable();


    /*
    * This is the warning/error handler for your Process
    * The scheduler will call this method immediatley after an error/warning condition occured in this process
    * Look at Includes.h for the different types of ProcessWarnings
    */
    virtual void handleWarning(ProcessWarning warning);


#ifdef _PROCESS_EXCEPTION_HANDLING

    /*
    * Calling this method will raise an Exception and immediatley
    * call your handleException method
    * This is useful if you need to jump out of an error condition in a deeply nested function call
    * NOTE: You might find it useful to store more detailed info about the
    * error condition in class attributes
    */
    virtual void raiseException(int e) { _scheduler.raiseException(e); }

    /*
    * This is the Exception handler for your Process' Service routine
    * If you raise an Exception, this method will immediatley be called
    * NOTE: The scheduler will go on to service the next process,
    * YOU WILL NOT GO BACK TO WHERE THE EXCEPTION WAS THROWN
    *
    * @return: Return true to catch the exception, otherwise the Exception will be passed down to
    * the handleException() method of the scheduler
    */
    virtual bool handleException(int e) { return false; }

#endif


////////////// YOU CAN IGNORE THE PRIVATE STUFF BELOW THIS LINE //////////////
private:
    // Return true if this Process needs servicing
    bool needsServicing(uint32_t start);
    // Called right before scheduler services
    void willService(uint32_t now);
    // Called right after scheduler services
    bool wasServiced(bool wasForced);
    // Returns true if this Process is over a period behind
    bool isPBehind(uint32_t curr);

    inline bool hasNext() { return _next; }
    // GETTERS
    inline Process *getNext() { return _next; }
    // SETTERS
    inline void setNext(Process *next) { this->_next = next; }
    inline void setID(uint8_t sid) { this->_sid = sid; }
    inline void decIterations() { _iterations--; }
    inline void setScheduledTS(uint32_t ts) { _scheduledTS = ts; }
    inline void setActualTS(uint32_t ts) { _actualTS = ts; }
    inline bool forceSet() { return _force; }

    inline void incrPBehind() { _pBehind++; }

    inline void setDisabled() { _enabled = false; }
    inline void setEnabled() { _enabled = true; }

    Scheduler &_scheduler;
    bool _enabled, _force;
    int _iterations;
    unsigned int _period;
    uint8_t _sid;
    uint32_t _scheduledTS, _actualTS;
    // Linked List
    Process *volatile _next;

    // Tracks overscheduled
    uint16_t _overSchedThresh, _pBehind;

    const ProcPriority _pLevel;


#ifdef _PROCESS_TIMEOUT_INTERRUPTS
    uint32_t _timeout;
#endif

#ifdef _PROCESS_STATISTICS
    bool statsWillOverflow(hIterCount_t iter, hTimeCount_t tm);
    void divStats(uint8_t div);
    inline void setHistIterations(hIterCount_t val) { _histIterations = val; }
    inline void setHistRuntime(hTimeCount_t val) { _histRunTime = val; }
    inline void setHistLoadPercent(uint8_t percent) { _histLoadPercent = percent; }
    inline hIterCount_t getHistIterations() { return _histIterations; }
    inline hTimeCount_t getHistRunTime() { return _histRunTime; }

    hIterCount_t _histIterations;
    hTimeCount_t _histRunTime;
    uint8_t _histLoadPercent;

#endif


};



#endif
