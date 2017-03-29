// ------------------ TaskScheduler implementation --------------------

#include "TaskScheduler.h"

/** Constructor, uses default values for the parameters
 * so could be called with no parameters.
 */
Task::Task( unsigned long aInterval, long aIterations, void (*aCallback)(), Scheduler* aScheduler, bool aEnable, bool (*aOnEnable)(), void (*aOnDisable)() ) {
	reset();
	set(aInterval, aIterations, aCallback, aOnEnable, aOnDisable);
	if (aScheduler) aScheduler->addTask(*this);
#ifdef _TASK_STATUS_REQUEST
	iStatusRequest = NULL;
#endif  // _TASK_STATUS_REQUEST
#ifdef _TASK_WDT_IDS
	iTaskID = ++__task_id_counter;
#endif  // _TASK_WDT_IDS
	if (aEnable) enable();
}


#ifdef _TASK_STATUS_REQUEST

/** Constructor with reduced parameter list for tasks created for 
 *  StatusRequest only triggering (always immediate and only 1 iteration)
 */
Task::Task( void (*aCallback)(), Scheduler* aScheduler, bool (*aOnEnable)(), void (*aOnDisable)() ) {
	reset();
	set(TASK_IMMEDIATE, TASK_ONCE, aCallback, aOnEnable, aOnDisable);
	if (aScheduler) aScheduler->addTask(*this);
	iStatusRequest = NULL;
#ifdef _TASK_WDT_IDS
	iTaskID = ++__task_id_counter;
#endif  // _TASK_WDT_IDS
}

/** Signals completion of the StatusRequest by one of the participating events
 *  @param: aStatus - if provided, sets the return code of the StatusRequest: negative = error, 0 (default) = OK, positive = OK with a specific status code
 *  Negative status will complete Status Request fully (since an error occured).
 *  @return: true, if StatusRequest is complete, false otherwise (still waiting for other events)
 */
bool StatusRequest::signal(int aStatus) {
	if ( iCount) {	// do not update the status request if it was already completed
		if (iCount > 0)  --iCount; 
		if ( (iStatus = aStatus) < 0 ) iCount = 0;   // if an error is reported, the status is requested to be completed immediately
	}
	return (iCount == 0); 
}

void StatusRequest::signalComplete(int aStatus) {
	if (iCount) { // do not update the status request if it was already completed
		iCount = 0; 
		iStatus = aStatus;
	}
}

/** Sets a Task to wait until a particular event completes 
 *  @param: aStatusRequest - a pointer for the StatusRequest to wait for.
 *  If aStatusRequest is NULL, request for waiting is ignored, and the waiting task is not enabled. 
 */
void Task::waitFor(StatusRequest* aStatusRequest, unsigned long aInterval, long aIterations) {
	if ( ( iStatusRequest = aStatusRequest) ) { // assign internal StatusRequest var and check if it is not NULL
		setIterations(aIterations);
		setInterval(aInterval); 
		iStatus.waiting = _TASK_SR_NODELAY;  // no delay
		enable();
	}
}

void Task::waitForDelayed(StatusRequest* aStatusRequest, unsigned long aInterval, long aIterations) {
	if ( ( iStatusRequest = aStatusRequest) ) { // assign internal StatusRequest var and check if it is not NULL
		setIterations(aIterations);
		if ( aInterval ) setInterval(aInterval);  // For the dealyed version only set the interval if it was not a zero
		iStatus.waiting = _TASK_SR_DELAY;  // with delay equal to the current interval
		enable();
	}
}
#endif  // _TASK_STATUS_REQUEST

/** Resets (initializes) the task/
 * Task is not enabled and is taken out 
 * out of the execution chain as a result
 */
void Task::reset() {
	iStatus.enabled = false;
	iStatus.inonenable = false;
	iPreviousMillis = 0;
	iInterval = iDelay = 0;
	iPrev = NULL;
	iNext = NULL;
	iScheduler = NULL;
	iRunCounter = 0;
#ifdef _TASK_TIMECRITICAL
	iOverrun = 0;
	iStartDelay = 0;
#endif  // _TASK_TIMECRITICAL
#ifdef _TASK_WDT_IDS
	iControlPoint = 0;
#endif  // _TASK_WDT_IDS
#ifdef _TASK_LTS_POINTER
	iLTS = NULL;
#endif  // _TASK_LTS_POINTER
#ifdef _TASK_STATUS_REQUEST
	iStatus.waiting = 0;
#endif  // _TASK_STATUS_REQUEST
}

