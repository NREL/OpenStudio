REM Setup the Visual Studio env
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

echo Testing %NUGET_NAME%.%NUGET_VERSION% with example csharp programs for x86 and x64 builds

SET ORIG_PATH=%PATH%


echo Adding nuget package to local source
echo nuget add %NUGET_PATH%\%NUGET_NAME%.%NUGET_VERSION%.nupkg -Source %NUGET_SOURCE_DIR%
nuget add %NUGET_PATH%\%NUGET_NAME%.%NUGET_VERSION%.nupkg -Source %NUGET_SOURCE_DIR%


echo List available nuget packages from local source
echo nuget list -Prerelease -Source %NUGET_SOURCE_DIR%
nuget list -Prerelease -Source %NUGET_SOURCE_DIR%

echo Install nuget packages from local source 
nuget install %NUGET_NAME% -Version %NUGET_VERSION% -Source %NUGET_SOURCE_DIR%

REM create the dirs to write the compiled output
mkdir x64
mkdir x86

echo Compiling x64 and x86 csharp programs using %NUGET_NAME%.%NUGET_VERSION%.nupkg
REM compile x64 csharp example file
echo csc /t:exe /platform:x64 /out:x64\test.exe /r:%NUGET_NAME%.%NUGET_VERSION%\lib\net35\OpenStudio.dll test_nuget.cs
csc /t:exe /platform:x64 /out:x64\test.exe /r:%NUGET_NAME%.%NUGET_VERSION%\lib\net35\OpenStudio.dll test_nuget.cs

REM compile x86 csharp example file
echo csc /t:exe /platform:x86 /out:x86\test.exe /r:%NUGET_NAME%.%NUGET_VERSION%\lib\net35\OpenStudio.dll test_nuget.cs
csc /t:exe /platform:x86 /out:x86\test.exe /r:%NUGET_NAME%.%NUGET_VERSION%\lib\net35\OpenStudio.dll test_nuget.cs

REM get the test directory
SET BASE_DIR_NAME=%~d0%~p0

REM add x64 dlls to path
REM echo set PATH=%BASE_DIR_NAME%%NUGET_NAME%.%NUGET_VERSION%\build\x64;%PATH%
set PATH=%BASE_DIR_NAME%%NUGET_NAME%.%NUGET_VERSION%\build\x64;%PATH%
REM Copy the dll so test.exe for runtime dependency
echo copy %NUGET_NAME%.%NUGET_VERSION%\lib\net35\OpenStudio.dll x64
copy %NUGET_NAME%.%NUGET_VERSION%\lib\net35\OpenStudio.dll x64
REM run the test.exe
echo "test running x64\test.exe"
echo x64\test.exe
x64\test.exe

echo "test running x86\test.exe"
REM add x86 dlls to path
set PATH=%ORIG_PATH%
REM add x86 dlls to path
REM echo set PATH=%BASE_DIR_NAME%%NUGET_NAME%.%NUGET_VERSION%\build\x86;%PATH%
set PATH=%BASE_DIR_NAME%%NUGET_NAME%.%NUGET_VERSION%\build\x86;%PATH%
REM Copy the dll so test.exe for runtime dependency
echo copy %NUGET_NAME%.%NUGET_VERSION%\lib\net35\OpenStudio.dll x86
copy %NUGET_NAME%.%NUGET_VERSION%\lib\net35\OpenStudio.dll x86
REM run the test.exe
echo "test running x64\test.exe"
echo x86\test.exe
x86\test.exe


echo Deleting nuget package to local source
echo nuget delete %NUGET_NAME% %NUGET_VERSION% -Source %NUGET_SOURCE_DIR% -NonInteractive
nuget delete %NUGET_NAME% %NUGET_VERSION% -Source %NUGET_SOURCE_DIR% -NonInteractive
