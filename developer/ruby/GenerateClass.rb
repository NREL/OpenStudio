########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
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
require_relative 'SubProjectClassGenerators/FileHeader.rb'
require_relative 'SubProjectClassGenerators/TranslatorGenerator.rb'


# Reverse Translation
def reverseTranslate(options)
  if options[:reverseTranslator]
    if options[:iddObjectType]
      # Strip out the "OS:"
      epIddName = options[:iddObjectType].to_s.sub(/^OS:/, '')
      tgen = TranslatorGenerator.new(epIddName)
      tgen.write_reverse_translator
    else
      raise "WARNING: cannot Reverse Translate without iddObjectType"
    end
  end
end

def forwardTranslate(options)
  if options[:forwardTranslator]
    if options[:iddObjectType]
      # Strip out the "OS:"
      epIddName = options[:iddObjectType].to_s.sub(/^OS:/, '')
      tgen = TranslatorGenerator.new(epIddName)
      tgen.write_forward_translator
    else
      raise "WARNING: cannot Forward Translate without iddObjectType"
    end
  end
end

def forwardReverseTranslatorTests(options)
  if options[:forwardTranslator] || options [:reverseTranslator]
    if options[:iddObjectType]
      # Strip out the "OS:"
      epIddName = options[:iddObjectType].to_s.sub(/^OS:/, '')
      tgen = TranslatorGenerator.new(epIddName)
      tgen.write_ft_rt_tests(options[:forwardTranslator], options[:reverseTranslator])
    else
      raise "WARNING: cannot Write FT/RT Tests without iddObjectType"
    end
  end
end

# HANDLE INPUT ARGUMENTS

# define the input parameters
options = Hash.new
optparse = OptionParser.new do |opts|

  opts.banner = 'Usage: openstudio GenerateClass.rb [options]'
  opts.separator ''

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
    options[:iddObjectType] = iddObjectType
  end

  opts.on( '-r', '--[no-]reverseTanslator', "Autogenerate the ReverseTranslator code for IddObjectType") do |rt|
    options[:reverseTranslator] = rt
  end

  opts.on( '-f', '--[no-]forwardTanslator', "Autogenerate the ReverseTranslator code for IddObjectType") do |ft|
    options[:forwardTranslator] = ft
  end

  opts.on( '--clang-format-exe CLANGFORMAT', "Path to the clang-format executable") do |exe|
    options[:clangFormatExe] = exe
  end

  opts.on_tail('-h', '--help', 'Print this help') do
    puts opts.help
    root_dir = File.expand_path("../../", File.dirname(__FILE__))
    puts "\nExample Usage:"
    puts '```'
    puts "  $ cd developer/ruby"
    puts '  $ /path/to/build/Products/openstudio GenerateClass.rb -c "SolarCollectorPerformancePhotovoltaicThermalBIPVT" -b "ModelObject"  -i "OS:SolarCollectorPerformance:PhotovoltaicThermal:BIPVT" -s model -o ' + File.join(root_dir, "src/model/") + ' -p -f -r'
    puts '```'
    return nil
  end

end

# parse the input parameters
optparse.parse!


puts options

# check for required parameters
if (not options[:className] or not options[:sourceDirectory])
  if options[:reverseTranslator] or options[:forwardTranslator]
    reverseTranslate(options)
    forwardTranslate(options)
  else
    puts optparse
  end
  exit(0)
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

# Get header from Subproject
fileHeader = fileHeader()

hpp = String.new
cpp = String.new
hpp << fileHeader << "\n"
cpp << fileHeader << "\n"

implHpp = String.new
cppPublicClass = String.new
if pImpl
  implHpp << fileHeader << "\n"
end

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
  hpp << "  namespace detail {\n\n"
  hpp << "    class " << className << "_Impl;\n\n"
  hpp << "  }  // namespace detail\n\n"

  implHpp << methodGenerator.implHppExternalForwardDeclarations
  implHpp << "namespace openstudio {\n"
  implHpp << methodGenerator.implHppOSForwardDeclarations
  implHpp << "namespace " << sourceFolders[0] << " {\n\n"
  implHpp << methodGenerator.implHppSubProjectForwardDeclarations
  implHpp << "  namespace detail {\n\n"

  cpp << "  namespace detail {\n\n"
