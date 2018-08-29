cd ..
git add *
setlocal
set /p msg="Message: "
git commit -m "%msg%"
endlocal
git push -u origin
pause