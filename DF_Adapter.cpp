/*
 *		File:	DFAdapter.cpp
 *		Brief:	OS API adapter definition
 *		Date:	2016-11-08
 *		Author:	Dong Xiaofei
 */

#include <string>
#include "DF_Adapter.h"
#include <process.h>

bool str_erase(std::string &str, std::string ori)
{
	std::string::size_type pos = 0;
	std::string::size_type no_pos = std::string::npos;
	int len = ori.length();

	while (1){
		pos = str.find(ori);
		if (no_pos == pos){
			break;
		}
		str.erase(pos, len);
	}
	return true;
}





DF_THREAD_HANDLE DF_CreateThread(DF_THREAD_ID &tid,			\
									unsigned int nStackSize,	\
									DF_THRDPROC pFunc,			\
									void *pArg,					\
									E_THREAD_FLAG eFlag			\
									)
{
#ifdef _WIN32
	/*
	LPDWORD dwID = 0;
	HANDLE hHandle = CreateThread(NULL, nStackSize, (LPTHREAD_START_ROUTINE)pFunc, pArg, NULL, dwID);
	if (NULL!=hHandle){
		if (E_DETACH_THREAD == eFlag){
			CloseHandle(hHandle);
		}
	}
	return (DF_THREAD_HANDLE)hHandle;
	*/
	_beginthread(pFunc, 0, pArg);
	return 0;

#else
	pthread_t id = 0;
	pthread_attr_t thrdAttr;
	const int nInit = pthread_attr_init(&thrdAttr);
	if (0 != nInit){
		return id;
	}

	int thed_size=THRD_STACK_SIZE;
	if(nStackSize>0){
		thed_size=nStackSize;
	}
	pthread_attr_setstacksize(&thrdAttr, THRD_STACK_SIZE);

	int nRet = pthread_create(&id, &thrdAttr, (void*(*)(void*))pFunc, pArg);

	pthread_attr_destroy(&thrdAttr);

	if (0 == nRet){
		if (E_DETACH_THREAD == eFlag){
			pthread_detach(id);
		}
		return id;
	}
	else{
		return 0;
	}
#endif
}






// DFMutex member function

DFMutex::DFMutex()
{
	m_pvHandle = NULL;
#ifdef WIN32
	m_pvHandle = new CRITICAL_SECTION;
	InitializeCriticalSection((CRITICAL_SECTION *)m_pvHandle);
#else
	m_pvHandle = new pthread_mutex_t;
	pthread_mutex_init((pthread_mutex_t*)m_pvHandle, NULL);
#endif
}


DFMutex::~DFMutex()
{
	if (NULL != m_pvHandle)
	{
#ifdef WIN32
		DeleteCriticalSection((CRITICAL_SECTION *)m_pvHandle);
		delete (CRITICAL_SECTION *)m_pvHandle;
#else
		pthread_mutex_destroy((pthread_mutex_t*)m_pvHandle);
		delete (pthread_mutex_t*)m_pvHandle;
#endif
	}
}


void DFMutex::Lock()
{
#ifdef WIN32
	EnterCriticalSection((CRITICAL_SECTION *)m_pvHandle);
#else
	pthread_mutex_lock((pthread_mutex_t *)m_pvHandle);
#endif
}


void DFMutex::Unlock()
{
#ifdef WIN32
	LeaveCriticalSection((CRITICAL_SECTION *)m_pvHandle);
#else
	pthread_mutex_unlock((pthread_mutex_t *)m_pvHandle);
#endif
}






DF_FileRW::DF_FileRW()
{
	phandle = NULL;
	mfile_size = 0;
}


DF_FileRW::~DF_FileRW()
{
	if (phandle){
		/* close file */
		CloseHandle(phandle);
	}
}


bool DF_FileRW::file_open(const char*file_path, int rw_flag, bool share_flag)
{
	if (!file_path)
		return false;

	int tem_rw_flag=0;
	int tem_share_flag = 0;
	int create_disp = 0;

	if (DF_FILE_READ == rw_flag){
		tem_rw_flag = GENERIC_READ;
		create_disp = OPEN_EXISTING;
		if (true == share_flag)
			tem_share_flag = FILE_SHARE_READ;
		else
			tem_share_flag = 0;
	}
	else if (DF_FILE_WRITE == rw_flag){
		tem_rw_flag = GENERIC_WRITE;
		create_disp = CREATE_ALWAYS;
		if (true == share_flag)
			tem_share_flag = FILE_SHARE_WRITE;
		else
			tem_share_flag = 0;
	}
	else{
		return false;
	}

	phandle = CreateFile(file_path, tem_rw_flag, tem_share_flag, NULL, create_disp, \
						FILE_ATTRIBUTE_NORMAL,NULL);
	if (INVALID_HANDLE_VALUE == phandle){
		phandle = NULL;
		return false;
	}
	return true;
}


bool DF_FileRW::file_close()
{
	if (phandle){
		CloseHandle(phandle);
		phandle = NULL;
	}
	return true;
}


int DF_FileRW::file_size()
{
	if (!phandle){
		return 0;
	}
	return GetFileSize(phandle, NULL);
}


int DF_FileRW::read(char*buff, int buff_size)
{
	if (!phandle){
		return 0;
	}
	DWORD dwBytesRead;
	ReadFile(phandle, buff, buff_size, &dwBytesRead, NULL);
	return dwBytesRead;
}


int DF_FileRW::write(char*buff, int buff_size)
{
	if (!phandle){
		return 0;
	}
	DWORD dwBytesWritten;
	WriteFile(phandle, buff, buff_size, &dwBytesWritten, NULL);
	return dwBytesWritten;
}