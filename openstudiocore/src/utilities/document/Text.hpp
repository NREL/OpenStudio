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

#ifndef UTILITIES_DOCUMENT_TEXT_HPP
#define UTILITIES_DOCUMENT_TEXT_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/document/SectionElement.hpp>

namespace openstudio {

namespace detail {
  class Text_Impl;
}

class UTILITIES_API Text : public SectionElement {
 public:
  /** @name Constructors */
  //@{

  Text();

  Text(const std::string& paragraph);

  Text(const std::vector<std::string>& paragraphs);

  virtual ~Text() {}

  //@}
  /** @name Getters */
  //@{

  /** Get all the paragraphs in this Text object. */
  std::vector<std::string> paragraphs() const;

  /** Get paragraph at index. Throws if index >= numParagraphs(). */
  std::string getParagraph(unsigned index) const;

  //@}
  /** @name Setters */
  //@{

  void clear();

  void replaceAll(const std::string& paragraph);

  void replaceAll(const std::vector<std::string>& paragraphs);

  /** Replace text at index with paragraph. Throws if index >= numParagraphs(). */
  void replace(unsigned index, const std::string& paragraph);

  void append(const std::string& paragraph);

  void append(const std::vector<std::string>& paragraphs);

  /** Insert paragraph at index. Throws if index > numParagraphs(). */
  void insert(unsigned index, const std::string& paragraph);

  /** Erase paragraph at index. Does nothing if index >= numParagraphs(). */
  void erase(unsigned index);

  //@}
  /** @name Queries */
  //@{

  unsigned numParagraphs() const;

  //@}
 protected:
  typedef detail::Text_Impl ImplType;

  friend class DocumentElement;
  friend class detail::DocumentElement_Impl;
  friend class detail::Text_Impl;

  Text(boost::shared_ptr<detail::Text_Impl> impl);

 private:
  REGISTER_LOGGER("openstudio.Text");

  // serialization support
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SectionElement);
  }
};

/** \relates Text */
typedef boost::optional<Text> OptionalText;

/** \relates Text */
typedef std::vector<Text> TextVector;

}

#endif // UTILITIES_DOCUMENT_TEXT_HPP