/** Explicitly set Task execution parameters
 * @param aInterval - execution interval in ms
 * @param aIterations - number of iterations, use -1 for no limit
 * @param aCallback - pointer to the callback method which executes the task actions
 * @param aOnEnable - pointer to the callback method which is called on enable()
 * @param aOnDisable - pointer to the callback method which is called on disable()
 */
void Task::set(unsigned long aInterval, long aIterations, void (*aCallback)(),bool (*aOnEnable)(), void (*aOnDisable)()) {
	setInterval(aInterval); 
	iSetIterations = iIterations = aIterations;
	iCallback = aCallback;
	iOnEnable = aOnEnable;
	iOnDisable = aOnDisable;
}

/** Sets number of iterations for the task
 * if task is enabled, schedule for immediate execution
 * @param aIterations - number of iterations, use -1 for no limit
 */
void Task::setIterations(long aIterations) { 
	iSetIterations = iIterations = aIterations; 
}

/** Enables the task 
 *  schedules it for execution as soon as possible,
 *  and resets the RunCounter back to zero
 */
void Task::enable() {
	if (iScheduler) { // activation without active scheduler does not make sense
		iRunCounter = 0;
		if ( iOnEnable && !iStatus.inonenable ) {
			Task *current = iScheduler->iCurrent;
			iScheduler->iCurrent = this;
			iStatus.inonenable = true;		// Protection against potential infinite loop
			iStatus.enabled = (*iOnEnable)();
			iStatus.inonenable = false;	  	// Protection against potential infinite loop
			iScheduler->iCurrent = current;
		}
		else {
			iStatus.enabled = true;
		}
		iPreviousMillis = _TASK_TIME_FUNCTION() - (iDelay = iInterval);
	}
}

/** Enables the task only if it was not enabled already
 * Returns previous state (true if was already enabled, false if was not)
 */
bool Task::enableIfNot() {
	bool previousEnabled = iStatus.enabled;
	if ( !previousEnabled ) enable();
	return (previousEnabled);
}

/** Enables the task 
 * and schedules it for execution after a delay = aInterval
 */
void Task::enableDelayed(unsigned long aDelay) {
	enable();
	delay(aDelay);
}

/** Delays Task for execution after a delay = aInterval (if task is enabled).
 * leaves task enabled or disabled
 * if aDelay is zero, delays for the original scheduling interval from now
 */
void Task::delay(unsigned long aDelay) {
//	if (!aDelay) aDelay = iInterval;
	iDelay = aDelay ? aDelay : iInterval;
	iPreviousMillis = _TASK_TIME_FUNCTION(); // - iInterval + aDelay;
}

/** Schedules next iteration of Task for execution immediately (if enabled)
 * leaves task enabled or disabled
 * Task's original schedule is shifted, and all subsequent iterations will continue from this point in time
 */
void Task::forceNextIteration() {
	iPreviousMillis = _TASK_TIME_FUNCTION() - (iDelay = iInterval);
}

/** Sets the execution interval.
 * Task execution is delayed for aInterval
 * Use  enable() to schedule execution ASAP
 * @param aInterval - new execution interval
 */
void Task::setInterval (unsigned long aInterval) {
 	iInterval = aInterval; 
	delay(); // iDelay will be updated by the delay() function
}

/** Disables task
 * Task will no longer be executed by the scheduler
 * Returns status of the task before disable was called (i.e., if the task was already disabled)
 */
bool Task::disable() {
	bool previousEnabled = iStatus.enabled;
	iStatus.enabled = false;
	iStatus.inonenable = false; 
	if (previousEnabled && iOnDisable) {
		Task *current = iScheduler->iCurrent;
		iScheduler->iCurrent = this;
		(*iOnDisable)();
		iScheduler->iCurrent = current;
	}
	return (previousEnabled);
}

/** Restarts task
 * Task will run number of iterations again
 */
void Task::restart() {
	 iIterations = iSetIterations;
	 enable();
}

/** Restarts task delayed
 * Task will run number of iterations again
 */
void Task::restartDelayed(unsigned long aDelay) {
	 iIterations = iSetIterations;
	 enableDelayed(aDelay);
}

// ------------------ Scheduler implementation --------------------

/** Default constructor.
 * Creates a scheduler with an empty execution chain.
 */
Scheduler::Scheduler() {
	init();
}

/** Initializes all internal varaibles
 */
