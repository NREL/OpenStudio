/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "IdfFile.hpp"
#include <utilities/idf/IdfObject_Impl.hpp>  // needed for serialization
#include "IdfRegex.hpp"
#include "ValidityReport.hpp"

#include "../idd/IddRegex.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../idd/CommentRegex.hpp"
#include "../idd/Comments.hpp"

#include "../plot/ProgressBar.hpp"
#include "../core/PathHelpers.hpp"
#include "../core/Assert.hpp"

#include <boost/iostreams/filter/newline.hpp>
#include <boost/iostreams/filtering_stream.hpp>

namespace openstudio {

// CONSTRUCTORS

IdfFile::IdfFile(IddFileType iddFileType) : m_iddFileAndFactoryWrapper(iddFileType) {
  addVersionObject();
}

IdfFile::IdfFile(const IddFile& iddFile) : m_iddFileAndFactoryWrapper(iddFile) {
  addVersionObject();
}

// GETTERS

std::string IdfFile::header() const {
  return m_header;
}

VersionString IdfFile::version() const {
  return VersionString(m_iddFileAndFactoryWrapper.version());
}

IddFile IdfFile::iddFile() const {
  return m_iddFileAndFactoryWrapper.iddFile();
}

IddFileType IdfFile::iddFileType() const {
  return m_iddFileAndFactoryWrapper.iddFileType();
}

boost::optional<IdfObject> IdfFile::versionObject() const {
  OptionalIdfObject result;
  if (m_versionObjectIndices.size() == 1u) {
    unsigned index = *(m_versionObjectIndices.begin());
    result = m_objects[index];
  }
  return result;
}

boost::optional<IdfObject> IdfFile::getObject(unsigned index) const {
  OptionalIdfObject result;
  if (index < m_objects.size()) {
    result = m_objects[index];
  }
  return result;
}

boost::optional<IdfObject> IdfFile::getObject(const Handle& handle) const {
  OptionalIdfObject result;
  if (handle.isNull()) {
    return result;
  }
  for (const IdfObject& object : m_objects) {
    if (object.handle() == handle) {
      result = object;
      break;
    }
  }
  return result;
}

std::vector<IdfObject> IdfFile::objects() const {
  IdfObjectVector result = m_objects;
  for (auto it = m_versionObjectIndices.rbegin(), itEnd = m_versionObjectIndices.rend(); it != itEnd; ++it) {
    auto oit = result.begin();
    for (unsigned i = 0; i < *it; ++i, ++oit) {
      ;
    }
    OS_ASSERT(oit->iddObject().isVersionObject()
              || ((oit->iddObject().type() == IddObjectType::Catchall) && (oit->numFields() > 0u)
                  && (boost::regex_match(oit->getString(0).get(), iddRegex::versionObjectName()))));
    result.erase(oit);
  }
  return result;
}

std::vector<IdfObject> IdfFile::getObjectsByType(IddObjectType objectType) const {
  IdfObjectVector result;
  for (const auto& idfObject : m_objects) {
    if (idfObject.iddObject().type() == objectType) {
      result.push_back(idfObject);
    }
  }
  return result;
}

std::vector<IdfObject> IdfFile::getObjectsByType(const IddObject& objectType) const {
  IdfObjectVector result;
  for (const IdfObject& object : objects()) {
    if (object.iddObject() == objectType) {
      result.push_back(object);
    }
  }
  return result;
}

std::vector<IdfObject> IdfFile::getObjectsByName(const std::string& name) const {
  IdfObjectVector result;
  for (const IdfObject& object : m_objects) {
    OptionalString objectName = object.name();
    if (objectName && openstudio::istringEqual(name, *objectName)) {
      result.push_back(object);
    }
  }
  return result;
}

// SETTERS

void IdfFile::setHeader(const std::string& header) {
  m_header = makeComment(header);
}

void IdfFile::addObject(const IdfObject& object) {
  m_objects.push_back(object);
  if (object.iddObject().isVersionObject()) {
    m_versionObjectIndices.insert(m_objects.size() - 1);
  } else if ((object.iddObject().type() == IddObjectType::Catchall) && (object.numFields() > 0u)
             && (boost::regex_match(object.getString(0).get(), iddRegex::versionObjectName()))) {
    m_versionObjectIndices.insert(m_objects.size() - 1);
  }
}

void IdfFile::addObjects(const std::vector<IdfObject>& objects) {
  for (const IdfObject& object : objects) {
    addObject(object);
  }
}

void IdfFile::insertObjectByIddObjectType(const IdfObject& object) {
  for (auto it = m_objects.begin(), itEnd = m_objects.end(); it != itEnd; ++it) {
    if (it == itEnd || object.iddObject().type() < it->iddObject().type()) {
      // insert object immediately before it
      it = m_objects.insert(it, object);
      if (object.iddObject().isVersionObject()
          || ((object.iddObject().type() == IddObjectType::Catchall) && (object.numFields() > 0u)
              && (boost::regex_match(object.getString(0).get(), iddRegex::versionObjectName())))) {
        unsigned index = unsigned(it - m_objects.begin());
        m_versionObjectIndices.insert(index);
        OS_ASSERT(m_objects[index] == object);
      }
      return;
    }
  }
}

bool IdfFile::removeObject(const IdfObject& object) {
  auto it =
    std::find_if(m_objects.begin(), m_objects.end(), [h = object.handle()](const IdfObject& obj) { return handleEquals<IdfObject, Handle>(obj, h); });

  if (it != m_objects.end()) {
    IdfObjectVector::size_type index(it - m_objects.begin());
    if (it->iddObject().isVersionObject()
        || ((object.iddObject().type() == IddObjectType::Catchall) && (object.numFields() > 0u)
            && (boost::regex_match(object.getString(0).get(), iddRegex::versionObjectName())))) {
      m_versionObjectIndices.erase(index);
    }
    m_objects.erase(it);
    std::vector<IdfObjectVector::size_type> toModify;
    for (const auto i : m_versionObjectIndices) {
      if (i > index) {
        toModify.push_back(i);
      }
    }
    for (auto i : toModify) {
      m_versionObjectIndices.erase(i);
      m_versionObjectIndices.insert(i - 1);
    }
    return true;
  }
  return false;
}

int IdfFile::removeObjects(const std::vector<IdfObject>& objects) {
  int result(0);
  for (const IdfObject& object : objects) {
    bool ok = removeObject(object);
    if (ok) {
      ++result;
    }
  }
  return result;
}

// QUERIES

bool IdfFile::empty() const {
  if (!m_header.empty()) {
    return false;
  }
  if (!objects().empty()) {
    return false;
  }
  return true;
}

unsigned IdfFile::numObjects() const {
  return objects().size();
}

unsigned IdfFile::numObjectsOfType(IddObjectType objectType) const {
  return getObjectsByType(objectType).size();
}

unsigned IdfFile::numObjectsOfType(const IddObject& objectType) const {
  return getObjectsByType(objectType).size();
}

bool IdfFile::isValid(StrictnessLevel level) const {
  ValidityReport report = validityReport(level);
  return (report.numErrors() == 0);
}

ValidityReport IdfFile::validityReport(StrictnessLevel level) const {
  ValidityReport report(level);

  // StrictnessLevel::Minimal
  // DataErrorType::NoIdd
  // \todo Only way there can be no IddFile is if IddFileType is set to UserCustom

  // by-object items
  for (const IdfObject& object : m_objects) {
    // object-level report
    ValidityReport objectReport = object.validityReport(level);
    OptionalDataError oError = objectReport.nextError();
    while (oError) {
      report.insertError(*oError);
      oError = objectReport.nextError();
    }

    // StrictnessLevel::Draft
    if (level > StrictnessLevel::Minimal) {
      // DataErrorType::NoIdd
      // object-level
      if (!m_iddFileAndFactoryWrapper.isInFile(object.iddObject().type())) {
        report.insertError(DataError(object, DataErrorType(DataErrorType::NoIdd)));
      }
    }  // StrictnessLevel::Draft
  }

  // StrictnessLevel::Final
  if (level > StrictnessLevel::Draft) {
    // DataErrorType::NullAndRequired
    // collection-level: required object missing
    IddObjectVector requiredObjects = m_iddFileAndFactoryWrapper.requiredObjects();
    for (const IddObject& iddObject : requiredObjects) {
      if (numObjectsOfType(iddObject.type()) < 1) {
        report.insertError(DataError(DataErrorType(DataErrorType::NullAndRequired), iddObject.type()));
      }
    }

    // DataErrorType::Duplicate
    // collection-level: unique object duplicated
    IddObjectVector uniqueObjects = m_iddFileAndFactoryWrapper.uniqueObjects();
    for (const IddObject& iddObject : uniqueObjects) {
      if (numObjectsOfType(iddObject.type()) > 1) {
        report.insertError(DataError(DataErrorType(DataErrorType::Duplicate), iddObject.type()));
      }
    }

  }  // StrictnessLevel::Final

  return report;
}

// SERIALIZATON

boost::optional<IdfFile> IdfFile::load(std::istream& is, const IddFileType& iddFileType, ProgressBar* progressBar) {
  IdfFile result(iddFileType);
  // remove initial version object
  if (OptionalIdfObject vo = result.versionObject()) {
    result.removeObject(*vo);
  }
  if (result.m_load(is, progressBar)) {
    // check for it again here
    result.addVersionObject();
    return result;
  }
  return boost::none;
}

OptionalIdfFile IdfFile::load(std::istream& is, const IddFile& iddFile, ProgressBar* progressBar) {
  IdfFile result(iddFile);
  // remove initial version object
  if (OptionalIdfObject vo = result.versionObject()) {
    result.removeObject(*vo);
  }
  if (result.m_load(is, progressBar)) {
    // check for it again here
    result.addVersionObject();
    return result;
  }
  return boost::none;
}

OptionalIdfFile IdfFile::load(const path& p, ProgressBar* progressBar) {
  // determine IddFileType
  IddFileType iddType(IddFileType::EnergyPlus);  // default

  // switch if file extension equal to modelFileExtension() or componentFileExtension()
  std::string pext = toString(openstudio::filesystem::extension(p));
  if (!pext.empty()) {
    // remove '.'
    pext = std::string(++pext.begin(), pext.end());
  }
  if ((pext == modelFileExtension()) || (pext == componentFileExtension())) {
    iddType = IddFileType(IddFileType::OpenStudio);
  }

  return load(p, iddType, progressBar);
}

OptionalIdfFile IdfFile::load(const path& p, const IddFileType& iddFileType, ProgressBar* progressBar) {
  // complete path
  path wp(p);

  std::string ext = getFileExtension(p);

  if (iddFileType == IddFileType::OpenStudio) {
    // can be Model or Component
    if (!(openstudio::istringEqual(ext, "osm") || openstudio::istringEqual(ext, "osc"))) {
      LOG_FREE(Warn, "openstudio.setFileExtension",
               "Path p, '" << toString(p) << "', has an unexpected file extension. Was expecting 'osm' or 'osc'.");
    }
  } else {
    std::string ext = getFileExtension(p);
    if (!(openstudio::istringEqual(ext, "idf") || openstudio::istringEqual(ext, "imf") || openstudio::istringEqual(ext, "ddy"))) {
      LOG_FREE(Warn, "openstudio.setFileExtension",
               "Path p, '" << toString(p) << "', has an unexpected file extension. Was expecting 'idf', 'ddy', or 'imf'.");
    }
  }

  // pass warnOnMisMatch as false since we warn above anyways
  // In fact, don't pass the ext param, skip the entire call to setFileExtension which is pointless since it won't force replace it
  wp = completePathToFile(wp, path(), "", false);

  // try to open file and parse
  openstudio::filesystem::ifstream inFile(wp);
  if (inFile) {
    try {
      return load(inFile, iddFileType, progressBar);
    } catch (...) {
      return boost::none;
    }
  }

  return boost::none;
}

OptionalIdfFile IdfFile::load(const path& p, const IddFile& iddFile, ProgressBar* progressBar) {
  // complete path
  path wp = completePathToFile(p, path(), "idf", false);

  // try to open file and parse
  openstudio::filesystem::ifstream inFile(wp);
  if (inFile) {
    try {
      return load(inFile, iddFile, progressBar);
    } catch (...) {
      return boost::none;
    }
  }

  return boost::none;
}

boost::optional<VersionString> IdfFile::loadVersionOnly(std::istream& is) {
  boost::optional<VersionString> result;
  IddFile catchallIdd = IddFile::catchallIddFile();
  IdfFile idf(catchallIdd);
  OS_ASSERT(!idf.versionObject());
  idf.m_load(is, nullptr, true);
  if (OptionalIdfObject oVersionObject = idf.versionObject()) {
    unsigned n = oVersionObject->numFields();
    // Note: oVersionObject->iddObject().type() == Catchall, so m_fields[0] is either "OS:Version" or "Version"
    // Added a prerelease field for OS:Version only... I could explicitly test if OS:Version, but it has more fields than the E+ one,
    // so testing if n==4 is enough, and will be faster
    // if (openstudio::istringEqual("OS:VERSION", oVersionObject->getString(0, false).get())) {
    if (n == 4) {  // This is m_fields.size, which includes "OS:Version" itself. So if n == 4, you do have a prerelease tag
      --n;         // penultimate, as ultimate = Prerelease tag
    }

    std::string versionString = oVersionObject->getString(n - 1, true).get();

    if (!versionString.empty()) {
      result = VersionString(versionString);
    }
  }
  return result;
}

boost::optional<VersionString> IdfFile::loadVersionOnly(const path& p) {
  boost::optional<VersionString> result;
  path wp = completePathToFile(p, path(), "idf", false);
  openstudio::filesystem::ifstream inFile(wp);
  if (inFile) {
    try {
      return loadVersionOnly(inFile);
    } catch (...) {
      return result;
    }
  }
  return result;
}

std::ostream& IdfFile::print(std::ostream& os) const {
  if (!m_header.empty()) {
    os << m_header << '\n';
  }
  os << '\n';
  for (const IdfObject& object : m_objects) {
    object.print(os);
  }
  return os;
}

bool IdfFile::save(const openstudio::path& p, bool overwrite) {

  // default extension
  std::string expectedExtension;
  bool enforceExtension = false;
  OptionalIddFileType iddType = m_iddFileAndFactoryWrapper.iddFileType();
  if (iddType) {
    if (*iddType == IddFileType::EnergyPlus) {
      expectedExtension = "idf";
      enforceExtension = true;
    } else if (*iddType == IddFileType::OpenStudio) {
      std::string ext = getFileExtension(p);
      if (ext == componentFileExtension()) {
        expectedExtension = componentFileExtension();
        // no need to enforce b/c already checked
      } else {
        expectedExtension = modelFileExtension();
        enforceExtension = true;
      }
    }
  }

  // set extension if appropriate
  path wp(p);
  if (enforceExtension) {
    wp = setFileExtension(p, expectedExtension, false, true);
  }

  // do not overwrite if not allowed
  if (!overwrite) {
    path temp = completePathToFile(wp, path());
    if (!temp.empty()) {
      LOG(Info, "Save method failed because instructed not to overwrite path '" << toString(wp) << "'.");
      return false;
    }
  }

  if (makeParentFolder(wp)) {
    openstudio::filesystem::ofstream outFile(wp);
    if (outFile) {
      try {
        print(outFile);
        outFile.close();
        return true;
      } catch (...) {
        LOG(Error, "Unable to write file to path '" << toString(wp) << "'.");
        return false;
      }
    }
  }

  LOG(Error, "Unable to write file to path '" << toString(wp) << "', because parent directory " << "could not be created.");
  return false;
}

// PRIVATE

// SERIALIZATION

bool IdfFile::m_load(std::istream& is, ProgressBar* progressBar, bool versionOnly) {

  [[maybe_unused]] int lineNum = 0;  // Idf line number
  int objectNum = 0;                 // number of objects, first is #1
  std::string line;                  // temp string to help with reading
  boost::smatch matches;             // matches to regular expressions
  std::string comment;               // keep running comment
  bool firstBlock = true;            // to capture first comment block as the header

  if (progressBar) {
    is.seekg(0, std::ios_base::end);
    int streamsize = static_cast<int>(is.tellg());
    progressBar->setMinimum(0);
    progressBar->setMaximum(streamsize);
    is.seekg(0, std::ios_base::beg);
  }

  // Use a boost filter to make sure that no matter what line endings come in,
  // they are converted to what is expected by the current os
  boost::iostreams::filtering_istream filt;

  //#ifdef _POSIX_VERSION
  filt.push(boost::iostreams::newline_filter(boost::iostreams::newline::posix));
  //#else
  //  filt.push(boost::iostreams::newline_filter(boost::iostreams::newline::dos));
  //#endif
  filt.push(is);

  // read the file line by line using regexes
  while (std::getline(filt, line)) {

    if (line == "\r") {
      // This is not a real line at all, just the vestiges from the
      // previous windows formatted line being parsed on unix, skip it,
      // don't even count it
      continue;
    }

    ++lineNum;

    if (progressBar) {
      int current = static_cast<int>(is.tellg());
      progressBar->setValue(current);
    }

    if (boost::regex_match(line, idfRegex::commentOnlyLine())) {
      // continue comment
      comment += (line + idfRegex::newLinestring());
    } else if (boost::regex_match(line, commentRegex::whitespaceOnlyLine())) {
      // end comment
      boost::trim(comment);

      if (!comment.empty()) {
        if (firstBlock) {
          // set this comment as the header
          setHeader(comment);
          firstBlock = false;
        } else {
          if (!versionOnly) {

            // make a comment only object to hold the comment
            OptionalIddObject commentOnlyIddObject = m_iddFileAndFactoryWrapper.getObject(IddObjectType::CommentOnly);
            if (!commentOnlyIddObject) {
              LOG(Error, "IddFile does not contain a CommentOnly object. Will not be able to save comment objects.");
              continue;
            }

            OptionalIdfObject commentOnlyObject;
            commentOnlyObject = IdfObject::load(commentOnlyIddObject->name() + ";" + comment, *commentOnlyIddObject);
            OS_ASSERT(commentOnlyObject);

            // put it in the object list
            addObject(*commentOnlyObject);
          }
        }
      }

      //clear out comment
      comment = "";

    } else {

      bool foundEndLine(false);
      firstBlock = false;
      bool isVersion = false;

      // peek at the object type and name for indexing in map
      std::string objectType;

      if (boost::regex_search(line, matches, idfRegex::line())) {
        objectType = std::string(matches[1].first, matches[1].second);
        boost::trim(objectType);
      } else {
        // can't figure out the object's type
        if (!versionOnly) {
          LOG(Warn, "Unrecognizable object type '" + line + "'. Defaulting to 'Catchall'.");
        }
        objectType = "Catchall";
      }
      if (boost::regex_match(objectType, iddRegex::versionObjectName())) {
        isVersion = true;
      }

      // get the corresponding idd object entry

      OptionalIddObject iddObject = m_iddFileAndFactoryWrapper.getObject(objectType);
      if (!iddObject) {
        if (!versionOnly) {
          LOG(Warn, "Cannot find object type '" + objectType + "' in Idd. Placing data in Catchall object.");
        }
        iddObject = IddObject();
        objectType = "Catchall";
      } else {
        OS_ASSERT(iddObject->type() != IddObjectType::Catchall);
      }

      // put the text for this object in a new string with a newline
      std::string text(comment + idfRegex::newLinestring() + line + idfRegex::newLinestring());
      comment = "";

      // check if this line also matches closing line object
      if (boost::regex_match(line, idfRegex::objectEnd())) {
        foundEndLine = true;
      }

      // continue reading until we have seen the entire object
      // last line will be thrown away, requires empty line between objects in Idf
      while ((!foundEndLine) && (std::getline(filt, line))) {
        ++lineNum;

        // add line to text, include newline separator
        text += (line + idfRegex::newLinestring());

        // check if we have found the last field
        if (boost::regex_match(line, idfRegex::objectEnd())) {
          foundEndLine = true;
        }
      }

      // construct the object
      if (foundEndLine && (!versionOnly || isVersion)) {
        OptionalIdfObject object = IdfObject::load(text, *iddObject);
        if (!object) {
          LOG(Error, "Unable to construct IdfObject from text: " << '\n'
                                                                 << text << '\n'
                                                                 << "Throwing this object out and parsing the remainder of the file.");
          continue;
        } else {
          // a valid Idf object to parse
          if (object->iddObject().type() != IddObjectType::Catchall) {
            ++objectNum;
          }

          // put it in the object list
          addObject(*object);
        }
      }

      if (versionOnly && isVersion) {
        // Increment objectNum to avoid triggering the warning below and return false
        ++objectNum;
        break;
      }
    }
  }

  // If we sucessfully parsed at least one object, we return true, otherwise false
  if (objectNum > 0) {
    return true;
  } else {
    LOG(Error, "Could not parse a single valid object in file.");
    return false;
  }
}

IddFileAndFactoryWrapper IdfFile::iddFileAndFactoryWrapper() const {
  return m_iddFileAndFactoryWrapper;
}

void IdfFile::setIddFileAndFactoryWrapper(const IddFileAndFactoryWrapper& iddFileAndFactoryWrapper) {
  m_iddFileAndFactoryWrapper = iddFileAndFactoryWrapper;
}

// PRIVATE

// SETTERS

void IdfFile::addVersionObject() {
  OptionalIddObject versionIdd = m_iddFileAndFactoryWrapper.versionObject();
  if (!versionIdd) {
    bool printWarn(true);
    if ((m_iddFileAndFactoryWrapper.objects().size() == 1u) && (m_iddFileAndFactoryWrapper.objects()[0] == IddObject())) {
      printWarn = false;
    }
    if (printWarn) {
      LOG(Warn, "Unable to add Version Idf object, because could not identify a Version " << "Idd object.");
    }
    return;
  }
  if (m_versionObjectIndices.empty()) {
    IdfObject versionObject(*versionIdd);
    // look for field named "Version Identifier"
    OptionalInt index = versionObject.iddObject().getFieldIndex("Version Identifier");
    if (!index) {
      LOG(Warn, "No 'Version Identifier' field found in the Version IddObject.");
    } else {
      versionObject.setString(*index, version().str());
    }
    addObject(versionObject);
  }
}

// QUERIES

IdfObjectVector IdfFile::m_objectsWithConflictingNames(const std::string& name, bool getAll) const {

  IdfObjectVector result;
  IdfObjectVector candidates = getObjectsByName(name);  // uses iequals

  unsigned n = candidates.size();
  if (n < 2) {
    return result;
  }  // no potential conflicts

  BoolVector hasConflict(candidates.size(), false);

  // main purpose of iteration is to determine if candidates[i] has conflict
  for (unsigned i = 0; i < n; ++i) {
    if (hasConflict[i]) {
      continue;
    }

    // IddObjectTypes must be unique. find all conflicts with type since inexpensive.
    IddObjectType type = candidates[i].iddObject().type();
    for (unsigned j = 0; j < n; ++j) {
      if (j == i) {
        continue;
      }
      if (candidates[j].iddObject().type() == type) {
        hasConflict[i] = true;
        hasConflict[j] = true;
      }
    }

    if (hasConflict[i]) {
      if (getAll) {
        continue;
      } else {
        break;
      }
    }

    // Reference List Names cannot overlap
    StringVector refs = candidates[i].iddObject().references();
    for (unsigned j = 0; j < n; ++j) {
      if (j == i) {
        continue;
      }
      for (const std::string& ref : candidates[j].iddObject().references()) {
        if (std::find_if(refs.begin(), refs.end(), [&ref](const auto& r) { return openstudio::istringEqual(r, ref); }) != refs.end()) {
          hasConflict[i] = true;
          hasConflict[j] = true;
          break;
        }
      }
      if (hasConflict[i]) {
        break;
      }
    }

    if (hasConflict[i] && !getAll) {
      break;
    }
  }

  for (unsigned i = 0; i < n; ++i) {
    if (hasConflict[i]) {
      result.push_back(candidates[i]);
    }
  }

  return result;
}

// NON-MEMBER FUNCTIONS

std::ostream& operator<<(std::ostream& os, const IdfFile& idfFile) {
  return idfFile.print(os);
}

}  // namespace openstudio
