#include "Scheduler.h"
#include "Process.h"

Process *Scheduler::_active = NULL;

#ifdef _PROCESS_EXCEPTION_HANDLING
jmp_buf Scheduler::_env = {};
#endif

#ifdef _PROCESS_TIMEOUT_INTERRUPTS
ISR(TIMER0_COMPA_vect)
{
    if (Scheduler::getActive()) { // routine is running
        uint32_t timeout = Scheduler::getActive()->getTimeout();
        if (timeout != PROCESS_NO_TIMEOUT && Scheduler::getCurrTS() - Scheduler::getActive()->getActualRunTS() >= timeout)
            longjmp(Scheduler::_env, LONGJMP_ISR_CODE);
    }
}
#endif

Scheduler::Scheduler()
: _pLevels{}
{
    _lastID = 0;
    // Create queue
    _queue = RingBuf_new(sizeof(QueableOperation), SCHEDULER_JOB_QUEUE_SIZE);
}

Scheduler::~Scheduler()
{
    processQueue();
    RingBuf_delete(_queue);
}

uint32_t Scheduler::getCurrTS()
{
    return TIMESTAMP();
}

Process *Scheduler::getActive()
{
    return _active;
}

bool Scheduler::isRunningProcess(Process &process)
{
    return _active && &process == _active;
}

Process *Scheduler::findProcById(uint8_t id)
{
    for (uint8_t i=0; i < NUM_PRIORITY_LEVELS; i++)
    {
        for (Process *serv = _pLevels[i].head; serv != NULL; serv = serv->getNext())
        {
            if (serv->getID() == id)
                return serv;
        }
    }
    return NULL;
}

bool Scheduler::isNotDestroyed(Process &process)
{
    return findNode(process);
}

bool Scheduler::isEnabled(Process &process)
{
    return process.isEnabled();
}

bool Scheduler::disable(Process &process)
{
    QueableOperation op(&process, QueableOperation::DISABLE_SERVICE);
    return op.queue(_queue);
}


bool Scheduler::enable(Process &process)
{
    QueableOperation op(&process, QueableOperation::ENABLE_SERVICE);
    return op.queue(_queue);
}

bool Scheduler::add(Process &process, bool enableIfNot)
{
    QueableOperation op(&process, QueableOperation::ADD_SERVICE);
    bool ret = op.queue(_queue);
    if (ret && enableIfNot)
        ret &= enable(process);
    return ret;
}

bool Scheduler::destroy(Process &process)
{
    QueableOperation op(&process, QueableOperation::DESTROY_SERVICE);
    return op.queue(_queue);
}


bool Scheduler::restart(Process &process)
{
    QueableOperation op(&process, QueableOperation::RESTART_SERVICE);
    return op.queue(_queue);
}

bool Scheduler::halt()
{
    QueableOperation op(QueableOperation::HALT);
    return op.queue(_queue);
}

uint8_t Scheduler::getID(Process &process)
{
    return process.getID();
}

uint8_t Scheduler::countProcesses(int priority, bool enabledOnly)
{

    uint8_t count=0;
    for (uint8_t i = (priority == ALL_PRIORITY_LEVELS) ? 0 : (uint8_t)priority; i < NUM_PRIORITY_LEVELS; i++)
    {
        for (Process *curr = _pLevels[i].head; curr != NULL; curr = curr->getNext())
        {
            count += enabledOnly ? curr->isEnabled() : 1;
        }
        if (priority != ALL_PRIORITY_LEVELS)
            break;
    }

    return count;
}

int Scheduler::run()
{
    // Already running in another call frame
    if (_active) return 0;

    uint8_t count = 0;
    for (uint8_t pLevel=0; pLevel < NUM_PRIORITY_LEVELS; pLevel++)
    {
        processQueue();

        _active = _pLevels[pLevel].next;
        if (!_active)
            continue;

        /////////// Run the correct process /////////
        uint32_t start = getCurrTS();

        if (_active->needsServicing(start))
        {
            bool force = _active->forceSet(); // Store whether it was a forced iteraiton
            _active->willService(start);

#ifdef _PROCESS_EXCEPTION_HANDLING
            int ret = setjmp(_env);

#ifdef _PROCESS_TIMEOUT_INTERRUPTS
            ENABLE_SCHEDULER_ISR();
#endif
            if (!ret) {
                _active->service();
            } else {
                jmpHandler(ret);
            }
#else
            _active->service();
#endif

#ifdef _PROCESS_TIMEOUT_INTERRUPTS
            DISABLE_SCHEDULER_ISR();
#endif

#ifdef _PROCESS_STATISTICS
            uint32_t runTime = getCurrTS() - start;
            // Make sure no overflow happens
            if (_active->statsWillOverflow(1, runTime))
                handleHistOverFlow(HISTORY_DIV_FACTOR);

            _active->setHistIterations(_active->getHistIterations()+1);
            _active->setHistRuntime(_active->getHistRunTime()+runTime);

#endif
            // Is it time to disable?
            if (_active->wasServiced(force)) {
                disable(*_active);
            }

            count++; // incr counter
        }
        //////////////////////END PROCESS SERVICING//////////////////////
        delay(0); // For esp8266

        // Determine what to do next ///
        if (!_active->hasNext()) {
#ifdef _PROCESS_REORDERING
            if(++_pLevels[pLevel].passes >= _PROCESS_REORDERING_AGGRESSIVENESS) {
                reOrderProcs((ProcPriority)pLevel);
                ++_pLevels[pLevel].passes = 0;
            }
#endif
            _pLevels[pLevel].next = _pLevels[pLevel].head; // Set next to first
        } else {
            _pLevels[pLevel].next = _active->getNext(); // Set next and break
            _active = NULL;
            break;
        }

        _active = NULL;
    }
    processQueue();
    return count;
}


