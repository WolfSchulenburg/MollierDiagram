#include "stdafx.h"
#include "CrashHandler.h"


CCrashHandler::CCrashHandler()
{
}


CCrashHandler::~CCrashHandler()
{
}

void CCrashHandler::SetProcessExceptionHandlers()
{
	// Install top-level SEH handler
	SetUnhandledExceptionFilter(SehHandler);

	// Catch pure virtual function calls.
	// Because there is one _purecall_handler for the whole process, 
	// calling this function immediately impacts all threads. The last 
	// caller on any thread sets the handler. 
	// http://msdn.microsoft.com/en-us/library/t296ys27.aspx
	_set_purecall_handler(PureCallHandler);

	// Catch new operator memory allocation exceptions
	_set_new_handler(NewHandler);

	// Catch invalid parameter exceptions.
	_set_invalid_parameter_handler(InvalidParameterHandler);

	// Set up C++ signal handlers

	_set_abort_behavior(_CALL_REPORTFAULT, _CALL_REPORTFAULT);

	// Catch an abnormal program termination
	signal(SIGABRT, SigabrtHandler);

	// Catch illegal instruction handler
	signal(SIGINT, SigintHandler);

	// Catch a termination request
	signal(SIGTERM, SigtermHandler);
}

void CCrashHandler::SetThreadExceptionHandlers()
{

	// Catch terminate() calls. 
	// In a multithreaded environment, terminate functions are maintained 
	// separately for each thread. Each new thread needs to install its own 
	// terminate function. Thus, each thread is in charge of its own termination handling.
	// http://msdn.microsoft.com/en-us/library/t6fk7h29.aspx
	set_terminate(TerminateHandler);

	// Catch unexpected() calls.
	// In a multithreaded environment, unexpected functions are maintained 
	// separately for each thread. Each new thread needs to install its own 
	// unexpected function. Thus, each thread is in charge of its own unexpected handling.
	// http://msdn.microsoft.com/en-us/library/h46t5b69.aspx  
	set_unexpected(UnexpectedHandler);

	// Catch a floating point error
	typedef void(*sigh)(int);
	signal(SIGFPE, (sigh)SigfpeHandler);

	// Catch an illegal instruction
	signal(SIGILL, SigillHandler);

	// Catch illegal storage access errors
	signal(SIGSEGV, SigsegvHandler);

}

// This method creates minidump of the process
void CCrashHandler::CreateMiniDump(EXCEPTION_POINTERS* pExcPtrs)
{
	HMODULE hDbgHelp = NULL;
	HANDLE hFile = NULL;
	MINIDUMP_EXCEPTION_INFORMATION mei;
	MINIDUMP_CALLBACK_INFORMATION mci;

	// Load dbghelp.dll
	hDbgHelp = LoadLibrary(_T("dbghelp.dll"));
	if (hDbgHelp == NULL)
	{
		// Error - couldn't load dbghelp.dll
		return;
	}

	// Create the minidump file
	hFile = CreateFile(
		_T("crashdump.dmp"),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		// Couldn't create file
		return;
	}

	// Write minidump to the file
	mei.ThreadId = GetCurrentThreadId();
	mei.ExceptionPointers = pExcPtrs;
	mei.ClientPointers = FALSE;
	mci.CallbackRoutine = NULL;
	mci.CallbackParam = NULL;

	typedef BOOL(WINAPI *LPMINIDUMPWRITEDUMP)(
		HANDLE hProcess,
		DWORD ProcessId,
		HANDLE hFile,
		MINIDUMP_TYPE DumpType,
		CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
		CONST PMINIDUMP_USER_STREAM_INFORMATION UserEncoderParam,
		CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

	LPMINIDUMPWRITEDUMP pfnMiniDumpWriteDump =
		(LPMINIDUMPWRITEDUMP)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
	if (!pfnMiniDumpWriteDump)
	{
		// Bad MiniDumpWriteDump function
		return;
	}

	HANDLE hProcess = GetCurrentProcess();
	DWORD dwProcessId = GetCurrentProcessId();

	BOOL bWriteDump = pfnMiniDumpWriteDump(
		hProcess,
		dwProcessId,
		hFile,
		MiniDumpNormal,
		&mei,
		NULL,
		&mci);

	if (!bWriteDump)
	{
		// Error writing dump.
		return;
	}

	// Close file
	CloseHandle(hFile);

	// Unload dbghelp.dll
	FreeLibrary(hDbgHelp);
}
