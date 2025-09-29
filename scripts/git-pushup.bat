@echo off
cd /d "%~dp0\.."

if "%~1"=="" (
    set "msg=自动更新: %date% %time%"
) else (
    set "msg=%~1"
)

git add .
git commit -m "%msg%"
git push origin main