/************ PROTECTED ***************/
void Scheduler::procDisable(Process &process)
{
    if (process.isEnabled() && isNotDestroyed(process)) {
        process.onDisable();
        process.setDisabled();
    }
}


void Scheduler::procEnable(Process &process)
{
    if (!process.isEnabled() && isNotDestroyed(process)) {
        process.resetTimeStamps();
        process.onEnable();
        process.setEnabled();
    }
}


void Scheduler::procDestroy(Process &process)
{
    if (isNotDestroyed(process)) {
        procDisable(process);
        process.cleanup();
        removeNode(process);
        process.setID(0);
    }
}


void Scheduler::procRestart(Process &process)
{
    if (isNotDestroyed(process)) {
        procDisable(process);
        process.cleanup();
    }
    process.resetTimeStamps();
    process.setup();
    procEnable(process);
}


void Scheduler::procAdd(Process &process)
{
    if (!isNotDestroyed(process)) {
        for (; process.getID() == 0 || findProcById(process.getID()) != NULL; process.setID(++_lastID)); // Find a free id
        process.resetTimeStamps();
        process.setup();
        procEnable(process);
        appendNode(process);

#ifdef _PROCESS_STATISTICS
        process.setHistRuntime(0);
        process.setHistIterations(0);
        process.setHistLoadPercent(0);
#endif
    }
}

void Scheduler::procHalt()
{

    for (uint8_t i = 0; i < NUM_PRIORITY_LEVELS; i++)
    {
        for (Process *curr = _pLevels[i].head; curr != NULL; curr = curr->getNext())
        {
            procDestroy(*curr);
        }
    }

    delay(100);

    HALT_PROCESSOR();
}


/* Queue object */
// This is so ugly, stupid namespace crap
Scheduler::QueableOperation::QueableOperation() : _process(NULL), _operation(static_cast<uint8_t>(NONE)) {}

Scheduler::QueableOperation::QueableOperation(Scheduler::QueableOperation::QueableOperation::OperationType op)
        : _process(NULL), _operation(static_cast<uint8_t>(op)) {}

Scheduler::QueableOperation::QueableOperation(Process *serv, Scheduler::QueableOperation::QueableOperation::OperationType op)
    : _process(serv), _operation(static_cast<uint8_t>(op)) {}

Process *Scheduler::QueableOperation::getProcess()
{
    return _process;
}

Scheduler::QueableOperation::QueableOperation::OperationType Scheduler::QueableOperation::getOperation()
{
    return static_cast<Scheduler::QueableOperation::QueableOperation::OperationType>(_operation);
}

bool Scheduler::QueableOperation::queue(RingBuf *queue)
{
    return queue->add(queue, this) >= 0;
}

/* end Queue object garbage */


//Only call when there is guarantee this is not running in another call frame
void Scheduler::processQueue()
{
    while(!_queue->isEmpty(_queue)) // Empty Queue
    {
        QueableOperation op;
        _queue->pull(_queue, &op);
        switch (op.getOperation())
        {
            case QueableOperation::ENABLE_SERVICE:
                procEnable(*op.getProcess());
                break;

            case QueableOperation::DISABLE_SERVICE:
                procDisable(*op.getProcess());
                break;

            case QueableOperation::ADD_SERVICE:
                procAdd(*op.getProcess());
                break;

            case QueableOperation::DESTROY_SERVICE:
                procDestroy(*op.getProcess());
                break;

            case QueableOperation::RESTART_SERVICE:
                procRestart(*op.getProcess());
                break;

            case QueableOperation::HALT:
                procHalt();
                break;

#ifdef _PROCESS_STATISTICS
            case QueableOperation::UPDATE_STATS:
                procUpdateStats();
                break;
#endif

            default:
                break;
        }
    }
}

#ifdef _PROCESS_STATISTICS
bool Scheduler::updateStats()
{
    QueableOperation op(QueableOperation::UPDATE_STATS);
    return op.queue(_queue);
}


