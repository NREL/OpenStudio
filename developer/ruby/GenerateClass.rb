########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

######################################################################
# == Synopsis
#
#   Generate starter files for an OpenStudio class.
#
# == Usage
#
#   ruby GenerateClass.rb -s "model" -c "Construction" -b "ConstructionBase" -o "." -p --no-qobject -i "OS:Construction"
#
######################################################################

require 'optparse'

require_relative 'SubProjectClassGenerators/SubProjectClassGenerator.rb'
require_relative 'SubProjectClassGenerators/ModelClassGenerator.rb'
require_relative 'SubProjectClassGenerators/ProjectClassGenerator.rb'
require_relative 'SubProjectClassGenerators/AnalysisClassGenerator.rb'

# HANDLE INPUT ARGUMENTS

# define the input parameters
options = Hash.new
optparse = OptionParser.new do |opts|

  opts.on( '-s', '--sourceDirectory SOURCEDIRECTORY', String, "Directory under src to which this class belongs") do |sourceDirectory|
    options[:sourceDirectory] = sourceDirectory
  end

  opts.on( '-c', '--className CLASSNAME', String, "Name of the class to be generated" ) do |className|
    options[:className] = className
  end

  opts.on( '-b', '--baseClassName BASECLASSNAME', String, "Name of the base class for the class to be generated" ) do |baseClassName|
    options[:baseClassName] = baseClassName
  end

  opts.on( '-o', '--outputDirectory OUTPUTDIRECTORY', String, "Directory to which the generated files should be saved") do |outputDirectory|
    options[:outputDirectory] = outputDirectory
  end

  opts.on( '-p', '--[no-]pImpl', "Use the pImpl idiom") do |pImpl|
    options[:pImpl] = pImpl
  end

  opts.on( '-q', '--[no-]qobject', "Make the class a QOBJECT") do |qobject|
    options[:qobject] = qobject
  end

  opts.on( '-i', '--iddObjectType IDDOBJECTTYPE', String, "IddObjectType to be wrapped by the ModelObject class being generated (ignored if not model sourceDirectory)") do |iddObjectType|
    options [:iddObjectType] = iddObjectType
  end

end

# parse the input parameters
optparse.parse!

# check for required parameters
if not options[:className] or not options[:sourceDirectory]
  puts optparse
  exit
end

# get options out
sourceDirectory = options[:sourceDirectory].to_s
className = options[:className].to_s
baseClassName = String.new
if options[:baseClassName]
  baseClassName = options[:baseClassName].to_s
end
outputDirectory = "."
if options[:outputDirectory]
  outputDirectory = options[:outputDirectory].to_s
end
pImpl = options[:pImpl]
qobject = options[:qobject]
iddObjectType = options[:iddObjectType].to_s

summary = "\nWriting files for class " + sourceDirectory + "/" + className
if not baseClassName.empty?
  summary += ", which derives from " + baseClassName + ","
end
summary += " to " + outputDirectory + ".\n\n"
puts summary
if pImpl
  puts className + " follows the pointer to implementation idiom.\n\n"
end
if qobject
  puts className + " is a QOBJECT.\n\n"
end
if not iddObjectType.empty?
  puts className + " wraps IddObjectType '" + iddObjectType + "'.\n\n"
end

# START ALL FILES

fileHeader = String.new
fileHeader << "/***********************************************************************************************************************\n"
fileHeader << " *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.\n"
fileHeader << " *\n"
fileHeader << " *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the\n"
fileHeader << " *  following conditions are met:\n"
fileHeader << " *\n"
fileHeader << " *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following\n"
fileHeader << " *  disclaimer.\n"
fileHeader << " *\n"
fileHeader << " *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the\n"
fileHeader << " *  following disclaimer in the documentation and/or other materials provided with the distribution.\n"
fileHeader << " *\n"
fileHeader << " *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote\n"
fileHeader << " *  products derived from this software without specific prior written permission from the respective party.\n"
fileHeader << " *\n"
fileHeader << " *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative\n"
fileHeader << " *  works may not use the \"OpenStudio\" trademark, \"OS\", \"os\", or any other confusingly similar designation without\n"
fileHeader << " *  specific prior written permission from Alliance for Sustainable Energy, LLC.\n"
fileHeader << " *\n"
fileHeader << " *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES,\n"
fileHeader << " *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n"
fileHeader << " *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR\n"
fileHeader << " *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,\n"
fileHeader << " *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED\n"
fileHeader << " *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n"
fileHeader << " *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
fileHeader << " **********************************************************************************************************************/\n"

