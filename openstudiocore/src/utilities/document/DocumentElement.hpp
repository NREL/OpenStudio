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

#ifndef UTILITIES_DOCUMENT_DOCUMENTELEMENT_HPP
#define UTILITIES_DOCUMENT_DOCUMENTELEMENT_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Serialization.hpp>

#include <utilities/document/DocumentEnums.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <string>
#include <vector>

namespace openstudio {

namespace detail {
  class DocumentElement_Impl;
}

class UTILITIES_API DocumentElement {
 public:

  /** @name Constructors */
  //@{

  virtual ~DocumentElement() {}

  DocumentElement clone() const;

  //@}
  /** @name Printing */
  //@{

  std::string print(DocumentFormat fmt) const;

  std::ostream& printToStream(std::ostream& os,DocumentFormat fmt) const;

  //@}
  /** @name Type Casting */
  //@{

  template <typename T>
  T cast() const {
    boost::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl) { throw(std::bad_cast()); }
    return T(impl);
  }

  template<typename T>
  boost::optional<T> optionalCast() const{
    boost::optional<T> result;
    boost::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){ result = T(impl); }
    return result;
  }

  //@}
 protected:
  typedef detail::DocumentElement_Impl ImplType;

  friend class detail::DocumentElement_Impl;

  DocumentElement(boost::shared_ptr<detail::DocumentElement_Impl> impl);

  template <typename T>
  boost::shared_ptr<T> getImpl() const {
    return boost::dynamic_pointer_cast<T>(m_impl);
  }

 private:
  boost::shared_ptr<detail::DocumentElement_Impl> m_impl;

  REGISTER_LOGGER("openstudio.DocumentElement");

  // bad default constructor for serialization
  DocumentElement() {};

  // serialization support
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_NVP(m_impl);
  }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(DocumentElement);

/** \relates DocumentElement */
typedef boost::optional<DocumentElement> OptionalDocumentElement;

/** \relates DocumentElement */
typedef std::vector<DocumentElement> DocumentElementVector;

}

#endif // UTILITIES_DOCUMENT_DOCUMENTELEMENT_HPP

