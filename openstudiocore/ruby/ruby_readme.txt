Building ruby bindings requires the additional tools:
  * ruby (via os package manager or from ruby-lang.org)
  * the ruby development package (ruby-dev or similar on linux, Windows and MacOS come with required header and library files) 
  * SWIG (www.swig.org)

To compile the bindings a full build is required, as most packages are dependencies of the bindings. On Unix this can be accomplished with a "make" command in the build directory.

To build ruby rdoc documentation, enable BUILD_DOCUMENTATION in cmake. Enabling this option requires the installation of two more tools: 
  * rdoc
  * ri

After cmake has been configured and the ruby bindings have been build the rdoc files can be built with
  * make ruby_rdoc

After the build has completed ri documentation will exist in <builddir>/ruby/rdocify. The generated ruby documentation is mostly a skeleton, designed to point the user to doxygen generated documentation from the C++ source code.

To generate documentation from the C++ sources, make <subproject>_doc should be executed (ex: make openstudio_utilities_doc). The resulting generated documenation will be in <builddir>/doc/<projectname>.

If you are interested in adding code to the library and you want it to make sure your additions are supported by the Ruby bindings, please see "SWIG Support" section of the Coding Standards documentation found in <sourcedir>/doc/CodingStandards.

-----

Working with ruby bindings

From within the build directory:

  * cd <builddir>/ruby

You can execute ruby bindings applications that reside in the source directory:

  * ruby <sourcedir>/ruby/openstudio/examples/test/Alpha1_Test.rb

If you are testing ruby code on Windows, you may need or want to specify that the Debug build of the ruby libraries be used instead of the Release build. This is necessary on Windows because visual studio builds debug and release builds in separate folders.

  * ruby <sourcedir>\ruby\openstudio\examples\test\Alpha1_Test.rb Debug

Note that it is always the last parameter that is checked to see which libraries should be used at runtime.
