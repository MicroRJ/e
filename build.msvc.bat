@IF "%INCLUDE%"=="" (
	@CALL vcvars64
)

@PUSHD rx
@CALL build.msvc.bat
@POPD

@CALL build.msvc.template.bat e
