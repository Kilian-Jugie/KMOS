@echo off
compilekernel.bat
if %errorlevel% == 0 (
	qemu-system-i386 -cdrom bin/myos.iso
)