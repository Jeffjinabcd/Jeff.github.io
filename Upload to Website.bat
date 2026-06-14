@echo off
title Upload to Website
echo ============================================================
echo   UPLOAD TO WEBSITE
echo   Copies your files, rebuilds the library, and pushes it
echo   to GitHub so the site updates.
echo ============================================================
echo.
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0sync.ps1"
echo.
echo ------------------------------------------------------------
echo   Done. The site will update in a minute or two.
echo   You can close this window.
echo ------------------------------------------------------------
pause
