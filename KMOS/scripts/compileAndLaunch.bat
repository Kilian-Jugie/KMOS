@echo off
bin\bash.exe scripts/linkbootfile
bin\bash.exe scripts/compilekernel
if %errorlevel% == 0 (
echo Compilation successfull ! Starting creating iso file
	bin\bash.exe scripts/createiso
	qemu-system-i386 -cdrom bin/myos.iso
)