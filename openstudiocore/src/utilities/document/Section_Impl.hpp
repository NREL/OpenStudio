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

#ifndef UTILITIES_DOCUMENT_SECTION_IMPL_HPP
#define UTILITIES_DOCUMENT_SECTION_IMPL_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/document/SectionElement_Impl.hpp>
#include <utilities/document/SectionHeading.hpp>

namespace openstudio {

namespace detail {

  class UTILITIES_API Section_Impl : public SectionElement_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    Section_Impl(const SectionHeading& heading=SectionHeading(),
                 const std::vector<SectionElement>& contents=std::vector<SectionElement>());

    virtual ~Section_Impl() {}

    virtual DocumentElement clone() const;

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

    Section appendSubsection(const std::string& headingText);

    void insert(unsigned index, const SectionElement& element);

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
    /** @name Printing */
    //@{

    virtual std::string print(DocumentFormat fmt) const;

    virtual std::ostream& printToStream(std::ostream& os, DocumentFormat fmt) const;

    //@}
   private:
    SectionHeading m_heading;
    std::vector<SectionElement> m_contents;

    REGISTER_LOGGER("openstudio.Section");

    // serialization support
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SectionElement_Impl);
      ar & BOOST_SERIALIZATION_NVP(m_heading);
      ar & BOOST_SERIALIZATION_NVP(m_contents);
    }
  };

} // detail

} // openstudio

#endif // UTILITIES_DOCUMENT_SECTION_IMPL_HPP
