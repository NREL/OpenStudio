> [Wiki](Home) ▸ **Suggested Visual Studio 2013 Configuration**

This document recommends resources and modifications that may improve your experience or increase the utility of using Visual Studio 2013 with OpenStudio.  Note that extensions are only supported by Visual Studio Professional or higher (they won't work with the Express Edition).

## Strongly Recommended:

#### Debugger Visualizers
Copy the contents of the [Visualizers](https://github.com/NREL/OpenStudio/tree/develop/developer/msvc/Visualizers) directory to `C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Packages\Debugger\Visualizers`.  This will allow you to easily inspect and debug Boost and Qt objects within Visual Studio.

#### Add Syntax Highlighting to .i Files
In `TOOLS -> Options -> Text Editor -> File Extension`, add the `i` extension with the `Microsoft Visual C++` editor.

#### Configure C++ Formatting and Style
The following settings file will automatically configure Visual Studio's C++ formatting to match OpenStudio's Coding Standards.  Go to `TOOLS -> Import and Export Settings -> Import selected environment settings` and select the following settings file: 
[cpp-settings.vssettings](/NREL/OpenStudio/wiki/files/Suggested-Visual-Studio-2013-Configuration/cpp-settings.vssettings)

## Optional:

#### Close File Shortcut
In `TOOLS -> Options -> Environment -> Keyboard`, search for the `File.Close` command and add the `Ctrl+W` shortcut in the `Text Editor` context.

#### Kill Build Shortcut
The purpose behind this tweak is to instantly stop the build when you want it stopped, without hitting Cancel and waiting for long-running processes to finish.

In `TOOLS -> External Tools`, add the following tool:

| FIELD              | VALUE                            |
|--------------------|----------------------------------|
| Title:             | &amp;Kill Build                  |
| Command:           | C:\Windows\System32\taskkill.exe |
| Arguments:         | /F /IM MSBuild.exe /T            |
| Initial directory: |                                  |

Then move it up in the list to the first position.  Next, open `TOOLS -> Options -> Environment -> Keyboard`, search for the `Tools.ExternalCommand1` command and add the `Ctrl+Shift+X` global shortcut.

#### Extensions *(minimum: Professional)*

##### [CMake Tools for Visual Studio](http://visualstudiogallery.msdn.microsoft.com/6d1586a9-1c98-4ac7-b54f-7615d5f9fbc7)
This adds syntax highlighting and IntelliSense support for CMake files.  Enable `TOOLS -> Options -> CMake Tools -> Commands In Lowercase`.

##### [Doxygen Comments](http://visualstudiogallery.msdn.microsoft.com/11a30c1c-593b-4399-a702-f23a56dd8548)
This adds syntax highlighting to Doxygen documentation.

##### [GoogleTest Runner](http://visualstudiogallery.msdn.microsoft.com/9dd47c21-97a6-4369-b326-c562678066f0)
This allows you to view, run, and track test results from all OpenStudio tests directly in Visual Studio.  Be sure to open `Test -> Windows -> Test Explorer`.

##### [Productivity Power Tools 2013](http://visualstudiogallery.msdn.microsoft.com/dbcb8670-889e-4a54-a226-a48a15e4cace)
This extension adds a large number of useful features, including highlighting project errors in the solution explorer, highlighting all occurrences of the selected text, filtering the solution explorer, scrollbar markers to help find related code, middle-click scrolling, fixing mixed tabs and spaces, aligning assignments, and much more.

##### [Trailing Whitespace Visualizer](https://marketplace.visualstudio.com/items?itemName=MadsKristensen.TrailingWhitespaceVisualizer)
This extension will highlight trailing whitespace in files and remove it on save.  This is the preferred formatting for source code.

##### [Visual Studio Spell Checker](http://visualstudiogallery.msdn.microsoft.com/a23de100-31a1-405c-b4b7-d6be40c3dfff)
This will highlight potential spelling mistakes in code strings and comments.
Import this user dictionary to avoid false positives in OpenStudio code: [OSDictionary.dic](/NREL/OpenStudio/wiki/files/Suggested-Visual-Studio-2013-Configuration/OSDictionary.dic)

`TOOLS -> Spell Checker -> Edit Configuration -> User Dictionary -> Import`

##### [VSCommands for Visual Studio 2013](http://visualstudiogallery.msdn.microsoft.com/c6d1c265-7007-405c-a68b-5606af238ece)
This extension also adds a large number of useful features, including coloring output window text color, showing a build summary, changing the window title to reflect the current Git branch, keeping documents open when reloading projects, canceling the build when the first project fails, and more.