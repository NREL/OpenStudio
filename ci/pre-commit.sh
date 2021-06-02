#!/bin/sh

# Installing the hook: at root of repo: cd .git/hooks && ln -s ../../ci/pre-commit.sh pre-commit

# stash unstaged changes
git stash -k -u
# Run clang format
./ci/clang-format.sh HEAD develop
# Pop stash
git stash pop
