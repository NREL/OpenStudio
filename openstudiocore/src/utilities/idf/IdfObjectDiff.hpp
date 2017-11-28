/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
    return T(std::move(impl));
  }

  /// cast to optional of type T
  template<typename T>
  boost::optional<T> optionalCast() const{
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){
      result = T(std::move(impl));
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
