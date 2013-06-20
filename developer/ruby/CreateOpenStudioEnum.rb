# Creates a class derived from openstudio::EnumToStringConverter from a text file where each non-empty
# line contains a string that should correspond to the element of an enum.
#
# Inputs:
#   ARGV[0] - namespace declaration (in addition to openstudio)
#   ARGV[1] - name of enum to be created
#   ARGV[2] - path to text file
#   ARGV[3] - path to output folder

require 'pathname'
require 'fileutils'
require '${CMAKE_BINARY_DIR}/developer/ruby/Parsers' 

debug = true

# GET INPUTS
namespaceString = ARGV[0]
enumName = ARGV[1]
textFilePath = Pathname.new(ARGV[2])
outputDir = Pathname.new(ARGV[3])

if debug
  puts "Input arguments:"
  puts "  " + namespaceString
  puts "  " + enumName
  puts "  " + textFilePath
  puts "  " + outputDir
end

# PARSE NAMESPACES
namespaces = ParseNamespaces(namespaceString)

if debug 
  puts "Namespaces"
  index = 1
  for i in 0..(namespaces.size()-1)
    puts "  " + index.to_s + ": " + namespaces[i]
    index = index + 1
  end
end


# GET STRINGS FROM TEXT FILE
textFile = File.open(textFilePath,'r')
rawEnumText = textFile.gets(nil)
textFile.close
rawEnumText = rawEnumText.split(/\n/)
enumStrings = Array.new()
for i in 0..(rawEnumText.size()-1)
  if not rawEnumText[i].empty?
    enumStrings.push(rawEnumText[i].chomp)
  end
end

if debug
  puts "There are " + enumStrings.size().to_s + " enumStrings."
  puts "enumStrings = "
  for i in 0..(enumStrings.size()-1)
    puts "  " + enumStrings[i]
  end
end

# CREATE ENUM KEYS FROM STRINGS
enumKeys = Array.new()
for i in 0..(enumStrings.size()-1)
  rawString = enumStrings[i]
  key = ""
  split1 = rawString.split(/ +/)
  for j in 0..(split1.size()-1)
    split2 = split1[j].split(/\W+/)
    split1[j] = ""
    for k in 0..(split2.size()-1)
      split2[k].capitalize!
      if k > 0
        split1[j] = split1[j] + "_"
      end
      split1[j] = split1[j] + split2[k]
    end
    key = key + split1[j]
  end
  enumKeys.push(key)
end

if debug
  puts "There are " + enumKeys.size().to_s + " enumKeys."
  puts "enumKeys = "
  for i in 0..(enumKeys.size()-1)
    puts "  " + enumKeys[i]
  end
end


# WRITE .HXX
str1 = enumName + ".hxx"
filepath = outputDir + str1
FileUtils.mkdir_p(outputDir);
hxxFile = File.new(filepath,"w")

# header include guard, and include statements
str1 = ""
sep = ""
for i in 0..(namespaces.size()-1)
  str1 = str1 + sep + namespaces[i].upcase
  sep = "_"
end
str1 = str1 + sep + enumName.upcase + "_HXX"
hxxFile.puts("#ifndef " + str1 + "\n" +
             "#define " + str1 + "\n" +
             "\n" +
             "#include <utilities/core/Enum.hpp>\n" +
             "\n" +
             "#include <boost/optional.hpp>\n" +
             "\n" +
             "#include <vector>\n" +
             "\n")
             
# open namespaces
hxxFile.puts("namespace openstudio {\n")
for i in 0..(namespaces.size()-1)
  hxxFile.puts("namespace " + namespaces[i] + " {\n")
end
hxxFile.puts("\n")

# define enum
hxxFile.puts("  OPENSTUDIO_ENUM( " + enumName + " ,\n")
0.upto(enumKeys.size()-1) { |i| 
  hxxFile.puts("      ((" + enumKeys[i] + ")(" + enumStrings[i] + "))\n")
}
hxxFile.puts("  )\n\n")          

# enum typedefs
hxxFile.puts("typedef boost::optional<" + enumName + "> " + "Optional" + enumName + ";\n" +
             "typedef std::vector<" + enumName + ">     " + enumName + "Vector;\n" +
             "\n")
             
# close namespaces 
(namespaces.size()-1).downto(0) { |i|
  hxxFile.puts("} // " + namespaces[i] + "\n")
}
hxxFile.puts("} // openstudio\n")
hxxFile.puts("\n")

# close include guard
hxxFile.puts("#endif // " + str1 + "\n")
hxxFile.close()