hpp = String.new
cpp = String.new
hpp << fileHeader << "\n"
cpp << fileHeader << "\n"

implHpp = String.new
cppPublicClass = String.new
if pImpl
  implHpp << fileHeader << "\n"
end

gtest = String.new
gtest << fileHeader << "\n"
gtest << "#include <gtest/gtest.h>\n\n"
saveGTest = false

aux = String.new
saveAux = false

# BEGIN INCLUDE GUARDS

re = Regexp.new('(\w+)(?:\/(\w+))?')
m = sourceDirectory.match(re)
sourceFolders = m[1,(m.size-1)]

defineString = String.new
sourceFolders.each { |folder|
  defineString += folder.upcase + "_" if folder != nil
}
defineString += className.upcase
hpp << "#ifndef " << defineString + "_HPP\n"
hpp << "#define " << defineString + "_HPP\n\n"

if pImpl
  implHpp << "#ifndef " << defineString + "_IMPL_HPP\n"
  implHpp << "#define " << defineString + "_IMPL_HPP\n\n"
end


# GET CLASS METHOD GENERATOR OBJECT

methodGenerator = SubProjectClassGenerator.new(className, baseClassName, pImpl, qobject)
if sourceFolders[0] == "model"
  methodGenerator = ModelClassGenerator.new(className, baseClassName, pImpl, qobject, iddObjectType)
elsif sourceFolders[0] == "project"
  methodGenerator = ProjectClassGenerator.new(className, baseClassName, pImpl, qobject)
elsif sourceFolders[0] == "analysis"
  methodGenerator = AnalysisClassGenerator.new(className, baseClassName, pImpl, qobject)
end


# INCLUDES

subprojectInTitleCase = sourceFolders[0].gsub(/\b\w/){$&.upcase}

hpp << "#include <" << sourceFolders[0] << "/" << subprojectInTitleCase << "API.hpp>\n"
cpp << "#include \"" << className << ".hpp\"\n"
if pImpl
  implHpp << "#include <" << sourceFolders[0] << "/" << subprojectInTitleCase << "API.hpp>\n"
  cpp << "#include \"" << className << "_Impl.hpp\"\n\n"
end

if not baseClassName.empty?
  hpp << "#include \"" << baseClassName << ".hpp\"\n\n"
  if pImpl
    implHpp << "#include \"" << baseClassName << "_Impl.hpp\"\n\n"
  end
end

hpp << methodGenerator.hppIncludes
implHpp << methodGenerator.implHppIncludes
cpp << methodGenerator.cppIncludes

if baseClassName.empty?
  hpp << "\n"
  if qobject and not pImpl
    hpp << "#include <QObject>\n\n"
  end
  if pImpl
    hpp << "#include <memory>\n"
  end
  hpp << "#include <boost/optional.hpp>\n\n"
  hpp << "#include <vector>\n\n"

  if pImpl
    implHpp << "\n"
    if qobject
      implHpp << "#include <QObject>\n\n"
    end
    implHpp << "#include <memory>\n"
  end
end


# BEGIN NAMESPACES

hpp << methodGenerator.hppExternalForwardDeclarations
hpp << "namespace openstudio {\n"
hpp << methodGenerator.hppOSForwardDeclarations
hpp << "namespace " << sourceFolders[0] << " {\n\n"
hpp << methodGenerator.hppSubProjectForwardDeclarations

cpp << "namespace openstudio {\n"
cpp << "namespace " << sourceFolders[0] << " {\n\n"


if pImpl
  hpp << "namespace detail {\n\n"
  hpp << "  class " << className << "_Impl;\n\n"
  hpp << "} // detail\n\n"

  implHpp << methodGenerator.implHppExternalForwardDeclarations
  implHpp << "namespace openstudio {\n"
  implHpp << methodGenerator.implHppOSForwardDeclarations
  implHpp << "namespace " << sourceFolders[0] << " {\n\n"
  implHpp << methodGenerator.implHppSubProjectForwardDeclarations
  implHpp << "namespace detail {\n\n"

  cpp << "namespace detail {\n\n"
