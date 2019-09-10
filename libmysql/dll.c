/* Copyright (C) 2000 MySQL AB & MySQL Finland AB & TCX DataKonsult AB
   
   This software is distributed with NO WARRANTY OF ANY KIND.  No author or
   distributor accepts any responsibility for the consequences of using it, or
   for whether it serves any particular purpose or works at all, unless he or
   she says so in writing.  Refer to the LICENSE file for details.
   
   Every copy of this file must include a copy of the License, normally in a
   plain ASCII text file named LICENSE.  The License grants you the right to
   copy, modify and redistribute this file, but only under certain conditions
   described in the License.  Among other things, the License requires that
   the copyright notice and this notice be preserved on all copies */

/*
** Handling initialization of the dll library
*/

#include <global.h>
#include <my_sys.h>
#include <my_pthread.h>

static bool libmysql_inited=0;

void libmysql_init(void)
{
  if (libmysql_inited)
    return;
  libmysql_inited=1;
  my_init();
  {
    DBUG_ENTER("libmysql_init");
#ifdef LOG_ALL
    DBUG_PUSH("d:t:S:O,c::\\tmp\\libmysql.log");
#else
    if (getenv("LIBMYSQL_LOG") != NULL)
      DBUG_PUSH(getenv("LIBMYSQL_LOG"));
#endif
    DBUG_VOID_RETURN;
  }
}

#ifdef __WIN__

static int inited=0,threads=0;
HINSTANCE NEAR s_hModule;	/* Saved module handle */
DWORD main_thread;

BOOL APIENTRY LibMain(HANDLE hInst,DWORD ul_reason_being_called,
		      LPVOID lpReserved)
{
  switch (ul_reason_being_called) {
  case DLL_PROCESS_ATTACH:	/* case of libentry call in win 3.x */
    if (!inited++)
    {
      s_hModule=hInst;
      libmysql_init();
      main_thread=GetCurrentThreadId();
    }
    break;
  case DLL_THREAD_ATTACH:
    threads++;
    my_thread_init();
    break;
  case DLL_PROCESS_DETACH:	/* case of wep call in win 3.x */
     if (!--inited)		/* Safety */
     {
       /* my_thread_init() */	/* This may give extra safety */
       my_end(0);
     }
    break;
  case DLL_THREAD_DETACH:
    /* Main thread will free by my_end() */
    threads--;
    if (main_thread != GetCurrentThreadId())
      my_thread_end();
    break;
  default:
    break;
  } /* switch */

  return TRUE;

  UNREFERENCED_PARAMETER(lpReserved);
} /* LibMain */

int __stdcall DllMain(HANDLE hInst,DWORD ul_reason_being_called,LPVOID lpReserved)
{
  return LibMain(hInst,ul_reason_being_called,lpReserved);
}

#elif defined(WINDOWS)

/****************************************************************************
**	This routine is called by LIBSTART.ASM at module load time.  All it
**	does in this sample is remember the DLL module handle.	The module
**	handle is needed if you want to do things like load stuff from the
**	resource file (for instance string resources).
****************************************************************************/

int _export FAR PASCAL libmain(HANDLE hModule,short cbHeapSize,
			       UCHAR FAR *lszCmdLine)
{
  s_hModule = hModule;
  libmysql_init();
  return TRUE;
}

#endif

#ifdef OS2

//
// This function is called automatically by _DLL_InitTerm
// Every dll runtime enviroment is not tz enabled, so tzset()
// must be called to enable TZ handling
// Also timezone is fixed.
//
extern "C" unsigned long _System DllMain(unsigned long modhandle,
                                        unsigned long flag)
{
   if (flag == 0) {
      tzset();			// Set tzname
      time_t currentTime = time(NULL);
      struct tm *ts = localtime(&currentTime);
      if (ts->tm_isdst > 0)
         _timezone -= 3600;
   }
}

#endif
