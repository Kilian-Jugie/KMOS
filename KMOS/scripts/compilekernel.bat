@echo off
bin\bash.exe scripts/linkbootfile
bin\bash.exe scripts/compilekernel
if %errorlevel% == 0 (
	bin\bash.exe scripts/createiso
)