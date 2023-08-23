@IF "%INCLUDE%"=="" (
  @CALL vcvars64
)

@SETLOCAL
@SET myName=%1

@CALL taskkill /F /IM %myName%.exe
@SET myCompilerOptions=/options:strict /nologo /TC /Od /Z7 /WX /W4 /D_DEBUG /D_HARD_DEBUG /D_DEVELOPER
@SET myLinkerOptions=/DEBUG /SUBSYSTEM:CONSOLE /INCREMENTAL:NO
@SET myInclude=/I. /Ifreetype/include

@CALL cl %myCompilerOptions% %myInclude% %myName%.cc /Fe%myName%.exe /link %myLinkerOptions%

@IF "%2"=="TRUE" (
	@CALL %myName%.exe
)

@ENDLOCAL