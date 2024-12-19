/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GenerateIddFactory.hpp"
#include "WriteEnums.hpp"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <exception>

namespace openstudio {

IddFileFactoryDataVector constructIddFileObjects(const std::vector<std::string>& iddArgs) {
  IddFileFactoryDataVector result;
  // Construct one IddFileFactoryData object per Idd file input by the user. Constructor will
  // throw if it is unable to parse iddArg or is unable to validate the path or name.
  for (const std::string& iddArg : iddArgs) {
    result.push_back(IddFileFactoryData(iddArg));
  }
  return result;
}

void initializeOutFiles(GenerateIddFactoryOutFiles& outFiles, const std::vector<IddFileFactoryData>& iddFiles) {
  // start IddEnums.hxx
  outFiles.iddEnumsHxx.tempFile << "#ifndef UTILITIES_IDD_IDDENUMS_HXX" << '\n'
                                << "#define UTILITIES_IDD_IDDENUMS_HXX" << '\n'
                                << '\n'
                                << "#include <utilities/core/Enum.hpp>" << '\n'
                                << '\n'
                                << "#include <boost/optional.hpp>" << '\n'
                                << '\n'
                                << "#include <vector>" << '\n'
                                << "#include <set>" << '\n'
                                << '\n'
                                << "namespace openstudio {" << '\n';

  // start IddFieldEnums.hxx
  outFiles.iddFieldEnumsHxx.tempFile << "#ifndef UTILITIES_IDD_IDDFIELDENUMS_HXX" << '\n'
                                     << "#define UTILITIES_IDD_IDDFIELDENUMS_HXX" << '\n'
                                     << '\n'
                                     << "/** \\file IddFieldEnums.hxx" << '\n'
                                     << " *  IddFieldEnums.hxx includes one file per IddObjectType (excluding Catchall, UserCustom, " << '\n'
                                     << " *  and CommentOnly). Each file so included contains one or two OPENSTUDIO_ENUM macros that " << '\n'
                                     << " *  provide enum and string access to the non-extensible fields and extensible groups of " << '\n'
                                     << " *  the corresponding IddObjects. This file should not be included directly, but rather, " << '\n'
                                     << " *  used as a reference to help locate the individual field enum files that should be " << '\n'
                                     << " *  included in a particular source file. */" << '\n'
                                     << '\n';

  // start IddFieldEnums.ixx
  outFiles.iddFieldEnumsIxx.tempFile << "#ifndef UTILITIES_IDD_IDDFIELDENUMS_IXX" << '\n'
                                     << "#define UTILITIES_IDD_IDDFIELDENUMS_IXX" << '\n'
                                     << '\n';

  // write IddFactory.hxx
  outFiles.iddFactoryHxx.tempFile << "#ifndef UTILITIES_IDD_IDDFACTORY_HXX" << '\n'
                                  << "#define UTILITIES_IDD_IDDFACTORY_HXX" << '\n'
                                  << '\n'
                                  << "#include <utilities/UtilitiesAPI.hpp>" << '\n'
                                  << '\n'
                                  << "#include <utilities/idd/IddObject.hpp>" << '\n'
                                  << "#include <utilities/idd/IddFile.hpp>" << '\n'
                                  << "#include <utilities/idd/IddEnums.hpp>" << '\n'
                                  << "#include <utilities/idd/IddEnums.hxx>" << '\n'
                                  << '\n'
                                  << "#include <utilities/core/Singleton.hpp>" << '\n'
                                  << "#include <utilities/core/Compare.hpp>" << '\n'
                                  << "#include <utilities/core/Logger.hpp>" << '\n'
                                  << '\n'
                                  << "#include <map>" << '\n'
                                  << '\n'
                                  << "namespace openstudio{" << '\n'
                                  << '\n'
                                  << "/** IddFactorySingleton can return \\link IddObject IddObjects\\endlink and \\link IddFile" << '\n'
                                  << " *  IddFiles\\endlink for the current version of EnergyPlus and OpenStudio. It can return \\link IddFile "
                                  << '\n'
                                  << " *  IddFiles\\endlink only (no link with other methods in this class, for instance, no " << '\n'
                                  << " *  by-IddObjectType access to individual \\link IddObject IddObjects\\endlink) for " << '\n'
                                  << " *  previous versions of OpenStudio, back to version 0.7.0. It also supports the default IddObject " << '\n'
                                  << " *  type, Catchall, which is provided to enable the display of IdfObjects with misspelled type " << '\n'
                                  << " *  names, and a CommentOnly object. Its primary function is to establish a single set of IDD schema " << '\n'
                                  << " *  across all of OpenStudio, thereby ensuring consistency and reducing file I-O. " << '\n'
                                  << " *  " << '\n'
                                  << " *  IddFactorySingleton should be used through the IddFactory typedef as in" << '\n'
                                  << " *  \\code" << '\n'
                                  << " *  IddFile osIddFile = IddFactory::instance().iddFile(IddFileType::OpenStudio);" << '\n'
                                  << " *  \\endcode */" << '\n'
                                  << "class UTILITIES_API IddFactorySingleton  {" << '\n'
                                  << '\n'
                                  << "  friend class Singleton<IddFactorySingleton>;" << '\n'
                                  << '\n'
                                  << " public:" << '\n'
                                  << "  /** @name Getters */" << '\n'
                                  << "  //@{ " << '\n'
                                  << '\n'
                                  << "  /** Get the file version. Throws if fileType == IddFileType::UserCustom or " << '\n'
                                  << "   *  IddFileType::WholeFactory. */" << '\n'
                                  << "  std::string getVersion(IddFileType fileType) const;" << '\n'
                                  << '\n'
                                  << "  /** Get the file header. Throws if fileType == IddFileType::UserCustom or " << '\n'
                                  << "   *  IddFileType::WholeFactory. */" << '\n'
                                  << "  std::string getHeader(IddFileType fileType) const;" << '\n'
                                  << '\n'
                                  << "  /** Return all \\link IddObject IddObjects\\endlink registered in factory. */" << '\n'
                                  << "  std::vector<IddObject> objects() const;" << '\n'
                                  << '\n'
                                  << "  /** Return all \\link IddObject IddObjects\\endlink in IddFileType type. */" << '\n'
                                  << "  std::vector<IddObject> getObjects(IddFileType fileType) const;" << '\n'
                                  << '\n'
                                  << "  /** Return all groups in the factory (e.g. \"\" and \"Simulation Parameters\"). */" << '\n'
                                  << "  std::vector<std::string> groups() const;" << '\n'
                                  << '\n'
                                  << "  /** Return all groups in fileType. */" << '\n'
                                  << "  std::vector<std::string> getGroups(IddFileType fileType) const;" << '\n'
                                  << '\n'
                                  << "  /** Return all \\link IddObject IddObjects\\endlink in group (e.g. \"Simulation Parameters\"). */" << '\n'
                                  << "  std::vector<IddObject> getObjectsInGroup(const std::string& group) const;" << '\n'
                                  << '\n'
                                  << "  /** Return all \\link IddObject IddObjects\\endlink in group and fileType. */" << '\n'
                                  << "  std::vector<IddObject> getObjectsInGroup(const std::string& group, IddFileType fileType) const;" << '\n'
                                  << '\n'
                                  << "  /** Return all \\link IddObject IddObjects\\endlink that match objectRegex. */" << '\n'
                                  << "  std::vector<IddObject> getObjects(const boost::regex& objectRegex) const;" << '\n'
                                  << '\n'
                                  << "  /** Return all \\link IddObject IddObjects\\endlink that match objectRegex and are in fileType. */" << '\n'
                                  << "  std::vector<IddObject> getObjects(const boost::regex& objectRegex, IddFileType fileType) const;" << '\n'
                                  << '\n'
                                  << "  /** Returns the version IddObject for fileType. Throws if fileType == IddFileType::UserCustom " << '\n'
                                  << "   *  or IddFileType::WholeFactory. */" << '\n'
                                  << "  IddObject getVersionObject(IddFileType fileType) const;" << '\n'
                                  << '\n'
                                  << "  /** Return the IddObject with .name() objectName, if it exists in the factory. */" << '\n'
                                  << "  boost::optional<IddObject> getObject(const std::string& objectName) const;" << '\n'
                                  << '\n'
                                  << "  /** Return the IddObject corresponding to objectType. Returns false if type == " << '\n'
                                  << "   *  IddObjectType::UserCustom. */" << '\n'
                                  << "  boost::optional<IddObject> getObject(IddObjectType objectType) const;" << '\n'
                                  << '\n'
                                  << "  /** Return all objects in the factory that are required. */" << '\n'
                                  << "  std::vector<IddObject> requiredObjects() const;" << '\n'
                                  << '\n'
                                  << "  /** Return all objects in IddFile fileType that are required. */" << '\n'
                                  << "  std::vector<IddObject> getRequiredObjects(IddFileType fileType) const;" << '\n'
                                  << '\n'
                                  << "  /** Return all objects in the factory that are unique. */" << '\n'
                                  << "  std::vector<IddObject> uniqueObjects() const;" << '\n'
                                  << '\n'
                                  << "  /** Return all objects in IddFile fileType that are unique. */" << '\n'
                                  << "  std::vector<IddObject> getUniqueObjects(IddFileType fileType) const;" << '\n'
                                  << '\n'
                                  << "  /** Return the (current) IddFile corresponding to type. */" << '\n'
                                  << "  IddFile getIddFile(IddFileType fileType) const;" << '\n'
                                  << '\n'
                                  << "  /** Return the IddFile corresponding to type and version. Unless version is equal to " << '\n'
                                  << "   *  VersionString(getVersion(fileType)), files are loaded from disk and returned as " << '\n'
                                  << "   *  IddFileType::UserCustom, to distinguish them from the IDD information wrapped by " << '\n'
                                  << "   *  the IddFactory. At this time, only IddFileType::OpenStudio files are supported, " << '\n'
                                  << "   *  and only for valid version identifiers >= VersionString(\"0.7.0\"). Returns false " << '\n'
                                  << "   *  in all other cases. */" << '\n'
                                  << "  boost::optional<IddFile> getIddFile(IddFileType fileType, const VersionString& version) const;" << '\n'
                                  << '\n'
                                  << "  //@}" << '\n'
                                  << "  /** @name Queries */" << '\n'
                                  << "  //@{" << '\n'
                                  << '\n'
                                  << "  /** Returns true if IddObject of objectType belongs to IddFile of fileType. " << '\n'
                                  << "   *  IddObjectType::Catchall is in no \\link IddFile IddFiles\\endlink; " << '\n'
                                  << "   *  IddObjectType::CommentOnly is in all \\link IddFile IddFiles\\endlink. */" << '\n'
                                  << "  bool isInFile(IddObjectType objectType, IddFileType fileType) const;" << '\n'
                                  << '\n'
                                  << "  //@}" << '\n'
                                  << " private:" << '\n'
                                  << '\n'
                                  << "  IddFactorySingleton();" << '\n';
  for (const IddFileFactoryData& iddFile : iddFiles) {
    outFiles.iddFactoryHxx.tempFile << "  void register" << iddFile.fileName() << "ObjectsInCallbackMap();" << '\n';
  }
  outFiles.iddFactoryHxx.tempFile << '\n'
                                  << "  REGISTER_LOGGER(\"utilities.idd.IddFactory\");" << '\n'
                                  << '\n'
                                  << "  typedef std::function<IddObject ()> CreateIddObjectCallback;" << '\n'
                                  << "  typedef std::map<IddObjectType,CreateIddObjectCallback> IddObjectCallbackMap;" << '\n'
                                  << "  IddObjectCallbackMap m_callbackMap;" << '\n'
                                  << '\n'
                                  << "  typedef std::multimap<IddObjectType,IddFileType> IddObjectSourceFileMap;" << '\n'
                                  << "  IddObjectSourceFileMap m_sourceFileMap;" << '\n'
                                  << '\n'
                                  << "  mutable std::map<VersionString,IddFile> m_osIddFiles;" << '\n'
                                  << "};" << '\n'
                                  << '\n'
                                  << "#if _WIN32 || _MSC_VER" << '\n'
                                  << "  // Explicitly instantiate and export IddFactorySingleton Singleton template instance" << '\n'
                                  << "  // so that the same instance is shared between DLLs that link to Utilities.dll." << '\n'
                                  << "  UTILITIES_TEMPLATE_EXT template class UTILITIES_API openstudio::Singleton<IddFactorySingleton>;" << '\n'
                                  << "#endif" << '\n'
                                  << '\n'
                                  << "/** Convenience typedef for accessing IddFactorySingleton. Usage:" << '\n'
                                  << " *  \\code" << '\n'
                                  << " *  unsigned n = IddFactory::instance().objects().size();" << '\n'
                                  << " *  \\endcode" << '\n'
                                  << " *  " << '\n'
                                  << " *  \\relates IddFactorySingleton */" << '\n'
                                  << "typedef openstudio::Singleton<IddFactorySingleton> IddFactory;" << '\n'
                                  << '\n'
                                  << "} // openstudio" << '\n'
                                  << '\n'
                                  << "#endif //UTILITIES_IDD_IDDFACTORY_HXX" << '\n';

  // start IddFactory.cxx
  outFiles.iddFactoryCxx.tempFile << "#include <utilities/idd/IddFactory.hxx>" << '\n'
                                  << "#include <utilities/idd/IddEnums.hxx>" << '\n'
                                  << "#include <utilities/idd/IddRegex.hpp>" << '\n'
                                  << '\n'
                                  << "#include <utilities/core/Assert.hpp>" << '\n'
                                  << "#include <utilities/core/Compare.hpp>" << '\n'
                                  << "#include <utilities/core/Containers.hpp>" << '\n'
                                  << "#include <utilities/embedded_files.hxx>" << '\n'
                                  << '\n'
                                  << "#include <OpenStudio.hxx>" << '\n'
                                  << '\n'
                                  << "namespace openstudio {" << '\n'
                                  << '\n'
                                  << "IddObject createCatchallIddObject() {" << '\n'
                                  << '\n'
                                  << "  static const IddObject object;" << '\n'
                                  << '\n'
                                  << "  // Catchall is the type of IddObject returned by the default constructor." << '\n'
                                  << "  OS_ASSERT(object.type() == IddObjectType::Catchall);" << '\n'
                                  << "  return object;" << '\n'
                                  << "}" << '\n';

  // start other IddFactory cxx files
  for (const std::shared_ptr<IddFactoryOutFile>& cxxFile : outFiles.iddFactoryIddFileCxxs) {
    cxxFile->tempFile << "#include <utilities/idd/IddFactory.hxx>" << '\n'
                      << "#include <utilities/idd/IddEnums.hxx>" << '\n'
                      << '\n'
                      << "#include <utilities/core/Assert.hpp>" << '\n'
                      << "#include <utilities/core/Compare.hpp>" << '\n'
                      << '\n'
                      << '\n'
                      << "namespace openstudio {" << '\n';
  }

  std::cout << "IddFactory files initialized." << '\n' << '\n';
}

void completeOutFiles(const IddFileFactoryDataVector& iddFiles, GenerateIddFactoryOutFiles& outFiles) {

  std::vector<std::pair<std::string, std::string>> filetypes{{"UserCustom", ""}, {"WholeFactory", ""}};
  for (const IddFileFactoryData& idd : iddFiles) {
    filetypes.emplace_back(idd.fileName(), "");
  }

  std::stringstream tempSS;
  writeDomain(tempSS, "IddFileType", filetypes, false);

  outFiles.iddEnumsHxx.tempFile << '\n' << "/** \\class IddFileType */" << '\n' << tempSS.str();
  tempSS.str("");

  std::vector<std::pair<std::string, std::string>> objtypes{{"Catchall", ""}, {"UserCustom", ""}};

  // loop through each IDD file
  for (const IddFileFactoryData& idd : iddFiles) {
    // write out an IddObjectType enum value for each object in the IDD file
    for (const StringPair& objectName : idd.objectNames()) {
      objtypes.emplace_back(objectName.first, objectName.second);
    }
  }

  objtypes.emplace_back("CommentOnly", "");
  writeDomain(tempSS, "IddObjectType", objtypes, false);

  outFiles.iddEnumsHxx.tempFile << '\n'
                                << "/** \\class IddObjectType */" << '\n'
                                << tempSS.str() << '\n'
                                << "}\n"
                                << "#endif // UTILITIES_IDD_IDDENUMS_HXX" << '\n';
  tempSS.str("");

  // complete and close IddFieldEnums.hxx
  outFiles.iddFieldEnumsHxx.tempFile << '\n' << "#endif // UTILITIES_IDD_IDDFIELDENUMS_HXX" << '\n';

  // complete and close IddFieldEnums.ixx
  outFiles.iddFieldEnumsIxx.tempFile << '\n' << "#endif // UTILITIES_IDD_IDDFIELDENUMS_IXX" << '\n';

  // complete and close IddFactory.cxx

  // create function for CommentOnly IddObject
  outFiles.iddFactoryCxx.tempFile << '\n'
                                  << "IddObject createCommentOnlyIddObject() {" << '\n'
                                  << '\n'
                                  << "  static const IddObject object = []{" << '\n'
                                  << "    // use C++11 statics and initialize on first use idiom to ensure static" << '\n'
                                  << "    // is initialized safely exactly once, eliminating need for mutexes" << '\n'
                                  << "    std::stringstream ss;" << '\n'
                                  << "    ss << \"CommentOnly; ! Autogenerated comment only object.\" << '\\n';" << '\n'
                                  << '\n'
                                  << "    IddObjectType objType(IddObjectType::CommentOnly);" << '\n'
                                  << "    OptionalIddObject oObj = IddObject::load(\"CommentOnly\"," << '\n'
                                  << "                                             \"\"," << '\n'
                                  << "                                             ss.str()," << '\n'
                                  << "                                             objType);" << '\n'
                                  << "    OS_ASSERT(oObj);" << '\n'
                                  << "    return *oObj;" << '\n'
                                  << "  }(); // immediately invoked lambda" << '\n'
                                  << '\n'
                                  << "  return object;" << '\n'
                                  << "}" << '\n';

  // constructor
  outFiles.iddFactoryCxx.tempFile << '\n'
                                  << "IddFactorySingleton::IddFactorySingleton() {" << '\n'
                                  << '\n'
                                  << "  // initialize callback map" << '\n'
                                  << '\n';
  // register create functions in the callback map
  // Catchall
  outFiles.iddFactoryCxx.tempFile << "  m_callbackMap.insert(IddObjectCallbackMap::value_type(IddObjectType::Catchall,"
                                  << "createCatchallIddObject));" << '\n';
  // parsed objects
  for (const IddFileFactoryData& idd : iddFiles) {
    outFiles.iddFactoryCxx.tempFile << "  register" << idd.fileName() << "ObjectsInCallbackMap();" << '\n';
  }
  // CommentOnly
  outFiles.iddFactoryCxx.tempFile << "  m_callbackMap.insert(IddObjectCallbackMap::value_type(IddObjectType::CommentOnly,"
                                  << "createCommentOnlyIddObject));" << '\n'
                                  << '\n'
                                  << "  // instantiate IddObjectType to IddFileType multimap" << '\n'
                                  << '\n';
  // register IddObjectTypes with IddFileTypes
  for (const IddFileFactoryData& idd : iddFiles) {
    std::string fileName = idd.fileName();

    // register local objects
    for (const StringPair& objectName : idd.objectNames()) {
      outFiles.iddFactoryCxx.tempFile << "  m_sourceFileMap.insert(IddObjectSourceFileMap::value_type(IddObjectType::" << objectName.first
                                      << ",IddFileType::" << fileName << "));" << '\n';
    }

    // register imported objects
    for (unsigned i = 0, ni = idd.numIncludedFiles(); i < ni; ++i) {
      /* Included files are identified by name. Optionally, there may be a list of removed objects
         that should not be inherited by the composite file. */
      IddFileFactoryData::FileNameRemovedObjectsPair includedFileData = idd.includedFile(i);
      // Get the included file by name. Will throw if there is no match.
      IddFileFactoryData includedFile = getFile(includedFileData.first, iddFiles);

      std::vector<std::string> excludedObjects;
      for (const StringPair& objectName : includedFile.objectNames()) {

        // If objectName is in list of removed objects, do not add it to m_sourceFileMap,
        if (std::find(includedFileData.second.begin(), includedFileData.second.end(), objectName.first) != includedFileData.second.end()) {
          // and keep its name in case we need to write a warning.
          excludedObjects.push_back(objectName.first);
          continue;
        }

        // objectName is not to be removed, so add it to the composite file.
        outFiles.iddFactoryCxx.tempFile << "  m_sourceFileMap.insert(IddObjectSourceFileMap::value_type(IddObjectType::" << objectName.first
                                        << ",IddFileType::" << fileName << "));" << '\n';

      }  // foreach

      // Write warning if we did not encounter all of the objects that were to be removed.
      if (excludedObjects.size() != includedFileData.second.size()) {
        std::cout << "Warning: Did not encounter all 'objects to remove' while including file " << '\n'
                  << "'" << includedFileData.first << "' in file '" << fileName << "'." << '\n'
                  << "\\remove-object Tags      Objects Actually Removed " << '\n'
                  << "------------------------- -------------------------" << '\n';
        for (int x = 0, y = 0, nx = includedFileData.second.size(), ny = excludedObjects.size(); (x < nx) || (y < ny); ++x, ++y) {
          if (x < nx) {
            std::cout << std::setw(25) << includedFileData.second[x] << " ";
          } else {
            std::cout << std::setw(25) << " " << " ";
          }
          if (y < ny) {
            std::cout << std::setw(25) << excludedObjects[y] << '\n';
          } else {
            std::cout << std::setw(25) << " " << '\n';
          }
        }  // for
      }  // if

    }  // for

    // Register CommentOnly object for all files.
    outFiles.iddFactoryCxx.tempFile << "  m_sourceFileMap.insert(IddObjectSourceFileMap::value_type(IddObjectType::CommentOnly"
                                    << ",IddFileType::" << fileName << "));" << '\n';
  }
  outFiles.iddFactoryCxx.tempFile << '\n' << "}" << '\n';

  // version and header getters
  outFiles.iddFactoryCxx.tempFile << '\n'
                                  << "std::string IddFactorySingleton::getVersion(IddFileType fileType) const {" << '\n'
                                  << "  std::string result;" << '\n'
                                  << '\n'
                                  << "  switch (fileType.value()) {" << '\n';
  for (const IddFileFactoryData& idd : iddFiles) {
    outFiles.iddFactoryCxx.tempFile << "    case IddFileType::" << idd.fileName() << " :" << '\n'
                                    << "      result = \"" << idd.version() << "\";" << '\n'
                                    << "      break;" << '\n';
  }
  outFiles.iddFactoryCxx.tempFile << "    default :" << '\n'
                                  << "      LOG_AND_THROW(\"No version to return for IddFileType \" << fileType.valueDescription() << \".\");" << '\n'
                                  << "  } // switch" << '\n'
                                  << '\n'
                                  << "  return result;" << '\n'
                                  << "}" << '\n'
                                  << '\n'
                                  << "std::string IddFactorySingleton::getHeader(IddFileType fileType) const {" << '\n'
                                  << "  std::stringstream result;" << '\n'
                                  << "  switch (fileType.value()) {" << '\n';
  for (const IddFileFactoryData& idd : iddFiles) {
    outFiles.iddFactoryCxx.tempFile << "    case IddFileType::" << idd.fileName() << " :" << '\n' << "      result";
    std::stringstream headerStream(idd.header());
    std::string line;
    while (std::getline(headerStream, line)) {
      outFiles.iddFactoryCxx.tempFile << '\n' << "        << \"" << line << "\" << '\\n'";
    }
    // print information about included files
    for (unsigned i = 0, ni = idd.numIncludedFiles(); i < ni; ++i) {
      IddFileFactoryData::FileNameRemovedObjectsPair includedFileData = idd.includedFile(i);
      IddFileFactoryData includedFile = getFile(includedFileData.first, iddFiles);
      outFiles.iddFactoryCxx.tempFile << '\n'
                                      << "        << \"!\\n\"" << '\n'
                                      << "        << \"! **************************************************************************\\n\"" << '\n'
                                      << "        << \"! Includes File: '" << includedFile.fileName() << "'\\n\"" << '\n'
                                      << "        << \"!\\n\"" << '\n'
                                      << "        << \"! Contains all objects from " << includedFile.fileName() << " IDD_Version "
                                      << includedFile.version() << ", except: \\n\"" << '\n';
      for (const std::string& objectName : includedFileData.second) {
        outFiles.iddFactoryCxx.tempFile << "        << \"!   " << objectName << "\\n\"" << '\n';
      }
      outFiles.iddFactoryCxx.tempFile << "        << \"! **************************************************************************\\n\"";
    }
    outFiles.iddFactoryCxx.tempFile << ";" << '\n' << '\n' << "      break;" << '\n';
  }
  outFiles.iddFactoryCxx.tempFile << "    default :" << '\n'
                                  << "      LOG_AND_THROW(\"No header to return for IddFileType \" << fileType.valueDescription() << \".\");" << '\n'
                                  << "  } // switch" << '\n'
                                  << "  return result.str();" << '\n'
                                  << "}" << '\n';

  // object getters
  outFiles.iddFactoryCxx.tempFile
    << '\n'
    << "std::vector<IddObject> IddFactorySingleton::objects() const {" << '\n'
    << "  IddObjectVector result;" << '\n'
    << '\n'
    << "  for (IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << '\n'
    << "       itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << '\n'
    << "    result.push_back(it->second());" << '\n'
    << "  }" << '\n'
    << '\n'
    << "  return result;" << '\n'
    << "}" << '\n'
    << '\n'
    << "std::vector<IddObject> IddFactorySingleton::getObjects(IddFileType fileType) const {" << '\n'
    << "  IddObjectVector result;" << '\n'
    << '\n'
    << "  for(IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << '\n'
    << "      itend = m_callbackMap.end(); it != itend; ++it) {" << '\n'
    << "    if (isInFile(it->first,fileType)) { " << '\n'
    << "      result.push_back(it->second()); " << '\n'
    << "    }" << '\n'
    << "  }" << '\n'
    << '\n'
    << "  return result;" << '\n'
    << "}" << '\n'
    << '\n'
    << "std::vector<std::string> IddFactorySingleton::groups() const {" << '\n'
    << "  StringSet result;" << '\n'
    << "  for (const IddObject& object : objects()) {" << '\n'
    << "    result.insert(object.group());" << '\n'
    << "  }" << '\n'
    << "  return StringVector(result.begin(),result.end());" << '\n'
    << "}" << '\n'
    << '\n'
    << "std::vector<std::string> IddFactorySingleton::getGroups(IddFileType fileType) const {" << '\n'
    << "  StringSet result;" << '\n'
    << "  for (const IddObject& object : getObjects(fileType)) {" << '\n'
    << "    result.insert(object.group());" << '\n'
    << "  }" << '\n'
    << "  return StringVector(result.begin(),result.end());" << '\n'
    << "}" << '\n'
    << '\n'
    << "std::vector<IddObject> IddFactorySingleton::getObjectsInGroup(const std::string& group) const {" << '\n'
    << "  IddObjectVector result;" << '\n'
    << "  for (const IddObject& object : objects()) {" << '\n'
    << "    if (istringEqual(object.group(),group)) {" << '\n'
    << "      result.push_back(object);" << '\n'
    << "    }" << '\n'
    << "  }" << '\n'
    << "  return result;" << '\n'
    << "}" << '\n'
    << '\n'
    << "std::vector<IddObject> IddFactorySingleton::getObjectsInGroup(const std::string& group, IddFileType fileType) const {" << '\n'
    << "  IddObjectVector result;" << '\n'
    << "  for (const IddObject& object : getObjects(fileType)) {" << '\n'
    << "    if (istringEqual(object.group(),group)) {" << '\n'
    << "      result.push_back(object);" << '\n'
    << "    }" << '\n'
    << "  }" << '\n'
    << "  return result;" << '\n'
    << "}" << '\n'
    << '\n'
    << "std::vector<IddObject> IddFactorySingleton::getObjects(const boost::regex& objectRegex) const {" << '\n'
    << "  IddObjectVector result;" << '\n'
    << '\n'
    << "  for (int value : IddObjectType::getValues()) {" << '\n'
    << "    IddObjectType candidate(value);" << '\n'
    << "    if (boost::regex_match(candidate.valueName(),objectRegex) || " << '\n'
    << "        boost::regex_match(candidate.valueDescription(),objectRegex))" << '\n'
    << "    {" << '\n'
    << "      if (OptionalIddObject object = getObject(candidate)) {" << '\n'
    << "        result.push_back(*object);" << '\n'
    << "      }" << '\n'
    << "    }" << '\n'
    << "  }" << '\n'
    << '\n'
    << "  return result;" << '\n'
    << "}" << '\n'
    << '\n'
    << "std::vector<IddObject> IddFactorySingleton::getObjects(const boost::regex& objectRegex," << '\n'
    << "                                                       IddFileType fileType) const " << '\n'
    << "{" << '\n'
    << "  IddObjectVector result;" << '\n'
    << '\n'
    << "  for (int value : IddObjectType::getValues()) {" << '\n'
    << "    IddObjectType candidate(value);" << '\n'
    << "    if (isInFile(candidate,fileType) && " << '\n'
    << "        (boost::regex_match(candidate.valueName(),objectRegex) || " << '\n'
    << "         boost::regex_match(candidate.valueDescription(),objectRegex)))" << '\n'
    << "    {" << '\n'
    << "      if (OptionalIddObject object = getObject(candidate)) {" << '\n'
    << "        result.push_back(*object);" << '\n'
    << "      }" << '\n'
    << "    }" << '\n'
    << "  }" << '\n'
    << '\n'
    << "  return result;" << '\n'
    << "}" << '\n'
    << '\n'
    << "IddObject IddFactorySingleton::getVersionObject(IddFileType fileType) const {" << '\n'
    << "  if (fileType == IddFileType::EnergyPlus) {" << '\n'
    << "    return getObject(IddObjectType(IddObjectType::Version)).get();" << '\n'
    << "  }" << '\n'
    << '\n'
    << "  if (fileType == IddFileType::OpenStudio) {" << '\n'
    << "    return getObject(IddObjectType(IddObjectType::OS_Version)).get();" << '\n'
    << "  }" << '\n'
    << '\n'
    << "  LOG_AND_THROW(\"Unable to identify unique version object for IddFileType \" << fileType.valueName() << \".\");" << '\n'
    << "  return IddObject();" << '\n'
    << "}" << '\n'
    << '\n'
    << "boost::optional<IddObject> IddFactorySingleton::getObject(const std::string& objectName) const" << '\n'
    << "{" << '\n'
    << "  OptionalIddObject result;" << '\n'
    << '\n'
    << "  // let IddObjectType OPENSTUDIO_ENUM handle the string processing" << '\n'
    << "  try {" << '\n'
    << "    IddObjectType objectType(objectName);" << '\n'
    << "    result = getObject(objectType);" << '\n'
    << "  }" << '\n'
    << "  catch (...) {}" << '\n'
    << '\n'
    << "  return result;" << '\n'
    << "}" << '\n'
    << '\n'
    << "boost::optional<IddObject> IddFactorySingleton::getObject(IddObjectType objectType) const" << '\n'
    << "{" << '\n'
    << "  OptionalIddObject result;" << '\n'
    << '\n'
    << "  IddObjectCallbackMap::const_iterator lookupPair;" << '\n'
    << "  lookupPair = m_callbackMap.find(objectType);" << '\n'
    << "  if (lookupPair != m_callbackMap.end()) { " << '\n'
    << "    result = lookupPair->second(); " << '\n'
    << "  }" << '\n'
    << "  else { " << '\n'
    << "    OS_ASSERT(objectType == IddObjectType::UserCustom); " << '\n'
    << "    LOG(Info,\"UserCustom objects are not available through the IddFactory. Please query your IddFile by IddObject.name().\");" << '\n'
    << "  }" << '\n'
    << '\n'
    << "  return result;" << '\n'
    << "}" << '\n';

  // required, unique, and required or unique objects
  outFiles.iddFactoryCxx.tempFile << '\n'
                                  << "std::vector<IddObject> IddFactorySingleton::requiredObjects() const {" << '\n'
                                  << '\n'
                                  << "  IddObjectVector result;" << '\n'
                                  << '\n'
                                  << "  for (IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << '\n'
                                  << "    itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << '\n'
                                  << "    const auto candidate = it->second();" << '\n'
                                  << "    if (candidate.properties().required) {" << '\n'
                                  << "      result.push_back(candidate);" << '\n'
                                  << "    }" << '\n'
                                  << "  }" << '\n'
                                  << '\n'
                                  << "  return result;" << '\n'
                                  << "}" << '\n'
                                  << '\n'
                                  << "std::vector<IddObject> IddFactorySingleton::getRequiredObjects(IddFileType fileType) const {" << '\n'
                                  << '\n'
                                  << "  IddObjectVector result; " << '\n'
                                  << '\n'
                                  << "  for(IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << '\n'
                                  << "      itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << '\n'
                                  << "    if (isInFile(it->first,fileType)) {" << '\n'
                                  << "      OptionalIddObject candidate = getObject(it->first);" << '\n'
                                  << "      if (candidate->properties().required) {" << '\n'
                                  << "        result.push_back(*candidate);" << '\n'
                                  << "      }" << '\n'
                                  << "    }" << '\n'
                                  << "  }" << '\n'
                                  << '\n'
                                  << "  return result;" << '\n'
                                  << "}" << '\n'
                                  << '\n'
                                  << "std::vector<IddObject> IddFactorySingleton::uniqueObjects() const {" << '\n'
                                  << '\n'
                                  << "  IddObjectVector result;" << '\n'
                                  << '\n'
                                  << "  for (IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << '\n'
                                  << "    itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << '\n'
                                  << "    const auto candidate = it->second();" << '\n'
                                  << "    if (candidate.properties().unique) {" << '\n'
                                  << "      result.push_back(candidate);" << '\n'
                                  << "    }" << '\n'
                                  << "  }" << '\n'
                                  << '\n'
                                  << "  return result;" << '\n'
                                  << "}" << '\n'
                                  << '\n'
                                  << "std::vector<IddObject> IddFactorySingleton::getUniqueObjects(IddFileType fileType) const {" << '\n'
                                  << '\n'
                                  << "  IddObjectVector result; " << '\n'
                                  << '\n'
                                  << "   for(IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << '\n'
                                  << "      itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << '\n'
                                  << "    if (isInFile(it->first,fileType)) {" << '\n'
                                  << "      OptionalIddObject candidate = getObject(it->first);" << '\n'
                                  << "      if (candidate->properties().unique) {" << '\n'
                                  << "        result.push_back(*candidate);" << '\n'
                                  << "      }" << '\n'
                                  << "    }" << '\n'
                                  << "  }" << '\n'
                                  << '\n'
                                  << "  return result;" << '\n'
                                  << '\n'
                                  << "}" << '\n';

  // iddFile getters
  outFiles.iddFactoryCxx.tempFile
    << '\n'
    << "IddFile IddFactorySingleton::getIddFile(IddFileType fileType) const {" << '\n'
    << "  IddFile result;" << '\n'
    << '\n'
    << "  if (fileType == IddFileType::UserCustom) {" << '\n'
    << "    return result; " << '\n'
    << "  }" << '\n'
    << '\n'
    << "  // Add the IddObjects." << '\n'
    << "  for(IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << '\n'
    << "      itend = m_callbackMap.end(); it != itend; ++it) {" << '\n'
    << "    if (isInFile(it->first,fileType)) {" << '\n'
    << "      result.addObject(it->second());" << '\n'
    << "    }" << '\n'
    << "  }" << '\n'
    << '\n'
    << "  // Set the file version and header." << '\n'
    << "  try {" << '\n'
    << "    result.setVersion(getVersion(fileType));" << '\n'
    << "    result.setHeader(getHeader(fileType));" << '\n'
    << "  }" << '\n'
    << "  catch (...) {}" << '\n'
    << '\n'
    << "  return result;" << '\n'
    << "}" << '\n'
    << '\n'
    << "boost::optional<IddFile> IddFactorySingleton::getIddFile(IddFileType fileType, const VersionString& version) const {" << '\n'
    << "  OptionalIddFile result;" << '\n'
    << '\n'
    << "  if (fileType == IddFileType::UserCustom) {" << '\n'
    << "    return result; " << '\n'
    << "  }" << '\n'
    << '\n'
    << "  if (fileType == IddFileType::WholeFactory) {" << '\n'
    << "    LOG(Warn,\"Cannot return the WholeFactory IddFile by version.\");" << '\n'
    << "    return result;" << '\n'
    << "  }" << '\n'
    << '\n'
    << "  if (fileType == IddFileType::EnergyPlus) {" << '\n'
    << "    LOG(Warn,\"At this time, OpenStudio cannot return EnergyPlus IDD files by version.\");" << '\n'
    << "    return result;" << '\n'
    << "  }" << '\n'
    << '\n'
    << "  VersionString currentVersion(openStudioVersion());" << '\n'
    << "  OS_ASSERT(fileType == IddFileType::OpenStudio);" << '\n'
    << "  if (version == currentVersion) {" << '\n'
    << "    return getIddFile(fileType);" << '\n'
    << "  }" << '\n'
    << "  else {" << '\n'
    << "    std::map<VersionString, IddFile>::const_iterator it = m_osIddFiles.find(version);" << '\n'
    << "    if (it != m_osIddFiles.end()) {" << '\n'
    << "      return it->second;" << '\n'
    << "    }" << '\n'
    << "    std::string iddPath = \":/idd/versions\";" << '\n'
    << "    std::stringstream folderString;" << '\n'
    << "    folderString << version.major() << \"_\" << version.minor() << \"_\" << version.patch().get();" << '\n'
    << "    iddPath += \"/\" + folderString.str() + \"/OpenStudio.idd\";" << '\n'
    << "    if (::openstudio::embedded_files::hasFile(iddPath) && (version < currentVersion)) {" << '\n'
    << "      std::stringstream ss;" << '\n'
    << "      ss << ::openstudio::embedded_files::getFileAsString(iddPath);" << '\n'
    << "      result = IddFile::load(ss);" << '\n'
    << "    }" << '\n'
    << "    if (result) {" << '\n'
    << "      m_osIddFiles[version] = *result;" << '\n'
    << "    }" << '\n'
    << "  }" << '\n'
    << "  return result;" << '\n'
    << "}" << '\n';

  // query whether object is in file
  outFiles.iddFactoryCxx.tempFile << '\n'
                                  << "bool IddFactorySingleton::isInFile(IddObjectType objectType, IddFileType fileType) const {" << '\n'
                                  << "  typedef IddObjectSourceFileMap::const_iterator const_iterator;" << '\n'
                                  << "  std::pair<const_iterator,const_iterator> range;" << '\n'
                                  << "  range = m_sourceFileMap.equal_range(objectType);" << '\n'
                                  << "  for (const_iterator it = range.first; it != range.second; ++it) {" << '\n'
                                  << "    if ((it->second == fileType) || (fileType == IddFileType::WholeFactory)) {" << '\n'
                                  << "      return true;" << '\n'
                                  << "    }" << '\n'
                                  << "  }" << '\n'
                                  << '\n'
                                  << "  return false;" << '\n'
                                  << "}" << '\n';

  // Implementation for IddObjectType and IddFileType
  writeBuildStringVec(outFiles.iddFactoryCxx.tempFile, "IddObjectType", objtypes, false);
  writeBuildStringVec(outFiles.iddFactoryCxx.tempFile, "IddFileType", filetypes, false);

  // close out file
  outFiles.iddFactoryCxx.tempFile << '\n' << "} // openstudio" << '\n';

  // close out other IddFactory cxx files
  for (const std::shared_ptr<IddFactoryOutFile>& cxxFile : outFiles.iddFactoryIddFileCxxs) {
    cxxFile->tempFile << '\n' << "} // openstudio" << '\n';
  }

  outFiles.finalize();

  std::cout << "IddFactory files generated." << '\n';
}

IddFileFactoryData getFile(const std::string& fileName, const IddFileFactoryDataVector& iddFiles) {
  for (const IddFileFactoryData& idd : iddFiles) {
    if (idd.fileName() == fileName) {
      return idd;
    }
  }

  // unsuccessful search, throw an informative message
  std::stringstream ss;
  ss << "Unable to located included Idd file '" << fileName << "' in list of Idd files. " << "The available files are named:" << '\n';
  for (const IddFileFactoryData& idd : iddFiles) {
    ss << "  " << idd.fileName() << '\n';
  }
  throw std::runtime_error(ss.str().c_str());
}

}  // namespace openstudio
