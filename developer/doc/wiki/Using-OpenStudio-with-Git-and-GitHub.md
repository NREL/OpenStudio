> [Wiki](Home) ▸ **Using OpenStudio with Git and GitHub**

## Contents
- [Interacting with Git](#interacting-with-git)
    * [GitHub for Windows](#github-for-windows)
    * [Command Line Interface](#command-line-interface)
    * [TortoiseGit](#tortoisegit)
    * [SmartGit/Hg](#smartgithg)
    * [Git Extensions](#git-extensions)
- [Creating an SSH Key](#creating-an-ssh-key)
- [Configuring Git Settings](#configuring-git-settings)
- [Information for Third Party Collaborators](#information-for-third-party-collaborators)
- [OpenStudio's Git Workflow](#openstudios-git-workflow)
- [Cloning the Repository to Your Local Computer](#cloning-the-repository-to-your-local-computer)
- [Listing Branches](#listing-branches)
- [Creating a Branch](#creating-a-branch)
- [Switching to an Existing Branch](#switching-to-an-existing-branch)
- [Committing Your Changes](#committing-your-changes)
- [Checking For Modifications](#checking-for-modifications)
- [Updating Your Local Repository, and Stashing](#updating-your-local-repository-and-stashing)
- [Reintegrating a Branch into Develop](#reintegrating-a-branch-into-develop)
- [Pushing All of Your Local Commits to GitHub](#pushing-all-of-your-local-commits-to-github)
- [Deleting A Branch](#deleting-a-branch)
- [Synchronizing Your Fork with NREL/OpenStudio](#synchronizing-your-fork-with-nrelopenstudio)
- [Other Useful Commands](#other-useful-commands)
    * [Getting the Latest Commit Hash](#getting-the-latest-commit-hash)
    * [Viewing the Log](#viewing-the-log)
    * [Reverting Working Directory Changes](#reverting-working-directory-changes)
    * [File Operations](#file-operations)
    * [Creating Your Own Git Commands](#creating-your-own-git-commands)

## Interacting with Git
Depending on your operating system and personal preferences, there are a variety of options for interacting with the repository on your computer:

### GitHub for Windows
[[/images/Using-OpenStudio-with-Git-and-GitHub/client.github.png]]

A very simple, intuitive GUI for Windows users.  This GUI will be sufficient for most third party contributors.  Only works with github.com.

[Windows](http://github-windows.s3.amazonaws.com/GitHubSetup.exe)

### Command Line Interface
[[/images/Using-OpenStudio-with-Git-and-GitHub/client.bash.png]]

Ideal for power users and for running commands directly on the repository.  It also includes a basic Git GUI, as well as gitk, a low-level Git GUI.  This may be a prerequisite for some of the following tools, and also comes packaged with some of the following tools.

[Windows](http://git-scm.com/download/win) – [OS X](http://git-scm.com/download/mac) – [Linux](http://git-scm.com/download/linux)

### TortoiseGit
[[/images/Using-OpenStudio-with-Git-and-GitHub/client.tortoisegit.png]]

A helpful GUI for normal users who are already familiar with TortoiseSVN and its Explorer integration.  In some cases, the interface can be unintuitive.

[Windows](https://code.google.com/p/tortoisegit/wiki/Download)

### SmartGit/Hg
[[/images/Using-OpenStudio-with-Git-and-GitHub/client.smartgit.png]]

The best graphical interface for Git – Understand that the non-commercial license can ONLY be used for open-source, non-commercial projects, such as OpenStudio.

[Windows](http://www.syntevo.com/smartgithg/download?file=smartgithg/smartgithg-win32-setup-jre-5_0_9.zip) – [OS X](http://www.syntevo.com/smartgithg/download?file=smartgithg/smartgithg-macosx-5_0_9.dmg) – [Linux](http://www.syntevo.com/smartgithg/download?file=smartgithg/smartgithg-generic-5_0_9.tar.gz)

### SourceTree
[[/images/Using-OpenStudio-with-Git-and-GitHub/client.sourcetree.png]]

A powerful, intuitive GUI rivaling SmartGit that can be used freely for commercial projects.

[Windows](http://downloads.atlassian.com/software/sourcetree/windows/SourceTreeSetup_1.5.2.exe) – [OS X](http://downloads.atlassian.com/software/sourcetree/SourceTree_1.9.1.dmg)

### Git Extensions
[[/images/Using-OpenStudio-with-Git-and-GitHub/client.gitex.png]]

A useful GUI for visualizing tree changes, Git Extensions also includes context-menu integration in Explorer.

[Windows](http://sourceforge.net/projects/gitextensions/files/latest/download?source=navbar) – [OS X](https://git-extensions-documentation.readthedocs.org/en/latest/getting_started.html#installation-mac) – [Linux](https://git-extensions-documentation.readthedocs.org/en/latest/getting_started.html#installation-linux)

## Creating an SSH Key
Establishing a secure connection to GitHub.com can be achieved over HTTPS with a username and password, or with SSH keys and an optional passphrase.  SSH keys are highly recommended for security and performance.  GitHub has thoroughly documented the process: Carefully follow the instructions for your platform to create an SSH key and add it to GitHub.com.

[Windows](https://help.github.com/articles/generating-ssh-keys#platform-windows) – [OS X](https://help.github.com/articles/generating-ssh-keys#platform-mac) – [Linux](https://help.github.com/articles/generating-ssh-keys#platform-linux)

On Windows, some GUI tools like TortoiseGit require a Putty SSH key instead of an OpenSSH key.  To convert the key you just generated to Putty format, launch Puttygen from the start menu, load your private key `~/.ssh/id_rsa`, and then save the new private key `~/.ssh/id_rsa.ppk`:

[[/images/Using-OpenStudio-with-Git-and-GitHub/tortoisegit.clone.png]]

## Configuring Git Settings
To start, you should configure some of Git's global settings, which are saved in `~/.gitconfig`.  If you leave out the `--global` flag, the settings will be applied only to your current repository, which is also fine.

```bash
git config --global user.name "John Doe"
git config --global user.email johndoe@example.com
git config --global push.default simple
git config --global remote.origin.prune true
git config --global branch.autosetuprebase always
```

The [user.name setting](https://help.github.com/articles/setting-your-username-in-git) can be anything, although we recommend using your real name.  The user.email setting should match an email that you have associated with your GitHub account.  The push.default setting is just to set the push behavior to the new default for Git 2.0 and avoid notices on the command line.  The remote.origin.prune setting will automatically remove remote branches that have been deleted from GitHub.com.  Finally, the branch.autosetuprebase setting will automatically use `git pull --rebase` when you call `git pull`, which helps avoid messy merge commits and divergent branch history.

On Windows, it is recommended to check out code using Windows style line endings and commit Unix-style line endings:

```bash
git config --global core.autocrlf true
```

On other platforms, it is recommended to checkout as-is, commit Unix-style line endings:

```bash
git config --global core.autocrlf input
```

There are [many more configuration settings](http://git-scm.com/book/en/Customizing-Git-Git-Configuration) as well.  To see all of your current settings:

    git config --list

## Information for Third Party Collaborators
If you'd to use a GUI for managing the repository, follow the [GUI instructions for third party collaborators](Information-for-Third-Party-Collaborators).

Otherwise, in the following clone step, replace _NREL_ in the clone URL with your GitHub username.
Finally, when your changes are ready to be approved for inclusion in the main OpenStudio repository, click the [[/images/Using-OpenStudio-with-Git-and-GitHub/compare.png]] Compare button in your fork and follow [GitHub's instructions](https://help.github.com/articles/creating-a-pull-request) for submitting a pull request.

## OpenStudio's Git Workflow
All work should be completed in feature branches created from the _develop_ branch.  Biweekly iterations will be branched from _develop_ to _iteration_, and releases will be branched from _iteration_ to _master_.  No commits or development work should be made to _iteration_ or _master_ unless you are authorized to modify that iteration or release.

[[/images/Using-OpenStudio-with-Git-and-GitHub/workflow.png]]

_Modified from http://nvie.com/posts/a-successful-git-branching-model/_

## Cloning the Repository to Your Local Computer
Now that you have your SSH key configured, you can create a local clone of the repository.  If you want to download the latest stable release, select the _master_ branch.  Otherwise, if you want to work with the latest development code, use the _develop_ branch:

    git clone -b develop git@github.com:NREL/OpenStudio.git .

The final dot is required if you want to clone into your current directory.  Without the dot, this command will create a directory called OpenStudio and clone into that.  This clone operation will download ~87MB of files and reconstruct the full develop branch within that directory (which will then total ~337MB).  This command also makes all branches available locally.

If you choose to use the HTTPS protocol instead of SSH, you may encounter a SSL certificate issue.  This can be caused by enterprise network equipment.  If you determine that the warning is a false positive, you can instruct Git to ignore the warning:

```bash
git config http.sslVerify "false"
```

## Listing Branches
When viewing branches, the current branch is marked with an asterisk.  To view local branches:

    git branch

To view remote branches that you have available:

    git branch -r

To view all local and remote branches that you have available:

    git branch -a

## Creating a Branch
To create a new local branch and switch to it:

    git checkout -b mynewbranch

To make this branch available to everyone, push the branch to GitHub:

    git push origin mynewbranch

Alternatively, you can do both these steps by typing the new branch name into the branch filter on GitHub and click Create branch:

[[/images/Using-OpenStudio-with-Git-and-GitHub/newbranch.png]]

## Switching to an Existing Branch
To switch to a remote branch that you haven't already downloaded (branches that were created after you cloned the repository or after your most recent fetch/pull), you should run the following command to get an updated list of remote branches and prune your branches.  Pruning removes all branches that have been deleted from GitHub:

    git fetch -p

Then, you can checkout and switch to any local or remote branch with the following command:

    git checkout mybranch
        or
    git checkout develop

## Committing Your Changes
To track new files, _and_ to stage modified files for commits:

    git add mynewfile
        or
    git add mymodifiedfile

After creating your branch and making changes, commit all your staged changes and modified/deleted files.  The first line of your commit message should be a very brief description of the commit, followed by more details:

```bash
git commit -m "#3 Bug number & brief commit summary go here (~65 chars)
> More details can go on the additional lines [delivers #12345678]"
```

Adding a `-a` flag to your commit command will automatically commit all modified files, even if you haven't explicitly used `git add` on them.

## Checking For Modifications
To check for staged changes, unstaged changes, and untracked files:

    git status
        or
    git status -sb

Adding the `-s` flag produces a very concise status, and the `-b` flag tells it to indicate the current branch.

## Updating Your Local Repository, and Stashing
To update your local repository and all branches, _and_ update the files in your current branch:

    git pull
        or, if you understand the consequences of rebasing,
    git pull --rebase

For more information about rebasing, please review the [online manual](http://git-scm.com/book/en/Git-Branching-Rebasing).

If you have uncommitted changes and you're not ready to commit them, you will first need to stash your changes, then perform the pull, and then pop the changes:

    git stash
    git pull
    git stash pop

Without stashing and popping, you may see a message like this:

[[/images/Using-OpenStudio-with-Git-and-GitHub/cmd.png]]

A good example of when you might want to do this is if you're not ready yet to commit the changes, but for one reason or another you need a clean working directory.

If the pop command triggered merge conflict resolution, the stashed changes won't be removed from the stash stack.  To explicitly remove the last item from the stash, use the following command:

    git stash drop stash@{0}
 
To view the current contents of the stash stack:

    git stash list

To view the contents of an individual stash:

    git stash show

## Reintegrating a Branch into Develop
If a code review is necessary and your changes are complete, click the [[/images/Using-OpenStudio-with-Git-and-GitHub/compare.png]] Compare button in your branch and follow [GitHub's instructions](https://help.github.com/articles/creating-a-pull-request) for submitting a pull request.  After creating the pull request, you can assign it to the bug or ticket owner for review.

However, if your branch does not require a code review, then it can be merged immediately:

    git checkout develop
    git merge --no-ff mybranch

The no-fast-forward `--no-ff` flag is important for merging to maintain branch history, and it stays consistent with GitHub's automatic merge settings:

[[/images/Using-OpenStudio-with-Git-and-GitHub/no-ff.png]]

_Credit: [nvie](http://nvie.com/posts/a-successful-git-branching-model/)_

## Pushing All of Your Local Commits to GitHub
When you're ready to share your changes and commits from any branch with the rest of the team:

    git push origin

## Deleting A Branch
To delete a local branch:

    git branch -d mybranch

To delete a remote branch directly, use the [[/images/Using-OpenStudio-with-Git-and-GitHub/deletebranch.png]] button on [GitHub](https://github.com/NREL/OpenStudio/branches).  Alternatively, you can use the following command:

    git push origin --delete mybranch

Note that if you delete an unmerged branch, the branch and all commits to it will be permanently deleted.

## Synchronizing Your Fork with NREL/OpenStudio
If you want to update your fork with the latest changes from OpenStudio's main repository, you will first have to add an upstream remote, and then merge the latest changes to your fork:

```bash
# set the upstream remote once
git remote add upstream git@github.com:NREL/OpenStudio.git

# grab the upstream remote's branches
git fetch upstream

# merge NREL/OpenStudio's develop branch into your fork's develop branch
git checkout develop
git merge upstream/develop
```

If you have made commits to your fork but want to reset it:
```bash
git fetch upstream
git checkout develop
git reset --hard upstream/develop
git push origin develop --force 
```

## Other Useful Commands
### Getting the Latest Commit Hash
To produce the SHA1 hashes of the latest commit, such as 932bca9f7dfab0d698dcdc04032762b6525237d5 or 932bca9:

```bash
# full 40 byte hash
git rev-parse HEAD

# 7 byte hash
git rev-parse --short HEAD

# 10 byte hash
git rev-parse --short=10 HEAD
```

### Viewing the Log
```bash
# full log
git log

# last 5 commits, one line per commit
git log --oneline --decorate -5

# show a tree graph of commits and merges succinctly
git log --graph --oneline

# view committed changes that haven't yet been pushed
git log origin/develop..develop
```

### Reverting Working Directory Changes
To revert all uncommitted changes in your working directory:

    git reset --hard

To destroy all untracked files and directories from your working directory:
```bash
# dry run - make sure you're not deleting something you want
git clean -f -d -n

# delete all untracked files and directories, not including directories that are ignored by git
git clean -f -d
```

To change the commit message of your last commit:

```bash
git commit --amend -m "New commit message"
```

To undo the last commit (while still keeping the file changes):

    git reset --soft HEAD^

To revert a single file:

    git checkout path/to/file/to/revert

### File Operations
To delete a file from your file system and the repository:

    git rm myfile

To rename or move a file:

    git mv myfile mynewfile

### Creating Your Own Git Commands
To create a shorthand command for a long command that you use frequently, you can create an alias.

To create the command `git rev` that returns a 10 byte revision hash *and* copies it to your clipboard:

```bash
# Windows with Git Bash
git config --global alias.rev '!git rev-parse --short=10 HEAD | tr -d "\n" | clip; git rev-parse --short=10 HEAD'

# OS X
git config --global alias.rev '!git rev-parse --short=10 HEAD | tr -d "\n" | pbcopy; git rev-parse --short=10 HEAD'

# Linux
sudo apt-get install xsel
git config --global alias.rev '!git rev-parse --short=10 HEAD | tr -d "\n" | xsel -b; git rev-parse --short=10 HEAD'
```

To make a much prettier log command with `git lg` (Credit: [jukie.net](http://www.jukie.net/bart/blog/pimping-out-git-log)):

```bash
git config --global alias.lg "log --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit --date=relative"
```

To make a small command to stash your changes, rebase with the latest commits, and pop your changes using `git p`:
```bash
git config --global alias.p '!git stash; git pull --rebase; git stash pop'
```

To delete an alias:

    git config --global --unset alias.myAlias