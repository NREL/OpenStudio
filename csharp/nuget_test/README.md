Simple command line steps to test the openstudio.nupkg package with a simple C# program. 

1 ) Install nuget.exe 
     https://www.nuget.org/downloads

2 ) Install the nupkg using nuget cli
    e.g.
    nuget.exe add C:\Users\OpenStudio.3.2.0.nupkg -Source C:\Users\Administrator\source\
    # verify package is installed 
    nuget.exe list -Prerelease -Source C:\Users\Administrator\source\

3 ) cd into directory where you want to install the nuget package and install
    nuget.exe install OpenStudio -Version 3.2.0 -Source C:\Users\Administrator\source\

4 ) compile c# program using nuget package
    csc /t:exe /out:test.exe  /r:OpenStudio.3.2.0\lib\net35\OpenStudio.dll   test_nuget.cs

5 ) copy all runtime dlls within the nuget package to directory or add it path that contains the compiled exe, in this case test.exe

6 ) run exec and test output


The nuget_setup.bat runs all the above for x64 and x86. You need to set the following before calling the script. 
e.g.
set NUGET_PATH=D:\OSN\build&& set NUGET_NAME=OpenStudio&&  set NUGET_VERSION=3.2.2-alpha&& set NUGET_SOURCE_DIR=D:\jenkins\nuget_packages&& test_nuget.bat