end


# PRE-CLASS ITEMS

hpp << methodGenerator.hppPreClass
implHpp << methodGenerator.implHppPreClass
cpp << methodGenerator.cppPreClass
cppPublicClass << methodGenerator.cppPublicClassPreClass


# START CLASSES

hpp << "  class " << sourceFolders[0].upcase << "_API " << className
if baseClassName.empty?
  if qobject and not pImpl
    hpp << " : public QObject"
  end
else
  hpp << " : public " << baseClassName
end
hpp << "\n  {\n"
if qobject and not pImpl
  hpp << "  Q_OBJECT;\n"
end

if pImpl
  implHpp << "    class " << sourceFolders[0].upcase << "_API " << className << "_Impl"
  if baseClassName.empty?
    implHpp << " : "
    if qobject
      implHpp << "public QObject, "
    end
    implHpp << "public std::enable_shared_from_this<" << className << "_Impl>"
  else
    implHpp << " : public " << baseClassName << "_Impl"
  end
  implHpp << "\n    {\n"
  implHpp << "      Q_OBJECT;\n" if qobject
end


# QMACROS

hpp << methodGenerator.hppQMacros
implHpp << methodGenerator.implHppQMacros


# PUBLIC TYPEDEFS

hpp << "   public:\n"

if pImpl
  implHpp << "     public:\n"
end

hpp << methodGenerator.hppPublicTypedefs
implHpp << methodGenerator.implHppPublicTypedefs


# CONSTRUCTORS

hpp << "    /** @name Constructors and Destructors */\n"
hpp << "    //@{\n\n"

if pImpl
  implHpp << "      /** @name Constructors and Destructors */\n"
  implHpp << "      //@{\n\n"
end

hpp << methodGenerator.hppConstructors
implHpp << methodGenerator.implHppConstructors
cpp << methodGenerator.cppConstructors
cppPublicClass << methodGenerator.cppPublicClassConstructors


# VIRTUAL DESTRUCTORS

hpp << "    virtual ~" << className << "() = default;\n"
hpp << "    // Default the copy and move operators because the virtual dtor is explicit\n"
hpp << "    " << className << "(const " << className << "& other) = default;\n"
hpp << "    " << className << "(" << className << "&& other) = default;\n"
hpp << "    " << className << "& operator=(const " << className << "&) = default;\n"
hpp << "    " << className << "& operator=(" << className << "&&) = default;\n\n"
hpp << "    //@}\n\n"

if pImpl
  implHpp << "      virtual ~" << className << "_Impl() = default;\n\n"
  implHpp << "      //@}\n"
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

hpp << "   protected:\n"
hpp << "    /// @cond\n"

cppPublicClass << "  /// @cond\n"

hpp << methodGenerator.hppProtectedImpl
hpp << methodGenerator.hppProtectedFriends
cppPublicClass << methodGenerator.cppPublicClassProtectedImpl

if pImpl
  implHpp << "     protected:\n"
end


# PROTECTED METHODS

hpp << methodGenerator.hppProtectedMethods
implHpp << methodGenerator.implHppProtectedMethods
cpp << methodGenerator.cppProtectedMethods
cppPublicClass << methodGenerator.cppPublicClassProtectedMethods

hpp << "    /// @endcond\n"
cppPublicClass << "  /// @endcond\n"

# PRIVATE DECLARATIONS

hpp << "   private:" << "\n"

if pImpl
  implHpp << "     private:" << "\n"

  if baseClassName.empty?
    hpp << "  std::shared_ptr<detail::" << className << "_Impl> m_impl;\n\n"
  end
end

hpp << "    REGISTER_LOGGER(\"openstudio." << sourceFolders[0] << "." << className << "\");\n"
if pImpl
  implHpp << "      REGISTER_LOGGER(\"openstudio." << sourceFolders[0] << "." << className << "\");\n"
end

hpp << methodGenerator.hppPrivateMethods
implHpp << methodGenerator.implHppPrivateMethods
cpp << methodGenerator.cppPrivateMethods
cppPublicClass << methodGenerator.cppPublicClassPrivateMethods

# END CLASSES

hpp << "  };\n\n"

if pImpl
  implHpp << "    };\n\n"
end


