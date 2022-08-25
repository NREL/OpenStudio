# OpenStudio Ruby

OpenStudio's Ruby implementation is divided into several pieces

* `interpreter`: This is the bare bones Ruby interpreter with only some very minimal OpenStudio
initializations and abstractions. There are two separate cmake targets related to the interpreter.

    * The target `rubyinterpreter-declarations` is an interface library that merely adds Ruby to the include path. This is used by the standalone ruby bindings.
    * The target `rubyinterpreter` adds Ruby to the include paths, but also links to the Ruby library. This is used by the OpenStudio Ruby engine.

* `bindings`: This contains an interface to initialize the OpenStudio API bindings for Ruby. (e.g. `init_openstudio_model`)
The `module` and `engine` components consume this.

* `module`: This is the conventional OpenStudio Ruby bindings, for loading into a standalone Ruby interpreter.

* `engine`: This is a combination of the OpenStudio Ruby bindings, the Ruby interpreter, and embedded files
to be used by the OpenStudio cli to run Ruby based workflows.
