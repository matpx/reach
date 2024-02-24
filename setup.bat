@echo off

meson setup build --wipe || goto :error

:error
exit /b %errorlevel%
