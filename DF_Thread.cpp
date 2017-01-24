#include "DF_Thread.h"

DFThread::DFThread()
{
	threadTid=0;
	threadState = THREAD_STATE_IDLE;
	pmutex = NULL;
}



DFThread::~DFThread()
{
	if (!pmutex){
		/* Thread not started */
		return;
	}

	while (!Stopped()){
		/* Thread still running */
		Stop();
		Sleep(50);
	}

	delete pmutex;
}



bool DFThread::Start()
{
	if (!pmutex){
		pmutex = new(std::nothrow) DFMutex;
		if (!pmutex)
			return false;
	}

	if (!Stopped()){
		/* Thread still running */
		return false;
	}

	/* Create new thread */
	DF_CreateThread(threadTid, 0, threadProxy, this, E_DETACH_THREAD);
	
	pmutex->Lock();
	threadState = THREAD_STATE_RUNNING;
	pmutex->Unlock();

	return true;
}


void DFThread::Stop()
{

}



bool DFThread::Stopped()
{
	if (!pmutex){
		/* Thread not start */
		return true;
	}
		
	pmutex->Lock();
	if (THREAD_STATE_RUNNING == threadState){
		pmutex->Unlock();
		return false;
	}
	else{
		pmutex->Unlock();
		return true;
	}
}





DF_THREAD_RETYPE DFThread::threadProxy(void*sp)
{
	printf("::::::: threadProxy start ::::::\n");

	DFThread* pthread=static_cast<DFThread*>(sp);
	
	pthread->threadLoop();

	pthread->threadState = THREAD_STATE_IDLE;

	return DF_THREAD_DEFAULT_RETURN;
}


