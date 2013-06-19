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

#ifndef UTILITIES_DOCUMENT_DOCUMENT_HPP
#define UTILITIES_DOCUMENT_DOCUMENT_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/document/SectionElement.hpp>

#include <utilities/core/Path.hpp>

namespace openstudio {

class Section;

class UTILITIES_API Document {
 public:
  /** @name Constructors */
  //@{

  Document(const std::string& title=std::string(),
           const std::vector<std::string>& authors=std::vector<std::string>(),
           DocumentFormat fmt=DocumentFormat(DocumentFormat::LaTeX),
           const std::string& header=std::string(),
           const std::string& footer=std::string());

  virtual ~Document() {}

  /** Make deep copy of Document. Clones all \link DocumentElement DocumentElements \endlink
   *  comprising this Document. */
  Document clone() const;

  //@}
  /** @name Getters */
  //@{

  std::string title() const;

  std::vector<std::string> authors() const;

  DocumentFormat format() const;

  std::string header() const;

  std::string footer() const;

  std::vector<SectionElement> contents() const;

  std::vector<Section> sections() const;

  unsigned topHeadingLevel() const;

  //@}
  /** @name Setters */
  //@{

  void setTitle(const std::string& title);

  void setAuthors(const std::vector<std::string>& authors);

  void addAuthor(const std::string& author);

  void setFormat(DocumentFormat fmt,
                 const std::string& header=std::string(),
                 const std::string& footer=std::string());

  /** Deletes all contents. Keeps title, authors, format. */
  void clear();

  void replaceAll(const std::vector<SectionElement>& contents);

  void replace(unsigned index, const SectionElement& element);

  void append(const SectionElement& element);

  void append(const std::vector<SectionElement>& elements);

  /** Appends new Section with heading at level 0. */
  Section appendSection(const std::string& headingText);

  void insert(unsigned index, const SectionElement& element);

  /** Inserts new Section with heading at level 0. */
  Section insertSection(unsigned index,const std::string& headingText);

  void erase(unsigned index);

  /** Sets the top heading level, and calls makeHeadingLevelsConsistent(). */
  void setTopHeadingLevel(unsigned level); 

  /** Set this section's subsections heading levels to be consistent with the section 
   *  structure. */
  void makeHeadingLevelsConsistent();

  //@}
  /** @name Queries */
  //@{

  unsigned numElements() const;

  unsigned numSections() const;

  std::string expectedFileExtension() const;

  //@}
  /** @name Printing and Serialization */
  //@{

  std::string print() const;

  std::ostream& printToStream(std::ostream& os) const;

  /** If the extension of p is osd, will save boost serialization in text format. Otherwise, will 
   *  print according to format(). */
  bool save(const openstudio::path& p, bool overwrite=false) const;

  /** Load from boost serialization text format */
  static boost::optional<Document> load(const openstudio::path& p);

  //@}
 protected:
  std::string m_title;
  std::vector<std::string> m_authors;
  DocumentFormat m_format;
  std::string m_header;
  std::string m_footer;
  std::vector<SectionElement> m_contents;
  unsigned m_topHeadingLevel;

 private:
  REGISTER_LOGGER("openstudio.Document");

  std::ostream& m_printHeader(std::ostream& os) const;
  std::ostream& m_printFooter(std::ostream& os) const;

  std::string m_authorString() const;

  // serialization support
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_NVP(m_title);
    ar & BOOST_SERIALIZATION_NVP(m_authors);
    ar & BOOST_SERIALIZATION_NVP(m_format);
    ar & BOOST_SERIALIZATION_NVP(m_header);
    ar & BOOST_SERIALIZATION_NVP(m_footer);
    ar & BOOST_SERIALIZATION_NVP(m_contents);
    ar & BOOST_SERIALIZATION_NVP(m_topHeadingLevel);
  }
};

/** \relates Document */
typedef boost::optional<Document> OptionalDocument;

/** \relates Document */
UTILITIES_API std::ostream& operator<<(std::ostream& os,const Document& doc);

} // openstudio

#endif // UTILITIES_DOCUMENT_DOCUMENT_HPP

