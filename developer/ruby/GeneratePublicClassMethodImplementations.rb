######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

######################################################################
# == Synopsis 
#
#   Create method implementations for public classes that use an impl.
#
# == Usage
#
#   ruby GeneratePublicClassMethodImplementations.rb ARGV[0] ARGV[1]
#     
#     ARGV[0] - File (.txt) containing list of methods, optionally with 
#               public class name already prepended.
#
#     ARGV[1] - Public class name (optional).
#
######################################################################

originalFilepath = ARGV[0]
newFilepath = File.dirname(originalFilepath) + "/" +
              File.basename(originalFilepath,".txt") + "_complete.txt"
              
className = String.new
className = ARGV[1] if (ARGV.size > 1) 

file = File.open(originalFilepath)
str = file.read

# split on semi-colons
re = Regexp.new(';')
strs = str.split(re)
puts strs.size

newStrs = []
strs.each { |str| 

  next if str.strip.empty?

  if className.empty?
    re = Regexp.new('^[\w:<>]*\&? (\w*)::')
    m = str.match(re)
    raise "Unable to extract class name." if (m == nil)
    className = m[1]
    puts "Found class name '" + className + "'."
  end
  
  # void or return?
  voidRe = Regexp.new('^void ')
  m = str.match(voidRe)
  printReturn = (m == nil)
  
  # extract method name
  methodRe = Regexp.new('^(const )?[\w:,<> ]*\&? (\w*::)?(\w*|operator ?\W+) ?\(')
  m = str.match(methodRe)
  raise "Unable to extract method name for string block " + "\n" + str if (m == nil)
  methodName = m[3]
  
  # note if class name prepends methodName
  printClassName = (m[2] == nil)
  
  # extract arguments
  # first match is red herring to get same indices for both regexes
  firstArgumentRe = Regexp.new('(\()(const )?[\w<>:&]+ &?(\w+)') 
  otherArgumentRe = Regexp.new('(, ?|^[ ]+)(const )?[\w<>:&]+ &?(\w+)')
  m = str.match(firstArgumentRe)
  arguments = []
  while (m != nil) 
    arguments.push(m[3])
    temp = m.post_match
    m = temp.match(otherArgumentRe)
  end
  
  # new line before open bracket?
  oneLineRe = Regexp.new('^\w.*\(.*\)( const)? *$')
  m = str.match(oneLineRe)
  newLine = true
  if (m != nil)
    newLine = false if m[0].size < 98
  end
  
  newStr = str
  
  # insert ClassName if needed
  if printClassName
    m = newStr.match(methodRe)
    newStr.insert(m.begin(3),className + "::")
  end
  
  if (newLine)
    newStr << "\n"
  else
    newStr << " "
  end
  newStr << "{" << "\n"
  
  newStr << "  "
  newStr << "return " if printReturn
  newStr << "getImpl<detail::" + className + "_Impl>()->" + methodName + "("
  sep = String.new
  arguments.each { |arg|
    newStr << sep << arg 
    sep = ","
  }
  newStr << ");"

  newStr << "\n"  
  newStr << "}"
  
  newStrs.push(newStr)
}


# write file
str = String.new
newStrs.each { |newStr|
  str << newStr
}
File.open(newFilepath,"w") do |file|
  file.write(str)
end

puts "Printed implementations to " + newFilepath + "."

