##Visual Studio 2013 Debugger Visualizers

Copy the contents of the Visualizers directory to `C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Packages\Debugger\Visualizers`

######To debug visualizer entries
Add the following registry setting:
[HKEY_CURRENT_USER\Software\Microsoft\VisualStudio\12.0_Config\Debugger]
"EnableNatvisDiagnostics"=dword:00000001  
