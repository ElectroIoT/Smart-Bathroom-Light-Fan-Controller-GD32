@echo off
rem ---------------------------------------------------------------------------
rem  Smart Bathroom Light and Fan Controller - one-click launcher (Windows)
rem
rem  Double-click for a menu, or run with a name:
rem     BathroomController.bat dashboard   live test dashboard (http://localhost:8765)
rem     BathroomController.bat test        PC logic tests
rem     BathroomController.bat build       build the firmware
rem     BathroomController.bat flash       build + flash the board (ST-Link)
rem     BathroomController.bat status      live status in text
rem     BathroomController.bat setup       first-time setup (compiler + OpenOCD)
rem ---------------------------------------------------------------------------
setlocal
cd /d "%~dp0"
title Bathroom Controller tools

where python >nul 2>nul
if errorlevel 1 (
  echo Python 3 was not found. Install it from https://www.python.org/downloads/
  echo and tick "Add python.exe to PATH" during the setup, then run this file again.
  pause
  exit /b 1
)

if not "%~1"=="" goto %~1

:menu
cls
echo ==============================================================
echo   Smart Bathroom Light and Fan Controller  (GD32F103C8T6)
echo ==============================================================
echo.
echo   1. Start the LIVE TEST DASHBOARD  (opens http://localhost:8765)
echo   2. Run the PC logic tests
echo   3. Build the firmware
echo   4. Flash the board  (ST-Link)
echo   5. Live status in text
echo   6. First-time setup  (installs the compiler and OpenOCD)
echo   0. Exit
echo.
set "choice="
set /p choice=Choose a number:
if "%choice%"=="1" goto dashboard
if "%choice%"=="2" goto test
if "%choice%"=="3" goto build
if "%choice%"=="4" goto flash
if "%choice%"=="5" goto status
if "%choice%"=="6" goto setup
if "%choice%"=="0" exit /b 0
goto menu

:dashboard
if not exist build\firmware.elf (
  echo build\firmware.elf not found: building the firmware first...
  python tools\build.py firmware || goto nocompiler
)
echo.
echo Starting the live dashboard. Keep this window open. Press Ctrl+C to stop it.
echo (Stop it before flashing: only one program can use the ST-Link at a time.)
echo.
python tools\dashboard.py
goto end

:test
python tools\build.py test
if errorlevel 1 goto nocompiler
goto end

:build
python tools\build.py firmware
if errorlevel 1 goto nocompiler
goto end

:flash
python tools\build.py firmware
if errorlevel 1 goto nocompiler
python tools\flash.py
goto end

:status
python tools\live_status.py 40 500
goto end

:setup
echo This downloads and installs: the compiler (pip package ziglang) and OpenOCD (winget).
set "ok="
set /p ok=Continue? (y/n):
if /i not "%ok%"=="y" goto end
python -m pip install --upgrade ziglang
winget install -e --id xpack-dev-tools.openocd-xpack --accept-package-agreements --accept-source-agreements
echo.
echo Setup finished. Close this window and open it again so the new tools are found.
goto end

:nocompiler
echo.
echo The build or tests failed. If the message above says a compiler is missing,
echo run the first-time setup (option 6) once, then try again.
goto end

:end
echo.
if "%~1"=="" (
  pause
  goto menu
)
exit /b 0
