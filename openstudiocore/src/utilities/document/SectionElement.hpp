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

#ifndef UTILITIES_DOCUMENT_SECTIONELEMENT_HPP
#define UTILITIES_DOCUMENT_SECTIONELEMENT_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/document/DocumentElement.hpp>

namespace openstudio {

namespace detail {
  class SectionElement_Impl;
} // detail

class UTILITIES_API SectionElement : public DocumentElement {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~SectionElement() {}

  //@}
 protected:
  typedef detail::SectionElement_Impl ImplType;

  friend class DocumentElement;
  friend class detail::DocumentElement_Impl;
  friend class detail::SectionElement_Impl;

  SectionElement(boost::shared_ptr<detail::SectionElement_Impl> impl);
 public:
  REGISTER_LOGGER("openstudio.SectionElement");

  // bad default constructor for serialization
  SectionElement();
 
  // serialization support
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DocumentElement);
  }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(SectionElement);

/** \relates SectionElement */
typedef boost::optional<SectionElement> OptionalSectionElement;

/** \relates SectionElement */
typedef std::vector<SectionElement> SectionElementVector;

} // openstudio

#endif // UTILITIES_DOCUMENT_SECTIONELEMENT_HPP
