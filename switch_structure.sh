#!/bin/bash

# To resolve potential problems you might have to use `git clean -fd`

declare -a DEFAULT_OS_STRUCT=(
"CMake"
"CMakeLists.txt"
"csharp"
"CTestConfig.cmake"
"doc"
"Doxyfile.in"
"embedded"
"icons"
"java"
"openstudioapp.desktop"
"OpenStudioCoreConfig.cmake.in"
"OpenStudio.in"
"OpenStudio.nuspec.in"
"OpenStudio.targets"
"osruby_config.h"
"pat"
"ProjectMacros.cmake"
"python"
"resources"
"ruby"
"SketchUpInstallName.rb"
"src"
"v8-node"
"x-openstudio.xml"
)

OS_STRUCT_BAK=.oscore_struct.bak
OS_STRUCT_FILE=.current_structure

if [ "$STRUCTURE" = "NEW" ]; then

  echo "Going from 3.x structure to historical structure"

  # Remake the openstudiocore level
  mkdir openstudiocore

  # Populate it

  # If an actual structure backup exists, use that
  if [ -f "$OS_STRUCT_BAK" ]; then
    # Read openstudiocore structure to move everything back in there
    while read -r line || [[ -n "$line" ]]; do
      mv $line openstudiocore/
    done < $OS_STRUCT_BAK
  else
    echo "Defaulting structure"
    for direntry in "${DEFAULT_OS_STRUCT[@]}"; do
      mv $direntry openstudiocore/
    done
  fi

  # Move the meta stuff to root, and delete meta/
  mv meta/* .
  rm -R meta

  # Move developper/doc to ./doc
  mv developer/doc .

  # Delete the backup file
  # rm $OS_STRUCT_BAK

  echo "STRUCTURE=OLD" > $OS_STRUCT_FILE

else
  echo "Going from historical structure to 3.x structure"

  # Make the meta/ level and move the super build stuff in there
  mkdir meta
  mv CMakeLists.txt meta/
  mv CMake meta/

  # Move the toplevel doc to developer/doc
  mv doc/ developer/

  # Save current openstudiocore structure
  ls openstudiocore/ > $OS_STRUCT_BAK
  # Move everything one level up and delete the openstudiocore level
  mv openstudiocore/* .
  rm -R openstudiocore

  echo "STRUCTURE=NEW" > $OS_STRUCT_FILE

fi
