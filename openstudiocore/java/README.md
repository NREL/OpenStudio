# Java Support for OpenStudio

To compile with Java support, set the `BUILD_JAVA_BINDINGS` flag using ccmake

* Note: You may need to set `JAVA_HOME=<pathtojavainstall>` in the environment before calling cmake *

When building the java files, the OpenStudio.jar file must be accessible. When executing java class files with OpenStudio, the jar file, swig bindings libraries and openstudio libraries must all be accessible. This will take some combination of the java classpath and `LD_LIBRARY_PATH` or `PATH` environment variables.

The real world example of building the example `.java` file and executing it on my system while using the build folder:

```sh
jason@ubuntu12:~/OpenStudio-main/openstudiocore/java$ /usr/lib/jvm/java-1.7.0-openjdk-amd64/bin/javac TestOpenStudio.java -classpath ~/openstudio-main-build/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/java/OpenStudio.jar
jason@ubuntu12:~/OpenStudio-main/openstudiocore/java$ LD_LIBRARY_PATH=~/openstudio-main-build/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/java /usr/lib/jvm/java-1.7.0-openjdk-amd64/bin/java -classpath ~/openstudio-main-build/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/java/OpenStudio.jar:.  TestOpenStudio
```

Notes and caveats:

 * Only linux is tested at this point, but any Unix and Windows are expected to work (possibly with path finding issues on Windows)
 * The java libraries are not yet being packaged. Some time will need to be spend investigating the best place to install the bindings
 * The Java SWIG module is probably the least refined I have used yet. The namespacing is not handled well. It has all of the caveats of the C# bindings, and then some.

