@echo off

REM set -x
REM ragel -T1 cmdline.rl
wcc -bt=dos -zq -oxhs -dVERSION_MAJOR=0 -dVERSION_MINOR=14 tnd232.c
wcc -bt=dos -zq -oxhs -dVERSION_MAJOR=0 -dVERSION_MINOR=14 emmhack.c
wcc -bt=dos -zq -oxhs -dVERSION_MAJOR=0 -dVERSION_MINOR=14 cmdline.c
wcc -bt=dos -zq -oxhs -dVERSION_MAJOR=0 -dVERSION_MINOR=14 tndinit.c
wcc -bt=dos -zq -oxhs -dVERSION_MAJOR=0 -dVERSION_MINOR=14 tndout.c
rem wcc -bt=dos -zq -oxhs -dVERSION_MAJOR=0 -dVERSION_MINOR=14 tnddebug.c
wcc -bt=dos -zq -oxhs -dVERSION_MAJOR=0 -dVERSION_MINOR=14 res_data.c
wasm -zq -dVERSION_MAJOR=0 -dVERSION_MINOR=14 resident.s
wasm -zq -dVERSION_MAJOR=0 -dVERSION_MINOR=14 res_end.s
wlink @tnd232.wl
