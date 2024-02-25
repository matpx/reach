@echo off

meson setup build --reconfigure || goto :error

:error
exit /b %errorlevel%
