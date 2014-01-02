:: install VC Express 2010
:: install windows sdk 7.1a
:: install KB2519277

call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
call "C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" /x64
:: msbuild OpenStudio.sln /m /p:Configuration=Debug
:: msbuild OpenStudio.sln /m /p:Configuration=Release
vcexpress OpenStudio.sln
