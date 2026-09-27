@echo off
title Build 3D Previews
echo ============================================================
echo   BUILD 3D PREVIEWS
echo   Bakes fast-loading GLB previews for every CAD model so
echo   they open in about a second on the website - even big STEP
echo   files. First run takes a few minutes; after that only new
echo   or changed files are processed.
echo ============================================================
echo.
python "%~dp0tools\build_previews.py"
echo.
echo ------------------------------------------------------------
echo   Done. Now run "Upload to Website" to publish the previews.
echo ------------------------------------------------------------
pause
