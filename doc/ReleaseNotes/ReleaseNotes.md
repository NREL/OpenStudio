# OpenStudio 2.0 Prerelease

This is an alpha quality build of OpenStudio 2.0. This is an aggressive overhaul of OpenStudio to make the SDK smaller and more accessible. The first component and the focal point of the overhaul is a command line interface that is capable of executing OpenStudio workflows. Principally this means that it is capable of running OpenStudio Measures. The command line is fully self contained, requiring no outside dependencies beyond the system runtime libraries. Ruby and standard librarys are statically linked and archived into the executable.

Additional components will be added as optional components in the installer as they are brought back online. Namely the OpenStudio Application and SketchUp Plugin. At this time the command line interface is the only executable in this package.

## Example Usage

This is an example workflow included in this package as ```compact_osw``` under the Examples directory. This example contains everything to run a simple OpenStudio workflow from seed model, to applying measures, to running EnergyPlus. 

```
cd path/to/Examples/compact_osw
./path/to/openstudio.exe run -w compacw.ows
```

The OpenStudio command line needs to access EnergyPlus. It will do this by first looking for the environment variable ENERGYPLUS_EXE_PATH, and then by looking in the standard EnergyPlus install location for the system.

