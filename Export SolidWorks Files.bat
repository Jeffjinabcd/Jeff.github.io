@echo off
title Export SolidWorks Files
echo ============================================================
echo   EXPORT SOLIDWORKS FILES
echo   Generates STL + STEP previews for every part/assembly,
echo   into STL\ and STEP\ folders beside your .sldprt files.
echo   Your SolidWorks files are NOT modified.
echo.
echo   IMPORTANT: Close SolidWorks first. This launches a
echo   hidden SolidWorks instance and will skip if SW is open.
echo ============================================================
echo.
pause
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0sw-export.ps1"
echo.
echo ------------------------------------------------------------
echo   Done. Now run "Upload to Website" to publish the new
echo   STL/STEP files to the site.
echo ------------------------------------------------------------
pause
