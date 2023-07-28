/*
**
**                      -+- e -+-
**
**                      An Editor?
**
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**  Copyright(C) Dayan Rodriguez, 2022, All Rights Reserved
**  *--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
**
**
**            [ TIMENTE MORTEM, OMNIA TIMEO ]
**
**
**                 github.com/MicroRJ/e
**
**
**                 USE AT YOUR OWN RISK
**
*/


HANDLE RunCommandStringEx(const char *commandLine, const char *workingDirectory);

HANDLE RunCommandString(const char *commandLine)
{
	return RunCommandStringEx(commandLine,NULL);
}

HANDLE RunCommandStringEx(const char *commandLine, const char *workingDirectory)
{
  STARTUPINFOA StartupInfo;
  ZeroMemory(&StartupInfo,sizeof(StartupInfo));
  StartupInfo.cb = sizeof(STARTUPINFOA);

  StartupInfo.   dwFlags = 0;
  StartupInfo. hStdInput = NULL;
  StartupInfo.hStdOutput = NULL;
  StartupInfo. hStdError = NULL;

  PROCESS_INFORMATION ProcessInfo;
  ZeroMemory(&ProcessInfo,sizeof(ProcessInfo));

  BOOL Success =
    CreateProcessA(NULL,(LPSTR)commandLine,
      NULL,NULL,TRUE,CREATE_UNICODE_ENVIRONMENT,NULL,(char*)workingDirectory,
        &StartupInfo,&ProcessInfo);

  HANDLE handle = ProcessInfo.hProcess;

  if(Success)
  {
    // *ccarradd(globalProcessArray,1) = ProcessInfo.hProcess;
  } else
  {
    ccdebuglog("failed to create process");
  }
  return handle;
}

# pragma comment(lib,"Comdlg32")
#include   "commdlg.h"

const char *fdlg()
{
  char DirBuff[MAX_PATH];
  GetCurrentDirectory(sizeof(DirBuff),DirBuff);

  OPENFILENAMEA OpenFileName;
  ZeroMemory(&OpenFileName,sizeof(OpenFileName));
  OpenFileName.lStructSize     = sizeof(OpenFileName);
  OpenFileName.hwndOwner       = NULL;
  OpenFileName.lpstrFile       = ccstatic_alloc(MAX_PATH,TRUE);
  OpenFileName.nMaxFile        = MAX_PATH;
  OpenFileName.lpstrFilter     = "All\0*.*\0Text\0*.TXT\0";
  OpenFileName.nFilterIndex    = 1;
  OpenFileName.lpstrFileTitle  = NULL;
  OpenFileName.nMaxFileTitle   = 0;
  OpenFileName.lpstrInitialDir = NULL;
  OpenFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  GetOpenFileNameA(&OpenFileName);

  SetCurrentDirectory(DirBuff);

  return OpenFileName.lpstrFile;
}