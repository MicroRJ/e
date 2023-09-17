@IF "%INCLUDE%"==""  (
   @CALL vcvars64
)

@PUSHD rx
@CALL build.msvc.bat
@POPD

@SETLOCAL
@SET myName=emain
@SET myAlias=emain
@SET myCompilerOptions=/options:strict /nologo /TC /Od /Z7 /WX /W4
@SET myDefines= /D_DEBUG /D_HARD_DEBUG /D_DEVELOPER
@SET myLinkerOptions=/DEBUG /SUBSYSTEM:CONSOLE /INCREMENTAL:NO
@SET myInclude=/I.. /I../rx /I ../rx/stb /I../freetype/include
@SET myLibs=/LIBPATH:../freetype
@SET myTarget=../src/%myName%.c
@SET mySummary=%myCompilerOptions% %myDefines% %myInclude% %myTarget%
@CALL taskkill /F /IM %myName%.exe
@PUSHD build
@CALL cl %mySummary% /Fe%myAlias%.exe /link %myLinkerOptions% %myLibs%
@POPD

@ENDLOCAL
