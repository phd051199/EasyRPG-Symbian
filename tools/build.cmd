@echo off
setlocal EnableExtensions DisableDelayedExpansion

set "BUILD_PROFILE=%~1"
if not defined BUILD_PROFILE set "BUILD_PROFILE=build"
if /I "%BUILD_PROFILE%"=="build" goto setup
if /I "%BUILD_PROFILE%"=="debug" goto setup
if /I "%BUILD_PROFILE%"=="clean" goto setup
echo Usage: build.cmd [build^|debug^|clean] >&2
exit /b 2

:setup
pushd "%~dp0.." >nul 2>&1
if errorlevel 1 goto project_error
set "PROJECT_DIR=%CD%"
popd

set "SDK_ROOT=%EASYRPG_SYMBIAN_SDK_ROOT%"
if not defined SDK_ROOT set "SDK_ROOT=C:\S60\devices\S60_3rd_FP2_SDK_v1.1"
if "%SDK_ROOT:~-1%"=="\" set "SDK_ROOT=%SDK_ROOT:~0,-1%"
if not exist "%SDK_ROOT%\epoc32" goto sdk_error

set "EPOCROOT=%SDK_ROOT:~2%\"
set "PATH=%SDK_ROOT%\epoc32\gcc\bin;%SDK_ROOT%\epoc32\tools;C:\Program Files\CSL Arm Toolchain\bin;C:\Program Files\CSL Arm Toolchain\arm-none-symbianelf\bin;C:\Program Files\CSL Arm Toolchain\libexec\gcc\arm-none-symbianelf\3.4.3;C:\Perl\bin;C:\Program Files\Common Files\Symbian\tools;%PATH%"
set "GROUP_DIR=%PROJECT_DIR%\group"
set "SIS_DIR=%PROJECT_DIR%\sis"
set "PACKAGE_DEF=%SIS_DIR%\EasyRPG.pkg"
set "STAGE_DIR=%SIS_DIR%\stage"
set "DEVICE_EXE=EasyRPG.exe"
set "RUNTIME_EXE=EasyRPGPlayer.exe"
set "ICON_SOURCE=%PROJECT_DIR%\resources\EasyRPG.svg"
set "ICON_TARGET=%SDK_ROOT%\epoc32\data\z\resource\apps\EasyRPG.mif"
set "ICON_TEMP=%PROJECT_DIR%\build\mif-temp"

if /I "%BUILD_PROFILE%"=="debug" (
    set "EASYRPG_BUILD_TARGET=udeb"
    set "SIS_OUTPUT=%SIS_DIR%\EasyRPG_debug.sis"
) else (
    set "EASYRPG_BUILD_TARGET=urel"
    set "SIS_OUTPUT=%SIS_DIR%\EasyRPG.sis"
)

where bldmake >nul 2>&1
if errorlevel 1 goto bldmake_error
where makesis >nul 2>&1
if errorlevel 1 goto makesis_error
where arm-none-symbianelf-g++.exe >nul 2>&1
if errorlevel 1 goto gcce_error

pushd "%GROUP_DIR%" >nul 2>&1
if errorlevel 1 goto group_error
call bldmake bldfiles
set "RESULT=%ERRORLEVEL%"
popd
if not "%RESULT%"=="0" goto build_error

if /I "%BUILD_PROFILE%"=="clean" goto clean

where mifconv >nul 2>&1
if errorlevel 1 goto mifconv_error
if not exist "%ICON_SOURCE%" goto icon_source_error
if not exist "%ICON_TEMP%" mkdir "%ICON_TEMP%"
if errorlevel 1 goto icon_build_error
if exist "%ICON_TARGET%" del /q "%ICON_TARGET%" >nul 2>&1
mifconv "%ICON_TARGET%" /T%ICON_TEMP% /S%SDK_ROOT%\epoc32\tools /c32 "%ICON_SOURCE%"
if errorlevel 1 goto icon_build_error
if not exist "%ICON_TARGET%" goto icon_output_error

echo Project: %PROJECT_DIR%
echo SDK: %SDK_ROOT%
echo Target: GCCE %EASYRPG_BUILD_TARGET%

set "GCCE_EXE=%SDK_ROOT%\epoc32\release\gcce\%EASYRPG_BUILD_TARGET%\%DEVICE_EXE%"
set "GCCE_RUNTIME_EXE=%SDK_ROOT%\epoc32\release\gcce\%EASYRPG_BUILD_TARGET%\%RUNTIME_EXE%"
if exist "%GCCE_EXE%" del /q "%GCCE_EXE%" >nul 2>&1
if exist "%GCCE_RUNTIME_EXE%" del /q "%GCCE_RUNTIME_EXE%" >nul 2>&1
if exist "%GCCE_EXE%" goto stale_output_error
if exist "%GCCE_RUNTIME_EXE%" goto stale_output_error
if exist "%SIS_OUTPUT%" del /q "%SIS_OUTPUT%" >nul 2>&1

