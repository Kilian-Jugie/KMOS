@echo off
bin\bash.exe scripts/linkbootfile
if %errorlevel% neq 0 (
	echo ERROR: cannot link bootfile, stopping
	exit 1
)
bin\bash.exe scripts/compilekernel
if %errorlevel% neq 0 (
	echo ERROR during compilation, stopping
	exit 1
)

bin\bash.exe scripts/createiso
if %errorlevel% neq 0 (
	echo ERROR during file creation, stopping
	exit 1
)
exit 0