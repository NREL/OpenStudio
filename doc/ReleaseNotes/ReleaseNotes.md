# OpenStudio 2.1.0 Overview

The OpenStudio 2.x line is an aggressive overhaul of OpenStudio to make the SDK smaller and more accessible for third party software developers. The new package includes two major new components: a command line interface (CLI) and a new version of the Parametric Analysis Tool (PAT).  A new component based installer gives users the choice to install only the components that they need. OpenStudio 2.1.0 is the first major release of the OpenStudio 2.x line.

The OpenStudio command line interface (CLI) allows software developers to execute a series of OpenStudio Measures and other simulation steps as described in an OpenStudio Workflow (OSW) file. The OSW defines a set of paths to search for OpenStudio Measures, initial seed model, weather file, and other supporting files. When an OSW is run through the CLI, the initial seed model is loaded and OpenStudio Model Measures are applied to the seed model.  After these measures, the OpenStudio Model is translated to EnergyPlus IDF format.  Once in EnergyPlus IDF format, OpenStudio EnergyPlus Measures are applied.  After these measures, the EnergyPlus simulation is executed.  Once the EnergyPlus simulation is complete, OpenStudio Reporting Measures are applied which generate reports.  Once the workflow is completed (successfully or unsuccessfully), an output OSW file is written which contains output related to running the workflow. The OpenStudio CLI contains a Ruby interpreter, Ruby standard library, OpenStudio Ruby bindings, and a core set of gems (including the OpenStudio Standards Gem). It does not have any shared library dependencies outside of system runtime libraries. EnergyPlus and Radiance are not embedded within the CLI, they are included as part of the OpenStudio installation. There is an example OSW included in this package in the compact_osw directory under the Examples directory. This example contains everything to run a simple OpenStudio workflow from loading a seed model, applying measures, running EnergyPlus, and running reporting measures. To run this example:

```
cd path/to/Examples/compact_osw
./path/to/openstudio.exe run -w compact.osw
```

More information about the OpenStudio CLI is available at: http://nrel.github.io/OpenStudio-user-documentation/reference/command_line_interface/

A new version of the OpenStudio Parametric Analysis Tool (PAT) is part of OpenStudio 2.1.0 packages. This new version of PAT has been rewritten from the ground up as an example of JavaScript application development with OpenStudio.  It has been packaged as an Electron desktop application and leverages OpenStudio Server for improved robustness, scalability, and capability.  This new version of PAT provides similar functionality (and we hope improved usability) as compared to PAT 1.14.0. However, the new version of PAT is not backwards compatible with PAT 1.14.0 projects. Additionally, this new version of PAT supports “algorithmic” workflows – methods for automatically generating large sets of analysis data points.  Choose from Design Of Experiments, Latin Hypercube Sampling, multiple optimization methods, and more to identify the most sensitive parameters in a model, calibrate a model against real consumption data, or optimize a model to balance multiple design objectives.  Algorithmic analyses tend to be computationally intensive, so the new PAT supports simulation on Amazon Web Service or other resources provisioned with OpenStudio Server.

The Ruby and C# binding footprints have been greatly reduced in the OpenStudio 2.1.0 package. Previously, deploying the Ruby and C# bindings required deployment of many shared library and file dependencies. In OpenStudio 2.1.0, the Ruby bindings are contained in a single openstudio.so (.bundle on Mac) file and the C# bindings in an OpenStudio.dll and two supporting dll files. These are the only files required to distribute the OpenStudio 2.1.0 bindings with a third party application. Note that the openstudio.so Ruby file does not contain any gems or the Ruby standard library; these must be configured using typical Gemfile configuration for your Ruby project. Similarly, the C# bindings do not include a Ruby interpreter or OpenStudio Ruby bindings needed to apply OpenStudio Measures. If a C# project desires to run measures or simulations, it can write an OSW file and make a system call to the CLI.

OpenStudio 2.1.0 is mostly backwards compatible with OpenStudio 1.14.0.  One primary difference is that PAT is not backwards compatible with PAT 1.14.0 projects.  Also, upgrading a 1.14.0 version OSM to 2.1.0 will not preserve the associated run.db, any measures associated with the OSM will have to be re-added in 2.1.0. The API for OpenStudio Model and other core namespaces was mostly preserved during the OpenStudio 2.1.0 conversion. However, the RunManager, Analysis, Project, and AnalysisDriver namespaces were completely removed. The CLI and OSW format replace the functionality of RunManager. The OpenStudio Analysis (OSA) format replaces the functionality of Analysis, Project, and AnalysisDriver. The Ruleset namespace has been renamed to Measure to better reflect its functionality.

OpenStudio 2.1.0 has been updated to EnergyPlus 8.7, Ruby 2.2.4, and is compatible with SketchUp 2017.  ResultsViewer is included in the 2.1.0 release.

## Known Issues

This is a list of known issues, if you find an issue not on this list please let us know so we can fix it.

Installer
* Spaces in the install path will cause PAT to fail.
* Firewall rules are not configured at install time, user is prompted to allow http communication between OpenStudioApp and the CLI as well as between PAT and the CLI.

Measures
* Measures written for OpenStudio 1.14.0 and earlier may not work with OpenStudio 2.0.0, measure authors are encouraged to test their measures and update them to ensure compatibility.
* Reporting measures do not have access to offline JavaScript libraries included with the OpenStudio installation.  Measure writers can include offline JavaScript libraries in the resources directory of their measure.

OpenStudioApp
* The app may appear to freeze on return from BCL measure dialog on Mac, a workaround is to select File -> Quit and when prompted to save the Model, select cancel to abort the quit operation or press the escape key.

PAT 2.0
* “Create One Design Alternative with Each Measure Option” duplicates options that already exist
* Cannot enter user defined string for Measure Choice arguments
* Display name is not shown  when browsing measures
* Measures associated with seed model are not imported


