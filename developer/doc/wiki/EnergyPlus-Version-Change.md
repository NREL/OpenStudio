> [Wiki](Home) ▸ **EnergyPlus Version Change**

# EnergyPlus Migration

## Procedure:

* Create new ProposedEnergy+.idd.  I do this by diffing previous version idd and new version idd and copying over changes.

* Analyze diffs in idd versions and adjust the energyplus translator as needed to write valid idf files in the new version.  Changes may also be made to the OpenStudio Model classes but they must not break the existing api.

* Version translate idf files located in openstudiocore/resources/ using the EnergyPlus version translator.

* Run unit tests and fix things that are broken. 

* In openstudiocore/sketchup_plugin/openstudio/sketchup_plugin/lib/PluginManager.rb, update the energyplus_version method to return the latest version.

* In CMakeLists.txt update E+ version.

* Update src/utilities/idd/Test/IddFile_GTest.cpp version string.

* In src/energyplus/ForwardTranslator.hpp, update `#define ENERGYPLUS_VERSION "X.Y"`.

* When field names change leave the field name unmodified in the Model API, but add a comment indicating the new name of the field in EnergyPlus.  See [here](https://github.com/NREL/OpenStudio/blob/c5bad04b2c03ddecf3bae3dd7d7721a56029212b/openstudiocore/src/model/ZoneHVACHighTemperatureRadiant.hpp#L62) for example.

* Run regression tests in openstudio-resources, especially model simulation tests.

## Utilities

The following script can be helpful to apply the E+ transition program in a non interactive script. I saved it as test.sh in the E+ products folder.

```
#!/bin/bash

set +e

printf "full\nyes\nno\n $1 \n" | ./Transition-V8-4-0-to-V8-5-0

mv "`echo $1 | sed 's/\.idf/\.idfnew/'`" "$1"
rm "`echo $1 | sed 's/\.idf/\.VCpErr/'`"
```

Run this script from the E+ build/Products folder in a developer setup. Transition exe likes the idd files right next to the exe, which is the case in a the E+ Products folder. Argument $1 should be a full path to an idf file to update.

Use find to grep for the idf files that need updating in the OS dev tree.

```
find /Users/kbenne/Development/OpenStudio2 -not -path "/Users/kbenne/Development/OpenStudio2/build/*" -not -path "/Users/kbenne/Development/OpenStudio2/.git/*" -name *.idf -exec ./test.sh {} \;
```