
REM ********** Set path for .net framework2.0, sandcastle,hhc,hxcomp****************************

set TOOLSPATH=%ProgramFiles%
if exist "%ProgramFiles% (x86)" set TOOLSPATH=%ProgramFiles(x86)%

if not DEFINED DXROOT set DXROOT=%TOOLSPATH%\Sandcastle
set PATH=%windir%\Microsoft.NET\Framework\v2.0.50727;%DXROOT%\ProductionTools;%PATH%

set PATH=%TOOLSPATH%\HTML Help Workshop;%TOOLSPATH%\Microsoft Help 2.0 SDK;%PATH%

if exist output rmdir output /s /q
if exist chm rmdir chm /s /q

REM ********** generate reflection data files for .net framework2.0****************************
::msbuild fxReflection.proj /Property:NetfxVer=2.0 /Property:PresentationStyle=vs2005

REM ********** Compile source files ****************************

if exist ZeroKit.xml copy /y ZeroKit.xml comments.xml

REM ********** Call MRefBuilder ****************************

MRefBuilder ZeroKit.dll /out:reflection.org

REM ********** Apply Transforms ****************************

XslTransform /xsl:"%DXROOT%\ProductionTransforms\ApplyVSDocModel.xsl" reflection.org /xsl:"%DXROOT%\ProductionTransforms\AddFriendlyFilenames.xsl" /out:reflection.xml /arg:IncludeAllMembersTopic=true /arg:IncludeInheritedOverloadTopics=true

XslTransform /xsl:"%DXROOT%\ProductionTransforms\ReflectionToManifest.xsl"  reflection.xml /out:manifest.xml

call "%DXROOT%\Presentation\vs2005\copyOutput.bat"

REM ********** Call BuildAssembler ****************************
BuildAssembler /config:"%DXROOT%\Presentation\vs2005\configuration\sandcastle.config" manifest.xml

REM **************Generate an intermediate Toc file that simulates the Whidbey TOC format.

XslTransform /xsl:"%DXROOT%\ProductionTransforms\createvstoc.xsl" reflection.xml /out:toc.xml 

REM ************ Generate CHM help project ******************************

if not exist chm mkdir chm
if not exist chm\html mkdir chm\html
if not exist chm\icons mkdir chm\icons
if not exist chm\scripts mkdir chm\scripts
if not exist chm\styles mkdir chm\styles
if not exist chm\media mkdir chm\media

xcopy output\icons\* chm\icons\ /y /r
xcopy output\media\* chm\media\ /y /r
xcopy output\scripts\* chm\scripts\ /y /r
xcopy output\styles\* chm\styles\ /y /r

ChmBuilder.exe /project:ZeroKit /html:Output\html /lcid:1033 /toc:Toc.xml /out:Chm

DBCSFix.exe /d:Chm /l:1033 

hhc chm\ZeroKit.hhp