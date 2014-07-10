##Visual Studio Debugger Visualizers

#####Visual Studio 2013

Copy the contents of the Visualizers directory to `C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Packages\Debugger\Visualizers`

######To debug visualizer entries
Add the following registry setting:
[HKEY_CURRENT_USER\Software\Microsoft\VisualStudio\12.0_Config\Debugger]
"EnableNatvisDiagnostics"=dword:00000001  

#####Visual Studio 2010

Open `C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\Packages\Debugger\autoexp.dat` with administrator rights

Copy the contents of AutoExpand.txt and paste it directy beneath `[AutoExpand]` in your autoexp.dat file, and then copy the contents of Visualizer.txt and paste it directly beneath `[Visualizer]`.
