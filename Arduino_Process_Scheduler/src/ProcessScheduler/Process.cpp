#include "Process.h"
#include "Scheduler.h"

    /*********** PUBLIC *************/
    Process::Process(Scheduler &scheduler, ProcPriority priority, uint32_t period,
            int iterations, uint16_t overSchedThresh)
    : _scheduler(scheduler), _pLevel(priority)
    {
        this->_enabled = false;
        this->_period = period;
        this->_iterations = iterations;
        this->_force = false;
        this->_overSchedThresh = overSchedThresh;
        resetTimeStamps();

#ifdef _PROCESS_TIMEOUT_INTERRUPTS
        setTimeout(PROCESS_NO_TIMEOUT);
#endif
    }

    void Process::resetTimeStamps()
    {
        ATOMIC_START
        {
        this->_scheduledTS = _scheduler.getCurrTS();
        this->_actualTS = _scheduler.getCurrTS();
        this->_pBehind = 0;
        }
        ATOMIC_END
    }

    bool Process::disable()
    {
        return _scheduler.disable(*this);
    }


    bool Process::enable()
    {
        return _scheduler.enable(*this);
    }


    bool Process::destroy()
    {
        return _scheduler.destroy(*this);
    }

    bool Process::add(bool enableIfNot)
    {
        return _scheduler.add(*this, enableIfNot);
    }

    bool Process::restart()
    {
        return _scheduler.restart(*this);
    }


    bool Process::needsServicing(uint32_t start)
    {
        return (isEnabled() && (_force ||
            ((getPeriod() == SERVICE_CONSTANTLY || start - getScheduledTS() >= getPeriod()) &&
            (getIterations() == RUNTIME_FOREVER || getIterations() > 0))));
    }

    void Process::setIterations(int iterations)
    {
        ATOMIC_START
        {
            _iterations = iterations;
        }
        ATOMIC_END
    }


    void Process::setPeriod(uint32_t period)
    {
        ATOMIC_START
        {
            _period = period;
        }
        ATOMIC_END
    }



    /*********** PROTECTED *************/

    // Fired on creation/destroy
    void Process::setup() { return; }
    void Process::cleanup() { return; }
    //called on enable/disable
    void Process::onEnable() { return; }
    void Process::onDisable() { return; }
    void Process::handleWarning(ProcessWarning warning)
    {
        if (warning == WARNING_PROC_OVERSCHEDULED)
            resetOverSchedWarning();
    }

    /*********** PRIVATE *************/
    bool Process::isPBehind(uint32_t curr)
    {
        return (curr - getScheduledTS()) >= getPeriod();
    }


    void Process::willService(uint32_t now)
    {
        if (!_force)
        {
            if (getPeriod() != SERVICE_CONSTANTLY)
                setScheduledTS(getScheduledTS() + getPeriod());
            else
                setScheduledTS(now);
        } else {
            _force = false;
        }

        setActualTS(now);

        // Handle scheduler warning
        if (getOverSchedThresh() != OVERSCHEDULED_NO_WARNING && isPBehind(now)) {
            incrPBehind();
            if (getCurrPBehind() >= getOverSchedThresh())
                handleWarning(WARNING_PROC_OVERSCHEDULED);
        } else {
            resetOverSchedWarning();
        }
    }


    // Return true if last if should disable
    bool Process::wasServiced(bool wasForced)
    {
        if (!wasForced && getIterations() > 0) { //Was an iteration
            decIterations();

            if (getIterations() == 0)
                return true;
        }
        return false;
    }


#ifdef _PROCESS_TIMEOUT_INTERRUPTS

    void Process::setTimeout(uint32_t timeout)
    {
        // Can not let interrupt happen
        ATOMIC_START
        {
            _timeout = timeout;
        }
        ATOMIC_END
    }


#endif

#ifdef _PROCESS_STATISTICS

    uint32_t Process::getAvgRunTime()
    {
        if (!_histIterations)
            return 0;

        return _histRunTime / _histIterations;
    }

    bool Process::statsWillOverflow(hIterCount_t iter, hTimeCount_t tm)
    {
        return (_histIterations > _histIterations + iter) || (_histRunTime > _histRunTime + tm);

    }

    void Process::divStats(uint8_t div)
    {
        ++_histIterations /= div;
        ++_histRunTime /= div;
    }

#endif
