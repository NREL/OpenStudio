/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "IddFile.hpp"
#include "IddFile_Impl.hpp"

#include "IddRegex.hpp"
#include "IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../core/PathHelpers.hpp"
#include "../core/Assert.hpp"

#include "../core/Containers.hpp"




namespace openstudio {

namespace detail {

  // CONSTRUCTORS

  IddFile_Impl::IddFile_Impl()
  {}

  // GETTERS

  std::string IddFile_Impl::version() const {
    return m_version;
  }

  std::string IddFile_Impl::build() const {
    return m_build;
  }

  std::string IddFile_Impl::header() const {
    return m_header;
  }

  std::vector<IddObject> IddFile_Impl::objects() const {
    return m_objects;
  }

  std::vector<std::string> IddFile_Impl::groups() const {
    StringSet result;
    for (const IddObject& object : objects()) {
      result.insert(object.group());
    }
    return StringVector(result.begin(),result.end());
  }

  std::vector<IddObject> IddFile_Impl::getObjectsInGroup(const std::string& group) const {
    IddObjectVector result;
    for (const IddObject& object : m_objects){
      if(istringEqual(object.group(), group)){
        result.push_back(object);
      }
    }
    return result;
  }

  std::vector<IddObject> IddFile_Impl::getObjects(const boost::regex &objectRegex) const {
    IddObjectVector result;

    for (const IddObject& object : m_objects) {
      if (boost::regex_match(object.name(),objectRegex)) {
        result.push_back(object);
      }
    }

    return result;
  }

  boost::optional<IddObject> IddFile_Impl::versionObject() const {
    if (m_versionObject) {
      return m_versionObject;
    }

    OptionalIddObject result;
    IddObjectVector candidates = getObjects(iddRegex::versionObjectName());
    if (candidates.size() == 1u) {
      result = candidates[0];
    }
    m_versionObject = result;
    return result;
  }

  boost::optional<IddObject> IddFile_Impl::getObject(const std::string& objectName) const
  {
    OptionalIddObject result;
    for (const IddObject& object : m_objects){
      if(istringEqual(object.name(), objectName)){
        result = object;
        break;
      }
    }
    return result;
  }

  boost::optional<IddObject> IddFile_Impl::getObject(IddObjectType objectType) const
  {
    OptionalIddObject result;

    if (objectType == IddObjectType::UserCustom) {
      LOG(Info,"Asked to return IddObject of type IddObjectType::UserCustom. Since "
          << "UserCustom object types are generally not unique, returning false rather than "
          << "an IddObject. Please specify a different IddObjectType, or use "
          << "getObject(const std::string&).");
      return result;
    }

    for (const IddObject& object : m_objects){
      if (object.type() == objectType) {
        result = object;
        break;
      }
    }

    return result;
  }

  std::vector<IddObject> IddFile_Impl::requiredObjects() const
  {
    IddObjectVector result;
    for (const IddObject& object : m_objects){
      if(object.properties().required){
        result.push_back(object);
      }
    }
    return result;
  }

  std::vector<IddObject> IddFile_Impl::uniqueObjects() const
  {
    IddObjectVector result;
    for (const IddObject& object : m_objects){
      if(object.properties().unique){
        result.push_back(object);
      }
    }
    return result;
  }

  // SETTERS

  void IddFile_Impl::setVersion(const std::string& version)
  {
    m_version = version;
  }

  void IddFile_Impl::setHeader(const std::string& header)
  {
    m_header = header;
  }

  void IddFile_Impl::addObject(const IddObject& object)
  {
    m_objects.push_back(object);
  }

  // SERIALIZATION

  std::shared_ptr<IddFile_Impl> IddFile_Impl::load(std::istream& is) {
    std::shared_ptr<IddFile_Impl> result;
    IddFile_Impl iddFileImpl;

    try {
      iddFileImpl.parse(is);
    }
    catch (...) { return result; }

    result = std::shared_ptr<IddFile_Impl>(new IddFile_Impl(iddFileImpl));
    return result;

  }


  std::ostream& IddFile_Impl::print(std::ostream& os) const
  {
    os << m_header << std::endl;
    std::string groupName;
    for (const IddObject& object : m_objects){
      if (object.group() != groupName) {
        groupName = object.group();
        os << "\\group " << groupName << std::endl << std::endl;
      }
      object.print(os);
    }
    return os;
  }


  // PRIVATE

