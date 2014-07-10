/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef UTILITIES_IDF_IDFOBJECTDIFF_HPP
#define UTILITIES_IDF_IDFOBJECTDIFF_HPP

#include "../UtilitiesAPI.hpp"

#include <boost/optional.hpp>

#include <memory>

namespace openstudio {
namespace detail {
  class IdfObjectDiff_Impl;
}

/** IdfObjectDiff represents a change to an IdfObject.
**/
class UTILITIES_API IdfObjectDiff {
public:

  IdfObjectDiff();

  IdfObjectDiff(boost::optional<unsigned> index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue);

  /// get the index that changed, uninitialized means no field (e.g. object level comment changed)
  boost::optional<unsigned> index() const;

  /// returns true if oldValue == newValue
  bool isNull() const;

  /// get the old value, uninitialized optional means that the field did not previously exist
  boost::optional<std::string> oldValue() const;

  /// get the new value, uninitialized optional means that the field no longer exists
  boost::optional<std::string> newValue() const;

  /// cast to type T, can throw std::bad_cast
  template<typename T>
  T cast() const{
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl){
      throw(std::bad_cast());
    }
    return T(impl);
  }

  /// cast to optional of type T
  template<typename T>
  boost::optional<T> optionalCast() const{
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){
      result = T(impl);
    }
    return result;
  }

protected:

  IdfObjectDiff(const std::shared_ptr<detail::IdfObjectDiff_Impl>& impl);

  // get the impl
  template<typename T>
    std::shared_ptr<T> getImpl() const
  {  return std::dynamic_pointer_cast<T>(m_impl); }

private:

  std::shared_ptr<detail::IdfObjectDiff_Impl> m_impl;

};

} // openstudio

#endif // UTILITIES_IDF_IDFOBJECTDIFF_HPP
