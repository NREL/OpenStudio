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

#ifndef UTILITIES_DOCUMENT_SECTIONHEADING_HPP
#define UTILITIES_DOCUMENT_SECTIONHEADING_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/document/DocumentElement.hpp>

namespace openstudio {

namespace detail {
  class SectionHeading_Impl;
}

class UTILITIES_API SectionHeading : public DocumentElement {
 public:
  /** @name Constructors */
  //@{

  SectionHeading(unsigned level = 0, const std::string& text = std::string());

  virtual ~SectionHeading() {}

  //@}
  /** @name Getters */
  //@{

  unsigned level() const;

  std::string text() const;

  //@}
  /** @name Setters */
  //@{

  void setLevel(unsigned level);

  void setText(const std::string& text);

  //@}
 protected:
  typedef detail::SectionHeading_Impl ImplType;

  friend class DocumentElement;
  friend class detail::DocumentElement_Impl;
  friend class detail::SectionHeading_Impl;

  SectionHeading(boost::shared_ptr<detail::SectionHeading_Impl> impl);

 private:
  REGISTER_LOGGER("openstudio.SectionHeading");

  // serialization support
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DocumentElement);
  }
};

/** \relates SectionHeading */
typedef boost::optional<SectionHeading> OptionalSectionHeading;

/** \relates SectionHeading */
typedef std::vector<std::string> SectionHeadingVector;

} // openstudio

#endif // UTILITIES_DOCUMENT_SECTIONHEADING_HPP
