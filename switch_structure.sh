#!/bin/bash

git clean -fd

OS_STRUCT_BAK=.oscore_struct.bak

if [ -f "$OS_STRUCT_BAK" ]; then
  echo "Going from OS App sep to old"

  # Remake the openstudiocore level
  mkdir openstudiocore
  # Read openstudiocore structure to move everything back in there
  while read -r line || [[ -n "$line" ]]; do
    mv $line openstudiocore/
  done < $OS_STRUCT_BAK

  # Move the meta stuff to root, and delete meta/
  mv meta/* .
  rm -R meta

  # Move developper/doc to ./doc
  mv developer/doc .

  # Delete the backup file
  rm $OS_STRUCT_BAK

else
  echo "Going from old to OS App sep"

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

fi
