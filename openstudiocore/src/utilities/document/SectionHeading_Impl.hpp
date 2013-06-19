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

#ifndef UTILITIES_DOCUMENT_SECTIONHEADING_IMPL_HPP
#define UTILITIES_DOCUMENT_SECTIONHEADING_IMPL_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/document/DocumentElement_Impl.hpp>

namespace openstudio {

namespace detail {

  class UTILITIES_API SectionHeading_Impl : public DocumentElement_Impl {
   public:
    /** @name Constructors */
    //@{

    SectionHeading_Impl(unsigned level = 0, const std::string& text = std::string());

    virtual ~SectionHeading_Impl() {}

    virtual DocumentElement clone() const;

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
    /** @name Printing */
    //@{

    virtual std::string print(DocumentFormat fmt) const;

    virtual std::ostream& printToStream(std::ostream& os, DocumentFormat fmt) const;

    //@}
   private:
    unsigned m_level;
    std::string m_text;

    std::string mf_printCOUT() const;
    std::string mf_printLaTeX() const;
    std::string mf_printXHTML() const;

    REGISTER_LOGGER("openstudio.SectionHeading");

    // serialization support
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DocumentElement_Impl);
      ar & BOOST_SERIALIZATION_NVP(m_level);
      ar & BOOST_SERIALIZATION_NVP(m_text);
    }

  };

} // detail

} // openstudio

#endif // UTILITIES_DOCUMENT_SECTIONHEADING_IMPL_HPP
