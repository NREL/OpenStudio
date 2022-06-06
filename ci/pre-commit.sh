#!/bin/sh

# Installing the hook: at root of repo: cd .git/hooks && ln -s ../../ci/pre-commit.sh pre-commit

# First, stash index and work dir, keeping only the
# to-be-committed changes in the working directory.
old_stash=$(git rev-parse -q --verify refs/stash)
git stash push --quiet --keep-index --include-untracked --message "os_pre_commit"
new_stash=$(git rev-parse -q --verify refs/stash)

# Run clang format
./ci/clang-format.sh HEAD develop
RESULT=$?
[ -f clang_format.patch ] && rm -f clang_format.patch

# Restore changes, only if we stashed something to begin with
if [ "$old_stash" != "$new_stash" ]; then
  # stash apply then drop is same as pop
  git stash apply --index --quiet && git stash drop --quiet
fi

# If the clang-format script returned non zero, don't allow the commit. The format changes will be in the unstaged queue, ready for approval
exit $RESULT
