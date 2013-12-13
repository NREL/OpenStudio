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

#ifndef UTILITIES_DOCUMENT_SECTIONELEMENT_IMPL_HPP
#define UTILITIES_DOCUMENT_SECTIONELEMENT_IMPL_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/document/DocumentElement_Impl.hpp>

namespace openstudio {

namespace detail {

  class UTILITIES_API SectionElement_Impl : public DocumentElement_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~SectionElement_Impl() {}

    //@}
   private:
    REGISTER_LOGGER("openstudio.SectionElement");

    // serialization support
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DocumentElement_Impl);
    }
  };

  BOOST_SERIALIZATION_ASSUME_ABSTRACT(SectionElement_Impl);

} // detail

} // openstudio

#endif // UTILITIES_DOCUMENT_SECTIONELEMENT_IMPL_HPP
