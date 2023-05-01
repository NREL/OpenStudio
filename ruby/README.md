# OpenStudio Ruby

OpenStudio's Ruby implementation is divided into several targets

* `bindings`: This contains an interface to initialize the OpenStudio API bindings for Ruby. (e.g. `init_openstudio_model`)
The `module` and `engine` components consume this.

* `module`: This is the conventional stand-alone OpenStudio Ruby bindings, for loading into a standalone Ruby interpreter.

* `engine`: This is a combination of the OpenStudio Ruby bindings, the Ruby interpreter, and embedded files
to be used by the OpenStudio cli to run Ruby based workflows.
