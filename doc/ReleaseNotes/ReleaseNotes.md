# OpenStudio 2.0.0 Overview

OpenStudio 2.0.0 is an aggressive overhaul of OpenStudio to make the SDK smaller and more accessible for third party software developers. The new package includes two major new components: a command line interface (CLI) and a new version of the Parametric Analysis Tool (PAT).  A new component based installer gives users the choice to install only the components that they need. OpenStudio 2.0.0 is a developer release, we encourage users to try it out and let us know about any issues.  However, because external users have not yet heavily tested OpenStudio 2.0.0 it is not yet recommended for production work.  OpenStudio 1.14.0 remains the current stable release.  In March of 2017, OpenStudio 2.1.0 release will replace OpenStudio 1.14.0 as the stable release.

The OpenStudio command line interface (CLI) allows software developers to execute a series of OpenStudio Measures and other simulation steps as described in an OpenStudio Workflow (OSW) file. The OSW defines a set of paths to search for OpenStudio Measures, initial seed model, weather file, and other supporting files. When an OSW is run through the CLI, the initial seed model is loaded and OpenStudio Model Measures are applied to the seed model.  After these measures, the OpenStudio Model is translated to EnergyPlus IDF format.  Once in EnergyPlus IDF format, OpenStudio EnergyPlus Measures are applied.  After these measures, the EnergyPlus simulation is executed.  Once the EnergyPlus simulation is complete, OpenStudio Reporting Measures are applied which generate reports.  Once the workflow is completed (successfully or unsuccessfully), an output OSW file is written which contains output related to running the workflow. The OpenStudio CLI contains a Ruby interpreter, Ruby standard library, OpenStudio Ruby bindings, and a core set of gems (including the OpenStudio Standards Gem). It does not have any shared library dependencies outside of system runtime libraries. EnergyPlus and Radiance are not embedded within the CLI, they are included as part of the OpenStudio installation. There is an example OSW included in this package in the compact_osw directory under the Examples directory. This example contains everything to run a simple OpenStudio workflow from loading a seed model, applying measures, running EnergyPlus, and running reporting measures. To run this example:

```
cd path/to/Examples/compact_osw
./path/to/openstudio.exe run -w compact.osw
```

More information about the OpenStudio CLI is available at: http://nrel.github.io/OpenStudio-user-documentation/reference/command_line_interface/

A new version of the OpenStudio Parametric Analysis Tool (PAT) is part of OpenStudio 2.0.0 packages. This new version of PAT has been rewritten from the ground up as an example of JavaScript application development with OpenStudio.  It has been packaged as an Electron desktop application and leverages OpenStudio Server for improved robustness, scalability, and capability.  The initial version of PAT in 2.0.0 provides similar functionality (and we hope improved usability) as compared to PAT 1.14.0.  Subsequent releases will add sampling and optimization capability that had previously been available via the “OpenStudio Spreadsheet.”  PAT 2.0.0 is not backwards compatible with PAT 1.14.0 projects. 

The Ruby and C# binding footprints have been greatly reduced in the OpenStudio 2.0.0 package. Previously, deploying the Ruby and C# bindings required deployment of many shared library and file dependencies. In OpenStudio 2.0, the Ruby bindings are contained in a single openstudio.so (.bundle on Mac) file and the C# bindings in an OpenStudio.dll and two supporting dll files. These are the only files required to distribute the OpenStudio 2.0.0 bindings with a third party application. Note that the openstudio.so Ruby file does not contain any gems or the Ruby standard library; these must be configured using typical Gemfile configuration for your Ruby project. Similarly, the C# bindings do not include a Ruby interpreter or OpenStudio Ruby bindings needed to apply OpenStudio Measures. If a C# project desires to run measures or simulations, it can write an OSW file and make a system call to the CLI.

OpenStudio 2.0.0 is mostly backwards compatible with OpenStudio 1.14.0.  One primary difference is that PAT 2.0.0 is not backwards compatible with PAT 1.14.0 projects.  Also, upgrading a 1.14.0 version OSM to 2.0.0 will not preserve the associated run.db, any measures associated with the OSM will have to be re-added in 2.0.0. The API for OpenStudio Model and other core namespaces was mostly preserved during the OpenStudio 2.0.0 conversion. However, the RunManager, Analysis, Project, and AnalysisDriver namespaces were completely removed. The CLI and OSW format replace the functionality of RunManager. The OpenStudio Analysis (OSA) format replaces the functionality of Analysis, Project, and AnalysisDriver. The Ruleset namespace has been renamed to Measure to better reflect its functionality.


## Known Issues

This is a list of known issues, if you find an issue not on this list please let us know so we can fix it.

Installer
* The 64 bit Windows installer incorrectly installs to ’ C:\Program Files (x86)\’ by default.
* Firewall rules are not configured at install time, user is prompted to allow http communication between OpenStudioApp and the CLI as well as between PAT and the CLI.

CLI
* The Ruby Gems library (require 'rubygems') cannot be loaded by the CLI. If you need to require Ruby Gems, you can use a system Ruby 2.0 installation and require the OpenStudio Ruby Bindings as a workaround.

Measures
* Measures written for OpenStudio 1.14.0 and earlier may not work with OpenStudio 2.0.0, measure authors are encouraged to test their measures and update them to ensure compatibility.
* Reporting measures do not have access to offline JavaScript libraries included with the OpenStudio installation.  Measure writers can include offline JavaScript libraries in the resources directory of their measure.

SketchUp Plug-in
* The OpenStudio SketchUp Plug-in does not yet support SketchUp 2017

OpenStudioApp
* The apply measure now dialog does not correctly report advanced output.
* The app may appear to freeze on return from BCL measure dialog on Mac, a workaround is to select File -> Quit and when prompted to save the Model, select cancel to abort the quit operation or press the escape key.
* The user is not prompted to save their model before running.
* Simulation results are deleted when saving an OSM on Mac.
* There is a crash if user turns on output variables then switch away from and back to the output variable tab.

PAT 2.0
* Ability to access or start remote OpenStudio Server instances is not yet available
* Algorithmic sampling and optimization capabilities are not yet available
* Cannot re-run a subset of the design alternatives
* OpenStudio Measures with errors can be added to the project without warning 
* Scatter Plot report does not work
* “Duplicate Measure & Option” button on analysis page not functioning
* “Create One Design Alternative with Each Measure Option” duplicates options that already exist
* Measure option descriptions are not populated with the measure description by default.
* Cannot enter user defined string for Measure Choice arguments
* Project paths cannot contain whitespace or be on network drives
* Columns do not correct size to data
* User is not prompted to replace or rename seed model if one with same name is imported
* Help text is not implemented 
* Simulation information is not properly formatted on the run tab
* User is not prompted with correct Save, Don't Save and Cancel options when closing project
* Display name is not shown  when browsing measures
* Copying a measure without renaming it causes an error
* “New Measure” button is missing from BCL dialog
* Measures associated with seed model are not imported
* Cannot create and run a single design alternative 
* Report name dropdown on run tab is not working correctly


