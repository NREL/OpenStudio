/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <generatedatadictionary/GenerateDataDictionary.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <sstream>

namespace openstudio {
namespace standardsinterface {

void initializeOutputFiles(const path& outPath,
                           const std::string& outputFileHeader,
                           GenerateDataDictionaryOutputFiles& outputFiles)
{
  std::stringstream ss;

  // initialize file streams
  openstudio::path p = outPath/toPath("DataDictionary.xml");
  outputFiles.ofDataDictionaryXml.open(p);
  if (!outputFiles.ofDataDictionaryXml) {
    ss << "Unable to open '" << toString(p) << "' for writing.";
    throw std::runtime_error(ss.str().c_str());
  }
  p = outPath/toPath("ObjectType.hxx");
  outputFiles.ofObjectTypeHxx.open(p);
  if (!outputFiles.ofObjectTypeHxx) {
    ss << "Unable to open '" << toString(p) << "' for writing.";
    throw std::runtime_error(ss.str().c_str());
  }
  p = outPath/toPath("BooleanAttribute.hxx");
  outputFiles.ofBooleanAttributeHxx.open(p);
  if (!outputFiles.ofBooleanAttributeHxx) {
    ss << "Unable to open '" << toString(p) << "' for writing.";
    throw std::runtime_error(ss.str().c_str());
  }
  p = outPath/toPath("IntegerAttribute.hxx");
  outputFiles.ofIntegerAttributeHxx.open(p);
  if (!outputFiles.ofIntegerAttributeHxx) {
    ss << "Unable to open '" << toString(p) << "' for writing.";
    throw std::runtime_error(ss.str().c_str());
  }
  p = outPath/toPath("DoubleAttribute.hxx");
  outputFiles.ofDoubleAttributeHxx.open(p);
  if (!outputFiles.ofDoubleAttributeHxx) {
    ss << "Unable to open '" << toString(p) << "' for writing.";
    throw std::runtime_error(ss.str().c_str());
  }
  p = outPath/toPath("EnumAttribute.hxx");
  outputFiles.ofEnumAttributeHxx.open(p);
  if (!outputFiles.ofEnumAttributeHxx) {
    ss << "Unable to open '" << toString(p) << "' for writing.";
    throw std::runtime_error(ss.str().c_str());
  }
  p = outPath/toPath("EnumAttributeValues.hxx");
  outputFiles.ofEnumAttributeValuesHxx.open(p);
  if (!outputFiles.ofEnumAttributeValuesHxx) {
    ss << "Unable to open '" << toString(p) << "' for writing.";
    throw std::runtime_error(ss.str().c_str());
  }
  p = outPath/toPath("DataDictionary.cxx");
  outputFiles.ofDataDictionaryCxx.open(p);
  if (!outputFiles.ofDataDictionaryCxx) {
    ss << "Unable to open '" << toString(p) << "' for writing.";
    throw std::runtime_error(ss.str().c_str());
  }

  // start DataDictionary.xml
  outputFiles.ofDataDictionaryXml 
    << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl
    << "<DataDictionary xsi:noNamespaceSchemaLocation=\"DataDictionary.xsd\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">" << std::endl
    << "  <Universal>" << std::endl; // \todo remove <Universal> tag in favor of deciding that  
                                     // during parsing. probably need analog to IddFileType passed 
                                     // in with input files then can split up enumerations (at 
                                     // least) between Title24_2011, ASHRAE90-1_2007, etc.

  // start ObjectType.hxx
  outputFiles.ofObjectTypeHxx
    << outputFileHeader << std::endl
    << "#ifndef STANDARDSINTERFACE_OBJECTTYPE_HXX" << std::endl
    << "#define STANDARDSINTERFACE_OBJECTTYPE_HXX" << std::endl
    << std::endl
    << "#include <utilities/core/Enum.hpp>" << std::endl
    << std::endl
    << "namespace openstudio {" << std::endl
    << "namespace standardsinterface {" << std::endl
    << std::endl
    << "/** \\class ObjectType */" << std::endl
    << "OPENSTUDIO_ENUM(ObjectType," << std::endl;

  // start BooleanAttribute.hxx
  outputFiles.ofBooleanAttributeHxx
    << outputFileHeader << std::endl
    << "#ifndef STANDARDSINTERFACE_BOOLEANATTRIBUTE_HXX" << std::endl
    << "#define STANDARDSINTERFACE_BOOLEANATTRIBUTE_HXX" << std::endl
    << std::endl
    << "#include <utilities/core/Enum.hpp>" << std::endl
    << std::endl
    << "namespace openstudio {" << std::endl
    << "namespace standardsinterface {" << std::endl
    << std::endl
    << "/** \\class BooleanAttribute */" << std::endl
    << "OPENSTUDIO_ENUM(BooleanAttribute," << std::endl;

  // start IntegerAttribute.hxx
  outputFiles.ofIntegerAttributeHxx
    << outputFileHeader << std::endl
    << "#ifndef STANDARDSINTERFACE_INTEGERATTRIBUTE_HXX" << std::endl
    << "#define STANDARDSINTERFACE_INTEGERATTRIBUTE_HXX" << std::endl
    << std::endl
    << "#include <utilities/core/Enum.hpp>" << std::endl
    << std::endl
    << "namespace openstudio {" << std::endl
    << "namespace standardsinterface {" << std::endl
    << std::endl
    << "/** \\class IntegerAttribute */" << std::endl
    << "OPENSTUDIO_ENUM(IntegerAttribute," << std::endl;

  // start DoubleAttribute.hxx
  outputFiles.ofDoubleAttributeHxx
    << outputFileHeader << std::endl
    << "#ifndef STANDARDSINTERFACE_DOUBLEATTRIBUTE_HXX" << std::endl
    << "#define STANDARDSINTERFACE_DOUBLEATTRIBUTE_HXX" << std::endl
    << std::endl
    << "#include <utilities/core/Enum.hpp>" << std::endl
    << std::endl
    << "namespace openstudio {" << std::endl
    << "namespace standardsinterface {" << std::endl
    << std::endl
    << "/** \\class DoubleAttribute */" << std::endl
    << "OPENSTUDIO_ENUM(DoubleAttribute," << std::endl;

  // start EnumAttribute.hxx
  outputFiles.ofEnumAttributeHxx
    << outputFileHeader << std::endl
    << "#ifndef STANDARDSINTERFACE_ENUMATTRIBUTE_HXX" << std::endl
    << "#define STANDARDSINTERFACE_ENUMATTRIBUTE_HXX" << std::endl
    << std::endl
    << "#include <utilities/core/Enum.hpp>" << std::endl
    << std::endl
    << "namespace openstudio {" << std::endl
    << "namespace standardsinterface {" << std::endl
    << std::endl
    << "/** \\class EnumAttribute */" << std::endl
    << "OPENSTUDIO_ENUM(EnumAttribute," << std::endl;

  // start EnumAttributeValues.hxx
  outputFiles.ofEnumAttributeValuesHxx
    << outputFileHeader << std::endl
    << "#ifndef STANDARDSINTERFACE_ENUMATTRIBUTEVALUES_HXX" << std::endl
    << "#define STANDARDSINTERFACE_ENUMATTRIBUTEVALUES_HXX" << std::endl
    << std::endl
    << "#include <utilities/core/Enum.hpp>" << std::endl
    << std::endl
    << "namespace openstudio {" << std::endl
    << "namespace standardsinterface {" << std::endl
    << std::endl;

  // start DataDictionary.cxx
  outputFiles.ofDataDictionaryCxx
    << outputFileHeader << std::endl
    << "#include <standardsinterface/DataDictionary.hpp>" << std::endl
    << std::endl
    << "#include <utilities/core/Assert.hpp>" << std::endl
    << "#include <utilities/core/Containers.hpp>" << std::endl
    << "#include <utilities/core/Compare.hpp>" << std::endl
    << std::endl
    << "#include <boost/foreach.hpp>" << std::endl
    << std::endl
    << "namespace openstudio {" << std::endl
    << "namespace standardsinterface {" << std::endl
    << std::endl;
}

void completeOutputFiles(GenerateDataDictionaryOutputFiles& outputFiles,
                         GenerateDataDictionaryHistory& history)
{
  // conclude and close DataDictionary.xml
  outputFiles.ofDataDictionaryXml
    << "  </Universal>" << std::endl
    << "</DataDictionary>" << std::endl;

  outputFiles.ofDataDictionaryXml.close();

  // conclude and close ObjectType.hxx
  outputFiles.ofObjectTypeHxx
    << ");" << std::endl
    << std::endl
    << "/** \\relates ObjectType */" << std::endl
    << "typedef std::vector<ObjectType> ObjectTypeVector;" << std::endl
    << std::endl
    << "} // standardsinterface" << std::endl
    << "} // openstudio" << std::endl
    << std::endl
    << "#endif // STANDARDSINTERFACE_OBJECTTYPE_HXX" << std::endl
    << std::endl;

  outputFiles.ofObjectTypeHxx.close();

  // conclude and close BooleanAttribute.hxx
  outputFiles.ofBooleanAttributeHxx
    << ");" << std::endl
    << std::endl
    << "/** \\relates BooleanAttribute */" << std::endl
    << "typedef std::vector<BooleanAttribute> BooleanAttributeVector;" << std::endl
    << std::endl
    << "} // standardsinterface" << std::endl
    << "} // openstudio" << std::endl
    << std::endl
    << "#endif // STANDARDSINTERFACE_BOOLEANATTRIBUTE_HXX" << std::endl
    << std::endl; 

  outputFiles.ofBooleanAttributeHxx.close();

  // conclude and close IntegerAttribute.hxx
  outputFiles.ofIntegerAttributeHxx
    << ");" << std::endl
    << std::endl
    << "/** \\relates IntegerAttribute */" << std::endl
    << "typedef std::vector<IntegerAttribute> IntegerAttributeVector;" << std::endl
    << std::endl
    << "} // standardsinterface" << std::endl
    << "} // openstudio" << std::endl
    << std::endl
    << "#endif // STANDARDSINTERFACE_INTEGERATTRIBUTE_HXX" << std::endl
    << std::endl; 

  outputFiles.ofIntegerAttributeHxx.close();

  // conclude and close DoubleAttribute.hxx
  outputFiles.ofDoubleAttributeHxx
    << ");" << std::endl
    << std::endl
    << "/** \\relates DoubleAttribute */" << std::endl
    << "typedef std::vector<DoubleAttribute> DoubleAttributeVector;" << std::endl
    << std::endl
    << "} // standardsinterface" << std::endl
    << "} // openstudio" << std::endl
    << std::endl
    << "#endif // STANDARDSINTERFACE_DOUBLEATTRIBUTE_HXX" << std::endl
    << std::endl; 

  outputFiles.ofDoubleAttributeHxx.close();

  // conclude and close EnumAttribute.hxx
  outputFiles.ofEnumAttributeHxx
    << ");" << std::endl
    << std::endl
    << "/** \\relates EnumAttribute */" << std::endl
    << "typedef std::vector<EnumAttribute> EnumAttributeVector;" << std::endl
    << std::endl
    << "} // standardsinterface" << std::endl
    << "} // openstudio" << std::endl
    << std::endl
    << "#endif // STANDARDSINTERFACE_ENUMATTRIBUTE_HXX" << std::endl
    << std::endl; 

  outputFiles.ofEnumAttributeHxx.close();

  // conclude and close EnumAttributeValues.hxx
  outputFiles.ofEnumAttributeValuesHxx
    << std::endl
    << "} // standardsinterface" << std::endl
    << "} // openstudio" << std::endl
    << std::endl
    << "#endif // STANDARDSINTERFACE_ENUMATTRIBUTEVALUES_HXX" << std::endl
    << std::endl; 

  outputFiles.ofEnumAttributeValuesHxx.close();

  // conclude and close DataDictionary.cxx
  outputFiles.ofDataDictionaryCxx
    << "std::vector<std::string> DataDictionary::enumValues(const EnumAttribute& ddEnumAttribute) {" << std::endl
    << "  StringVector result;" << std::endl;
  BOOST_FOREACH(const std::string& standardName,history.enumsWithValues)
  {
    outputFiles.ofDataDictionaryCxx
      << "  if (ddEnumAttribute.valueName() == \"" << standardName << "\") {" << std::endl
      << "    IntSet vals = " << standardName << "::getValues();" << std::endl
      << "    BOOST_FOREACH(int val,vals) { result.push_back(" << standardName << "(val).valueDescription()); }" << std::endl
      << "  }" << std::endl;
  }
  outputFiles.ofDataDictionaryCxx
    << "  return result;" << std::endl
    << "}" << std::endl
    << std::endl
    << "bool DataDictionary::isEnumValue(const EnumAttribute& ddEnumAttribute,const std::string& candidate) {" << std::endl;
  BOOST_FOREACH(const std::string& standardName,history.enumsWithValues)
  {
    outputFiles.ofDataDictionaryCxx
      << "  if (ddEnumAttribute.valueName() == \"" << standardName << "\") {" << std::endl
      << "    try {" << std::endl
      << "      " << standardName << " val(candidate);" << std::endl
      << "      return true;" << std::endl
      << "    }" << std::endl
      << "    catch (...) {}" << std::endl
      << "    return false;" << std::endl
      << "  }" << std::endl;
  }
  outputFiles.ofDataDictionaryCxx
    << "  return false;" << std::endl
    << "}" << std::endl
    << std::endl
    << "bool DataDictionary::areEquivalentEnumValues(const EnumAttribute& ddEnumAttribute," << std::endl
    << "                                             const std::string& value1," << std::endl
    << "                                             const std::string& value2)" << std::endl
    << "{" << std::endl;
  BOOST_FOREACH(const std::string& standardName,history.enumsWithValues)
  {
    outputFiles.ofDataDictionaryCxx
      << "  if (ddEnumAttribute.valueName() == \"" << standardName << "\") {" << std::endl
      << "    try {" << std::endl
      << "      " << standardName << " val1(value1);" << std::endl
      << "      " << standardName << " val2(value2);" << std::endl
      << "      return (val1 == val2);" << std::endl
      << "    }" << std::endl
      << "    catch (...) {}" << std::endl
      << "    return false;" << std::endl
      << "  }" << std::endl;
  }
  outputFiles.ofDataDictionaryCxx
    << "  return false;" << std::endl
    << "}" << std::endl
    << std::endl
    << "} // standardsinterface" << std::endl
    << "} // openstudio" << std::endl
    << std::endl;

  outputFiles.ofDataDictionaryCxx.close();
}

void parseInputFile(const std::string& inputFilePath,
                    GenerateDataDictionaryOutputFiles& outputFiles,
                    GenerateDataDictionaryHistory& history) 
{
  path p = toPath(inputFilePath);
  std::stringstream ss;

  boost::filesystem::ifstream inFile(p);
  if (!inFile) {
    ss << "Unable to open '" << toString(p) << "' for reading.";
    throw std::runtime_error(ss.str().c_str());
  }

  ss << inFile.rdbuf();
  std::string inFileString(ss.str());
  ss.str("");

  std::cout << "Parsing file " << inputFilePath << ", which has " << inFileString.size() << " characters." << std::endl << std::endl;

  // use regexes to determine entry type and get separate pieces of data
  std::string definitionRegexString("((?:[^,$\"]*)|(?:\"(?:[^\"]*|\"\")*\"))");
  boost::regex removeQuotes("\"((?:[^\"]*|\"\")*)\"");
  ss << "^([^,$\"]+)," << definitionRegexString << "$";
  boost::regex objectRegex(ss.str()); ss.str("");
  ss << "^([^,$\"]+),([^,$\"]*)," << definitionRegexString << "$";
  boost::regex measurementRegex(ss.str()); ss.str("");
  ss << "^([^,$\"]+),boolean,," << definitionRegexString << "$";
  boost::regex booleanRegex(ss.str()); ss.str("");
  ss << "^([^,$\"]+),integer,," << definitionRegexString << "$";
  boost::regex integerRegex(ss.str()); ss.str("");
  ss << "^([^,$\"]+),decimal,," << definitionRegexString << "$";
  boost::regex doubleRegex(ss.str()); ss.str("");
  ss << "^([^,$\"]+),enumerated list,([^,$\"]*)," << definitionRegexString << "$";
  boost::regex enumRegex(ss.str()); ss.str("");
  ss << "^([^,$\"]+),text,," << definitionRegexString << "$";
  boost::regex textRegex(ss.str()); ss.str(""); // throw these out for now
  boost::regex enumValue("#?([^;]*);#\\d*;?");

  // based on match, write to files
  boost::smatch matches;
  bool matchFound = true;
  while (matchFound && !inFileString.empty()) {
    matchFound = false;
    std::string name, definition;
    if (boost::regex_search(inFileString,matches,booleanRegex)) {
      // get elements
      name = std::string(matches[1].first,matches[1].second);
      definition = std::string(matches[2].first,matches[2].second);
      std::string temp(std::string::const_iterator(inFileString.begin()),matches[0].first);
      temp += std::string(matches[0].second,std::string::const_iterator(inFileString.end()));
      inFileString = temp;
      if (boost::regex_match(definition,matches,removeQuotes)) {
        definition = std::string(matches[1].first,matches[1].second);
      }
      boost::trim(name); boost::trim(definition);
      // write to DataDictionary.xml
      outputFiles.ofDataDictionaryXml
        << "    <Term>" << std::endl
        << "      <name>" << name << "</name>" << std::endl
        << "      <definition>" << definition << "</definition>" << std::endl
        << "      <type><boolean/></type>" << std::endl
        << "    </Term>" << std::endl;
      // write to BooleanAttribute.hxx or write error to screen
      if (history.booleanAttributesMap.find(name) != history.booleanAttributesMap.end()) {
        std::cout << "Duplicate boolean attribute named '" << name 
          << "' found in file " << inputFilePath << ". Attribute originally listed in " 
          << history.booleanAttributesMap.find(name)->second << ". Both definitions included "
          << "in XML file." << std::endl << std::endl;
      }
      else {
        outputFiles.ofBooleanAttributeHxx
          << "  ((" << convertName(name) << ")(" << name << "))" << std::endl;
        history.booleanAttributesMap.insert(GenerateDataDictionaryHistory::
            NameToFirstInputFilePathMap::value_type(name,inputFilePath));
      }
      matchFound = true;      
      continue;
    }
    if (boost::regex_search(inFileString,matches,integerRegex)) {
      // get elements
      name = std::string(matches[1].first,matches[1].second);
      definition = std::string(matches[2].first,matches[2].second);
      std::string temp(std::string::const_iterator(inFileString.begin()),matches[0].first);
      temp += std::string(matches[0].second,std::string::const_iterator(inFileString.end()));
      inFileString = temp;
      if (boost::regex_match(definition,matches,removeQuotes)) {
        definition = std::string(matches[1].first,matches[1].second);
      }
      boost::trim(name); boost::trim(definition);
      // write to DataDictionary.xml
      outputFiles.ofDataDictionaryXml
        << "    <Term>" << std::endl
        << "      <name>" << name << "</name>" << std::endl
        << "      <definition>" << definition << "</definition>" << std::endl
        << "      <type><integer/></type>" << std::endl
        << "    </Term>" << std::endl;
      // write to IntegerAttribute.hxx or write error to screen
      if (history.integerAttributesMap.find(name) != history.integerAttributesMap.end()) {
        std::cout << "Duplicate integer attribute named '" << name 
          << "' found in file " << inputFilePath << ". Attribute originally listed in " 
          << history.integerAttributesMap.find(name)->second << ". Both definitions included "
          << "in XML file." << std::endl << std::endl;
      }
      else {
        outputFiles.ofIntegerAttributeHxx
          << "  ((" << convertName(name) << ")(" << name << "))" << std::endl;
        history.integerAttributesMap.insert(GenerateDataDictionaryHistory::
            NameToFirstInputFilePathMap::value_type(name,inputFilePath));
      }
      matchFound = true;      
      continue;
    }
    if (boost::regex_search(inFileString,matches,doubleRegex)) {
      // get elements
      name = std::string(matches[1].first,matches[1].second);
      definition = std::string(matches[2].first,matches[2].second);
      std::string temp(std::string::const_iterator(inFileString.begin()),matches[0].first);
      temp += std::string(matches[0].second,std::string::const_iterator(inFileString.end()));
      inFileString = temp;
      if (boost::regex_match(definition,matches,removeQuotes)) {
        definition = std::string(matches[1].first,matches[1].second);
      }
      boost::trim(name); boost::trim(definition);
      // write to DataDictionary.xml
      outputFiles.ofDataDictionaryXml
        << "    <Term>" << std::endl
        << "      <name>" << name << "</name>" << std::endl
        << "      <definition>" << definition << "</definition>" << std::endl
        << "      <type><double/></type>" << std::endl
        << "    </Term>" << std::endl;
      // write to DoubleAttribute.hxx or write error to screen
      if (history.doubleAttributesMap.find(name) != history.doubleAttributesMap.end()) {
        std::cout << "Duplicate double attribute named '" << name 
          << "' found in file " << inputFilePath << ". Attribute originally listed in " 
          << history.doubleAttributesMap.find(name)->second << ". Both definitions included "
          << "in XML file." << std::endl << std::endl;
      }
      else {
        outputFiles.ofDoubleAttributeHxx
          << "  ((" << convertName(name) << ")(" << name << "))" << std::endl;
        history.doubleAttributesMap.insert(GenerateDataDictionaryHistory::
            NameToFirstInputFilePathMap::value_type(name,inputFilePath));
      }
      matchFound = true;      
      continue;
    }
    if (boost::regex_search(inFileString,matches,measurementRegex)) {
      // get elements
      name = std::string(matches[1].first,matches[1].second);
      // units--in L'Monte's IP format is matches[2]
      definition = std::string(matches[3].first,matches[3].second);
      std::string temp(std::string::const_iterator(inFileString.begin()),matches[0].first);
      temp += std::string(matches[0].second,std::string::const_iterator(inFileString.end()));
      inFileString = temp;
      if (boost::regex_match(definition,matches,removeQuotes)) {
        definition = std::string(matches[1].first,matches[1].second);
      }
      boost::trim(name); boost::trim(definition);
      // write to DataDictionary.xml
      outputFiles.ofDataDictionaryXml
        << "    <Term>" << std::endl
        << "      <name>" << name << "</name>" << std::endl
        << "      <definition>" << definition << "</definition>" << std::endl
        << "      <type><double/></type>" << std::endl
        << "    </Term>" << std::endl;
      // write to DoubleAttribute.hxx or write error to screen
      if (history.doubleAttributesMap.find(name) != history.doubleAttributesMap.end()) {
        std::cout << "Duplicate double attribute named '" << name 
          << "' found in file " << inputFilePath << ". Attribute originally listed in " 
          << history.doubleAttributesMap.find(name)->second << ". Both definitions included "
          << "in XML file." << std::endl << std::endl;
      }
      else {
        outputFiles.ofDoubleAttributeHxx
          << "  ((" << convertName(name) << ")(" << name << "))" << std::endl;
        history.doubleAttributesMap.insert(GenerateDataDictionaryHistory::
            NameToFirstInputFilePathMap::value_type(name,inputFilePath));
      }
      matchFound = true;      
      continue;
    }
    if (boost::regex_search(inFileString,matches,enumRegex)) {
      // get elements
      name = std::string(matches[1].first,matches[1].second);
      std::string enumValuesString(matches[2].first,matches[2].second);
      definition = std::string(matches[3].first,matches[3].second);
      std::string temp(std::string::const_iterator(inFileString.begin()),matches[0].first);
      temp += std::string(matches[0].second,std::string::const_iterator(inFileString.end()));
      inFileString = temp;
      if (boost::regex_match(definition,matches,removeQuotes)) {
        definition = std::string(matches[1].first,matches[1].second);
      }
      boost::trim(name); boost::trim(enumValuesString); boost::trim(definition);
      // parse enumValues
      std::vector<std::string> enumValues;
      while (boost::regex_search(enumValuesString,matches,enumValue)) {
        enumValues.push_back(std::string(matches[1].first,matches[1].second));
        std::string temp(std::string::const_iterator(enumValuesString.begin()),matches[0].first);
        temp += std::string(matches[0].second,std::string::const_iterator(enumValuesString.end()));
        enumValuesString = temp;
      }
      boost::trim(enumValuesString);
      if (!enumValuesString.empty()) {
        std::cout << "Unable to parse all of '" << name << "'s enumValuesString. Leftovers: " 
                  << enumValuesString << std::endl << std::endl;
      }
      // write to DataDictionary.xml
      outputFiles.ofDataDictionaryXml
        << "    <Term>" << std::endl
        << "      <name>" << name << "</name>" << std::endl
        << "      <definition>" << definition << "</definition>" << std::endl;
      if (enumValues.empty()) {
        outputFiles.ofDataDictionaryXml
          << "      <type><enumeration/></type>" << std::endl;
      }
      else {
        outputFiles.ofDataDictionaryXml
          << "      <type>" << std::endl
          << "        <enumeration>" << std::endl;
        BOOST_FOREACH(const std::string v,enumValues) {
          outputFiles.ofDataDictionaryXml
            << "        <value>" << v << "</value>" << std::endl;
        }
        outputFiles.ofDataDictionaryXml
          << "        </enumeration>" << std::endl
          << "      </type>" << std::endl;
      }
      outputFiles.ofDataDictionaryXml
        << "    </Term>" << std::endl;
      // write to EnumAttribute.hxx or write error to screen
      if (history.enumAttributesMap.find(name) != history.enumAttributesMap.end()) {
        std::cout << "Duplicate enum attribute named '" << name 
          << "' found in file " << inputFilePath << ". Attribute originally listed in " 
          << history.enumAttributesMap.find(name)->second << ". Both definitions included "
          << "in XML file." << std::endl << std::endl;
      }
      else {
        outputFiles.ofEnumAttributeHxx
          << "  ((" << convertName(name) << ")(" << name << "))" << std::endl;
        // write to EnumAttributeValues.hxx
        if (!enumValues.empty()) {
          outputFiles.ofEnumAttributeValuesHxx
            << "/** \\class " << convertName(name) << " */" << std::endl
            << "OPENSTUDIO_ENUM(" << convertName(name) << "," << std::endl;
          BOOST_FOREACH(std::string v,enumValues) {
            outputFiles.ofEnumAttributeValuesHxx
              << "  ((" << convertName(v) << ")(" << v << "))" << std::endl;
          }
          outputFiles.ofEnumAttributeValuesHxx
            << ");" << std::endl
            << std::endl;
          history.enumsWithValues.push_back(convertName(name));
        }
        history.enumAttributesMap.insert(GenerateDataDictionaryHistory::
            NameToFirstInputFilePathMap::value_type(name,inputFilePath));
      }
      matchFound = true;      
      continue;
    }
    if (boost::regex_search(inFileString,matches,textRegex)) {
      // get elements
      name = std::string(matches[1].first,matches[1].second);
      definition = std::string(matches[2].first,matches[2].second);
      std::string temp(std::string::const_iterator(inFileString.begin()),matches[0].first);
      temp += std::string(matches[0].second,std::string::const_iterator(inFileString.end()));
      inFileString = temp;
      if (boost::regex_match(definition,matches,removeQuotes)) {
        definition = std::string(matches[1].first,matches[1].second);
      }
      boost::trim(name); boost::trim(definition);
      // throw out for now
      matchFound = true;
      continue;
    }
    if (boost::regex_search(inFileString,matches,objectRegex)) {
      // get elements
      name = std::string(matches[1].first,matches[1].second);
      definition = std::string(matches[2].first,matches[2].second);
      std::string temp(std::string::const_iterator(inFileString.begin()),matches[0].first);
      temp += std::string(matches[0].second,std::string::const_iterator(inFileString.end()));
      inFileString = temp;
      if (boost::regex_match(definition,matches,removeQuotes)) {
        definition = std::string(matches[1].first,matches[1].second);
      }
      boost::trim(name); boost::trim(definition);
      // write to DataDictionary.xml
      outputFiles.ofDataDictionaryXml
        << "    <Term>" << std::endl
        << "      <name>" << name << "</name>" << std::endl
        << "      <definition>" << definition << "</definition>" << std::endl
        << "      <type><noun/></type>" << std::endl
        << "    </Term>" << std::endl;
      // write to ObjectType.hxx or write error to screen
      if (history.objectTypesMap.find(name) != history.objectTypesMap.end()) {
        std::cout << "Duplicate object type named '" << name 
          << "' found in file " << inputFilePath << ". Object type originally listed in " 
          << history.objectTypesMap.find(name)->second << ". Both definitions included "
          << "in XML file." << std::endl << std::endl;
      }
      else {
        outputFiles.ofObjectTypeHxx
          << "  ((" << convertName(name) << ")(" << name << "))" << std::endl;
        history.objectTypesMap.insert(GenerateDataDictionaryHistory::
            NameToFirstInputFilePathMap::value_type(name,inputFilePath));
      }
      matchFound = true;      
      continue;
    }
  }

  boost::trim(inFileString);
  if (!inFileString.empty()) {
    std::cout << "Have part of file left: " << std::endl
              << inFileString << std::endl << std::endl;
  }

}

std::string convertName(const std::string& originalName) {
  std::string result(originalName);
  boost::trim(result);
  result = boost::regex_replace(result,boost::regex("^100 ?%"),"All");
  result = boost::regex_replace(result,boost::regex("[ \\-]+"),"\\u");
  result = boost::regex_replace(result,boost::regex("[:/,\\(\\)%]+"),"_");
  result = boost::regex_replace(result,boost::regex("[\\.\\?\\]\\[]"),"");
  result = boost::regex_replace(result,boost::regex("="),"_EQUAL_");
  result = boost::regex_replace(result,boost::regex("\\*\\*"),"_POW_");
  result = boost::regex_replace(result,boost::regex("\\+"),"_PLUS_");
  result = boost::regex_replace(result,boost::regex("\\*"),"_TIMES_");
  result = boost::regex_replace(result,boost::regex("^\\d[.]*"),"_$&");
  return result;
}

} // standardsinterface
} // openstudio
