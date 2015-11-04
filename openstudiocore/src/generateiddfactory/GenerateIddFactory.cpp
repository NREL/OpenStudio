/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

void initializeOutFiles(GenerateIddFactoryOutFiles& outFiles,
                        const std::vector<IddFileFactoryData>& iddFiles) 
{
  // start IddEnums.hxx
  outFiles.iddEnumsHxx.tempFile
    << "#ifndef UTILITIES_IDD_IDDENUMS_HXX" << std::endl
    << "#define UTILITIES_IDD_IDDENUMS_HXX" << std::endl
    << std::endl
    << "#include <utilities/core/Enum.hpp>" << std::endl
    << std::endl
    << "#include <boost/optional.hpp>" << std::endl
    << std::endl
    << "#include <vector>" << std::endl
    << "#include <set>" << std::endl
    << std::endl
    << "#include <QMetaType>" << std::endl
    << std::endl
    << "namespace openstudio {" << std::endl;

  // start IddFieldEnums.hxx
  outFiles.iddFieldEnumsHxx.tempFile
    << "#ifndef UTILITIES_IDD_IDDFIELDENUMS_HXX" << std::endl
    << "#define UTILITIES_IDD_IDDFIELDENUMS_HXX" << std::endl
    << std::endl
    << "/** \\file IddFieldEnums.hxx" << std::endl
    << " *  IddFieldEnums.hxx includes one file per IddObjectType (excluding Catchall, UserCustom, " << std::endl
    << " *  and CommentOnly). Each file so included contains one or two OPENSTUDIO_ENUM macros that " << std::endl
    << " *  provide enum and string access to the non-extensible fields and extensible groups of "  << std::endl
    << " *  the corresponding IddObjects. This file should not be included directly, but rather, " << std::endl
    << " *  used as a reference to help locate the individual field enum files that should be " << std::endl
    << " *  included in a particular source file. */" << std::endl
    << std::endl;

  // start IddFieldEnums.ixx
  outFiles.iddFieldEnumsIxx.tempFile
    << "#ifndef UTILITIES_IDD_IDDFIELDENUMS_IXX" << std::endl
    << "#define UTILITIES_IDD_IDDFIELDENUMS_IXX" << std::endl
    << std::endl;

  // write IddFactory.hxx
  outFiles.iddFactoryHxx.tempFile
    << "#ifndef UTILITIES_IDD_IDDFACTORY_HXX" << std::endl
    << "#define UTILITIES_IDD_IDDFACTORY_HXX" << std::endl
    << std::endl
    << "#include <utilities/UtilitiesAPI.hpp>" << std::endl
    << std::endl
    << "#include <utilities/idd/IddObject.hpp>" << std::endl
    << "#include <utilities/idd/IddFile.hpp>" << std::endl
    << "#include <utilities/idd/IddEnums.hpp>" << std::endl
    << "#include <utilities/idd/IddEnums.hxx>" << std::endl
    << std::endl
    << "#include <utilities/core/Singleton.hpp>" << std::endl
    << "#include <utilities/core/Compare.hpp>" << std::endl
    << "#include <utilities/core/Logger.hpp>" << std::endl
    << "#include <QMutex>" << std::endl
    << std::endl
    << "#include <map>" << std::endl
    << std::endl
    << "namespace openstudio{" << std::endl
    << std::endl
    << "/** IddFactorySingleton can return \\link IddObject IddObjects\\endlink and \\link IddFile" << std::endl
    << " *  IddFiles\\endlink for the current version of EnergyPlus and OpenStudio. It can return \\link IddFile " << std::endl
    << " *  IddFiles\\endlink only (no link with other methods in this class, for instance, no " << std::endl
    << " *  by-IddObjectType access to individual \\link IddObject IddObjects\\endlink) for " << std::endl
    << " *  previous versions of OpenStudio, back to version 0.7.0. It also supports the default IddObject " << std::endl
    << " *  type, Catchall, which is provided to enable the display of IdfObjects with misspelled type " << std::endl
    << " *  names, and a CommentOnly object. Its primary function is to establish a single set of IDD schema " << std::endl
    << " *  across all of OpenStudio, thereby ensuring consistency and reducing file I-O. " << std::endl
    << " *  " << std::endl
    << " *  IddFactorySingleton should be used through the IddFactory typedef as in" << std::endl
    << " *  \\code" << std::endl
    << " *  IddFile osIddFile = IddFactory::instance().iddFile(IddFileType::OpenStudio);" << std::endl
    << " *  \\endcode */" << std::endl
    << "class UTILITIES_API IddFactorySingleton  {" << std::endl
    << std::endl
    << "  friend class Singleton<IddFactorySingleton>;" << std::endl
    << std::endl
    << " public:" << std::endl
    << "  /** @name Getters */" << std::endl
    << "  //@{ " << std::endl
    << std::endl
    << "  /** Get the file version. Throws if fileType == IddFileType::UserCustom or " << std::endl
    << "   *  IddFileType::WholeFactory. */" << std::endl
    << "  std::string getVersion(IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Get the file header. Throws if fileType == IddFileType::UserCustom or " << std::endl
    << "   *  IddFileType::WholeFactory. */" << std::endl
    << "  std::string getHeader(IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Return all \\link IddObject IddObjects\\endlink registered in factory. */" << std::endl
    << "  std::vector<IddObject> objects() const;" << std::endl
    << std::endl
    << "  /** Return all \\link IddObject IddObjects\\endlink in IddFileType type. */" << std::endl
    << "  std::vector<IddObject> getObjects(IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Return all groups in the factory (e.g. \"\" and \"Simulation Parameters\"). */" << std::endl
    << "  std::vector<std::string> groups() const;" << std::endl
    << std::endl
    << "  /** Return all groups in fileType. */" << std::endl
    << "  std::vector<std::string> getGroups(IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Return all \\link IddObject IddObjects\\endlink in group (e.g. \"Simulation Parameters\"). */" << std::endl
    << "  std::vector<IddObject> getObjectsInGroup(const std::string& group) const;" << std::endl
    << std::endl
    << "  /** Return all \\link IddObject IddObjects\\endlink in group and fileType. */" << std::endl
    << "  std::vector<IddObject> getObjectsInGroup(const std::string& group, IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Return all \\link IddObject IddObjects\\endlink that match objectRegex. */" << std::endl
    << "  std::vector<IddObject> getObjects(const boost::regex& objectRegex) const;" << std::endl
    << std::endl
    << "  /** Return all \\link IddObject IddObjects\\endlink that match objectRegex and are in fileType. */" << std::endl
    << "  std::vector<IddObject> getObjects(const boost::regex& objectRegex, IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Returns the version IddObject for fileType. Throws if fileType == IddFileType::UserCustom " << std::endl
    << "   *  or IddFileType::WholeFactory. */" << std::endl
    << "  IddObject getVersionObject(IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Return the IddObject with .name() objectName, if it exists in the factory. */" << std::endl
    << "  boost::optional<IddObject> getObject(const std::string& objectName) const;" << std::endl
    << std::endl
    << "  /** Return the IddObject corresponding to objectType. Returns false if type == " << std::endl
    << "   *  IddObjectType::UserCustom. */" << std::endl
    << "  boost::optional<IddObject> getObject(IddObjectType objectType) const;" << std::endl
    << std::endl
    << "  /** Return all objects in the factory that are required. */" << std::endl
    << "  std::vector<IddObject> requiredObjects() const;" << std::endl
    << std::endl
    << "  /** Return all objects in IddFile fileType that are required. */" << std::endl
    << "  std::vector<IddObject> getRequiredObjects(IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Return all objects in the factory that are unique. */" << std::endl
    << "  std::vector<IddObject> uniqueObjects() const;" << std::endl
    << std::endl
    << "  /** Return all objects in IddFile fileType that are unique. */" << std::endl
    << "  std::vector<IddObject> getUniqueObjects(IddFileType fileType) const;" << std::endl    
    << std::endl
    << "  /** Return the (current) IddFile corresponding to type. */" << std::endl
    << "  IddFile getIddFile(IddFileType fileType) const;" << std::endl
    << std::endl
    << "  /** Return the IddFile corresponding to type and version. Unless version is equal to " << std::endl
    << "   *  VersionString(getVersion(fileType)), files are loaded from disk and returned as " << std::endl
    << "   *  IddFileType::UserCustom, to distinguish them from the IDD information wrapped by " << std::endl
    << "   *  the IddFactory. At this time, only IddFileType::OpenStudio files are supported, " << std::endl
    << "   *  and only for valid version identifiers >= VersionString(\"0.7.0\"). Returns false " << std::endl
    << "   *  in all other cases. */" << std::endl
    << "  boost::optional<IddFile> getIddFile(IddFileType fileType, const VersionString& version) const;" << std::endl
    << std::endl
    << "  //@}" << std::endl
    << "  /** @name Queries */" << std::endl
    << "  //@{" << std::endl
    << std::endl
    << "  /** Returns true if IddObject of objectType belongs to IddFile of fileType. " << std::endl
    << "   *  IddObjectType::Catchall is in no \\link IddFile IddFiles\\endlink; " << std::endl
    << "   *  IddObjectType::CommentOnly is in all \\link IddFile IddFiles\\endlink. */" << std::endl
    << "  bool isInFile(IddObjectType objectType, IddFileType fileType) const;" << std::endl
    << std::endl
    << "  //@}" << std::endl
    << " private:" << std::endl
    << std::endl
    << "  IddFactorySingleton();" << std::endl;
  for (const IddFileFactoryData& iddFile : iddFiles) {
    outFiles.iddFactoryHxx.tempFile
      << "  void register" << iddFile.fileName() << "ObjectsInCallbackMap();" << std::endl;
  }
  outFiles.iddFactoryHxx.tempFile
    << std::endl
    << "  REGISTER_LOGGER(\"utilities.idd.IddFactory\");" << std::endl
    << std::endl
    << "  typedef std::function<IddObject ()> CreateIddObjectCallback;" << std::endl
    << "  typedef std::map<IddObjectType,CreateIddObjectCallback> IddObjectCallbackMap;" << std::endl
    << "  IddObjectCallbackMap m_callbackMap;" << std::endl
    << "  mutable QMutex m_callbackmutex;" << std::endl
    << std::endl
    << "  typedef std::multimap<IddObjectType,IddFileType> IddObjectSourceFileMap;" << std::endl
    << "  IddObjectSourceFileMap m_sourceFileMap;" << std::endl
    << std::endl
    << "  mutable std::map<VersionString,IddFile> m_osIddFiles;" << std::endl
    << "};" << std::endl
    << std::endl
    << "#if _WIN32 || _MSC_VER" << std::endl
    << "  // Explicitly instantiate and export IddFactorySingleton Singleton template instance" << std::endl
    << "  // so that the same instance is shared between DLLs that link to Utilities.dll." << std::endl
    << "  UTILITIES_TEMPLATE_EXT template class UTILITIES_API openstudio::Singleton<IddFactorySingleton>;" << std::endl
    << "#endif" << std::endl
    << std::endl
    << "/** Convenience typedef for accessing IddFactorySingleton. Usage:" << std::endl
    << " *  \\code" << std::endl
    << " *  unsigned n = IddFactory::instance().objects().size();" << std::endl
    << " *  \\endcode" << std::endl
    << " *  " << std::endl
    << " *  \\relates IddFactorySingleton */" << std::endl
    << "typedef openstudio::Singleton<IddFactorySingleton> IddFactory;" << std::endl
    << std::endl
    << "} // openstudio" << std::endl
    << std::endl
    << "#endif //UTILITIES_IDD_IDDFACTORY_HXX" << std::endl;

  // start IddFactory.cxx
  outFiles.iddFactoryCxx.tempFile
    << "#include <utilities/idd/IddFactory.hxx>" << std::endl
    << "#include <utilities/idd/IddEnums.hxx>" << std::endl
    << "#include <utilities/idd/IddRegex.hpp>" << std::endl
    << std::endl
    << "#include <utilities/core/Assert.hpp>" << std::endl
    << "#include <utilities/core/Compare.hpp>" << std::endl
    << "#include <utilities/core/Containers.hpp>" << std::endl
    << "#include <utilities/core/ApplicationPathHelpers.hpp>" << std::endl
    << std::endl
    << "#include <OpenStudio.hxx>" << std::endl
    << std::endl
    << "#include <QMutexLocker>" << std::endl
    << "#include <QMetaType>" << std::endl
    << std::endl
    << "int _IddObjectType_id = qRegisterMetaType<openstudio::IddObjectType>(\"openstudio::IddObjectType\");" << std::endl
    << "int _IddFileType_id = qRegisterMetaType<openstudio::IddFileType>(\"openstudio::IddFileType\");" << std::endl
    << std::endl
    << "namespace openstudio {" << std::endl
    << std::endl
    << "IddObject createCatchallIddObject() {" << std::endl
    << std::endl
    << "  static IddObject object;" << std::endl
    << std::endl
    << "  // Catchall is the type of IddObject returned by the default constructor." << std::endl
    << "  OS_ASSERT(object.type() == IddObjectType::Catchall);" << std::endl
    << "  return object;" << std::endl
    << "}" << std::endl;

  // start other IddFactory cxx files
  for (std::shared_ptr<IddFactoryOutFile>& cxxFile : outFiles.iddFactoryIddFileCxxs) {
    cxxFile->tempFile
      << "#include <utilities/idd/IddFactory.hxx>" << std::endl
      << "#include <utilities/idd/IddEnums.hxx>" << std::endl
      << std::endl
      << "#include <utilities/core/Assert.hpp>" << std::endl
      << "#include <utilities/core/Compare.hpp>" << std::endl
      << std::endl
      << "#include <QMutexLocker>" << std::endl
      << std::endl
      << "namespace openstudio {" << std::endl;
  }

  std::cout << "IddFactory files initialized." << std::endl << std::endl;
}


