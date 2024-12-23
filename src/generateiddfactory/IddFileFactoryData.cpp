/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "IddFileFactoryData.hpp"
#include "WriteEnums.hpp"

#include "../utilities/idd/IddRegex.hpp"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <sstream>
#include <exception>

namespace openstudio {

IddFileFactoryData::IddFileFactoryData(const std::string& fileNameAndPathPair) {
  std::cout << "Creating new IddFileFactoryData object from input argument '" << fileNameAndPathPair << "'." << '\n';
  std::stringstream ss;

  // decompose based on comma
  boost::regex re("([^,]+),?([^,]*)");
  boost::smatch match;
  bool ok = boost::regex_match(fileNameAndPathPair, match, re);
  if (!ok) {
    ss << "Unable to decompose " << fileNameAndPathPair << " into name and path. Type 'GenerateIddFactory --help' if usage is unclear.";
    throw std::runtime_error(ss.str().c_str());
  }
  std::string firstStr = std::string(match[1].first, match[1].second);
  std::string secondStr = std::string(match[2].first, match[2].second);
  boost::trim(firstStr);
  boost::trim(secondStr);
  if (secondStr.empty()) {
    secondStr = firstStr;
    firstStr.clear();
  }
  m_fileName = firstStr;
  m_filePath = path(secondStr);

  // validate filePath
  m_filePath = openstudio::filesystem::system_complete(m_filePath);
  if (!openstudio::filesystem::is_regular_file(m_filePath)) {
    ss << "Unable to locate intended idd file from input argument '" << fileNameAndPathPair << "'. User-supplied path resolved to '"
       << m_filePath.string() << "'.";
    throw std::runtime_error(ss.str().c_str());
  }

  // validate fileName
  if (m_fileName.empty()) {
    m_fileName = m_filePath.stem().string();
  }
  re = boost::regex("[a-zA-Z]\\w*");
  ok = boost::regex_match(m_fileName, match, re);
  if (!ok) {
    ss << "'" << m_fileName << "' is an invalid Idd file name. " << "Please use Perl regex word characters, and start the name with a letter.";
    throw std::runtime_error(ss.str().c_str());
  }

  std::cout << "IddFileFactoryData object created: " << '\n'
            << "  fileName = " << m_fileName << '\n'
            << "  filePath = " << m_filePath.string() << '\n'
            << '\n';
}

void IddFileFactoryData::parseFile(const path& outPath, const std::string& outFileHeader, GenerateIddFactoryOutFiles& outFiles, int iddFileIndex) {
  std::stringstream ss;

  openstudio::filesystem::ifstream iddFile(m_filePath);
  if (!iddFile) {
    ss << "Unable to open Idd file " << m_fileName << " located at " << m_filePath.string() << ".";
    throw std::runtime_error(ss.str().c_str());
  }

  std::stringstream header;
  std::string line;
  std::string trimLine;
  boost::smatch matches;
  int lineNum = 1;
  const std::shared_ptr<IddFactoryOutFile>& cxxFile = outFiles.iddFactoryIddFileCxxs[iddFileIndex];

  // get version
  std::getline(iddFile, line);
  trimLine = line;
  boost::trim(trimLine);
  bool ok = boost::regex_search(trimLine, matches, iddRegex::version());
  if (!ok) {
    ss << "Idd file " << m_fileName << " located at " << m_filePath.string() << " does not list its version on the first line, which is: " << '\n'
       << line;
    throw std::runtime_error(ss.str().c_str());
  }
  m_version = std::string(matches[1].first, matches[1].second);
  boost::trim(m_version);
  header << m_readyLineForOutput(line) << '\n';

  // get rest of header
  while (std::getline(iddFile, line)) {
    ++lineNum;
    trimLine = line;
    boost::trim(trimLine);
    if (trimLine.empty()) {
      break;
    }
    if (!boost::regex_match(trimLine, iddRegex::commentOnlyLine())) {
      ss << "Idd file " << m_fileName << "'s header is not separated from other data by a blank line.";
      throw std::runtime_error(ss.str().c_str());
    }
    header << m_readyLineForOutput(line) << '\n';
  }
  m_header = header.str();

  // parse body of file
  std::string group;
  std::string includeFile;
  while (std::getline(iddFile, line)) {
    ++lineNum;
    trimLine = line;
    boost::trim(trimLine);

    if (trimLine.empty()) {
      continue;
    }
    if (boost::regex_match(trimLine, iddRegex::commentOnlyLine())) {
      continue;
    }

    // look for group identifier
    if (boost::regex_search(trimLine, matches, iddRegex::group())) {
      group = std::string(matches[1].first, matches[1].second);
      boost::trim(group);
      continue;
    }

    // look for include file
    if (boost::regex_search(trimLine, matches, iddRegex::includeFile())) {
      includeFile = std::string(matches[1].first, matches[1].second);
      boost::trim(includeFile);
      m_includedFiles.push_back(FileNameRemovedObjectsPair(includeFile, std::vector<std::string>()));
      continue;
    }

    // look for object to remove from include file
    if (boost::regex_search(trimLine, matches, iddRegex::removeObject())) {
      if (includeFile.empty()) {
        ss << "Cannot process \\remove-object identifier: " << '\n'
           << line << '\n'
           << "found on line " << lineNum << " of Idd file '" << m_fileName << "', because it " << '\n'
           << "is not preceded by a non-empty \\include-file identifier.";
        throw std::runtime_error(ss.str().c_str());
      }
      std::string objectToRemove = std::string(matches[1].first, matches[1].second);
      objectToRemove = m_convertName(objectToRemove);
      m_includedFiles.back().second.push_back(objectToRemove);
      continue;
    }

    // look for IddObject
    StringPair objectName;
    ok = boost::regex_search(trimLine, matches, iddRegex::line());
    if (!ok) {
      ss << "Unable to extract object name from line " << lineNum << "," << '\n' << line << '\n' << ", of Idd file '" << m_fileName << "'.";
      throw std::runtime_error(ss.str().c_str());
    }
    objectName.second = std::string(matches[1].first, matches[1].second);
    boost::trim(objectName.second);
    objectName.first = m_convertName(objectName.second);
    m_objectNames.push_back(objectName);

    // start writing create function
    cxxFile->tempFile << '\n'
                      << "IddObject create" << objectName.first << "IddObject() {" << '\n'
                      << '\n'
                      << "  static const IddObject object = []{" << '\n'
                      << '\n'
                      << "    // Rely on C++11 static initialization and Initialize on First Use Idiom" << '\n'
                      << "    // to make sure all statics are initialized properly, thread safely" << '\n'
                      << "    std::stringstream ss;" << '\n'
                      << "    ss << \"" << m_readyLineForOutput(line) << "\\n\";";

    // start collecting field names
    // (requires \field tag, which is expected to occur one per line)
    std::vector<std::string> fieldNames;
    std::vector<std::string> extensibleFieldNames;

    // get rest of object
    while (std::getline(iddFile, line)) {
      ++lineNum;
      trimLine = line;
      boost::trim(trimLine);
      if (trimLine.empty()) {
        // finish writing create function
        cxxFile->tempFile << '\n'
                          << '\n'
                          << "    IddObjectType objType(IddObjectType::" << objectName.first << ");" << '\n'
                          << "    OptionalIddObject oObj = IddObject::load(\"" << objectName.second << "\"," << '\n'
                          << "                                             \"" << group << "\"," << '\n'
                          << "                                             ss.str()," << '\n'
                          << "                                             objType);" << '\n'
                          << "    OS_ASSERT(oObj);" << '\n'
                          << "    return *oObj;" << '\n'
                          << "  }(); // immediately invoked lambda" << '\n'
                          << '\n'
                          << "  OS_ASSERT(object.type() == IddObjectType::" << objectName.first << ");" << '\n'
                          << "  return object;" << '\n'
                          << "}" << '\n';

        // write field enums
        if (!fieldNames.empty() || !extensibleFieldNames.empty()) {
          // file for this object
          ss << objectName.first << "_FieldEnums.hxx";
          std::string filename = ss.str();
          ss.str("");

          IddFactoryOutFile fieldEnumHxx(filename, outPath, outFileHeader);
          std::string upperObjectName(objectName.first);
          boost::algorithm::to_upper(upperObjectName);

          std::stringstream tempSS;

          fieldEnumHxx.tempFile << "#ifndef UTILITIES_IDD_" << upperObjectName << "_FIELDENUMS_HXX" << '\n'
                                << "#define UTILITIES_IDD_" << upperObjectName << "_FIELDENUMS_HXX" << '\n'
                                << '\n'
                                << "#include <utilities/UtilitiesAPI.hpp>" << '\n'
                                << "#include <utilities/core/Enum.hpp>" << '\n'
                                << '\n'
                                << "namespace openstudio {";

          if (!fieldNames.empty()) {
            std::list<std::pair<std::string, std::string>> fieldNamePairs;
            for (const std::string& name : fieldNames) {
              fieldNamePairs.emplace_back(m_convertName(name), name);
            }
            writeEnumFast(tempSS, objectName.first + "Fields", fieldNamePairs);

            fieldEnumHxx.tempFile << '\n'
                                  << "/** \\class " << objectName.first << "Fields" << '\n'
                                  << " *  \\brief Enumeration of " << objectName.second << "'s Idd and Idf non-extensible fields. " << '\n'
                                  << " *  \\details This enumeration provides readable, and semi-robust tags to use instead" << '\n'
                                  << " *  of bare unsigned integer field indices. (The enumeration value names are more likely" << '\n'
                                  << " *  to be valid across EnergyPlus and OpenStudio releases, and will break if they become" << '\n'
                                  << " *  invalid.) See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro" << '\n'
                                  << " *  call is:" << '\n'
                                  << " *" << '\n'
                                  << " *  \\code"
                                  << '\n'
                                  //              << tempSS.str() << '\n'
                                  << " *  \\endcode */" << '\n'
                                  << tempSS.str() << '\n';

            tempSS.str("");
          }

          if (!extensibleFieldNames.empty()) {
            std::list<std::pair<std::string, std::string>> extensibleFieldNamePairs;
            for (const std::string& name : extensibleFieldNames) {
              extensibleFieldNamePairs.emplace_back(m_convertName(name), name);
            }
            writeEnumFast(tempSS, objectName.first + "ExtensibleFields", extensibleFieldNamePairs);

            fieldEnumHxx.tempFile << '\n'
                                  << "/** \\class " << objectName.first << "ExtensibleFields" << '\n'
                                  << " *  \\brief Enumeration of " << objectName.second << "'s Idd and Idf extensible fields" << '\n'
                                  << " *  to be used in conjunction with ExtensibleIndex and ExtensibleGroup." << '\n'
                                  << " *  \\details This enumeration provides readable, and semi-robust tags to use instead" << '\n'
                                  << " *  of bare unsigned integer ExtensibleIndex.field indices. (The enumeration value names" << '\n'
                                  << " *  are more likely to be valid across EnergyPlus and OpenStudio releases, and will break" << '\n'
                                  << " *  if they become invalid.)See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro"
                                  << '\n'
                                  << " *  call is:" << '\n'
                                  << " *" << '\n'
                                  << " *  \\code"
                                  << '\n'
                                  //              << tempSS.str() << '\n'
                                  << " *  \\endcode */" << '\n'
                                  << tempSS.str() << '\n';

            tempSS.str("");
          }

          fieldEnumHxx.tempFile << '\n'
                                << "} // openstudio" << '\n'
                                << '\n'
                                << "#endif // UTILITIES_IDD_" << upperObjectName << "_FIELDENUMS_HXX" << '\n';
          outFiles.finalizeIddFactoryOutFile(fieldEnumHxx);

          // add to aggregate headers
          outFiles.iddFieldEnumsHxx.tempFile << "#include <utilities/idd/" << filename << ">" << '\n';
          outFiles.iddFieldEnumsIxx.tempFile << "%include <utilities/idd/" << filename << ">" << '\n';
        }

        break;
      }

      // continue writing create function
      cxxFile->tempFile << '\n' << "    ss << \"" << m_readyLineForOutput(line) << "\\n\";";

      // look for field name
      std::string fieldName;
      if (boost::regex_search(trimLine, matches, iddRegex::name())) {
        fieldName = std::string(matches[1].first, matches[1].second);
        fieldName = boost::regex_replace(fieldName, boost::regex(","), "");
        boost::trim(fieldName);
        if (extensibleFieldNames.empty()) {
          fieldNames.push_back(fieldName);
        } else {
          // strip out numbers
          fieldName = boost::regex_replace(fieldName, iddRegex::numberAndPrecedingSpace(), "", boost::format_first_only);
          boost::trim(fieldName);
          extensibleFieldNames.push_back(fieldName);
        }
      }

      // look for extensible identifier
      if (boost::regex_search(trimLine, iddRegex::beginExtensible())) {
        if (fieldNames.empty()) {
          std::cerr << "Extensible processing unable to proceed because " << objectName.second
                    << " is extensible but is missing \\begin-extensible flag." << '\n'
                    << '\n';
        }
        assert(!fieldNames.empty());
        // cppcheck 1.x chokes on this, 2.3 doesn't, but ignore it in case someone has an older version
        // cppcheck-suppress containerOutOfBounds
        fieldName = fieldNames.back();
        // strip out numbers and transfer from fieldNames to extensibleFieldNames
        fieldName = boost::regex_replace(fieldName, iddRegex::numberAndPrecedingSpace(), "", boost::format_first_only);
        boost::trim(fieldName);
        extensibleFieldNames.push_back(fieldName);
        fieldNames.pop_back();
      }

    }  // while -- IddObject

  }  // while -- IddFile

  iddFile.close();
  std::cout << "Parsed Idd file " << m_fileName << " located at " << m_filePath.string() << "," << '\n'
            << "which contains " << m_objectNames.size() << " objects." << '\n'
            << '\n';
  if (!m_includedFiles.empty()) {
    for (const FileNameRemovedObjectsPair& p : m_includedFiles) {
      std::cout << "Idd file '" << m_fileName << "' includes all but " << p.second.size() << " objects of Idd file '" << p.first << "'." << '\n'
                << '\n';
    }
  }

  // register objects with CallbackMap
  cxxFile->tempFile << "void IddFactorySingleton::register" << fileName() << "ObjectsInCallbackMap() {" << '\n';
  for (const StringPair& objectName : objectNames()) {
    cxxFile->tempFile << "  m_callbackMap.insert(IddObjectCallbackMap::value_type(IddObjectType::" << objectName.first << ",create"
                      << objectName.first << "IddObject));" << '\n';
  }
  cxxFile->tempFile << "}" << '\n' << '\n';
}

std::string IddFileFactoryData::fileName() const {
  return m_fileName;
}

std::string IddFileFactoryData::version() const {
  return m_version;
}

std::string IddFileFactoryData::header() const {
  return m_header;
}

std::vector<std::pair<std::string, std::string>> IddFileFactoryData::objectNames() const {
  return m_objectNames;
}

unsigned IddFileFactoryData::numIncludedFiles() const {
  return m_includedFiles.size();
}

IddFileFactoryData::FileNameRemovedObjectsPair IddFileFactoryData::includedFile(unsigned index) const {
  assert(index < numIncludedFiles());
  return m_includedFiles[index];
}

std::string IddFileFactoryData::m_convertName(const std::string& originalName) {
  std::string result(originalName);
  boost::trim(result);
  result = boost::regex_replace(result, boost::regex("^100 ?%"), "All");
  result = boost::regex_replace(result, boost::regex("[ \\-]+"), "\\u");
  result = boost::regex_replace(result, boost::regex("[:/,\\(\\)%]+"), "_");
  result = boost::regex_replace(result, boost::regex("[\\.\\?\\]\\[]"), "");
  result = boost::regex_replace(result, boost::regex("="), "_EQUAL_");
  result = boost::regex_replace(result, boost::regex("\\*\\*"), "_POW_");
  result = boost::regex_replace(result, boost::regex("\\+"), "_PLUS_");
  result = boost::regex_replace(result, boost::regex("\\*"), "_TIMES_");
  return result;
}

std::string IddFileFactoryData::m_readyLineForOutput(const std::string& line) {
  std::string result(line);
  result = boost::regex_replace(result, boost::regex("\\\\"), "\\\\\\\\");
  result = boost::regex_replace(result, boost::regex("\""), "\\\\\"");
  boost::trim(result);
  return result;
}

}  // namespace openstudio
