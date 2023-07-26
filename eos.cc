
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