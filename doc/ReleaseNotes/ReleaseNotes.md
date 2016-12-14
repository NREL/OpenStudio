# OpenStudio 2.0 Prerelease

This prerelease of OpenStudio 2.0 provides a preview of components that will be officially released as OpenStudio 2.0 in the near future (we are currently targeting a December 2016 release).  At this point, OpenStudio 2.0 should be used for investigation purposes only, it should not be used for production work.  Future backwards compatibility with prerelease builds is not guaranteed.  Any testing, usability, or bug reports are incredibly helpful as we are working to tighten OpenStudio 2.0 up for the first official release.  Please send any such reports through email at OpenStudio@nrel.gov, Slack (https://unmethours.com/question/11053/unmet-hours-slack-channel-chat/), or github (https://github.com/NREL/OpenStudio/issues).  Please make sure to indicate that you are using an OpenStudio 2.0 prerelease in your communications.  We hope you like all that is new in OpenStudio 2.0 and we cannot thank you enough for your help getting OpenStudio 2.0 ready for prime time!

# What's New in OpenStudio 2.0

OpenStudio 2.0 is an aggressive overhaul of OpenStudio to make the SDK smaller and more accessible for third party software developers. The first component, and the focal point of the overhaul, is a command line interface (CLI) openstudio.exe. The primary function of this CLI is to run simulation steps described in a OpenStudio Workflow (OSW) file. The OSW file format is described in JSON schema at https://github.com/NREL/OpenStudio-workflow-gem/tree/osw/spec/schema.  The OSW defines a set of paths to search for OpenStudio Measures, initial seed model, weather file, and other supporting files.  These paths may be absolute or relative, if relative they are evaluated relative to the directory containing the OSW.  Next the OSW defines an initial OpenStudio Model (OSM) and weather file to use as the starting point for the simulation workflow.  Finally, the OSW defines a series of OpenStudio Measures that are run as part of the simulation workflow.  OpenStudio Model Measures will be run first, the model is translated to EnergyPlus IDF format and EnergyPlus Measures are run second, the EnergyPlus simulation occurs next, and finally Reporting Measures are run.  The CLI contains the Ruby interpreter, Ruby standard library, OpenStudio Ruby bindings, and a core set of gems (including the OpenStudio Standards Gem).  It does not have any shared library dependencies outside of system runtime libraries.  EnergyPlus and Radiance are not embedded within the CLI, these must be installed separately.  No configuration is needed if EnergyPlus and Radiance are installed to the default system location.  If EnergyPlus is installed in a non-standard location, the ENERGYPLUS_EXE_PATH environment variable is read by the CLI.

The Ruby and C# binding footprints have also been greatly reduced in the OpenStudio 2.0 package.  Previously, deploying the Ruby and C# bindings required deployment of many shared library and file dependencies.  In OpenStudio 2.0, the Ruby bindings are contained in a single openstudio.so (.bundle on Mac) file and the C# bindings in an OpenStudio.dll and openstudio_csharp.dll file.  These are the only files required to distribute the OpenStudio 2.0 bindings with a third party application.  Note that the openstudio.so Ruby file does not contain any gems or the Ruby standard library, these must be configured using typical Gemfile configuration for your project.  Similarly, the C# bindings do not include a Ruby interpreter or OpenStudio Ruby bindings needed to apply OpenStudio Measures.  If a C# project desires to run measures or simulations, it can write an OSW file and make a system call to the CLI.

A new version of the OpenStudio Parametric Analysis Tool (PAT) under construction and will be coming soon to the OpenStudio 2.0 prerelease packages.  This new version of PAT is being written from the ground up in JavaScript as an Electron desktop application.  The new PAT is written around the new OpenStudio 2.0 framework and will be more compatible with the OpenStudio Server for large scale analysis.

## Example CLI Usage

This is an example OSW included in this package as ```compact_osw``` under the Examples directory. This example contains everything to run a simple OpenStudio workflow from loading a seed model, applying measures, running EnergyPlus, and running reporting measures. To run this example:

```
cd path/to/Examples/compact_osw
./path/to/openstudio.exe run -w compact.osw
```

The OpenStudio command line needs to access EnergyPlus. It will do this by first looking for the environment variable ENERGYPLUS_EXE_PATH, and then by looking in the standard EnergyPlus install location for the system.

## Note about versioning

```
openstudio.exe openstudio_version
```

This command will report out the version of the OpenStudio command line interface. At this time, even though this is a 2.0 preview, the version will be reported as 1.x.y corresponding to the version of the OpenStudio Model format for which it is compatible.  This behavior was chosen to allow users of the OpenStudio 2.0 preview to try loading OSM models developed with the current 1.13.0 release.

## API Changes

The API for OpenStudio Model and other core namespaces was mostly preserved during the OpenStudio 2.0 conversion.  However, the RunManager, Analysis, Project, AnalysisDriver namespaces were completely removed.  The CLI and OSW format replace the functionality of RunManager.  The OpenStudio Analysis (OSA) format replaces the functionality of Analysis, Project, and AnalysisDriver.  The Ruleset namespace has been renamed to Measure to better reflect its functionality.

## Known Issues

This is a list of known issues, if you find an issue not on this list please let us know so we can fix it.

CLI
* The Ruby Gems library (require 'rubygems') cannot be loaded by the CLI.  If you need to require Ruby Gems, you can use a system Ruby 2.0 installation and require the OpenStudio Ruby Bindings as a workaround.

Measures
* Standard Reporting and other measures are not distributed at this time, they can be downloaded from the BCL as a workaround.
* Reporting measures do not have access to offline JavaScript libraries.

SketchUp Plug-in
* Creating an OSM in the SketchUp Plug-in does not create a companion OSW file.
* Cannot load OpenStudio Standards in the SketchUp Plug-in.

OpenStudioApp
* The apply measure now dialog does not correct report errors, warnings, and advanced output.
* The synch measure dialog does not work.
* The standard reporting measure is not automatically added to a new OSM file.
* The purge resource buttons do not work.
* The user is not prompted to save their model before running.
* The app freezes after downloading measures from the BCL.
* There is a crash if user turns on output variables then switch away from and back to the output variable tab.
* Firewall rules are not configured at install time, user is prompted to allow http communication between OpenStudioApp and the CLI.

PAT 2.0
* PAT 2.0 is not currently included in the OpenStudio 2.0 prerelease, it will be included in future prereleases ahead of the official 2.0 release.