void completeOutFiles(const IddFileFactoryDataVector& iddFiles,
                      GenerateIddFactoryOutFiles& outFiles) {


  std::vector<std::pair<std::string, std::string>> filetypes{ {"UserCustom", ""}, {"WholeFactory", ""} };
  for (const IddFileFactoryData& idd : iddFiles) {
    filetypes.emplace_back(idd.fileName(), "");
  }


  std::stringstream tempSS;
  writeDomain(tempSS, "IddFileType", filetypes, false);

  outFiles.iddEnumsHxx.tempFile
    << std::endl
    << "/** \\class IddFileType */" << std::endl
    << tempSS.str();
  tempSS.str("");

  std::vector<std::pair<std::string, std::string>> objtypes{ {"Catchall", ""}, {"UserCustom", ""} };

  // loop through each IDD file
  for (const IddFileFactoryData& idd : iddFiles) {
    // write out an IddObjectType enum value for each object in the IDD file
    for (const StringPair& objectName : idd.objectNames()) {
      objtypes.emplace_back(objectName.first, objectName.second);
    }
  }

  objtypes.emplace_back("CommentOnly", "");
  writeDomain(tempSS, "IddObjectType", objtypes, false);

  outFiles.iddEnumsHxx.tempFile
    << std::endl
    << "/** \\class IddObjectType */" << std::endl
    << tempSS.str()
    << std::endl
    << "}\n"
    << "#endif // UTILITIES_IDD_IDDENUMS_HXX" << std::endl;
  tempSS.str("");
  
  // complete and close IddFieldEnums.hxx
  outFiles.iddFieldEnumsHxx.tempFile
    << std::endl
    << "#endif // UTILITIES_IDD_IDDFIELDENUMS_HXX" << std::endl;

  // complete and close IddFieldEnums.ixx
  outFiles.iddFieldEnumsIxx.tempFile
    << std::endl
    << "#endif // UTILITIES_IDD_IDDFIELDENUMS_IXX" << std::endl;

  // complete and close IddFactory.cxx

  // create function for CommentOnly IddObject
  outFiles.iddFactoryCxx.tempFile
    << std::endl
    << "IddObject createCommentOnlyIddObject() {" << std::endl
    << std::endl
    << "  static IddObject object;" << std::endl
    << std::endl
    << "  if (object.type() == IddObjectType::Catchall) {" << std::endl
    << "    std::stringstream ss;" << std::endl
    << "    ss << \"CommentOnly; ! Autogenerated comment only object.\" << std::endl;" << std::endl
    << std::endl
    << "    IddObjectType objType(IddObjectType::CommentOnly);" << std::endl
    << "    OptionalIddObject oObj = IddObject::load(\"CommentOnly\"," << std::endl
    << "                                             \"\"," << std::endl
    << "                                             ss.str()," << std::endl
    << "                                             objType);" << std::endl
    << "    OS_ASSERT(oObj);" << std::endl
    << "    object = *oObj;" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  return object;" << std::endl
    << "}" << std::endl;

  // constructor
  outFiles.iddFactoryCxx.tempFile
    << std::endl
    << "IddFactorySingleton::IddFactorySingleton() {" << std::endl
    << std::endl
    << "  // initialize callback map" << std::endl
    << std::endl;
  // register create functions in the callback map
  // Catchall
  outFiles.iddFactoryCxx.tempFile
    << "  m_callbackMap.insert(IddObjectCallbackMap::value_type(IddObjectType::Catchall,"
    << "createCatchallIddObject));" << std::endl;
  // parsed objects
  for (const IddFileFactoryData& idd : iddFiles) {
    outFiles.iddFactoryCxx.tempFile
      << "  register" << idd.fileName() << "ObjectsInCallbackMap();" << std::endl;
  }
  // CommentOnly
  outFiles.iddFactoryCxx.tempFile
    << "  m_callbackMap.insert(IddObjectCallbackMap::value_type(IddObjectType::CommentOnly,"
    << "createCommentOnlyIddObject));" << std::endl
    << std::endl
    << "  // instantiate IddObjectType to IddFileType multimap" << std::endl
    << std::endl;
  // register IddObjectTypes with IddFileTypes
  for (const IddFileFactoryData& idd : iddFiles) {
    std::string fileName = idd.fileName();

    // register local objects
    for (const StringPair& objectName : idd.objectNames()) {
      outFiles.iddFactoryCxx.tempFile
        << "  m_sourceFileMap.insert(IddObjectSourceFileMap::value_type(IddObjectType::" 
        << objectName.first << ",IddFileType::" << fileName << "));" << std::endl;
    }

    // register imported objects
    for (unsigned i = 0, ni = idd.numIncludedFiles(); i < ni; ++i) {
      /* Included files are identified by name. Optionally, there may be a list of removed objects
         that should not be inherited by the composite file. */
      IddFileFactoryData::FileNameRemovedObjectsPair includedFileData = idd.includedFile(i);
      // Get the included file by name. Will throw if there is no match.
      IddFileFactoryData includedFile = getFile(includedFileData.first,iddFiles);

      std::vector<std::string> excludedObjects;
      for (const StringPair& objectName : includedFile.objectNames()) {

        // If objectName is in list of removed objects, do not add it to m_sourceFileMap,
        if (std::find(includedFileData.second.begin(),includedFileData.second.end(),objectName.first) 
            != includedFileData.second.end()) {
          // and keep its name in case we need to write a warning.
          excludedObjects.push_back(objectName.first);  
          continue;
        }

        // objectName is not to be removed, so add it to the composite file.
        outFiles.iddFactoryCxx.tempFile
          << "  m_sourceFileMap.insert(IddObjectSourceFileMap::value_type(IddObjectType::" 
          << objectName.first << ",IddFileType::" << fileName << "));" << std::endl;

      } // foreach

      // Write warning if we did not encounter all of the objects that were to be removed.
      if (excludedObjects.size() != includedFileData.second.size()) {
        std::cout << "Warning: Did not encounter all 'objects to remove' while including file " << std::endl
                  << "'" << includedFileData.first << "' in file '" << fileName << "'." << std::endl
                  << "\\remove-object Tags      Objects Actually Removed " << std::endl
                  << "------------------------- -------------------------" << std::endl;
        for (int x = 0, y = 0, nx = includedFileData.second.size(), ny = excludedObjects.size();
             (x < nx) || (y < ny); ++x, ++y) {
          if (x < nx) { std::cout << std::setw(25) << includedFileData.second[x] << " "; }
          else { std::cout << std::setw(25) << " " << " "; }
          if (y < ny) { std::cout << std::setw(25)<< excludedObjects[y] << std::endl; }
          else { std::cout << std::setw(25) << " " << std::endl; }
        } // for
      } // if

    } // for

    // Register CommentOnly object for all files.
    outFiles.iddFactoryCxx.tempFile
        << "  m_sourceFileMap.insert(IddObjectSourceFileMap::value_type(IddObjectType::CommentOnly"
        << ",IddFileType::" << fileName << "));" << std::endl;
  }
  outFiles.iddFactoryCxx.tempFile
    << std::endl
    << "}" << std::endl;

  // version and header getters
  outFiles.iddFactoryCxx.tempFile
      << std::endl
      << "std::string IddFactorySingleton::getVersion(IddFileType fileType) const {" << std::endl
      << "  std::string result;" << std::endl
      << std::endl
      << "  switch (fileType.value()) {" << std::endl;
  for (const IddFileFactoryData& idd : iddFiles) {
    outFiles.iddFactoryCxx.tempFile
        << "    case IddFileType::" << idd.fileName() << " :" << std::endl
        << "      result = \"" << idd.version() << "\";" << std::endl
        << "      break;" << std::endl;
  }
  outFiles.iddFactoryCxx.tempFile
      << "    default :" << std::endl
      << "      LOG_AND_THROW(\"No version to return for IddFileType \" << fileType.valueDescription() << \".\");" << std::endl
      << "  } // switch" << std::endl
      << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl
      << std::endl
      << "std::string IddFactorySingleton::getHeader(IddFileType fileType) const {" << std::endl
      << "  std::stringstream result;" << std::endl
      << "  switch (fileType.value()) {" << std::endl;
  for (const IddFileFactoryData& idd : iddFiles) {
    outFiles.iddFactoryCxx.tempFile
        << "    case IddFileType::" << idd.fileName() << " :" << std::endl
        << "      result";
    std::stringstream headerStream(idd.header());
    std::string line;
    while (std::getline(headerStream,line)) {
      outFiles.iddFactoryCxx.tempFile
          << std::endl
          << "        << \"" << line << "\" << std::endl";
    }
    // print information about included files
    for (unsigned i = 0, ni = idd.numIncludedFiles(); i < ni; ++i) {
      IddFileFactoryData::FileNameRemovedObjectsPair includedFileData = idd.includedFile(i);
      IddFileFactoryData includedFile = getFile(includedFileData.first,iddFiles);
      outFiles.iddFactoryCxx.tempFile
        << std::endl
        << "        << \"!\\n\"" << std::endl
        << "        << \"! **************************************************************************\\n\"" << std::endl
        << "        << \"! Includes File: '" << includedFile.fileName() << "'\\n\"" << std::endl
        << "        << \"!\\n\"" << std::endl
        << "        << \"! Contains all objects from " << includedFile.fileName() << " IDD_Version " << includedFile.version() << ", except: \\n\"" << std::endl;
      for (const std::string& objectName : includedFileData.second) {
        outFiles.iddFactoryCxx.tempFile
          << "        << \"!   " << objectName << "\\n\"" << std::endl;
      }
      outFiles.iddFactoryCxx.tempFile
        << "        << \"! **************************************************************************\\n\"";
    }
    outFiles.iddFactoryCxx.tempFile
      << ";" << std::endl
      << std::endl
      << "      break;" << std::endl;
  }
 outFiles.iddFactoryCxx.tempFile
     << "    default :" << std::endl
     << "      LOG_AND_THROW(\"No header to return for IddFileType \" << fileType.valueDescription() << \".\");" << std::endl
     << "  } // switch" << std::endl
     << "  return result.str();" << std::endl
     << "}" << std::endl;

  // object getters
  outFiles.iddFactoryCxx.tempFile
      << std::endl
      << "std::vector<IddObject> IddFactorySingleton::objects() const {" << std::endl
      << "  IddObjectVector result;" << std::endl
      << std::endl
      << "  for (IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << std::endl
      << "       itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << std::endl
      << "    QMutexLocker l(&m_callbackmutex);" << std::endl
      << "    result.push_back(it->second());" << std::endl
      << "  }" << std::endl
      << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl
      << std::endl
      << "std::vector<IddObject> IddFactorySingleton::getObjects(IddFileType fileType) const {" << std::endl
      << "  IddObjectVector result;" << std::endl
      << std::endl
      << "  for(IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << std::endl
      << "      itend = m_callbackMap.end(); it != itend; ++it) {" << std::endl
      << "    if (isInFile(it->first,fileType)) { " << std::endl
      << "      // This lock is necessary to protect construction of the statics used in the callbacks " << std::endl
      << "      QMutexLocker l(&m_callbackmutex);" << std::endl
      << "      result.push_back(it->second()); " << std::endl
      << "    }" << std::endl
      << "  }" << std::endl
      << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl
      << std::endl
      << "std::vector<std::string> IddFactorySingleton::groups() const {" << std::endl
      << "  StringSet result;" << std::endl
      << "  for (const IddObject& object : objects()) {" << std::endl
      << "    result.insert(object.group());" << std::endl
      << "  }" << std::endl
      << "  return StringVector(result.begin(),result.end());" << std::endl
      << "}" << std::endl
      << std::endl
      << "std::vector<std::string> IddFactorySingleton::getGroups(IddFileType fileType) const {" << std::endl
      << "  StringSet result;" << std::endl
      << "  for (const IddObject& object : getObjects(fileType)) {" << std::endl
      << "    result.insert(object.group());" << std::endl
      << "  }" << std::endl
      << "  return StringVector(result.begin(),result.end());" << std::endl
      << "}" << std::endl
      << std::endl
      << "std::vector<IddObject> IddFactorySingleton::getObjectsInGroup(const std::string& group) const {" << std::endl
      << "  IddObjectVector result;" << std::endl
      << "  for (const IddObject& object : objects()) {" << std::endl
      << "    if (istringEqual(object.group(),group)) {" << std::endl
      << "      result.push_back(object);" << std::endl
      << "    }" << std::endl
      << "  }" << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl
      << std::endl
      << "std::vector<IddObject> IddFactorySingleton::getObjectsInGroup(const std::string& group, IddFileType fileType) const {" << std::endl
      << "  IddObjectVector result;" << std::endl
      << "  for (const IddObject& object : getObjects(fileType)) {" << std::endl
      << "    if (istringEqual(object.group(),group)) {" << std::endl
      << "      result.push_back(object);" << std::endl
      << "    }" << std::endl
      << "  }" << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl
      << std::endl
      << "std::vector<IddObject> IddFactorySingleton::getObjects(const boost::regex& objectRegex) const {" << std::endl
      << "  IddObjectVector result;" << std::endl
      << std::endl
      << "  for (int value : IddObjectType::getValues()) {" << std::endl
      << "    IddObjectType candidate(value);" << std::endl
      << "    if (boost::regex_match(candidate.valueName(),objectRegex) || " << std::endl
      << "        boost::regex_match(candidate.valueDescription(),objectRegex))" << std::endl
      << "    {" << std::endl
      << "      if (OptionalIddObject object = getObject(candidate)) {" << std::endl
      << "        result.push_back(*object);" << std::endl
      << "      }" << std::endl
      << "    }" << std::endl
      << "  }" << std::endl
      << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl
      << std::endl
      << "std::vector<IddObject> IddFactorySingleton::getObjects(const boost::regex& objectRegex," << std::endl
      << "                                                       IddFileType fileType) const " << std::endl
      << "{" << std::endl
      << "  IddObjectVector result;" << std::endl
      << std::endl
      << "  for (int value : IddObjectType::getValues()) {" << std::endl
      << "    IddObjectType candidate(value);" << std::endl
      << "    if (isInFile(candidate,fileType) && " << std::endl
      << "        (boost::regex_match(candidate.valueName(),objectRegex) || " << std::endl
      << "         boost::regex_match(candidate.valueDescription(),objectRegex)))" << std::endl
      << "    {" << std::endl
      << "      if (OptionalIddObject object = getObject(candidate)) {" << std::endl
      << "        result.push_back(*object);" << std::endl
      << "      }" << std::endl
      << "    }" << std::endl
      << "  }" << std::endl
      << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl
      << std::endl
      << "IddObject IddFactorySingleton::getVersionObject(IddFileType fileType) const {" << std::endl
      << "  if (fileType == IddFileType::EnergyPlus) {" << std::endl
      << "    return getObject(IddObjectType(IddObjectType::Version)).get();" << std::endl
      << "  }" << std::endl
      << std::endl
      << "  if (fileType == IddFileType::OpenStudio) {" << std::endl
      << "    return getObject(IddObjectType(IddObjectType::OS_Version)).get();" << std::endl
      << "  }" << std::endl
      << std::endl
      << "  LOG_AND_THROW(\"Unable to identify unique version object for IddFileType \" << fileType.valueName() << \".\");" << std::endl
      << "  return IddObject();" << std::endl
      << "}" << std::endl
      << std::endl
      << "boost::optional<IddObject> IddFactorySingleton::getObject(const std::string& objectName) const" << std::endl
      << "{" << std::endl
      << "  OptionalIddObject result;" << std::endl
      << std::endl
      << "  // let IddObjectType OPENSTUDIO_ENUM handle the string processing" << std::endl
      << "  try {" << std::endl
      << "    IddObjectType objectType(objectName);" << std::endl
      << "    result = getObject(objectType);" << std::endl
      << "  }" << std::endl
      << "  catch (...) {}" << std::endl
      << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl
      << std::endl
      << "boost::optional<IddObject> IddFactorySingleton::getObject(IddObjectType objectType) const" << std::endl
      << "{" << std::endl
      << "  OptionalIddObject result;" << std::endl
      << std::endl
      << "  IddObjectCallbackMap::const_iterator lookupPair;" << std::endl
      << "  lookupPair = m_callbackMap.find(objectType);" << std::endl
      << "  if (lookupPair != m_callbackMap.end()) { " << std::endl
      << "    QMutexLocker l(&m_callbackmutex);" << std::endl
      << "    result = lookupPair->second(); " << std::endl
      << "  }" << std::endl
      << "  else { " << std::endl
      << "    OS_ASSERT(objectType == IddObjectType::UserCustom); " << std::endl
      << "    LOG(Info,\"UserCustom objects are not available through the IddFactory. Please query your IddFile by IddObject.name().\");" << std::endl
      << "  }" << std::endl
      << std::endl
      << "  return result;" << std::endl
      << "}" << std::endl;

  // required, unique, and required or unique objects
  outFiles.iddFactoryCxx.tempFile
    << std::endl
    << "std::vector<IddObject> IddFactorySingleton::requiredObjects() const {" << std::endl
    << std::endl
    << "  IddObjectVector result;" << std::endl
    << std::endl
    << "  for (IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << std::endl
    << "    itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << std::endl
    << "    // This lock is necessary to protect construction of the statics used in the callbacks " << std::endl
    << "    QMutexLocker l(&m_callbackmutex);" << std::endl
    << "    IddObject candidate = it->second();" << std::endl
    << "    l.unlock(); " << std::endl
    << "    if (candidate.properties().required) {" << std::endl
    << "      result.push_back(candidate);" << std::endl
    << "    }" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  return result;" << std::endl
    << "}" << std::endl
    << std::endl
    << "std::vector<IddObject> IddFactorySingleton::getRequiredObjects(IddFileType fileType) const {" << std::endl
    << std::endl
    << "  IddObjectVector result; " << std::endl
    << std::endl
    << "  for(IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << std::endl
    << "      itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << std::endl
    << "    if (isInFile(it->first,fileType)) {" << std::endl
    << "      OptionalIddObject candidate = getObject(it->first);" << std::endl
    << "      if (candidate->properties().required) {" << std::endl
    << "        result.push_back(*candidate);" << std::endl
    << "      }" << std::endl
    << "    }" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  return result;" << std::endl
    << "}" << std::endl
    << std::endl
    << "std::vector<IddObject> IddFactorySingleton::uniqueObjects() const {" << std::endl
    << std::endl
    << "  IddObjectVector result;" << std::endl
    << std::endl
    << "  for (IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << std::endl
    << "    itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << std::endl
    << "    QMutexLocker l(&m_callbackmutex);" << std::endl
    << "    IddObject candidate = it->second();" << std::endl
    << "    l.unlock(); " << std::endl
    << "    if (candidate.properties().unique) {" << std::endl
    << "      result.push_back(candidate);" << std::endl
    << "    }" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  return result;" << std::endl
    << "}" << std::endl
    << std::endl
    << "std::vector<IddObject> IddFactorySingleton::getUniqueObjects(IddFileType fileType) const {" << std::endl
    << std::endl
    << "  IddObjectVector result; " << std::endl
    << std::endl
    << "   for(IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << std::endl
    << "      itEnd = m_callbackMap.end(); it != itEnd; ++it) {" << std::endl
    << "    if (isInFile(it->first,fileType)) {" << std::endl
    << "      OptionalIddObject candidate = getObject(it->first);" << std::endl
    << "      if (candidate->properties().unique) {" << std::endl
    << "        result.push_back(*candidate);" << std::endl
    << "      }" << std::endl
    << "    }" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  return result;" << std::endl
    << std::endl
    << "}" << std::endl;

  // iddFile getters
  outFiles.iddFactoryCxx.tempFile
    << std::endl
    << "IddFile IddFactorySingleton::getIddFile(IddFileType fileType) const {" << std::endl
    << "  IddFile result;" << std::endl
    << std::endl
    << "  if (fileType == IddFileType::UserCustom) {" << std::endl
    << "    return result; " << std::endl
    << "  }" << std::endl
    << std::endl
    << "  // Add the IddObjects." << std::endl
    << "  for(IddObjectCallbackMap::const_iterator it = m_callbackMap.begin()," << std::endl
    << "      itend = m_callbackMap.end(); it != itend; ++it) {" << std::endl
    << "    if (isInFile(it->first,fileType)) {" << std::endl
    << "      // This lock is necessary to protect construction of the statics used in the callbacks " << std::endl
    << "      QMutexLocker l(&m_callbackmutex);" << std::endl
    << "      result.addObject(it->second());" << std::endl
    << "    }" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  // Set the file version and header." << std::endl
    << "  try {" << std::endl
    << "    result.setVersion(getVersion(fileType));" << std::endl
    << "    result.setHeader(getHeader(fileType));" << std::endl
    << "  }" << std::endl
    << "  catch (...) {}" << std::endl
    << std::endl
    << "  return result;" << std::endl
    << "}" << std::endl
    << std::endl
    << "boost::optional<IddFile> IddFactorySingleton::getIddFile(IddFileType fileType, const VersionString& version) const {" << std::endl
    << "  OptionalIddFile result;" << std::endl
    << std::endl
    << "  if (fileType == IddFileType::UserCustom) {" << std::endl
    << "    return result; " << std::endl
    << "  }" << std::endl
    << std::endl
    << "  if (fileType == IddFileType::WholeFactory) {" << std::endl
    << "    LOG(Warn,\"Cannot return the WholeFactory IddFile by version.\");" << std::endl
    << "    return result;" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  if (fileType == IddFileType::EnergyPlus) {" << std::endl
    << "    LOG(Warn,\"At this time, OpenStudio cannot return EnergyPlus IDD files by version.\");" << std::endl
    << "    return result;" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  VersionString currentVersion(openStudioVersion());" << std::endl
    << "  OS_ASSERT(fileType == IddFileType::OpenStudio);" << std::endl
    << "  if (version == currentVersion) {" << std::endl
    << "    return getIddFile(fileType);" << std::endl
    << "  }" << std::endl
    << "  else {" << std::endl
    << "    std::map<VersionString, IddFile>::const_iterator it = m_osIddFiles.find(version);" << std::endl
    << "    if (it != m_osIddFiles.end()) {" << std::endl
    << "      return it->second;" << std::endl
    << "    }" << std::endl
    << "    openstudio::path iddPath = getSharedResourcesPath() / toPath(\"osversion\");" << std::endl
    << "    std::stringstream folderString;" << std::endl
    << "    folderString << version.major() << \"_\" << version.minor() << \"_\" << version.patch().get();" << std::endl
    << "    iddPath = iddPath / toPath(folderString.str() + \"/OpenStudio.idd\");" << std::endl
    << "    if (boost::filesystem::exists(iddPath) && (version < currentVersion)) {" << std::endl
    << "      result = IddFile::load(iddPath);" << std::endl
    << "    }" << std::endl
    << "    if (result) {" << std::endl
    << "      QMutexLocker l(&m_callbackmutex);" << std::endl
    << "      m_osIddFiles[version] = *result;" << std::endl
    << "    }" << std::endl
    << "  }" << std::endl
    << "  return result;" << std::endl
    << "}" << std::endl;

  // query whether object is in file
  outFiles.iddFactoryCxx.tempFile
    << std::endl
    << "bool IddFactorySingleton::isInFile(IddObjectType objectType, IddFileType fileType) const {" << std::endl
    << "  typedef IddObjectSourceFileMap::const_iterator const_iterator;" << std::endl
    << "  std::pair<const_iterator,const_iterator> range;" << std::endl
    << "  range = m_sourceFileMap.equal_range(objectType);" << std::endl
    << "  for (const_iterator it = range.first; it != range.second; ++it) {" << std::endl
    << "    if ((it->second == fileType) || (fileType == IddFileType::WholeFactory)) {" << std::endl
    << "      return true;" << std::endl
    << "    }" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  return false;" << std::endl
    << "}" << std::endl;

  // Implementation for IddObjectType and IddFileType
  writeBuildStringVec(outFiles.iddFactoryCxx.tempFile, "IddObjectType", objtypes, false);
  writeBuildStringVec(outFiles.iddFactoryCxx.tempFile, "IddFileType", filetypes, false);

  // close out file
  outFiles.iddFactoryCxx.tempFile
    << std::endl
    << "} // openstudio" << std::endl;

  // close out other IddFactory cxx files
  for (std::shared_ptr<IddFactoryOutFile>& cxxFile : outFiles.iddFactoryIddFileCxxs) {
    cxxFile->tempFile
      << std::endl
      << "} // openstudio" << std::endl;
  }

  outFiles.finalize();

  std::cout << "IddFactory files generated." << std::endl;
}

IddFileFactoryData getFile(const std::string& fileName,
                           const IddFileFactoryDataVector& iddFiles) {
  for (const IddFileFactoryData& idd : iddFiles) {
    if (idd.fileName() == fileName) { return idd; }
  }

  // unsuccessful search, throw an informative message
  std::stringstream ss;
  ss << "Unable to located included Idd file '" << fileName << "' in list of Idd files. "
     << "The available files are named:" << std::endl;
  for (const IddFileFactoryData& idd : iddFiles) {
    ss << "  " << idd.fileName() << std::endl;
  }
  throw std::runtime_error(ss.str().c_str());
}

} // openstudio
