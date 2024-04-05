@echo off

if exist build rmdir /s /q build || goto :error
meson setup build || goto :error

:error
exit /b %errorlevel%
