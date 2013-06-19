call "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat"
nmake confclean
configure -debug-and-release -opensource -shared -stl -qt-sql-sqlite -qt-sql-odbc -accessibility -nomake examples -nomake demos -platform win32-msvc2008 -mp
nmake

:: remove all '-Zc:wchar_t-' in mkspecs/win32-msvc*/qmake.conf
:: remove all '-GL' in mkspecs/win32-msvc*/qmake.conf