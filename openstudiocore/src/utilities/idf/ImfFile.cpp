/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "ImfFile.hpp"
#include "IdfRegex.hpp"
#include "../idd/CommentRegex.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../core/Assert.hpp"
#include "../core/PathHelpers.hpp"
#include "../core/String.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/iostreams/filter/newline.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <sstream>

namespace openstudio{

// CONSTRUCTORS

ImfFile::ImfFile(IddFileType iddFileType) : m_iddFileAndFactoryWrapper(iddFileType) {}

// GETTERS

IddFile ImfFile::iddFile() const {
  return m_iddFileAndFactoryWrapper.iddFile();
}

IddFileType ImfFile::iddFileType() const {
  return m_iddFileAndFactoryWrapper.iddFileType();
}

StringVector ImfFile::sectionNames() const {
  StringVector result;
  for (const SectionMapType::value_type& pair : m_sectionMap){
    result.push_back(pair.first);
  }
  return result;
}

IdfObjectVector ImfFile::section(const std::string& section) const {
  IdfObjectVector result;
  auto it = m_sectionMap.find(section);
  if (it != m_sectionMap.end()) {
    result = it->second;
  }
  return result;
}

// QUERIES

unsigned ImfFile::numSections() const {
  return m_sectionMap.size();
}

// SERIALIZATION

OptionalImfFile ImfFile::load(std::istream& is,IddFileType iddFileType) {
  OptionalImfFile result;
  ImfFile temp{IddFileType::EnergyPlus};
  temp.m_iddFileAndFactoryWrapper.setIddFile(iddFileType);
  bool success = temp.m_load(is);
  if (success) { result = temp; }
  return result;
}

OptionalImfFile ImfFile::load(std::istream& is,const IddFile& iddFile) {
  OptionalImfFile result;
  ImfFile temp{IddFileType::EnergyPlus};
  temp.m_iddFileAndFactoryWrapper.setIddFile(iddFile);
  bool success = temp.m_load(is);
  if (success) { result = temp; }
  return result;
}

OptionalImfFile ImfFile::load(const openstudio::path& p, IddFileType iddFileType) {
  // complete path
  path wp = completePathToFile(p,path(),"imf",true);

  // try to open file and parse
  boost::filesystem::ifstream inFile(wp);
  if (inFile) {
    try { return load(inFile,iddFileType); }
    catch (...) { return boost::none; }
  }

  return boost::none;
}

OptionalImfFile ImfFile::load(const openstudio::path& p, const IddFile& iddFile) {
  // complete path
  path wp = completePathToFile(p,path(),"imf",true);

  // try to open file and parse
  boost::filesystem::ifstream inFile(wp);
  if (inFile) {
    try { return load(inFile,iddFile); }
    catch (...) { return boost::none; }
  }

  return boost::none;
}

std::ostream& ImfFile::print(std::ostream& os) const
{
  for (const SectionMapType::value_type& pair : m_sectionMap){
    os << "##def " << pair.first << "[]" << std::endl;
    for (const IdfObject& object : pair.second) {
      os << object;
    }
    os << "##enddef " << pair.first << std::endl << std::endl;
  }
  return os;
}

bool ImfFile::save(const openstudio::path& p, bool overwrite) {

  path wp = completePathToFile(p,path(),"imf",true);
  if (!wp.empty() && (overwrite == false)) { 
    LOG(Info,"ImfFile save method failed because instructed not to overwrite path '" 
        << toString(p) << "'.");
    return false;
  }
  if (makeParentFolder(p)) {
    boost::filesystem::ofstream outFile(p);
    if (outFile) {
      try {
        print(outFile);
        return true;
      }
      catch (...) {
        LOG(Error,"Unable to write ImfFile to path '" << toString(p) << "'.");
        return false;
      }
    }
  }

  LOG(Error,"Unable to write ImfFile to path '" << toString(p) 
      << "', because parent directory could not be created.");
  return false;
}

// PRIVATE

bool ImfFile::m_load(std::istream& is) {

  // keep track of line number in the Idf
  int lineNum = 0;

  // number of object in the Idf, 1 is first object
  int objectNum = 0;

  // temp string to read file
  std::string line;

  // this will contain matches to regular expressions
  boost::smatch matches;

  // keep running comment
  std::string comment;

  // keep track of the current section
  std::string section = "";

  // get CommentOnly IddObject
  OptionalIddObject commentOnlyIddObject;
  commentOnlyIddObject = IddFactory::instance().getObject(IddObjectType::CommentOnly);
  OS_ASSERT(commentOnlyIddObject);

  // Use a boost filter to make sure that no matter what line endings come in,
  // they are converted to what is expected by the current os
  boost::iostreams::filtering_istream filt;

//#ifdef _POSIX_VERSION
  filt.push(boost::iostreams::newline_filter(boost::iostreams::newline::posix));
//#else
//  filt.push(boost::iostreams::newline_filter(boost::iostreams::newline::dos));
//#endif
  filt.push(is);

  // read the rest of the file line by line
  // todo, do this by regex
  while (getline(filt, line)) {
    ++lineNum;

    if (boost::regex_match(line, idfRegex::commentOnlyLine())){
      // continue comment
      comment += (line + idfRegex::newLinestring());
      continue;

    }
    else if (boost::regex_match(line, commentRegex::whitespaceOnlyLine())){

      // end comment
      boost::trim(comment);

      if (!comment.empty()){
        // make a comment only object to hold the comment
        OptionalIdfObject commentOnlyObject;
        commentOnlyObject = IdfObject::load(commentOnlyIddObject->name() + ";" + comment,
                                            *commentOnlyIddObject);
        OS_ASSERT(commentOnlyObject);

        // put it in the object list
        m_sectionMap[section].push_back(*commentOnlyObject);
      }

      //clear out comment
      comment = "";

      continue;

    }
    else if (boost::regex_search(line, matches, idfRegex::imfSection())){

      // end comment
      boost::trim(comment);

      if (!comment.empty()){
        // make a comment only object to hold the comment
        OptionalIddObject commentOnlyIddObject = IddFactory::instance().getObject(IddObjectType::CommentOnly);
        OS_ASSERT(commentOnlyIddObject);

        OptionalIdfObject commentOnlyObject;
        commentOnlyObject = IdfObject::load(commentOnlyIddObject->name() + ";" + comment,
                                            *commentOnlyIddObject);
        OS_ASSERT(commentOnlyObject);

        // put it in the object list
        m_sectionMap[section].push_back(*commentOnlyObject);
      }

      //clear out comment
      comment = "";

      // get the new imf section
      section = std::string(matches[1].first, matches[1].second); boost::trim(section);

    }
    else if (boost::regex_search(line, matches, idfRegex::imfSectionEnd())) {
      // end comment
      boost::trim(comment);

      if (!comment.empty()){
        // make a comment only object to hold the comment
        OptionalIdfObject commentOnlyObject;
        commentOnlyObject = IdfObject::load(commentOnlyIddObject->name() + ";" + comment,
                                            *commentOnlyIddObject);
        OS_ASSERT(commentOnlyObject);

        // put it in the object list
        m_sectionMap[section].push_back(*commentOnlyObject);
      }

      //clear out comment
      comment = "";
    }
    else { // IdfObject

      bool foundEndLine(false);

      // a valid Idf object to parse
      ++objectNum;

      // peek at the object type and name for indexing in map
      std::string objectType;
      std::string objectName;
      //bool objectHasName(false);

      if (boost::regex_search(line, matches, idfRegex::objectTypeAndName())){
        objectType = std::string(matches[1].first, matches[1].second); boost::trim(objectType);
        objectName = std::string(matches[2].first, matches[2].second); boost::trim(objectType);
      }
      else if (boost::regex_search(line, matches, idfRegex::line())){
        // doesn't match name, just type
        objectType = std::string(matches[1].first, matches[1].second); boost::trim(objectType);
      }
      else{
        // can't figure out the object's type
        LOG(Warn, "Unrecognizable object type '" + line + "'. Defaulting to 'Catchall'.");
        objectType = "Catchall";
      }

      // get the corresponding idd object entry
      OptionalIddObject iddObject = m_iddFileAndFactoryWrapper.getObject(objectType);
      if (!iddObject){
        LOG(Warn, "Cannot find object type '" + objectType + "' in Idd. Placing data in Catchall object.");
        iddObject = IddObject();
        objectType = "Catchall";
      }

      // put the text for this object in a new string with a newline
      std::string text(line + idfRegex::newLinestring());

        // check if this line also matches closing line object
      if (boost::regex_match(line, idfRegex::objectEnd())){
        foundEndLine = true;
      }

      // continue reading until we have seen the entire object
      // last line will be thrown away, requires empty line between objects in Idf
      while((!foundEndLine) && (std::getline(filt, line))){
        ++lineNum;

        // add line to text, include newline separator
        text += (line + idfRegex::newLinestring());

        // check if we have found the last field
        if (boost::regex_match(line, idfRegex::objectEnd())){
            foundEndLine = true;
        }
      }

      // construct the object
      OptionalIdfObject object = IdfObject::load(text,*iddObject);
      if (!object) {
        LOG(Error,"Unable to construct IdfObject from text: " << std::endl << text 
            << std::endl << "Throwing this object out and parsing the remainder of the file.");
        continue;
      }

      // put it in the object list
      m_sectionMap[section].push_back(*object);
    }
  }

  return true;

}

std::ostream& operator<<(std::ostream& os, const ImfFile& imfFile) {
  return imfFile.print(os);
}

} // openstudio
