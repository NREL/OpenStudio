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
#   Create Relationship Q_PROPERTY statements, method declarations, 
#   and method definitions.
#
# == Usage
#
#   ruby GenerateRelationships.rb ARGV[0] ARGV[1]
#     
#     ARGV[0] - File (.txt) containing list of object getter methods
#               to be turned into Relationships.
#
#     ARGV[1] - Public class name.
#
######################################################################

originalFilepath = ARGV[0]
newFilepath = File.dirname(originalFilepath) + "/" +
              File.basename(originalFilepath,".txt") + "_complete.txt"
              
className = ARGV[1]

file = File.open(originalFilepath)
str = file.read

# split on semi-colons
re = Regexp.new(';')
strs = str.split(re)
puts strs.size

qProperties = []
getterDeclarations = []
setterDeclarations = []
getterDefinitions = []
setterDefinitions = []
strs.each { |str| 

  next if str.strip.empty?
  
  # required?
  optionalRe = Regexp.new('boost::optional')
  m = str.match(optionalRe)
  required = (m == nil)
  
  # singular (0-1)?
  multipleRe = Regexp.new('std::vector')
  m = str.match(multipleRe)
  singular = (m == nil)
  
  # extract names
  methodRe = Regexp.new('(boost::optional<|std::vector<)?(\w+)>? (\w+)\(')
  m = str.match(methodRe)
  raise "Unable to extract relationship name for string block " + "\n" + str if (m == nil)
  relationshipType = m[2]
  relationshipName = m[3]
  uccRelationshipName = relationshipName.gsub(/\b\w/){$&.upcase}
  
  str = String.new
  str << "    Q_PROPERTY("
  if singular
    str << "boost::optional<openstudio::model::ModelObject> " << relationshipName << " READ " << relationshipName 
    str << "AsModelObject WRITE set" << uccRelationshipName << "AsModelObject"
    if not required
      str << " RESET reset" << uccRelationshipName
    end
  else
    str << "std::vector<openstudio::model::ModelObject> " << relationshipName << " READ " << relationshipName
    str << "AsModelObjects WRITE set" << uccRelationshipName << "AsModelObjects RESET clear" << uccRelationshipName
  end
  str << ");\n"
  qProperties.push(str)
  
  str = String.new
  if singular
    str << "    boost::optional<ModelObject> " << relationshipName << "AsModelObject() const;\n"
  else 
    str << "    std::vector<ModelObject> " << relationshipName << "AsModelObjects() const;\n"
  end
  getterDeclarations.push(str)
  
  str = String.new
  if singular
    str << "  boost::optional<ModelObject> " << className << "_Impl::" << relationshipName 
    str << "AsModelObject() const {\n"
    if required
      str << "    OptionalModelObject result = " << relationshipName << "();\n"
    else
      str << "    OptionalModelObject result;\n"
      str << "    Optional" << relationshipType << " intermediate = " << relationshipName << "();\n"
      str << "    if (intermediate) {\n"
      str << "      result = *intermediate;\n"
      str << "    }\n"
    end    
  else 
    str << "  std::vector<ModelObject> " << className << "_Impl::" << relationshipName 
    str << "AsModelObjects() const {\n"
    str << "    ModelObjectVector result = castVector<ModelObject>(" << relationshipName << "());\n"
  end
  str << "    return result;\n"
  str << "  }\n\n"
  getterDefinitions.push(str)
  
  str = String.new
  str << "    bool set" << uccRelationshipName 
  if singular
    str << "AsModelObject(const boost::optional<ModelObject>& modelObject);\n"
  else 
    str << "AsModelObjects(const std::vector<ModelObject>& modelObjects);\n"
  end
  setterDeclarations.push(str)
  
  str = String.new
  str << "  bool " << className << "_Impl::set" << uccRelationshipName
  if singular
    str << "AsModelObject(const boost::optional<ModelObject>& modelObject) {\n"
    str << "    if (modelObject) {\n"
    str << "      Optional" << relationshipType << " intermediate = modelObject->optionalCast<" << relationshipType << ">();\n"
    str << "      if (intermediate) {\n"
    str << "        return set" << uccRelationshipName << "(*intermediate);\n"
    str << "      }\n"
    if required
      str << "    }\n"
      str << "    return false;\n"
    else
      str << "      else {\n"
      str << "        return false;\n"
      str << "      }\n"
      str << "    }\n"
      str << "    else {\n"
      str << "      reset" << uccRelationshipName << "();\n"
      str << "    }\n"
      str << "    return true;\n"
    end
    str << "  }\n\n"
  else
    str << "AsModelObjects(const std::vector<ModelObject>& modelObjects) {\n"
    str << "    " << relationshipType << "Vector intermediates = castVector<" << relationshipType << ">(modelObjects);\n"
    str << "    if (intermediates.size() != modelObjects.size()) {\n"
    str << "      return false;\n"
    str << "    }\n"
    str << "    return set" << uccRelationshipName << "(intermediates);\n"
    str << "  }\n\n"
  end
  setterDefinitions.push(str)
}


# write file
str = String.new
qProperties.each { |qprop|
  str << qprop
}
str << "\n"
getterDeclarations.each { |decl|
  str << decl
}
str << "\n"
setterDeclarations.each { |decl|
  str << decl
}
str << "\n"
getterDefinitions.each { |defn|
  str << defn
}
setterDefinitions.each { |defn|
  str << defn
}

File.open(newFilepath,"w") do |file|
  file.write(str)
end

