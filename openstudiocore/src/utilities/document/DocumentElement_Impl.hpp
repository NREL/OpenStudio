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

#ifndef UTILITIES_DOCUMENT_DOCUMENTELEMENT_IMPL_HPP
#define UTILITIES_DOCUMENT_DOCUMENTELEMENT_IMPL_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Serialization.hpp>

#include <utilities/document/DocumentEnums.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/optional.hpp>

#include <string>
#include <vector>

namespace openstudio {

namespace detail {

  class UTILITIES_API DocumentElement_Impl : public boost::enable_shared_from_this<DocumentElement_Impl> {
   public:

    /** @name Constructors */
    //@{

    virtual ~DocumentElement_Impl() {}

    virtual DocumentElement clone() const=0;

    //@}
    /** @name Printing */
    //@{

    virtual std::string print(DocumentFormat fmt) const=0;

    virtual std::ostream& printToStream(std::ostream& os, DocumentFormat fmt) const=0;

    //@}
    /** @name Type Casting */
    //@{

    /** Get an object that wraps this impl. */
    template<typename T>
    T getObject() const {
      T result(boost::dynamic_pointer_cast<typename T::ImplType>(
                 boost::const_pointer_cast<DocumentElement_Impl>(shared_from_this())));
      return result;
    }

    //@}
   private:
    REGISTER_LOGGER("openstudio.DocumentElement");

    // serialization support
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {}
  };

  BOOST_SERIALIZATION_ASSUME_ABSTRACT(DocumentElement_Impl);

} // detail

} // openstudio

#endif // UTILITIES_DOCUMENT_DOCUMENTELEMENT_IMPL_HPP
