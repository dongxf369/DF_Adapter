/*
file:	DFAdapter.h
brief:	OS API adapter definition
date:	2016-11-08
author:	Dong Xiaofei
*/

#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>


#ifdef _WIN32
#include <io.h>
//#include <winnt.h>
//#include <windows.h>
#include <afx.h>
#include <WINSOCK2.H>
#else
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>					//io.h£¨In linux is <unistd.h>£© 
#include <stdlib.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>				//for TCP_NODELAY   
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
#endif



/****************	Definations about Thread	*****************/

#ifdef _WIN32
#define DFSleepMsec(a) Sleep(a);
#else
#define DFSleepMsec(a) usleep(a*1000);
#endif



/*****************	Definations about Thread	*****************/

#ifdef _WIN32
#define JTHREADID_NULL -1
typedef unsigned int DF_THREAD_ID;
typedef HANDLE	DF_THREAD_HANDLE;
typedef void (*DF_THRDPROC)(void *pvParam);
#else
typedef pthread_t	DF_THREADID;
typedef pthread_t	DF_THREAD_HANDLE;
#define JTHREADID_NULL 0
typedef void* (*DF_THRDPROC)(void *pvParam);
#endif





#define E_THREAD_FLAG	int

#define E_THREAD_FLAG_UNKNOWN	-1
#define E_JOIN_THREAD			1
#define E_DETACH_THREAD			2
#define E_THREAD_FLAG_COUNT		3


//Thread create function
DF_THREAD_HANDLE DF_CreateThread(	DF_THREAD_ID &tid, 						/* Thread' id. */ \
									unsigned int nStackSize, 					/* Set the thread stack's size. */ \
									DF_THRDPROC pFunc, 						/* Set thread's loop func. */ \
									void *pArg, 								/* This para will be send to the new thread. */ \
									E_THREAD_FLAG eFlag 						/* The thread's ctrl flag */ \
									);




/*****************	Mutex class	*****************/

class DFMutex
{
public:
	DFMutex();
	virtual ~DFMutex();
	virtual void Lock();
	virtual void Unlock();
private:
	void * m_pvHandle;
};



/*	File io class */
#ifdef WIN32
#define DF_FILE_HADLE	HANDLE
#else
#define DF_FILE_HADLE	FILE*
#endif

#define DF_FILE_READ	0
#define DF_FILE_WRITE	1

class DF_FileRW
{
public:
	DF_FileRW();
	~DF_FileRW();
	
	bool file_open(const char*file_path,int rw_flag,bool share_flag);
	bool file_close();
	int file_size();
	int read(char*buff,int buff_size);
	int write(char*buff,int buff_size);

private:
	DF_FILE_HADLE phandle;
	int mfile_size;
};