void Scheduler::init() { 
	iFirst = NULL; 
	iLast = NULL; 
	iCurrent = NULL; 
#ifdef _TASK_PRIORITY
	iHighPriority = NULL;
#endif  // _TASK_PRIORITY
#ifdef _TASK_SLEEP_ON_IDLE_RUN
	allowSleep(true);
#endif  // _TASK_SLEEP_ON_IDLE_RUN
}

/** Appends task aTask to the tail of the execution chain.
 * @param &aTask - reference to the Task to be appended.
 * @note Task can only be part of the chain once.
 */
 void Scheduler::addTask(Task& aTask) {

	aTask.iScheduler = this;
// First task situation: 
	if (iFirst == NULL) {
		iFirst = &aTask;
		aTask.iPrev = NULL;
	}
	else {
// This task gets linked back to the previous last one
		aTask.iPrev = iLast;
		iLast->iNext = &aTask;
	}
// "Previous" last task gets linked to this one - as this one becomes the last one
	aTask.iNext = NULL;
	iLast = &aTask;
}

/** Deletes specific Task from the execution chain
 * @param &aTask - reference to the task to be deleted from the chain
 */
void Scheduler::deleteTask(Task& aTask) {
	if (aTask.iPrev == NULL) {
		if (aTask.iNext == NULL) {
			iFirst = NULL;
			iLast = NULL;
			return;
		}
		else {
			aTask.iNext->iPrev = NULL;
			iFirst = aTask.iNext;
			aTask.iNext = NULL;
			return;
		}
	}

	if (aTask.iNext == NULL) {
		aTask.iPrev->iNext = NULL;
		iLast = aTask.iPrev;
		aTask.iPrev = NULL;
		return;
	}

	aTask.iPrev->iNext = aTask.iNext;
	aTask.iNext->iPrev = aTask.iPrev;
	aTask.iPrev = NULL;
	aTask.iNext = NULL;
}

/** Disables all tasks in the execution chain
 * Convenient for error situations, when the only
 * task remaining active is an error processing task
 * @param aRecursive - if true, tasks of the higher priority chains are disabled as well recursively
 */
void Scheduler::disableAll(bool aRecursive) {
	Task	*current = iFirst;
	while (current) {
		current->disable();
		current = current->iNext;
	}
#ifdef _TASK_PRIORITY
	if (aRecursive && iHighPriority) iHighPriority->disableAll(true);
#endif  // _TASK_PRIORITY
}


/** Enables all the tasks in the execution chain
 * @param aRecursive - if true, tasks of the higher priority chains are enabled as well recursively
 */
 void Scheduler::enableAll(bool aRecursive) {
	Task	*current = iFirst;
	while (current) {
		current->enable();
		current = current->iNext;
	}
#ifdef _TASK_PRIORITY
	if (aRecursive && iHighPriority) iHighPriority->enableAll(true);
#endif  // _TASK_PRIORITY
}

/** Sets scheduler for the higher priority tasks (support for layered task priority)
 * @param aScheduler - pointer to a scheduler for the higher priority tasks
 */
#ifdef _TASK_PRIORITY
void Scheduler::setHighPriorityScheduler(Scheduler* aScheduler) {
	if (aScheduler != this) iHighPriority = aScheduler;  // Setting yourself as a higher priority one will create infinite recursive call
#ifdef _TASK_SLEEP_ON_IDLE_RUN
	if (iHighPriority) {
		iHighPriority->allowSleep(false);		// Higher priority schedulers should not do power management
	}
#endif  // _TASK_SLEEP_ON_IDLE_RUN
};
#endif  // _TASK_PRIORITY


#ifdef _TASK_SLEEP_ON_IDLE_RUN
void Scheduler::allowSleep(bool aState) { 
	iAllowSleep = aState; 

#ifdef ARDUINO_ARCH_ESP8266
	wifi_set_sleep_type( iAllowSleep ? LIGHT_SLEEP_T : NONE_SLEEP_T );
#endif  // ARDUINO_ARCH_ESP8266

}
#endif  // _TASK_SLEEP_ON_IDLE_RUN


void Scheduler::startNow( bool aRecursive ) {
	unsigned long t = _TASK_TIME_FUNCTION();
	
	iCurrent = iFirst;
	while (iCurrent) {
		if ( iCurrent->iStatus.enabled ) iCurrent->iPreviousMillis = t - iCurrent->iDelay;
		iCurrent = iCurrent->iNext;
	}
	
#ifdef _TASK_PRIORITY
	if (aRecursive && iHighPriority) iHighPriority->startNow( true );
#endif  // _TASK_PRIORITY
}