  void IddFile_Impl::parse(std::istream& is)
  {

    // keep track of line number in the idd
    int lineNum = 0;

    // number of object in the idd, 1 is first object
    int objectNum = 0;

    // stream for header
    std::stringstream header;

    // have we read the entire header yet
    bool headerClosed = false;

    std::string currentGroup = "";

    // fake a comment only object and put it in the object list and object map
    OptionalIddObject commentOnlyObject = IddObject::load(iddRegex::commentOnlyObjectName(),
                                                          currentGroup,
                                                          iddRegex::commentOnlyObjectText(),
                                                          IddObjectType::CommentOnly);
    OS_ASSERT(commentOnlyObject);
    m_objects.push_back(*commentOnlyObject);

    // temp string to read file
    std::string line;

    // this will contain matches to regular expressions
    boost::smatch matches;

    // read in the version from the first line
    getline(is, line);
    if (boost::regex_search(line, matches, iddRegex::version())){

      m_version = std::string(matches[1].first,matches[1].second);

      // this line belongs to the header
      header << line << std::endl;

    }else{
      // idd file must have a version on the first line of input
      LOG_AND_THROW("Idd file does not contain version on first line: '" << line << "'");
    }

    // read the rest of the file line by line
    // todo, do this by regex
    while(getline(is, line)){
      ++lineNum;

      // remove whitespace
      boost::trim(line);

      if (line.empty()){

        headerClosed = true;

        // empty line
        continue;
      }else if (boost::regex_search(line, matches, iddRegex::build())) {
        m_build = std::string(matches[1].first,matches[1].second);
        // this line belongs to the header
        header << line << std::endl;

      }else if (boost::regex_match(line, iddRegex::commentOnlyLine())){

        if (!headerClosed){
          header << line << std::endl;
        }

        // comment only line
        continue;
      }else if (boost::regex_search(line, matches, iddRegex::group())){

        headerClosed = true;

        // get the group name
        std::string groupName(matches[1].first, matches[1].second); boost::trim(groupName);

        // set the current group
        currentGroup = groupName;

        continue;
      }else{

        headerClosed = true;

        //int beginLineNum(lineNum);
        bool foundClosingLine(false);

        // a valid idd object to parse
        ++objectNum;

        // peek at the object name for indexing in map
        std::string objectName;
        if (boost::regex_search(line, matches, iddRegex::line())){
          objectName = std::string(matches[1].first, matches[1].second); boost::trim(objectName);
        }else{
          // can't figure out the object's name
          LOG_AND_THROW("Cannot determine object name on line " << lineNum <<
                         ": '" << line << "'");
        }

        // put the text for this object in a new string
        std::string text(line);

          // check if the object has no fields
        if (boost::regex_match(line, iddRegex::objectNoFields())){
          foundClosingLine = true;
        }

        // check if the object has fields, and last field on this line
        if (boost::regex_match(line, iddRegex::closingField())){
          foundClosingLine = true;
        }

        // continue reading until we have seen the entire object
        // last line will be thrown away, requires empty line between objects in idd
        while(getline(is, line)){
          ++lineNum;

          // remove whitespace
          boost::trim(line);

          // found last field and this is not a field comment
          if (foundClosingLine && (!boost::regex_match(line, iddRegex::metaDataComment()))){
            break;
          }

          if (!line.empty()){
            // if the line is not empty add it to the text
            // note, text does not include newlines
            text += line;

            // check if we have found the last field
            if (boost::regex_match(line, iddRegex::closingField())){
              foundClosingLine = true;
            }
          }
        }

        // construct the IddObject using default UserCustom type
        OptionalIddObject object = IddObject::load(objectName, currentGroup, text);

        // construct a new object and put it in the object vector
        if (object) { m_objects.push_back(*object); }
        else {
          LOG_AND_THROW("Unable to construct IddObject from text: " << std::endl << text);
        }

      }
    }

    // set header
    m_header = header.str();
  }

} // detail

// CONSTRUCTORS

IddFile::IddFile()
  : m_impl(std::shared_ptr<detail::IddFile_Impl>(new detail::IddFile_Impl()))
{}

IddFile::IddFile(const IddFile& other)
  : m_impl(other.m_impl)
{}

IddFile IddFile::catchallIddFile() {
  IddFile result;
  result.addObject(IddObject());
  return result;
}

// GETTERS

std::string IddFile::version() const
{
  return m_impl->version();
}

std::string IddFile::build() const
{
  return m_impl->build();
}
std::string IddFile::header() const
{
  return m_impl->header();
}

std::vector<IddObject> IddFile::objects() const
{
  return m_impl->objects();
}

std::vector<std::string> IddFile::groups() const
{
  return m_impl->groups();
}

std::vector<IddObject> IddFile::getObjectsInGroup(const std::string& group) const
{
  return m_impl->getObjectsInGroup(group);
}

std::vector<IddObject> IddFile::getObjects(const boost::regex& objectRegex) const {
  return m_impl->getObjects(objectRegex);
}

boost::optional<IddObject> IddFile::versionObject() const {
  return m_impl->versionObject();
}

boost::optional<IddObject> IddFile::getObject(const std::string& objectName) const
{
  return m_impl->getObject(objectName);
}

boost::optional<IddObject> IddFile::getObject(IddObjectType objectType) const
{
  return m_impl->getObject(objectType);
}

std::vector<IddObject> IddFile::requiredObjects() const
{
  return m_impl->requiredObjects();
}

std::vector<IddObject> IddFile::uniqueObjects() const
{
  return m_impl->uniqueObjects();
}

// SERIALIZATION

OptionalIddFile IddFile::load(std::istream& is)
{
  std::shared_ptr<detail::IddFile_Impl> p = detail::IddFile_Impl::load(is);
  if (p) { return IddFile(p); }
  return boost::none;
}

OptionalIddFile IddFile::load(const openstudio::path& p) {
  openstudio::path wp = completePathToFile(p,path(),"idd",true);
  if (wp.empty()) { return boost::none; }
  openstudio::filesystem::ifstream inFile(wp);
  if (!inFile) { return boost::none; }
  return load(inFile);
}

std::ostream& IddFile::print(std::ostream& os) const
{
  return m_impl->print(os);
}

std::pair<VersionString, std::string> IddFile::parseVersionBuild(const openstudio::path &p)
{
  std::ifstream ifs(openstudio::toSystemFilename(p));

  if (!ifs.good()) {
    throw std::runtime_error("Unable to open file for reading: " + openstudio::toString(p));
  }

  ifs.seekg(0, std::ios_base::end);
  const auto end = ifs.tellg();
  ifs.seekg(0, std::ios_base::beg);

  const auto length_to_read = std::min(std::streampos(10000), end);

  std::vector<char> data(length_to_read);
  ifs.read(data.data(), length_to_read);
  const std::string strdata(data.cbegin(), data.cend());

  std::string build;
  boost::smatch matches;
  if (boost::regex_search(strdata, matches, iddRegex::build())) {
    build = std::string(matches[1].first,matches[1].second);
  }

  if (boost::regex_search(strdata, matches, iddRegex::version())) {
    return std::make_pair(VersionString(std::string(matches[1].first, matches[1].second)), build);
  }

  throw std::runtime_error("Unable to parse version from IDD: " + openstudio::toString(p));
}

bool IddFile::save(const openstudio::path& p, bool overwrite) {
  path wp = completePathToFile(p,path(),"idd",true);
  if (!wp.empty() && (overwrite == false)) {
    LOG(Info,"IddFile save method failed because instructed not to overwrite path '"
        << toString(p) << "'.");
    return false;
  }
  if (makeParentFolder(p)) {
    openstudio::filesystem::ofstream outFile(p);
    if (outFile) {
      try {
        print(outFile);
        outFile.close();
        return true;
      }
      catch (...) {
        LOG(Error,"Unable to write IddFile to path '" << toString(p) << "'.");
        return false;
      }
    }
  }

  LOG(Error,"Unable to write IddFile to path '" << toString(p)
      << "', because parent directory could not be created.");
  return false;
}

// PROTECTED

void IddFile::setVersion(const std::string& version)
{
  m_impl->setVersion(version);
}

void IddFile::setHeader(const std::string& header)
{
  m_impl->setHeader(header);
}

void IddFile::addObject(const IddObject& object)
{
  m_impl->addObject(object);
}

// PRIVATE

IddFile::IddFile(const std::shared_ptr<detail::IddFile_Impl>& impl) : m_impl(impl) {}

// NON-MEMBER FUNCTIONS

std::ostream& operator<<(std::ostream& os, const IddFile& iddFile){
  return iddFile.print(os);
}

} // openstudio