end


# PRE-CLASS ITEMS

hpp << methodGenerator.hppPreClass
implHpp << methodGenerator.implHppPreClass
cpp << methodGenerator.cppPreClass
cppPublicClass << methodGenerator.cppPublicClassPreClass


# START CLASSES

hpp << "class " << sourceFolders[0].upcase << "_API " << className
if baseClassName.empty?
  if qobject and not pImpl
    hpp << " : public QObject"
  end
else
  hpp << " : public " << baseClassName
end
hpp << " {\n"
if qobject and not pImpl
  hpp << "  Q_OBJECT;\n"
end

if pImpl
  implHpp << "  class " << sourceFolders[0].upcase << "_API " << className << "_Impl"
  if baseClassName.empty?
    implHpp << " : "
    if qobject
      implHpp << "public QObject, "
    end
    implHpp << "public std::enable_shared_from_this<" << className << "_Impl>"
  else
    implHpp << " : public " << baseClassName << "_Impl"
  end
  implHpp << " {\n"
  implHpp << "    Q_OBJECT;\n" if qobject
end


# QMACROS

hpp << methodGenerator.hppQMacros
implHpp << methodGenerator.implHppQMacros


# PUBLIC TYPEDEFS

hpp << " public:\n"

if pImpl
  implHpp << "   public:\n"
end

hpp << methodGenerator.hppPublicTypedefs
implHpp << methodGenerator.implHppPublicTypedefs


# CONSTRUCTORS

hpp << "  /** @name Constructors and Destructors */\n"
hpp << "  //@{\n\n"

if pImpl
  implHpp << "    /** @name Constructors and Destructors */\n"
  implHpp << "    //@{\n\n"
end

hpp << methodGenerator.hppConstructors
implHpp << methodGenerator.implHppConstructors
cpp << methodGenerator.cppConstructors
cppPublicClass << methodGenerator.cppPublicClassConstructors


# VIRTUAL DESTRUCTORS

hpp << "  virtual ~" << className << "() {}\n\n"
hpp << "  //@}\n\n"

if pImpl
  implHpp << "    virtual ~" << className << "_Impl() {}\n\n"
  implHpp << "    //@}\n"
end


# PUBLIC METHODS

hpp << methodGenerator.hppPublicMethods
implHpp << methodGenerator.implHppPublicMethods
cpp << methodGenerator.cppPublicMethods
cppPublicClass << methodGenerator.cppPublicClassPublicMethods


# TYPE CASTING

if pImpl and baseClassName.empty?

  hpp << "  /** @name Type Casting */\n"
  hpp << "  //@{\n"
  hpp << "\n"
  hpp << "  /** Get the impl pointer */\n"
  hpp << "  template<typename T>\n"
  hpp << "  std::shared_ptr<T> getImpl() const {\n"
  hpp << "    return std::dynamic_pointer_cast<T>(m_impl);\n"
  hpp << "  }\n"
  hpp << "\n"
  hpp << "  /** Cast to type T. Throws std::bad_cast if object is not a T. */\n"
  hpp << "  template<typename T>\n"
  hpp << "  T cast() const {\n"
  hpp << "    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();\n"
  hpp << "    if (!impl) {\n"
  hpp << "      throw(std::bad_cast());\n"
  hpp << "    }\n"
  hpp << "    return T(impl);\n"
  hpp << "  }\n"
  hpp << "\n"
  hpp << "  /** Cast to boost::optional<T>. Return value is boost::none (evaluates to false) \n"
  hpp << "   *  if object is not a T. */\n"
  hpp << "  template<typename T>\n"
  hpp << "  boost::optional<T> optionalCast() const{\n"
  hpp << "    boost::optional<T> result;\n"
  hpp << "    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();\n"
  hpp << "    if (impl){\n"
  hpp << "      result = T(impl);\n"
  hpp << "    }\n"
  hpp << "    return result;\n"
  hpp << "  }\n"
  hpp << "\n"
  hpp << "  //@}\n"

  implHpp << "    /** @name Type Casting */\n"
  implHpp << "    //@{\n"
  implHpp << "\n"
  implHpp << "    /** Get a public object that wraps this impl.*/\n"
  implHpp << "    template<typename T>\n"
  implHpp << "    T getPublicObject() const {\n"
  implHpp << "      T result(std::dynamic_pointer_cast<typename T::ImplType>(\n"
  implHpp << "                 std::const_pointer_cast<" << className << "_Impl>(shared_from_this())));\n"
  implHpp << "      return result;\n"
  implHpp << "    }\n"
  implHpp << "\n"
  implHpp << "    //@}\n"

