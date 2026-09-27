@echo off
title Model Organizer
echo ============================================================
echo   MODEL ORGANIZER (private - runs only on your computer)
echo   A browser tab will open where you can preview models,
echo   make named groups, and sort them. Click Save when done,
echo   then run "Upload to Website" to publish your arrangement.
echo.
echo   Leave this window open while you sort. Close it when done.
echo ============================================================
python "%~dp0tools\curate_server.py"