pushd "%GROUP_DIR%" >nul 2>&1
if errorlevel 1 goto group_error
call abld -v build gcce %EASYRPG_BUILD_TARGET%
set "RESULT=%ERRORLEVEL%"
echo ABLD returned %RESULT%
popd
if not "%RESULT%"=="0" goto build_error

if not exist "%GCCE_EXE%" goto exe_error
if not exist "%GCCE_RUNTIME_EXE%" goto exe_error
if not exist "%SDK_ROOT%\epoc32\data\z\resource\apps\EasyRPG.rsc" goto resource_error
if not exist "%ICON_TARGET%" goto icon_output_error
if not exist "%SDK_ROOT%\epoc32\data\z\private\10003a3f\apps\EasyRPG_reg.rsc" goto registration_error
if not exist "%PACKAGE_DEF%" goto package_error

if exist "%STAGE_DIR%" rmdir /s /q "%STAGE_DIR%"
mkdir "%STAGE_DIR%"
if errorlevel 1 goto stage_error
copy /b /y "%GCCE_EXE%" "%STAGE_DIR%\%DEVICE_EXE%" >nul
if errorlevel 1 goto stage_error
copy /b /y "%GCCE_RUNTIME_EXE%" "%STAGE_DIR%\%RUNTIME_EXE%" >nul
if errorlevel 1 goto stage_error
copy /b /y "%SDK_ROOT%\epoc32\data\z\resource\apps\EasyRPG.rsc" "%STAGE_DIR%\EasyRPG.rsc" >nul
if errorlevel 1 goto stage_error
copy /b /y "%ICON_TARGET%" "%STAGE_DIR%\EasyRPG.mif" >nul
if errorlevel 1 goto stage_error
copy /b /y "%SDK_ROOT%\epoc32\data\z\private\10003a3f\apps\EasyRPG_reg.rsc" "%STAGE_DIR%\EasyRPG_reg.rsc" >nul
if errorlevel 1 goto stage_error

pushd "%SIS_DIR%" >nul 2>&1
if errorlevel 1 goto sis_dir_error
makesis "EasyRPG.pkg" "%SIS_OUTPUT%"
set "RESULT=%ERRORLEVEL%"
popd
if not "%RESULT%"=="0" goto makesis_build_error
if not exist "%SIS_OUTPUT%" goto makesis_output_error
if exist "%STAGE_DIR%" rmdir /s /q "%STAGE_DIR%"
if exist "%GROUP_DIR%\ABLD.BAT" del /q "%GROUP_DIR%\ABLD.BAT" >nul 2>&1

for %%F in ("%SIS_OUTPUT%") do echo SIS: %%~fF ^(%%~zF bytes^)
echo Build completed.
exit /b 0

:clean
pushd "%GROUP_DIR%" >nul 2>&1
if errorlevel 1 goto group_error
call abld clean gcce urel
set "RESULT=%ERRORLEVEL%"
if "%RESULT%"=="0" call abld clean gcce udeb
if "%RESULT%"=="0" set "RESULT=%ERRORLEVEL%"
popd
if not "%RESULT%"=="0" goto build_error
del /q "%SIS_DIR%\EasyRPG*.sis" >nul 2>&1
if exist "%STAGE_DIR%" rmdir /s /q "%STAGE_DIR%"
if exist "%ICON_TARGET%" del /q "%ICON_TARGET%" >nul 2>&1
echo Clean completed.
exit /b 0

:project_error
echo Cannot resolve project directory. >&2
exit /b 1
:sdk_error
echo Invalid S60 SDK: %SDK_ROOT% >&2
exit /b 1
:bldmake_error
echo bldmake not found. >&2
exit /b 1
:makesis_error
echo makesis not found. >&2
exit /b 1
:gcce_error
echo GCCE compiler not found. >&2
exit /b 1
:mifconv_error
echo mifconv not found. >&2
exit /b 1
:icon_source_error
echo Application icon source missing: %ICON_SOURCE% >&2
exit /b 1
:icon_build_error
echo mifconv failed while building the application icon. >&2
exit /b 1
:icon_output_error
echo Application icon was not produced: %ICON_TARGET% >&2
exit /b 1
:group_error
echo Cannot open group directory. >&2
exit /b 1
:build_error
echo Build failed. >&2
exit /b 1
:stale_output_error
echo Cannot remove stale GCCE executable before build: %GCCE_EXE% >&2
exit /b 1
:exe_error
echo GCCE executable was not produced by this build. >&2
exit /b 1
:resource_error
echo Application resource was not produced. >&2
exit /b 1
:registration_error
echo Registration resource was not produced. >&2
exit /b 1
:package_error
echo Package definition missing. >&2
exit /b 1
:stage_error
echo Cannot stage GCCE executable/resources for makesis. >&2
exit /b 1
:sis_dir_error
echo Cannot open SIS directory. >&2
exit /b 1
:makesis_build_error
echo makesis failed. >&2
exit /b 1
:makesis_output_error
echo makesis did not produce a SIS. >&2
exit /b 1