end


# PROTECTED TYPEDEFS, FRIENDS, CONSTRUCTORS

hpp << " protected:\n"
hpp << "  /// @cond\n"

cppPublicClass << "/// @cond\n"

hpp << methodGenerator.hppProtectedImpl
hpp << methodGenerator.hppProtectedFriends
cppPublicClass << methodGenerator.cppPublicClassProtectedImpl

if pImpl
  implHpp << "   protected:\n"
end


# PROTECTED METHODS

hpp << methodGenerator.hppProtectedMethods
implHpp << methodGenerator.implHppProtectedMethods
cpp << methodGenerator.cppProtectedMethods
cppPublicClass << methodGenerator.cppPublicClassProtectedMethods

hpp << "  /// @endcond\n"
cppPublicClass << "/// @endcond\n"

# PRIVATE DECLARATIONS

hpp << " private:" << "\n"

if pImpl
  implHpp << "   private:" << "\n"

  if baseClassName.empty?
    hpp << "  std::shared_ptr<detail::" << className << "_Impl> m_impl;\n\n"
  end
end

hpp << "  REGISTER_LOGGER(\"openstudio." << sourceFolders[0] << "." << className << "\");\n"
if pImpl
  implHpp << "    REGISTER_LOGGER(\"openstudio." << sourceFolders[0] << "." << className << "\");\n"
end

hpp << methodGenerator.hppPrivateMethods
implHpp << methodGenerator.implHppPrivateMethods
cpp << methodGenerator.cppPrivateMethods
cppPublicClass << methodGenerator.cppPublicClassPrivateMethods

# END CLASSES

hpp << "};\n\n"

if pImpl
  implHpp << "  };\n\n"
end


# POST-CLASS ITEMS

hpp << methodGenerator.hppPostClass
implHpp << methodGenerator.implHppPostClass
cpp << methodGenerator.cppPostClass
cppPublicClass << methodGenerator.cppPublicClassPostClass


# END NAMESPACES

hpp << "} // " << sourceFolders[0] << "\n"
hpp << "} // openstudio" << "\n\n"

if pImpl
  implHpp << "} // detail\n\n"
  implHpp << "} // " << sourceFolders[0] << "\n"
  implHpp << "} // openstudio" << "\n" << "\n"

  cpp << "} // detail" << "\n\n"
  cpp << cppPublicClass << "\n"
end

cpp << "} // " << sourceFolders[0] << "\n"
cpp << "} // openstudio" << "\n" << "\n"


# END INCLUDE GUARDS

hpp << "#endif // " << defineString << "_HPP\n\n"

if pImpl
  implHpp << "#endif // " << defineString << "_IMPL_HPP\n\n"
end


# CREATE TESTS
originalSize = gtest.size
gtest << methodGenerator.gtest
saveGTest = true if (gtest.size > originalSize)


# CREATE AUXILLIARY CODE
originalSize = aux.size
aux << methodGenerator.aux
saveAux = true if (aux.size > originalSize)


# WRITE OUT FILES

File.open((outputDirectory + "/" + className + ".hpp"),"w") do |file|
  file.write(hpp)
end

File.open((outputDirectory + "/" + className + ".cpp"),"w") do |file|
  file.write(cpp)
end

if pImpl
  File.open((outputDirectory + "/" + className + "_Impl.hpp"),"w") do |file|
    file.write(implHpp)
  end
end

if saveGTest
  if not File.directory?(outputDirectory + "/test")
    Dir.mkdir(outputDirectory + "/test")
  end
  File.open((outputDirectory + "/test/" + className + "_GTest.cpp"),"w") do |file|
    file.write(gtest)
  end
end

if saveAux
  File.open((outputDirectory + "/" + className + ".aux"),"w") do |file|
    file.write(aux)
  end
end

