#!/bin/bash

# This script compares two branches (eg. pr-bugfix - <> develop )
# and finds files of types cpp, hpp, c, h 
# that have changed between branches and processes each of these files using clang-format. 
# The default behavior of clang-format is to auto change the file and format according to the style guide (.clang-format).  If these files change the CI will alert fail and alert the user to correct the changes. The user should run this script locally and accept the auto changes proposed by clang-format. 

display_usage() { 
  echo -e "\nUsage:\ PR_BRANCH_NAME TARGET_BRANCH_NAME \n" 
} 


if [  $# -le 1 ] 
then 
  display_usage
  exit 1
fi 

PR_BRANCH_NAME=$1
TARGET_BRANCH_NAME=$2

echo $PR_BRANCH_NAME  
echo $TARGET_BRANCH_NAME 

# gather files that have changed and pipe to clang-format

git diff $PR_BRANCH_NAME $TARGET_BRANCH_NAME --name-only | grep '.*\.\(cpp\|c\)' | xargs clang-format -style=file -i -fallback-style=none


# clang-format will auto correct files so prepare the diff and use this as artifact
git diff > clang_format.patch

# Delete if nothhing otherwise exit 1 to indicate a failed job
if [ ! -s clang_format.patch ]
then
    rm clang_format.patch
    exit 0
else 
  exit 1
fi

exit 0

