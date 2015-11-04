-------- NOTES --------

New CMAKE Build Options added
  BUILD_V8_BINDINGS - Builds SWIG Bindings for V8
  BUILD_NODE_MODULES
    Builds the V8 bindings specifically as Node loadable add-ons and enabled v8 unit tests. 
    If disabled libraries are generated and built but are untested since V8 does
      not provide any kind of module loader or cli. 
    Enables internal features of SWIG-V8 module to ensure backward compatibility
      with older version of V8 that is shipped with Node.js
  BUILD_NODE - Downloads and builds Node.js into Node-prefix folder. After
    compilation uses npm to install jasmine-node
  BUILD_V8 - Checks out and builds V8 as of 3/16/2013 into V8-prefix folder

Generated libraries are in OSCore-prefix/src/OSCore-build/Products/v8

Tests exist in two locations. 
  Simple assertions using the Node.js assert library are in openstudiocore/v8-node/test 
  node-jasmine based tests are in openstudiocore/v8-node/jasmine
  The number of "gotchas" that needed to be address with unit tests where few
    exceptions - this is just to ensure that exception handling is working. All
      C++ exceptions are translated into simple v8 exceptions which can be caught in
      the javascript
    optionals - there was no problem with the implementation, but it's the least
      expected part of the OpenStudio API, I wanted to make sure it was working
    inheritance - Some issues were discovered relating to calling inherited
      functions and passing derived objects to function

Addressing Node's local-context when loading modules
  The modules now use the global v8 context to find each other in a SWIG-friendly manner
  A helper script, openstudiocore/v8-node/OpenStudio.js loads the libraries in
    proper order and fixes them into something that user might expect

Disabled libraries
  OpenStudioLib
  OpenStudioModelEditor
  OpenStudioPlugin
  Each has been disabled because they provide no real functionality to the
    scripting user and disabling them decreases module load time

Known issues
  MacOS: Building on MacOS currently requires autoconf to be installed since we
    are building SWIG from git. A source package can be made without much
    difficulty.

-------- BUILDING ---------

The recommended configuration for building and testing is, using CMake,
enable:

BUILD_V8_BINDINGS - Enables the V8 bindings
BUILD_NODE_MODULES - Enables V8 bindings as Node addons
BUILD_NODE - Downloads and compiles node to be used locally 
BUILD_SWIG - Clones the latest Git SWIG from https://github.com/lefticus/swig-v8 with V8 Support
BUILD_TESTING - Enables testing


make -j<number of cpus>

There is a lot of code to compile and it can take some time. If you did not
already have an appropriate version of Qt and Boost installed it will build
and install them for local use as well.

-------- RUNNING TESTS ---------

If OpenStudio was compiled with testing enabled, the tests can be run two
different ways:

cd <builddir>/OSCore-prefix/src/OSCore-build


make test # Runs all tests

- or -

ctest # Runs all tests



ctest can be further refined with regular expressions. Run run only the v8
tests:

ctest -R V8



-------- CHANGES REQUIRED TO GET V8 WORKING --------

The changes required from the OpenStudio side were rather minimal

CMakeLists.txt  - updated to include the switches mentioned in the first section of this document
openstudiocore/CMakeLists.txt  - updated to include the switches mentioned in the first section of this document
openstudiocore/v8-node  - This directory with associated tests and the OpenStudio.js file was added
openstudiocore/ProjectMacros.cmake   - updated to create SWIG V8 bindings in addition to the existing .NET, Ruby and Python bindings macros
openstudiocore/model/Model*.i : updated to reorganize some class definitions to make sure they are available at the correct time
openstudiocore/utilities/core/Exception.i  - Exception handling for V8 SWIG was enabled
openstudiocore/utilities/core/Path.i - Path construction helper code was added 
openstudiocore/utilities/core/UUID.i : destructor was made public
openstudiocore/utilities/core/v8/LanguageSpecific.i  - added which fixes up some v8 specific things including adding operator overload
                                                     aliases. The aliases are not pretty, but at least the are available since
                                                     operator overloading does not exist in JavaScript.
openstudiocore/utilities/idf/idf.i : set and map of UUID were updated from the typed alias of Handle.


The majority of the work required was in the SWIG V8 code wrapper generator.


Specifically:

  - Fix handling of overloaded method names in different classes 
  - Make sure install grabs v8 swig files as well   
  - Fix incorrect include of stdexcept in std_pair.i for javascript v8 
  - Fix compiling of returning of const std::string & 
  - Add support for %begin section in javascript/v8 module 
  - More tightly integrate Node.i (For support of generating Node add-ons)
  - Clean up init call name in Node implementation 
  - Better integrate Node.js and fix emitting of inheritance when base class 
      is not defined in current module
  - Support intermodule dependencies for V8 
  - Support the latest revision of v8 while still working with Node 
  - Fix type checking. Inspiration was taken from the Ruby SWIG Module 

These changes are tracked and available at https://github.com/lefticus/swig-v8
