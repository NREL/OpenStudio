OpenStudio is supported on Windows 7 - 10 (32-bit & 64-bit), OS X 10.9 - 10.10 (64-bit), and Ubuntu 14.04 (64-bit).

OpenStudio 1.9.0 works with [EnergyPlus 8.3.0](https://github.com/NREL/EnergyPlus/releases/tag/v8.3.0), which is now bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately.

#### Download and install SketchUp

1. The OpenStudio SketchUp Plug-in requires [SketchUp 2015](http://www.sketchup.com/) (not available for Linux). Older versions of SketchUp are not supported. SketchUp 2015 is available in 32 and 64-bit versions; the 32-bit version of OpenStudio on Windows will only work with the 32-bit version of SketchUp 2015 and the 64-bit version of OpenStudio will only work with the 64-bit version of SketchUp 2015.

#### Download and install OpenStudio

1. Login to the [OpenStudio website](https://www.openstudio.net/). Create an account if you don't have one.
2. Click *Downloads* at the top of the page.
3. Choose the installer that matches your operating system. The OpenStudio package contains the following tools:
  - SketchUp Plug-in
  - OpenStudio Application
  - ParametricAnalysisTool (PAT)
  - ResultsViewer

#### Optional - Create a Building Component Library (BCL) Account
BCL content can be accessed from within the OpenStudio SketchUp Plug-in and from the standalone OpenStudio application. To take advantage of this integration, you will need to follow the steps outlined here to request a BCL key.

1. Login to the [Building Component Library (BCL)](https://bcl.nrel.gov/). Create an account if you don't have one. Follow the instructions from the email sent to you upon registration and then login.
2. Click on *My Dashboard* near the top right of the website.
3. Copy the text after *API v1.1 key*: then paste the key into the SketchUp Plug-in, the OpenStudio application, or the Parametric Analysis Tool (PAT). You only need to configure the key in one place and it will be used globally throughout the OpenStudio tools. Additionally, this key will be maintained when you install updates to OpenStudio.

Enter the key through the OpenStudio SketchUp Plug-in under the menu `Plugins->OpenStudio User Scripts->Building Component Library->Setup BCL Key` and click *OK*. Or register the key through the OpenStudio Application under the menu `Components & Measures->Find Components`. If a key is not already registered, it will prompt you for one.

#### Radiance
OpenStudio 1.9.0 supports Radiance 5.0.a.5, which is bundled with the OpenStudio installer. It is no longer necessary to download and install Radiance separately.

#### Optional - Install Ruby
If you plan to use the OpenStudio SDK Ruby bindings via command prompt on Windows, you must install Ruby. OS X already has Ruby installed.

1. Download the [Ruby 2.0.0](http://rubyinstaller.org/downloads/) installer.
2. Add `C:\Ruby200\bin` to the *PATH* environment variable. [Detailed instructions](http://geekswithblogs.net/renso/archive/2009/10/21/how-to-set-the-windows-path-in-windows-7.aspx).
3. Create a text file called with the following text inside:

    ```
    require 'C:\Program Files (x86)\OpenStudio 1.9.0\Ruby\openstudio.rb'
    ```

4. Save the file as `openstudio.rb` here: `C:\Ruby200\lib\ruby\site_ruby\openstudio.rb` (next to the 2.0.0 folder).
