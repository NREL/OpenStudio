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

#include <utilities/document/Document.hpp>
#include <utilities/document/DocumentRegex.hpp>
#include <utilities/document/SectionHeading.hpp>
#include <utilities/document/SectionHeading_Impl.hpp>
#include <utilities/document/Section.hpp>
#include <utilities/document/Section_Impl.hpp>
#include <utilities/document/Table.hpp>
#include <utilities/document/Table_Impl.hpp>
#include <utilities/document/Text.hpp>
#include <utilities/document/Text_Impl.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <utilities/core/PathHelpers.hpp>

#include <boost/foreach.hpp>
#include <boost/archive/archive_exception.hpp>

BOOST_CLASS_EXPORT(openstudio::SectionHeading);
BOOST_CLASS_EXPORT(openstudio::Section);
BOOST_CLASS_EXPORT(openstudio::Text);
BOOST_CLASS_EXPORT(openstudio::Table);
BOOST_CLASS_EXPORT(openstudio::detail::SectionHeading_Impl);
BOOST_CLASS_EXPORT(openstudio::detail::Section_Impl);
BOOST_CLASS_EXPORT(openstudio::detail::Text_Impl);
BOOST_CLASS_EXPORT(openstudio::detail::Table_Impl);

namespace openstudio {

// CONSTRUCTORS

Document::Document(const std::string& title,
                   const std::vector<std::string>& authors,
                   DocumentFormat fmt,
                   const std::string& header,
                   const std::string& footer) 
  : m_title(title), 
    m_authors(authors), 
    m_format(fmt), 
    m_header(header), 
    m_footer(footer), 
    m_topHeadingLevel(0)
{}

Document Document::clone() const {
  Document result(m_title,m_authors,m_format,m_header,m_footer);
  BOOST_FOREACH(const SectionElement& element,m_contents) {
    result.append(element.clone().cast<SectionElement>());
  }
  return result;
}

// GETTERS

std::string Document::title() const {
  return m_title;
}

std::vector<std::string> Document::authors() const {
  return m_authors;
}

DocumentFormat Document::format() const {
  return m_format;
}

std::string Document::header() const {
  return m_header;
}

std::string Document::footer() const {
  return m_footer;
}

std::vector<SectionElement> Document::contents() const {
  return m_contents;
}

std::vector<Section> Document::sections() const {
  SectionVector result;
  BOOST_FOREACH(const SectionElement& element,m_contents) {
    OptionalSection oSection = element.optionalCast<Section>();
    if (oSection) { result.push_back(*oSection); }
  }
  return result;
}

unsigned Document::topHeadingLevel() const {
  return m_topHeadingLevel;
}

// SETTERS

void Document::setTitle(const std::string& title) {
  m_title = title;
}

void Document::setAuthors(const std::vector<std::string>& authors) {
  m_authors = authors;
}

void Document::addAuthor(const std::string& author) {
  m_authors.push_back(author);
}

void Document::setFormat(DocumentFormat fmt,
                         const std::string& header,
                         const std::string& footer) {
  m_format = fmt;
  m_header = header;
  m_footer = footer;
}

void Document::clear() {
  m_contents.clear();
}

void Document::replaceAll(const std::vector<SectionElement>& contents) {
  m_contents = contents;
}

void Document::replace(unsigned index, const SectionElement& element) {
  if (index > numElements()) {
    LOG_AND_THROW("Cannot replace element " << index << " of Document '" << m_title 
        << "', because such an element does not exist.");
  }
  if (index == numElements()) {
    LOG(Warn,"Asked to replace element " << index << " of Document '" << m_title
        << "', but appending instead, because the section currently has " << numElements() 
        << " elements.");
    append(element);
    return;
  }
  m_contents[index] = element;
}

void Document::append(const SectionElement& element) {
  m_contents.push_back(element);
}

void Document::append(const std::vector<SectionElement>& elements) {
  m_contents.insert(m_contents.end(),elements.begin(),elements.end());
}

Section Document::appendSection(const std::string& headingText) {
  SectionHeading heading(m_topHeadingLevel,headingText);
  Section result(heading);
  append(result);
  return result;
}

void Document::insert(unsigned index, const SectionElement& element) {
  unsigned n = numElements();
  if (index > n) {
    LOG_AND_THROW("Cannot insert an element at index " << index << ", because there are "
        << "currently only " << n << " elements in Document '" << m_title << ".");
  }
  if (index == n) { 
    append(element); 
    return;
  }
  SectionElementVector::iterator it = m_contents.begin();
  for (unsigned i = 0; i < index; ++i, ++it) {}
  m_contents.insert(it,element);
}

Section Document::insertSection(unsigned index,const std::string& headingText) {
  if (index > numElements()) {
    LOG_AND_THROW("Cannot insert a Section at index " << index << ", because there are "
        << "currently only " << numElements() << " elements in Document '" << m_title << "'.");
  }
  SectionHeading heading(m_topHeadingLevel,headingText);
  Section result(heading);
  insert(index,result);
  return result;
}

void Document::erase(unsigned index) {
  if (index < numElements()) {
    SectionElementVector::iterator it = m_contents.begin();
    for (unsigned i = 0; i < index; ++i, ++it) {}
    m_contents.erase(it);
  }
  else {
    LOG(Warn,"No element to erase at index " << index << " of Document '" << m_title << "'.");
  }
}

void Document::setTopHeadingLevel(unsigned level) {
  m_topHeadingLevel = level;
  makeHeadingLevelsConsistent();
}

void Document::makeHeadingLevelsConsistent() {
  SectionVector sections = this->sections();
  BOOST_FOREACH(Section& section,sections) {
    section.heading().setLevel(m_topHeadingLevel);
    section.makeHeadingLevelsConsistent();
  }
}

// QUERIES

unsigned Document::numElements() const {
  return m_contents.size();
}

unsigned Document::numSections() const {
  return sections().size();
}

std::string Document::expectedFileExtension() const {
  switch (m_format.value()) {
    case DocumentFormat::COUT :
      return std::string("txt");
      break;
    case DocumentFormat::LaTeX :
      return std::string("tex");
      break;
    case DocumentFormat::XHTML :
      return std::string("html");
      break;
    default : ;
  };
  return std::string(); // default
}

// PRINTING AND SERIALIZATION

std::string Document::print() const {
  std::stringstream ss;
  printToStream(ss);
  return ss.str();
}

std::ostream& Document::printToStream(std::ostream& os) const {
  m_printHeader(os);
  BOOST_FOREACH(const SectionElement& element,m_contents) {
    element.printToStream(os,m_format);
  }
  m_printFooter(os);
  return os;
}

bool Document::save(const openstudio::path& p, bool overwrite) const {
  // save out printed document, or boost serialization?
  std::string expectedExtension = expectedFileExtension();
  std::string pext = toString(boost::filesystem::extension(p));
  if (!pext.empty()) {
    // remove '.'
    pext = std::string(++pext.begin(),pext.end());
  }
  if (pext == documentFileExtension()) {
    expectedExtension = pext; // boost serialization
  }

  // complete path
  path wp = completePathToFile(p,path(),expectedExtension,true);
  if (!wp.empty() && (overwrite == false)) { 
    LOG(Info,"Document save method failed because instructed not to overwrite path '" 
        << toString(p) << "'.");
    return false;
  }
  if (wp.empty()) { wp = p; }

  if (makeParentFolder(wp)) {
    boost::filesystem::ofstream outFile(wp);
    if (outFile) {
      if (expectedExtension == documentFileExtension()) {
        try {
          boost::archive::text_oarchive out(outFile);
          out << boost::serialization::make_nvp("document",*this);
          outFile.close();
          return true;
        }
        catch (boost::archive::archive_exception e) {
          LOG(Error,"Boost archive exception while writing Document out to boost serialization "
              << "text format (.osd). Code " << e.code << ". Message: " << e.what());
          return false;
        }
        catch (...) {
          LOG(Error,"Error serializing Document to boost serializaiton text format (.osd).");
          return false;
        }
      }
      else {
        try {
          printToStream(outFile);
          outFile.close();
          return true;
        }
        catch (...) {
          LOG(Error,"Unable to write Document to path '" << toString(p) << "'.");
          return false;
        }
      }
    }
  }

  LOG(Error,"Unable to write Document to path '" << toString(p) << "', because parent "
      << "directory could not be created.");
  return false;
}

boost::optional<Document> Document::load(const openstudio::path& p) {
  OptionalDocument result;
  openstudio::path wp = completePathToFile(p,path(),documentFileExtension(),true);
  boost::filesystem::ifstream inFile(wp);
  if (!inFile) { return result; }

  try {
    boost::archive::text_iarchive in(inFile);
    Document tmp;
    in >> boost::serialization::make_nvp("document",tmp);
    result = tmp;
  }
  catch (boost::archive::archive_exception e) {
    LOG_FREE(Error,"openstudio.Document","Boost archive exception while loading Document from boost serialization text "
        << "format (.osd). Code " << e.code << ". Message: " << e.what());
  }
  catch (...) {
    LOG_FREE(Error,"openstudio.Document","Error deserializing Document from boost serialization text format (.osd).");
  }
  return result;
}

// PRIVATE

std::ostream& Document::m_printHeader(std::ostream& os) const {
  std::string wHeader(m_header);
  // substitute title
  wHeader = boost::regex_replace(wHeader,titlePlaceholder(),m_title);
  // substitute authors
  wHeader = boost::regex_replace(wHeader,authorPlaceholder(),m_authorString());
  os << wHeader;
  return os;
}


std::ostream& Document::m_printFooter(std::ostream& os) const {
  os << m_footer;
  return os;
}

std::string Document::m_authorString() const {
  unsigned n = m_authors.size();
  std::string sep("");
  std::stringstream ss;
  for (unsigned i = 0; i < n; ++i) {
    ss << sep << m_authors[i];
    if (i < n - 1) {
      if (n == 2) { sep = std::string(" and "); }
      else if ( i == n-2) { sep = std::string(", and "); }
      else { sep = std::string(", "); }
    }
  }
  return ss.str();
}

std::ostream& operator<<(std::ostream& os,const Document& doc) {
  return doc.printToStream(os);
}

} // openstudio
