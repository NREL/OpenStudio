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

#ifndef GENERATEDATADICTIONARY_GENERATEDATADICTIONARY_HPP
#define GENERATEDATADICTIONARY_GENERATEDATADICTIONARY_HPP

#include <utilities/core/Path.hpp>

#include <boost/filesystem/fstream.hpp>

#include <map>
#include <vector>
#include <string>

namespace openstudio {
namespace standardsinterface {

/** Structure to hold GenerateDataDictionary's output files as they are being written. */
struct GenerateDataDictionaryOutputFiles {
  boost::filesystem::ofstream ofDataDictionaryXml; 
  boost::filesystem::ofstream ofObjectTypeHxx;
  boost::filesystem::ofstream ofBooleanAttributeHxx;
  boost::filesystem::ofstream ofIntegerAttributeHxx;
  boost::filesystem::ofstream ofDoubleAttributeHxx;
  boost::filesystem::ofstream ofEnumAttributeHxx;
  boost::filesystem::ofstream ofEnumAttributeValuesHxx;
  boost::filesystem::ofstream ofDataDictionaryCxx;
};

struct GenerateDataDictionaryHistory {
  typedef std::map<std::string,std::string> NameToFirstInputFilePathMap;
  NameToFirstInputFilePathMap objectTypesMap;
  NameToFirstInputFilePathMap booleanAttributesMap;
  NameToFirstInputFilePathMap integerAttributesMap;
  NameToFirstInputFilePathMap doubleAttributesMap;
  NameToFirstInputFilePathMap enumAttributesMap;
  std::vector<std::string> enumsWithValues;
};

/** Open the output file streams and start writing the output files. All generated files are 
 *  prefixed with outputFileHeader. Call this function before parsing the input files. */
void initializeOutputFiles(const path& outPath,
                           const std::string& outputFileHeader,
                           GenerateDataDictionaryOutputFiles& outputFiles);

/** Finish writing the output files. Call this function after parsing all of the input files. */
void completeOutputFiles(GenerateDataDictionaryOutputFiles& outputFiles,
                         GenerateDataDictionaryHistory& history);

/** Parse an input file. */
void parseInputFile(const std::string& inputFilePath,
                    GenerateDataDictionaryOutputFiles& outputFiles,
                    GenerateDataDictionaryHistory& history);

/** Creates enum-ready name from originalName. */
std::string convertName(const std::string& originalName);

} // standardsinterface
} // openstudio

#endif // GENERATEDATADICTIONARY_GENERATEDATADICTIONARY_HPP