void Scheduler::procUpdateStats()
{

    uint8_t count = countProcesses(ALL_PRIORITY_LEVELS, false);
    hTimeCount_t sTime[count];

    // Thread safe in case of interrupts
    hTimeCount_t totalTime = 0;
    Process *p;
    uint8_t i;

    for (uint8_t l = 0; l < NUM_PRIORITY_LEVELS; l++)
    {
        for (i = 0, p = _pLevels[l].head; p != NULL && i < count; p = p->getNext(), i++)
        {
            // to ensure no overflows
            sTime[i] = (p->getHistRunTime() + count/2) / count;
            totalTime += sTime[i];
        }
    }

    for (uint8_t l = 0; l < NUM_PRIORITY_LEVELS; l++)
    {
        for (i = 0, p = _pLevels[l].head; p != NULL && i < count; p = p->getNext(), i++)
        {
            // to ensure no overflows have to use double
            if (!totalTime) {
                p->setHistLoadPercent(0);
            } else {
                double tmp = 100*((double)sTime[i]/(double)totalTime);
                p->setHistLoadPercent((uint8_t)tmp);
            }
        }
    }

    return;
}

// Make sure it is locked
void Scheduler::handleHistOverFlow(uint8_t div)
{
    for (uint8_t i = 0; i < NUM_PRIORITY_LEVELS; i++)
    {
        for (Process *p = _pLevels[i].head; p != NULL; p = p->getNext())
        {
            p->divStats(div);
        }
    }

}

#endif


#ifdef _PROCESS_EXCEPTION_HANDLING
    void Scheduler::raiseException(int e)
    {
        longjmp(_env, e);
    }


    bool Scheduler::jmpHandler(int e)
    {
        if (e != 0 && _active)
        {
            switch(e)
            {
#ifdef _PROCESS_TIMEOUT_INTERRUPTS
                case LONGJMP_ISR_CODE:
                    _active->handleWarning(ERROR_PROC_TIMED_OUT);
                    break;
#endif
                case LONGJMP_YIELD_CODE:
                    //no nothing
                    break;

                default:
                    if (!_active->handleException(e))
                        handleException(*_active, e);
                    break;

            }
            return true;
        }
        return false;
    }
#endif



bool Scheduler::appendNode(Process &node)
{
    node.setNext(NULL);

    ProcPriority p = node.getPriority();

    if (!_pLevels[p].head) { // adding to head
        _pLevels[p].head = &node;
        _pLevels[p].next = &node;
    } else {
        Process *next = _pLevels[p].head; // adding not to head
        for(; next->hasNext(); next = next->getNext()); //run through list
        // Update pointers
        next->setNext(&node);
    }
    return true;
}

bool Scheduler::removeNode(Process &node)
{
    ProcPriority p = node.getPriority();

    if (&node == _pLevels[p].head) { // node is head
        _pLevels[p].head = node.getNext();
    } else {
        // Find the previous node
        Process *prev = findPrevNode(node);

        if (!prev)
            return false; // previous node does not exist

        prev->setNext(node.getNext());
    }

    if (_pLevels[p].next == &node)
        _pLevels[p].next = node.hasNext() ? node.getNext() : _pLevels[p].head;

    return true;
}

bool Scheduler::findNode(Process &node)
{
    for (Process *p = _pLevels[node.getPriority()].head; p != NULL; p = p->getNext())
    {
        if (p == &node)
            return true;
    }
    return false;
}

Process *Scheduler::findPrevNode(Process &node)
{
    Process *prev = _pLevels[node.getPriority()].head;
    for (; prev != NULL && prev->getNext() != &node; prev = prev->getNext());
    return prev;
}

/*
void Scheduler::reOrderProcs(ProcPriority level)
{
    for(Process *p = _pLevels[level].head; p != NULL && p->hasNext(); p = p->getNext())
    {
        Process *next = p->getNext();

        if (p->getAvgRunTime() > next->getAvgRunTime())
        {
            Serial.println("Swapping");
            if(!swapNode(*p, *next)) //this should never fail, but to be safe
                break;

            p = next;
        }
    }
}

bool Scheduler::swapNode(Process &n1, Process &n2)
{
    if (&n1 == &n2 || n1.getPriority() != n2.getPriority())
        return false;


    Process *n1_prev = findPrevNode(n1);
    Process *n2_prev = findPrevNode(n2);

    if (!n1_prev) { //n1 was head
        _pLevels[n1.getPriority()].head = &n2;
    } else { //n1 was not head
        n1_prev->setNext(&n2);
    }

    if (!n2_prev) { //n2 was head
        _pLevels[n1.getPriority()].head = &n1;
    } else { //n2 was not head
        n2_prev->setNext(&n1);
    }

    Process *tmp = n2.getNext();
    n2.setNext(n1.getNext());
    n1.setNext(tmp);
    return true;
}

bool Scheduler::removeNodeAfter(Process &prev)
{
    Process *toRemove = prev.getNext();

    if (!toRemove)
        return false;

    prev.setNext(toRemove->getNext());

    return true;
}

bool Scheduler::insertNodeAfter(Process &prev, Process &toAdd)
{
    Process *next = prev.getNext();

    prev.setNext(&toAdd);
    toAdd.setNext(&prev);

    return true;
}
*/
