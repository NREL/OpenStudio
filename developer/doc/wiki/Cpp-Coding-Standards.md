> [Wiki](Home) ▸ **C++ Coding Standards**

### Contents

1. [Introduction](#1-introduction)
2. [General Guidelines](#2-general-guidelines)
3. [External Dependencies](#3-external-dependencies)
4. [Formatting](#4-formatting)
5. [Project Layout](#5-project-layout)
6. [Naming](#6-naming)
7. [Classes](#7-classes)
8. [Functions](#8-functions)
9. [Other](#9-other)
10. [Portability](#10-portability)

## 1. Introduction
This coding standards documentation provides a starting point and reference for OpenStudio development. The purpose of this coding standard is to help existing developers, new team members, and subcontractors work together as a cohesive team to create high quality software. Requiring the use of certain language features while prohibiting or restricting the use of others is one aspect of this coding standard. The other aspect is enforcing common idioms, styles, and conventions to help developers understand each other's code. 

## 2. General Guidelines
These guidelines are not hard and fast rules, but rather general suggestions to keep in mind when making design decisions:

- Create branches for all work and use pull requests to allow review and discussion. Avoid long running branches if possible, keep your branch up to date with develop if it does last for a while.
- Aim for modularity and reuse. Put code in libraries as opposed to GUIs, break functionality into reusable pieces instead of large single-purpose chunks, and design interfaces to avoid duplicating code.
- Simplify public interfaces. Outward facing interfaces should not expose templates or advanced programming concepts unless absolutely necessary to keep the public C++ interface as similar as possible to the SWIG interfaces. Remember that the target audience for OpenStudio (users and developers, C++ and SWIG targets) includes engineers with little programming experience.
- Implement generic interfaces first to allow access to the largest amount of functionality. Provide specialized interfaces as needed later.
- Do not use advanced concepts unnecessarily. For example, objects which need to be shared by many other objects should be implemented with the Pimpl idiom, objects which do not need features offered by Pimpl should not use it.
- Write unit tests that outline the task first.  The task is complete when unit tests pass. This will help with scope creep, and keep our collection of unit tests up to date.
- Use const to signify that a given method is not designed to modify a given class or input parameter, but do not worry about absolute const correctness. Instead use a limited interface to assure that object state remains valid.
- Document all public classes and functions using Doxygen syntax.
- Alphabetize long lists for easy searching.
- Try to fit in.  Adopt the standards and conventions that you see in surrounding code.

### 3. External Dependencies
Dependencies impose a significant burden on developers and on the project, therefore decisions to add or update dependencies must be discussed by the entire team. Removing unneeded dependencies is encouraged.  To add a new dependency the following conditions must be met:

- Dependency license passes legal review for compatibility with OpenStudio license.
- Must be active open source project with commercial adoption.
- Need for new functionality outside of current dependencies must be justified.
- Must be approved by majority of OpenStudio team.

When selecting classes or items from a library to do a specific task we prefer to use more general libraries and tools first. If the functionality does not already exist in OpenStudio we prefer to use, in order, the C++ Standard Template Library and then Qt.  The use of boost is now discouraged. Small libraries or tools may be included directly in the main OpenStudio project and built using CMake. Large libraries or tools should be installed separately. Certain dependencies may not be required based on conditional configuration variables to reduce the burden on other developers.
 
### 4. Formatting

#### Indentation and Wrapping

Wrap lines at 150 columns. If a line needs to be split, then the split should occur at some functional location (e.g., before/after the operator) and not just after the 150th character.

Indentation level/tab character is two spaces as follows:

```c++
#ifndef MODEL_THERMALZONE_HPP // no indent
#define MODEL_THERMALZONE_HPP

namespace openstudio{ // no indent
namespace model{ // no indent

class MODEL_API ThermalZone : public HVACComponent // no indent
{
public: // no indent

  explicit ThermalZone(const Model& model); // 2 spaces

  virtual ~ThermalZone() {} // 2 spaces

  int multiplier() const; // 2 spaces

private: // no indent

  REGISTER_LOGGER("openstudio.model.ThermalZone"); // 2 spaces
};

// close each namespace on its own line, comment with name of namespace
} // model
} // openstudio

// close the include guard
#endif // MODEL_THERMALZONE_HPP
```

Functions declarations and definitions:

```c++
// Preferred
ReturnType ClassName::FunctionName(Type name1, Type name2)
{
  int a = 0; // 2 space indent
  ...
}

// You may wish to prefix input arguments with 't_' to distinguish them from local or member variables
ReturnType ClassName::FunctionName(Type t_name1, Type t_name2)
{
  int a = 0; // 2 space indent
  ...
}

// If there is too much text to fit on one line:
ReturnType ClassName::ReallyLongFunctionName(Type name1,
                                             Type name2,
                                             Type name3)
{
  int a = 0; // 2 space indent
  ...
}

// If you cannot fit even the first parameter:
ReturnType LongClassName::ReallyReallyReallyLongFunctionName(
    Type name1, // 4 space indent
    Type name2,
    Type name3) 
{
  int a = 0; // 2 space indent
  ...
}

// Const always goes on same line with closing ) and the last parameter
ReturnType LongClassName::ReallyReallyReallyLongConstFunctionName(
    Type name1, // 4 space indent
    Type name2,
    Type name3) const 
{
  int a = 0; // 2 space indent
  ...
}
```

__Details__

- The return type is always on the same line as the function name.
- The open parenthesis is always on the same line as the function name.
- There is never a space between the function name and the open parenthesis.
- The open curly brace may be at the end of the same line as the last parameter or on the next line.
- The closing curly brace is either on the last line by itself or (if other style rules permit) on the same line as the open curly brace.
- There should be a space between the close parenthesis and the open curly brace if they are on the same line.
- All parameters should be named, with identical names in the declaration and implementation.
- All parameters should be aligned if possible.
- Default indentation is 2 spaces.  Wrapped parameters have a 4 space indent.

#### Comments
Documentation is an important part of any software project. Even the best code is hard to understand or maintain if it is not properly commented. Developers are encouraged to insert as many comments as needed to explain the code.

Doxygen style syntax is preferred for documenting classes, methods, and free functions:

```c++
/// Brief description. More descriptive text follows the first period.

/** Brief description. More descriptive text follows the first period. This
 * format is a little more readable (in the code) when there are multiple
 * lines */
```

Such descriptive code blocks should be placed directly above the item being documented, when it is declared (in its header file). 

Doxygen provides basic mark-up commands to help with formatting, see http://www.stack.nl/~dimitri/doxygen/commands.html, and http://www.stack.nl/~dimitri/doxygen/htmlcmds.html for more information.

To avoid needlessly verbose documentation, we prefer to document input arguments and return types in the descriptive text, rather than using `\param`. Instead, focus on documenting pre- and post-conditions, exceptional situations, and providing example usage.  

Macros are called out in [Doxyfile.in](https://github.com/NREL/OpenStudio/blob/develop/openstudiocore/Doxyfile.in) for Doxygen to expand in place whenever it is used. Consider adding your macro to this list if you are having trouble documenting the use of your macro in the public interface.

A free function may be associated with a class by pre-pending its documentation with a `\relates` command. For example:

```c++
/** \relates WorkspaceObject
 *  Helper function to get the handles of a vector of objects */
UTILITIES_API std::vector<Handle> getHandles(const std::vector<WorkspaceObject>& objects);
```
causes the documentation for `getHandles` to show up on the `WorkspaceObject` (class) documentation page, rather than on the `openstudio` (namespace) page.

Implementation code should be well commented so that the intent of each step in the algorithm is clear. Use C++ style comments, i.e. the double-slash.  Do not use `/*` and `*/` in implementation code as this makes it difficult to comment out code for testing.  Periodically clean up old commented-out code which is no longer related to the surrounding "real" code.  Mark known issues with `//todo` comments and add your initials so other developers know who to ask questions related to a piece of particular code.

### 5. Project Layout

#### Project Files
We use CMake to generate Visual Studio projects and makefiles. Thus, our CMake files, as the definition of the OpenStudio project, are kept in the repository alongside the source files while Visual Studio projects and makefiles are generated files that should not be modified or checked in to the repository.  Any file generated as part of the build process should not be mixed in with the other source files and should not be checked in to the repository.

#### File Names
C++ header and source files are kept in the same directory. File names must start with a letter, and cannot be all uppercase. File names follow UpperCamelCase convention, and can include digits, but not special characters (including '.', '-', and '_'). Also, unit tests are stored in a `test` directory immediately beneath the tested source code.

Files are named corresponding to the class that they contain, e.g. the class `Building` is located in `Building.hpp` and `Building.cpp`. Typically we limit ourselves to one class per file. However, small helper classes may be defined alongside a primary object, e.g. `BuildingParameters` could also be defined in `Building.hpp` and `Building.cpp`. Impl classes should be declared in separate header files, e.g. `Building_Impl.hpp`, to signify that users should not access these classes.  However, Impl classes may be implemented in the same source file as the non-Impl class for compactness.

We use the following file extensions to differentiate between file types as well as whether or not the file is autogenerated. Autogenerated files should not be modified by hand as they are rebuilt automatically during the build process. Ensure that autogenerated files are deleted when the clean target is built and that they are generated in the build rather than the source directory. Additionally, unit tests and test fixtures end in `_GTest.hpp`, `_GTest.cpp`, `Fixture.hpp`, or `Fixture.cpp` respectively so they may be excluded from code coverage analysis.

- C header files — .h
- C source files — .c
- C++ header files — .hpp
- C++ source files — .cpp
- Swig interface files — .i
- Autogenerated header (C or C++) files (do not edit) — .hxx
- Autogenerated source (C or C++) files (do not edit) — .cxx
- Autogenerated Swig interface files (do not edit) — .ixx
- Unit test files — \*\_GTest.hpp or \*\_GTest.cpp
- Unit test fixtures — \*Fixture.hpp or \*Fixture.cpp

#### Header Files
Every header file must contain an "include guard" that prevents it from being included multiple times. Traditional include guards are thought to be more portable than `#pragma once` syntax. Place an include guard macro at the top and bottom of every header file. The include guard defines a symbol which mirrors the entire file path from the source directory, ensuring that it will be unique across the entire project.

```c++
#ifndef UTILITIES_CORE_STRING_HPP
#define UTILITIES_CORE_STRING_HPP
// ...
#endif //UTILITIES_CORE_STRING_HPP
```

Prefer to use forward declarations rather than include statements in header files whenever possible.  This reduces the amount of code that must be compiled when changing any header file.

Include files in the source tree using include statements relative to the current source file. For example, `#include "AirGap.hpp"` rather than `#include <model/AirGap.hpp>` and `#include "../utilities/units/Unit.hpp"` rather than `#include <utilities/units/Unit.hpp>`.  To include generated files in the build directory you must use includes with search path relative to the included build directory, e.g. `#include <utilities/idd/OS_Material_AirGap_FieldEnums.hxx>`.

List include files in the following order (alphabetizing within each group if the group is large):

1. Header files being implemented in this .cpp file.
2. Header files from the same project.
3. Other OpenStudio header files.
4. External headers files, grouped by namespace (boost, std, etc.).

__Do not__:

- Use any using directives in any header file at the file level.
- Use any namespace aliases in any header file at the file level.
- Use any using directives to expose all names in a namespace, e.g. `using namespace std`.

__You may__:

- Use a class or function-specific using declaration anywhere in a source file, e.g. `using std::vector`.
- Use a class or function-specific using declaration in functions, methods, or classes in .hpp files (below file scope).
- Use a namespace alias anywhere in a source file, e.g. `namespace building = openstudio::building`.
- Use a namespace alias in functions, methods or classes in .hpp files (below file scope).

Implementing code in header files (other than template code) is discouraged as any changes to that code may require compilation of a large number of other files.   

#### Doxygen Mainpages
Each OpenStudio sub-project is documented by a special header file, `mainpage.hpp` that lives in that project's folder under `src`, and may be listed in the corresponding `CMakeLists.txt` file to make it easily editable through the developer environment. For large sub-projects (like `utilities`), there may also be use for one or more `page.hpp` files. Both types of files should primarily consist of one large code block surrounded by the namespace declaration of the project. For example:

```
namespace openstudio {
/** \mainpage OpenStudio Utilities
 *
 *  The utilities library provides common functionality used throughout OpenStudio.
 *  OpenStudio's utilities live in the top level namespace of OpenStudio, openstudio.
 *  The following sub projects are defined:
 *  ...
 */
} // openstudio
```

Placing the comment block in the namespace of the project being documented helps Doxygen link to related documentation. If there are items in a sub-namespace that need to be referred to, you can use the syntax `\link foo::Bar Bar\endlink`. Mainpages show up as `index.html` in the generated HTML documentation, and are the top-most link in the navigation pane.

Following the above `mainpage.hpp` example, `page.hpp` files should start with a `\page` command, which differs in syntax from `\mainpage` in that it takes a label argument in addition to a title:

```
/** \page idd_page OpenStudio Idd
```

The reference tag (`idd_page` in the example) can then be used in that library's documentation to provide a link to the page:

```
\ref idd_page
```

The `\section` command, which should be used in both `mainpage.hpp` and `page.hpp` files to break up the text into logical segments follows a similar convention, and its labels are also available through the `ref` command.

### 6. Naming

#### Naming Descriptiveness
Namespace, class, variable, and function names should be sufficiently descriptive that they convey their meaning and typical usage. We prefer to spell out words rather than use abbreviations to maintain clarity and readability. Names should be as specific to the context as possible, e.g. `Coordinate` rather than `Number`. We have seen issues with SWIG when class names are not globally unique, e.g. `openstudio::model::Building` and `openstudio::contam::Building`.  Therefore, class names and function names in the public API should be unique across all OpenStudio namespaces.

#### Namespaces
- The top level namespace for OpenStudio code is `openstudio`. All classes and functions should be placed in the `openstudio` namespace. All macro definitions are written in the global namespace.
- Namespaces are all lowercase so they can be easily distinguished from classes. For instance, `openstudio::energyplus::ReverseTranslator` indicates the `ReverseTranslator` class located in the namespace `openstudio::energyplus`.
- Namespace structure includes openstudio plus the first level of source code hierarchy, except that utilities is not used as a namespace (`openstudio::IdfFile` rather than `openstudio::utilities::IdfFile` or `openstudio::utilities::idf::IdfFile`), because classes in utilities are used widely throughout OpenStudio.
- Use the `detail` namespace to hide classes or functions which are not meant to be part of the public interface (e.g. `openstudio::idd::detail::IddObject_Impl`).

#### Other Naming Conventions
- Classes are upper camel case, e.g. `DetailedGeometry`
- Member functions and local variables are lower camel case, e.g. `addVertex()` and `timeOfDay`
- Private member variables are lower camel case prefixed with `m_`, e.g. `m_name`. Private member functions are not prefixed with `m_`, member variables which are function pointers are prefixed by `m_`.
- Implementation classes using the Pimpl idiom are postfixed by `_Impl`, e.g. `IddFile_Impl`.
- Enumerations and enumerated items are upper camel case, e.g. `enum Items{SmallItem, BigItem}` not `enum Items{smallItem, bigItem}`
- Unit test cases and fixtures are upper camel case. To ensure that test cases are easily recognizable in standard output, use `_` to indicate spaces, e.g. `TEST_F(ModelFixture, Building_Constructor)`.
- For "getter" type methods (e.g. to get a name) we prefer `object.name()` to `object.name` or `object.getName()`, unless the method accepts an argument. In that case, we prefer a `get` prefix to clarify the verb associated with the argument(s). For example `object.getVariable(const std::string& varName)` is preferred to `object.variable(const std::string& varName)`. The `get` prefix applies even when the argument is an index into an array. For example, `object.getFoo(unsigned index)` is preferred to `object.foo(unsigned index)`.
- For "setter" type methods (e.g. to set a name) we prefer `object.setName(const std::string& name)` to `object.name(const std::string& name)`.

### 7. Classes

#### Object Oriented Design
We encourage the use of Object Oriented (OO) design because it helps encapsulate behavior and logic. Care should be taken to limit the coupling between classes so that they can be used in more general ways than originally anticipated.

#### Class Definitions
- The protection of every base class should be specifically stated. When one class is derived from another, the default protection is private. Example:

```c++
class D : B {}; // D is privately derived from B
```

This is counter-intuitive as public inheritance is more commonly used. Thus, the compiler will generate a warning for unspecified protection levels. It is best to specifically state whether the inheritance is public, protected, or private. Example:

```c++
class D : public B {}; // correct
```

Most designs require only public inheritance, any exceptions to this should be thoroughly documented.

To provide a common layout of the interface a class offers, the definition of the class should contain the following, in this order:

1. declaration of any friend functions and classes
2. the `public:` specifier
3. declarations of any static public functions
4. declarations of any static public data members
5. declarations of any public member functions
6. declarations of any public data members
7. the `protected:` specifier
8. declarations of any static protected functions
9. declarations of any static protected data members
10. declarations of any protected member functions
11. declarations of any protected data members
12. the `private:` specifier
13. declarations of any static private functions
14. declarations of any static private data members
15. declarations of any private member functions
16. declarations of any private data members

#### Inheritance and Virtual Functions
- Every class with one or more virtual functions should have a virtual destructor. Since a derived class object may be deleted through a base class pointer, the base class must have a virtual destructor to ensure that the derived class destructor gets called. Otherwise, only the base class destructor will be called, resulting in a probable memory leak.
- A derived class should not redefine a default parameter for a virtual function.
- A derived class should not redefine an inherited non-virtual function.

#### Friends
- Since friends violate encapsulation and information hiding, they should be avoided whenever accessor functions can be used just as efficiently.
- Classes that require some amount of friendship with other classes should consider whether the scope of the friendship can be limited to certain methods.

#### Nested Classes
Nested classes or structs other than enumerations are not well supported by SWIG and should not be used in the public API.

#### Initialize Members
Initialize class members using the initializer list.

#### Public Members
In general, member data should not be public; member data  should be private and accessed through public member functions.  Simple structs may choose to use public members but even this may be dangerous if there is not a constructor which initializes the members correctly.

#### SWIG Support
All C++ types in the public API should have a definition available to SWIG. This will produce the most effective and usable target language wrappers. To accomplish this goal, a few things should be kept mind.

__Exported templates__

Template classes must be explicitly instantiated and exported in SWIG. In general, specific uses of a template class that are common enough to have a typedef should be exported. The same name as the typedef should be used for consistency.

__Limit the Number of Types Exposed__

The SWIG maintenance burden is primarily driven by the number of types that are exposed. Therefore, be judicious in choosing types to export.

__Be Careful about STL Types__

The STL should be relied on as heavily as possible for yielding portable and reliable code. However, not all STL types are supported by all SWIG target languages, so the STL types utilized should be carefully chosen. A reading of the .i files provided for each target language in the SWIG distribution (http://sourceforge.net/p/swig/code/HEAD/tree/trunk/Lib/ruby/, http://sourceforge.net/p/swig/code/HEAD/tree/trunk/Lib/csharp/) provides the most reliable and accurate information about which types have shipped support. Choose the minimum set of all types that are supported across all languages you want to target.

All STL container types support easy conversions between themselves via iterators. Therefore, it is possible that using `std::deque` internal to the application is the best choice while having accessors that return `std::vector`.

```c++
std::vector<MyType> MyClass::getdata() 
{
  return std::vector<MyType>(m_deque.begin(), m_deque.end());
}
```

__Use the Pimpl Idiom__

A common C++ technique known as the Pimpl Idiom can be used to provide a streamlined interface to a complex, internal class (http://c2.com/cgi/wiki?PimplIdiom, Sutter and Alexandrescu (2004)). This technique is preferred for OpenStudio classes that require both SWIG export and features primarily supported by pointers (polymorphism, data sharing).

### 8. Functions

#### Function Overloading
Except for constructors, function overloading should be reserved for functions in which the meaning of the overload is clear. Overloading and calling overloaded functions can decrease readability because it may not be clear to the reader which version of the function is being called. Functions should not be overloaded on the const-ness of the input arguments unless absolutely necessary and thoroughly documented.

#### Passing Arguments
- Pass `double`, `int`, `unsigned`, and `bool` as such, no need for const modifiers or passing by reference (passing primitives by value is more efficient).
- Pass all other function arguments as `const&` references whenever possible to avoid unnecessary copying.

#### Return Values
For getter functions that may fail we prefer to return unset optionals rather than throw exceptions:

```c++
// Getter can fail
boost::optional<double> MyClass::foo() const
{
  if (m_foo > 0.0){
    return m_foo;
  }
  return boost::none;
}
```

For setter functions that may fail we return a bool, if the setter always succeeds then we return void:

```c++
// Setter can fail
bool MyClass::setFoo(double foo)
{
  if (foo > 0.0){
    m_foo = foo;
    return true;
  }
  return false;
}

// Setter cannot fail
void MyClass::setBar(double bar)
{
  m_bar = bar;
}
```

When returning multiple values from functions we prefer to return an object or struct. Alternatively, pair or tuple types may be returned provided that the meaning of each member of return type is well documented. We do not suggest returning multiple values by passing in reference arguments:

```c++
// Double the value and return it.
double doubleFunc(double x)
{
  double result = 2.0*x;
  return result;
}

// Double the values and return them in custom struct
MyStruct doubleFunc(double x, double y, double z)
{
  MyStruct result;
  result.x = 2.0*x;
  result.y = 2.0*y;
  result.z = 2.0*z;
  return result;
}

// Double the values and return them in a tuple, order is fairly clear
std::tuple<double, double, double> doubleFunc(double x, double y, double z)
{
  return std::make_tuple(2.0*x, 2.0*y, 2.0*z);
}

// Passing by reference is not preferred and must be thoroughly documented:
void doubleFunc(double& x, double& y, double& z)
{
  x *= 2.0;
  y *= 2.0;
  z *= 2.0;
}

// Don't do this!!  Mixing return values and pass by reference is confusing:
double doubleFunc(double x, double& y, double& z)
{
  double result = 2.0*x;
  y *= 2.0;
  z *= 2.0;
  return result;
}
```

Consider returning a const reference to a vector or large data member stored in a class:

```c++
class BigObject
{
public:

  // return a const reference to the vector, consider this a const method
  // it may be better to return a copy and not a reference for multi-threading
  // applications
  const std::vector<Object>& objects() const {return m_objects;}

 private:

  std::vector<Object> m_objects;
};
```

#### Const-Correctness
Use const whenever possible (Meyers 2005; Sutter and Alexandrescu 2004).

__Details__

Effective C++ suggests using const for any value that should never be modified, to provide compile-time guarantees against logic errors. Example:

```c++
void foo (const std::vector<int> &v)
{
  const size_t size = v.size(); // Cache size in a const value
  ... // use size_t as a cached value
}
```

Similarly, any methods that should not modify object data, such as "getter" methods should be declared const:

```c++
class MyType
{
 public:
  int value() const; // const getter

  void setValue(int t_value); // non-const setter
 private:
  int m_value;
};
```

Getter methods on custom containers that return references to internal data need to provide both const and non-const versions to ensure that the container can be used in as many contexts as possible.

Standard containers such as std::vector are implemented in this way.

```c++
class Container
{
  ...
  Type &operator[](int index) { ... }
  const Type &operator[](int index) const { ... }
  ...
};
```

__Exceptions__

Do not provide both const and non-const overloads for parameters. The subtleties for when one version would be chosen over another are nuanced, and the meaning of the function is likely unclear:

```c++
// Big don't: Functions overloaded by argument constness
void doSomething(const std::vector<int> &p);
void doSomething(std::vector<int> &p);
```

Similarly, do not implement both const and non-const versions of templated types, unless the design specifically needs it. The danger is that the number of permutations would grow too quickly and the meaning of the code being called is likely lost and the implementation is probably duplicated:

```c++
// Similar don't (unless absolutely necessary) do this:
void doSomething (const std::shared_ptr<Data> &p);
void doSomething (const std::shared_ptr<const Data> > &p);
```

Also:

```c++
// Unnecessary except in the most strictly defined code:
std::shared_ptr<Data> Class::getData();
std::shared_ptr<const Data> Class::getData() const;
// just do this instead (operation does not change Class):
std::shared_ptr<Data> Class::getData() const;
```

__SWIG__

Even though SWIG discards const qualifiers (http://swig.org/Doc2.0/SWIGDocumentation.html#SWIGPlus_const), we should not let that modify our use of const. Our exposed C++ will rarely, if ever, live in isolation. We want to make the most use of the C++ type system; appropriate application of `const` can help us find logic errors in our code at compile time (Sutter and Alexandrescu 2004).

We should be aware (and document where possible) that if both `const` and non-`const` versions of a function exist the `const` version will likely never be called from within our SWIG code.

### 9. Other

#### Strings
We are using `std::string` to store all character data and in all public APIs. We assume that all `std::string` objects are UTF-8 encoded. String conversion functions are provided [`utilities/core/String.hpp`](https://github.com/NREL/OpenStudio/blob/develop/openstudiocore/src/utilities/core/String.hpp). Other types of strings (such as `QString` or `std::wstring`) may be used for implementation purposes but should not be exposed in the public APIs.

#### Paths
Use the typedef `openstudio::path` in [`utilities/core/Path.hpp`](https://github.com/NREL/OpenStudio/blob/develop/openstudiocore/src/utilities/core/Path.hpp) for all operations which access the file system.

__UPDATE__: OpenStudio will likely be changing this typedef from `boost::filesystem::path` to `QFileInfo` in the future, stay tuned....

#### Typedefs
It is acceptable to use typedefs to shorten the name of templates. In order to provide a consistent look and feel `std::vector`'s of objects should be suffixed with `Vector` (e.g. `IddKeyVector`), `std::shared_ptr` should be suffixed with `Ptr` (e.g. `IddKeyPtr`), and `boost::optional` prefixed with `Optional` (e.g. `OptionalIddKey`).  These names will correspond to the class names given to template instantiations exported by SWIG.

__Exception__: Use of these convenience typedefs is encouraged in all settings except in public API header files, where we prefer to use the full type name as this helps Doxygen and SWIG.

##### Macros
The use of macros is discouraged if the same effect can be achieved with another method that is easier to debug and maintain. Any macros deemed necessary must be well commented to help with debugging and maintenance burden. If there are parameters, then the comment must describe them, including their type. The comment must describe what the macro is supposed to do. Example usage should be provided to clarify the intended use. If there are any side effects associated with the macro (such as incrementing a global variable, the need for certain local identifiers to be defined, etc), then list them.

#### Memory Management
Avoid using `malloc`, `free`, or `delete`. Use smart pointers instead, and only call `new` within a smart pointer constructor. This leaves memory management to the smart pointer and helps avoid crashes and memory leaks.  The only exception to this is when using Qt's parent/child system for memory management (parent's delete children on destruction).  If you are using this system, make sure to set the parent on any object that you create with `new`.

#### Flow Control
All control structures must have braces surrounding the body of the code within the structure, even if the body is only one statement, or even no statements. The body must be indented relative to the control statement.

All switch statements must have a default case at the bottom even if its only purpose is to report an error. In C++, the switch statement must be manually exited with a break statement, otherwise execution continues to fall through. Usually, falling through is undesirable behavior. If falling through is actually desired for a particular switch statement, that case must be commented well to explain why falling through is required.

#### Never begin a preprocessor definition or identifier with _

If you do, you risk broaching on names reserved for implementation use: http://stackoverflow.com/questions/228783/what-are-the-rules-about-using-an-underscore-in-a-c-identifier.

#### Use nullptr

C++11 introduces `nullptr` which is a special type denoting a null pointer value. This should be used instead of 0 or NULL to indicate a null pointer.

#### Use auto

C++11 introduces `auto` which is a special keyword which evaluates to the correct type.  This feature may be used in cases where the type of the variable is clearly understood and no specific type conversions are required.  The auto keyword should not be used as the return type in a function declaration.

#### Limit Variable Scope

Variables should be declared as late as possible, and ideally, only when it's possible to initialize the object. Reduced variable scope results in less memory being used, more efficient code in general, and helps the compiler optimize the code further.

```c++
// Good idea
for (int i = 0; i < 15; ++i) {
  MyObject obj(i);
  // do something with obj
}

// Bad Idea
MyObject obj; // meaningless object initialization
for (int i = 0; i < 15; ++i) {
  obj = MyObject(i); // unnecessary assignment operation
  // do something with obj
}
// obj is still taking up memory for no reason
```

#### Prefer ++i to i++

Pre-increment is faster then post-increment because it does not require a copy of the object to be made.

#### Exceptions
We allow our code to throw exceptions. However, the number of exceptions thrown should be limited to the absolute minimum required. Exceptions should not be used to pass expected information between components and should not be used as "convenient" methods for changing program execution paths. An exception means something very bad has happened and the code cannot continue on in the manner it was called. All thrown exceptions must be logged for debugging purposes; use of the `LOG_AND_THROW` or `LOG_FREE_AND_THROW` macros is encouraged. Valid situations to throw exceptions include object constructors called with parameters that would invalidate an object's state (e.g. conversion from string to number fails) or failed attempts to access resources (e.g. missing files or out of memory errors). All exceptions thrown in our code must be derived from `std::exception`, and should not be identically equal to `std::exception` (because gcc does not recognize `std::exception(const char*)`). Additionally, all methods that throw exceptions must be documented as such; it is assumed that unmarked methods do not throw. Because the C++ language does not provide a method to enforce the types of exceptions thrown at compile time we mark exception throwing methods with comments like `/** throws ExceptionType1, ExceptionType2, ... */`.

Function calls to other libraries that may throw exceptions under relatively normal circumstances (`boost::filesystem` comes to mind) must be wrapped in a try/catch block. If an exception is caught it may be re-thrown following the above guidelines.

#### Logging
Log messages alert users of unexpected conditions and assist developers with debugging. Several convenient logging macros are provided. All classes should use the macro `REGISTER_LOGGER("my.log.channel")` within their private scope to register a logging channel associated with the class. The log channel name is hierarchical with levels separated by `.`. Each object should scope its log to its namespace and class name. For instance the class `openstudio::model::Building` should use channel `openstudio.model.Building`. The following logging levels are available:

1. Debug - detailed information for developers to use in debugging
2. Info - similar to Debug, but at a higher level, and possibly of interest to super-users
3. Warn - a situation which is likely the result of bad input or will result in suspect output and should
4. Error - a situation which is definitely the result of bad input, will result in bad output, and must be brought to the attention of the user
5. Fatal - the program cannot continue

Logging macros are also provided for Free functions. However, there is no `REGISTER_LOGGER` macro for free functions so they must use the macros `LOG_FREE`, which requires an additional argument to specify the logging channel. The channel should be the namespace and then function name. So function `int openstudio::model::foo()` would log to `openstudio.model.foo`. The following example shows how to use the logging macros:

```c++
namespace foo {

// A class with logging
class Bar 
{

public:
  Bar() {}
  void doSomething();

private:
  REGISTER_LOGGER("foo.Bar");
};

// using logging in a member of a class with logging
Bar::doSomething()
{
  // appropriate debug statement
  LOG(Debug, "Entering Bar::doSomething");

  bool trouble = isTrouble();
  if (trouble) {
    // appropriate error statement
    LOG(Error, "Ran into trouble");
  }

  // using stream operators in a log statement
  LOG(Debug, "Leaving Bar::doSomething, trouble = " << trouble);
}

// using logging in a free function
bool isTrouble()
{
  int a = 1;
  int b = 1;
  int sum = a + b;

  // first argument is the channel, second is the log message
  LOG_FREE(Debug, "foo.isTrouble", "Entering foo::isTrouble");

  bool result = false;
  if ((a != b) || (sum != 2)) {
    LOG_FREE(Error, "foo.isTrouble", "Trouble detected, a = " << a <<
                    ", b = " << b << ", sum = " << sum);
    result = true;
  }

  LOG_FREE(Debug, "foo.isTrouble", "Leaving foo::isTrouble, " <<
                  "result = " << result);
  return result;
}
```

### 10. Portability
The following simple rules will help code move more easily from one target platform to another. Word lengths, byte and word order, alignment, argument evaluation, signs, and sign extensions are the major causes of portability problems. To avoid such issues:

- Avoid casting variables.
- Avoid assumptions about signed variables (especially characters). Be very careful when mixing signed and unsigned variables and when using the modulus operator.
- Use standardized functions, macros, and typedefs porting inconsistencies.
- Avoid code that depends on the evaluation order of function arguments.
- All characters that are used in non-arithmetic cases (bit fields, etc.) must be declared to be unsigned.

Intentional non-portable code should be well documented and separated from portable code. For example, if a certain piece of functionality requires several lines of non-portable code, then the developer should put the non-portable code in a separate function, method or macro and invoke it from the primary function.

__Standard libraries__

Prefer the C++ standard libraries unless operating system specific calls are required. Place all operating system specific implementations in one or more classes that can be easily replaced at compile time, centralizing
all of your OS specific code.

__Multiple Compilers__

Developers are encouraged to use multiple compilers to ensure portability. Furthermore, an extra level of confidence can be gained by knowing that your code generates few or no warnings at high levels across multiple platforms.

__C+11__

The use of c++11 is now allowed in OpenStudio C++ code.

#### Compiler Warnings
Compiler warnings should be enabled to the highest "normal" levels. Compiler warnings should be eliminated before code release.

__Details__

All warnings that the compiler authors think are important should be enabled. They will often help you find portability and logic problems. For example, in Visual C++ at least warning level 3 `/W3` should be enabled, as it is recommended for production code (http://msdn.microsoft.com/en-us/library/thxezb7y.aspx). Similarly, `-Wall` warnings should be enabled in G++ (http://gcc.gnu.org/onlinedocs/gcc-4.4.2/gcc/Warning-Options.html). An interesting option for G++ that is not enabled with `-Wall` is `-Weffc++`, which generates warnings for programming standards specified in Meyers (2005).