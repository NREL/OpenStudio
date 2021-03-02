> [Wiki](Home) ▸ **Writing Style Guide**

### Contents

1. [Introduction](#1-introduction)
2. [Editing Process](#2-editing-process)
3. [Writing Style](#3-writing-style)
4. [Formatting](#4-formatting)

### 1. Introduction

Our written documentation, currently at https://github.com/NREL/OpenStudio/wiki, is OpenStudio's public face to the world.  In order to project a polished and professional appearance, OpenStudio documentation needs to conform to some common standards.  This document will help OpenStudio team members to write using a common voice.  

### 2. Editing Process

OpenStudio documentation is written using markdown syntax and is published by committing to the main documentation repository, currently at `git@github.com:NREL/OpenStudio.wiki.git`.  Minor updates to existing documentation may be made by committing directly to the documentation repository.  However, substantial modifications or new material should be reviewed before being published on the main documentation site.  To do this, we follow some of the same procedures that we follow when working on the OpenStudio source code.  Unfortunately, github does not presently support the same interfaces for branching and merging documentation repositories as it does for source code. Therefore, we have to modify this workflow somewhat for documentation tasks.

The first step, which only needs to be done once, is to create a fork of the main OpenStudio documentation repository.  To do this, first make a fork of the entire NREL/OpenStudio repository on github by clicking on the Fork button.  This will create a new OpenStudio repository under your github username, e.g. `git@github.com:yourname/OpenStudio.git`.  This will also create a fork of the OpenStudio documentation repository under your github username, e.g. `git@github.com:yourname/OpenStudio.wiki.git`.  Check out this new repository as you would check out any other repository.

```
cd yourname-openstudio-wiki
git clone git@github.com:yourname/OpenStudio.wiki.git .
```

Next you will need to synch your checkout of the wiki with the main OpenStudio documentation repository. To do this, first add the main OpenStudio wiki repository as a remote.  You can give the remote any name you prefer, here we will use `upstream`.

```
git remote add upstream git@github.com:NREL/OpenStudio.wiki.git
```

Now fetch the latest changes from the main repository and merge them into your checkout.

```
git fetch --all
git merge upstream/master
```

You can now edit files and commit them to your repository, the pages will appear at https://github.com/yourname/OpenStudio/wiki.  

When your edits are ready you can synch with the main documentation repository one last time to ensure you are up to date and then force push your to the main documentation repository.

```
git push upstream master
```

If you are reviewing another team member's repository you can add a remote for their repository, merge their changes into your checkout, and push your changes back to their repository or to the main one when you are done.

### 3. Writing Style

The following are general guidelines for writing OpenStudio documentation.

- Write in the present tense
- Write in the first person 
- Address the reader as you and we
- Always define acronyms when first used
- Verify that any included hyperlinks are functional
- And always spell check your work!

### 4. Formatting

Here are some helpful links:

- [Basic syntax] (http://daringfireball.net/projects/markdown/basics)
- [GitHub Cheatsheet] (https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)
- [Gollum Wiki Features] (https://github.com/gollum/gollum/wiki)

If you would like to preview your rendered markdown before pushing it to a github repository the CuteMarkEd, http://cloose.github.io/CuteMarkEd/, is a good tool to try.  Mac users may wish to try http://mouapp.com.
