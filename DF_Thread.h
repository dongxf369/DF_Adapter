#ifndef DF_THREAD_H
#define DF_THREAD_H



#include "DF_Adapter.h"

class DFThread
{
private:
	/* Thread's tid */
	DF_THREAD_ID threadTid;
	/*Thread state*/
	enum State{				
		THREAD_STATE_IDLE=0,
		THREAD_STATE_RUNNING=1,
	};
	State threadState;

public:
	DFThread();
	virtual ~DFThread();
	virtual void threadLoop()=0;
	virtual bool Start();
	virtual void Stop();
	virtual bool Stopped();
	static DF_THREAD_RETYPE threadProxy(void*sp);

public:
	DFMutex*pmutex;

};


#endif