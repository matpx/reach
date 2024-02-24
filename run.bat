@echo off

meson compile -C build || goto :error
.\build\reach.exe || goto :error

:error
exit /b %errorlevel%
