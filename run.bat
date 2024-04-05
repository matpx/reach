@echo off

meson compile -C build reach || goto :error
.\build\reach.exe || goto :error

:error
exit /b %errorlevel%