# POST-CLASS ITEMS

hpp << methodGenerator.hppPostClass
implHpp << methodGenerator.implHppPostClass
cpp << methodGenerator.cppPostClass
cppPublicClass << methodGenerator.cppPublicClassPostClass


# END NAMESPACES

hpp << "}  // namespace " << sourceFolders[0] << "\n"
hpp << "}  // namespace openstudio" << "\n\n"

if pImpl
  implHpp << "  }  // namespace detail\n\n"
  implHpp << "}  // namespace " << sourceFolders[0] << "\n"
  implHpp << "}  // namespace openstudio" << "\n" << "\n"

  cpp << "  }  // namespace detail" << "\n\n"
  cpp << cppPublicClass << "\n"
end

cpp << "}  // namespace " << sourceFolders[0] << "\n"
cpp << "}  // namespace openstudio\n"


# END INCLUDE GUARDS

hpp << "#endif  // " << defineString << "_HPP\n"

if pImpl
  implHpp << "#endif  // " << defineString << "_IMPL_HPP\n"
end


# CREATE TESTS
gtest = String.new
gtest << fileHeader << "\n"
gtest << methodGenerator.gtestIncludes
gtest << methodGenerator.gtestGetterSetters

# CREATE AUXILLIARY CODE
originalSize = aux.size
aux << methodGenerator.aux
saveAux = true if (aux.size > originalSize)


# WRITE OUT FILES

files_written = []

hpp_path = outputDirectory + "/" + className + ".hpp"
files_written.append(hpp_path)
File.open(hpp_path, "w") do |file|
  file.write(hpp)
end

cpp_path = outputDirectory + "/" + className + ".cpp"
files_written.append(cpp_path)
File.open(cpp_path, "w") do |file|
  file.write(cpp)
end

if pImpl
  impl_path = outputDirectory + "/" + className + "_Impl.hpp"
  files_written.append(impl_path)
  File.open(impl_path,"w") do |file|
    file.write(implHpp)
  end
end

if not File.directory?(outputDirectory + "/test")
  Dir.mkdir(outputDirectory + "/test")
end
gtest_path = outputDirectory + "/test/" + className + "_GTest.cpp"
files_written.append(gtest_path)
File.open(gtest_path,"w") do |file|
  file.write(gtest)
end

if saveAux
  File.open((outputDirectory + "/" + className + ".aux"),"w") do |file|
    file.write(aux)
  end
end

###############################################################################
#                       Reverse And Forward Translation                       #
###############################################################################

has_ft_or_rt = false
if options[:reverseTranslator]
  has_ft_or_rt = true
  files_written << reverseTranslate(options)
end

if options[:forwardTranslator]
  has_ft_or_rt = true
  files_written << forwardTranslate(options)
end

if has_ft_or_rt
  files_written << forwardReverseTranslatorTests(options)
end

files_written.map!{|f| File.absolute_path(f) }
puts "Generated the following files"
puts files_written

# Cross-platform way of finding an executable in the $PATH.
#
#   which('ruby') #=> /usr/bin/ruby
def which(cmd)
  exts = ENV['PATHEXT'] ? ENV['PATHEXT'].split(';') : ['']
  ENV['PATH'].split(File::PATH_SEPARATOR).each do |path|
    exts.each do |ext|
      exe = File.join(path, "#{cmd}#{ext}")
      return exe if File.executable?(exe) && !File.directory?(exe)
    end
  end
  nil
end

clang_format_exe = nil
auto_detected = ""
if options[:clangFormatExe]
  clang_format_exe = options[:clangFormatExe]
else
  clang_format_exe = which('clang-format')
  if clang_format_exe
    auto_detected = "auto-detected "
  end
end
if clang_format_exe
  puts "\nReformatting the files using the #{auto_detected}clang-format at #{clang_format_exe}"
  require 'open3'
  root_dir = File.expand_path("../../", File.dirname(__FILE__))

  files_written.each do |file_written|
    command = "#{clang_format_exe} -style=file -i -fallback-style=none #{file_written}"
    puts command
    Open3.popen3(command, chdir: root_dir) do |i, o, e, w|
      result = w.value.exitstatus
      if result != 0
        puts "Failed to reformat #{files_written}"
      end
    end
  end
end