/** Makes one pass through the execution chain.
 * Tasks are executed in the order they were added to the chain
 * There is no concept of priority
 * Different pseudo "priority" could be achieved
 * by running task more frequently 
 */
bool Scheduler::execute() {
	bool	 idleRun = true;
	register unsigned long m, i;  // millis, interval;

#ifdef ARDUINO_ARCH_ESP8266
	  unsigned long t1 = micros();
	  unsigned long t2 = 0;
#endif  // ARDUINO_ARCH_ESP8266

	iCurrent = iFirst;
	
	while (iCurrent) {
		
#ifdef _TASK_PRIORITY
	// If scheduler for higher priority tasks is set, it's entire chain is executed on every pass of the base scheduler
		if (iHighPriority) idleRun = iHighPriority->execute() && idleRun; 
		iCurrentScheduler = this;
#endif  // _TASK_PRIORITY

		do {
			if ( iCurrent->iStatus.enabled ) {

#ifdef _TASK_WDT_IDS
	// For each task the control points are initialized to avoid confusion because of carry-over:
				iCurrent->iControlPoint = 0;
#endif  // _TASK_WDT_IDS
	
	// Disable task on last iteration:
				if (iCurrent->iIterations == 0) {
					iCurrent->disable();
					break;
				}
				m = _TASK_TIME_FUNCTION();
				i = iCurrent->iInterval;

#ifdef  _TASK_STATUS_REQUEST
	// If StatusRequest object was provided, and still pending, and task is waiting, this task should not run
	// Otherwise, continue with execution as usual.  Tasks waiting to StatusRequest need to be rescheduled according to 
	// how they were placed into waiting state (waitFor or waitForDelayed)
				if ( iCurrent->iStatus.waiting ) {
					if ( (iCurrent->iStatusRequest)->pending() ) break;
					if (iCurrent->iStatus.waiting == _TASK_SR_NODELAY) {
						iCurrent->iPreviousMillis = m - (iCurrent->iDelay = i);
					}
					else {
						iCurrent->iPreviousMillis = m;
					}
					iCurrent->iStatus.waiting = 0;
				}
#endif  // _TASK_STATUS_REQUEST

				if ( m - iCurrent->iPreviousMillis < iCurrent->iDelay ) break;

				if ( iCurrent->iIterations > 0 ) iCurrent->iIterations--;  // do not decrement (-1) being a signal of never-ending task
				iCurrent->iRunCounter++;
				iCurrent->iPreviousMillis += iCurrent->iDelay;

#ifdef _TASK_TIMECRITICAL
	// Updated_previous+current interval should put us into the future, so iOverrun should be positive or zero. 
	// If negative - the task is behind (next execution time is already in the past) 
				unsigned long p = iCurrent->iPreviousMillis;
				iCurrent->iOverrun = (long) ( p + i - m );
				iCurrent->iStartDelay = (long) ( m - p ); 
#endif  // _TASK_TIMECRITICAL

				iCurrent->iDelay = i;
				if ( iCurrent->iCallback ) {
					( *(iCurrent->iCallback) )();
					idleRun = false;
				}
			}
		} while (0); 	//guaranteed single run - allows use of "break" to exit 
		iCurrent = iCurrent->iNext;
	}

#ifdef _TASK_SLEEP_ON_IDLE_RUN
  	if (idleRun && iAllowSleep) {

#ifdef ARDUINO_ARCH_AVR	// Could be used only for AVR-based boards. 
  	  set_sleep_mode(SLEEP_MODE_IDLE);
  	  sleep_enable();
	  /* Now enter sleep mode. */
	  sleep_mode();
	  
	  /* The program will continue from here after the timer timeout ~1 ms */
	  sleep_disable(); /* First thing to do is disable sleep. */
#endif // ARDUINO_ARCH_AVR

#ifdef ARDUINO_ARCH_ESP8266
// to do: find suitable sleep function for esp8266
	  t2 = micros() - t1;
	  if (t2 < _TASK_ESP8266_DLY_THRESHOLD) delay(1); 	// ESP8266 implementation of delay() uses timers and yield
#endif  // ARDUINO_ARCH_ESP8266
	}
#endif  // _TASK_SLEEP_ON_IDLE_RUN

	return (idleRun);
}