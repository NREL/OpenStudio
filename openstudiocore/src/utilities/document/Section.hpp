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

#ifndef UTILITIES_DOCUMENT_SECTION_HPP
#define UTILITIES_DOCUMENT_SECTION_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/document/SectionElement.hpp>
#include <utilities/document/SectionHeading.hpp>

namespace openstudio {

namespace detail {
  class Section_Impl;
}

class UTILITIES_API Section : public SectionElement {
 public:
  /** @name Constructors and Destructors */
  //@{

  Section(const SectionHeading& heading=SectionHeading(),
          const std::vector<SectionElement>& contents=std::vector<SectionElement>());

  virtual ~Section() {}

  //@}
  /** @name Getters */
  //@{

  SectionHeading heading() const;

  std::vector<SectionElement> contents() const;

  SectionElement getElement(unsigned index) const;

  std::vector<Section> subsections() const;

  //@}
  /** @name Setters */
  //@{

  /** Deletes all contents. Keeps heading. */
  void clear();

  void replaceAll(const std::vector<SectionElement>& contents);

  void replace(unsigned index, const SectionElement& element);

  void append(const SectionElement& element);

  void append(const std::vector<SectionElement>& elements);

  /** Appends new Section with heading at level heading().level()+1. */
  Section appendSubsection(const std::string& headingText);

  void insert(unsigned index, const SectionElement& element);

  /** Inserts new Section with heading at level heading().level()+1. */
  Section insertSubsection(unsigned index,const std::string& headingText);

  void erase(unsigned index);

  /** Set this section's subsections heading levels to be consistent with the section 
   *  structure. */
  void makeHeadingLevelsConsistent();

  //@}
  /** @name Queries */
  //@{

  unsigned numElements() const;

  unsigned numSubsections() const;

  //@}
 protected:
  typedef detail::Section_Impl ImplType;

  friend class DocumentElement;
  friend class detail::DocumentElement_Impl;
  friend class detail::Section_Impl;

  Section(boost::shared_ptr<detail::Section_Impl> impl);  

 private:
  REGISTER_LOGGER("openstudio.Section");

  // serialization support
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SectionElement);
  }
};

/** \relates Section */
typedef boost::optional<Section> OptionalSection;

/** \relates Section */
typedef std::vector<Section> SectionVector;

} // openstudio

#endif // UTILITIES_DOCUMENT_SECTION_HPP
