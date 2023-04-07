/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_IDFOBJECTDIFF_IMPL_HPP
#define UTILITIES_IDF_IDFOBJECTDIFF_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include <boost/optional.hpp>

#include <string>

namespace openstudio {
namespace detail {

  class UTILITIES_API IdfObjectDiff_Impl
  {
   public:
    IdfObjectDiff_Impl();

    IdfObjectDiff_Impl(boost::optional<unsigned> index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue);

    virtual ~IdfObjectDiff_Impl() = default;

    /// get the index that changed, uninitialized means no field (e.g. object level comment changed)
    boost::optional<unsigned> index() const;

    /// returns true if oldValue == newValue
    bool isNull() const;

    /// get the old value, uninitialized optional means that the field did not previously exist
    boost::optional<std::string> oldValue() const;

    /// get the new value, uninitialized optional means that the field no longer exists
    boost::optional<std::string> newValue() const;

   private:
    boost::optional<unsigned> m_index;
    boost::optional<std::string> m_oldValue;
    boost::optional<std::string> m_newValue;
  };

}  // namespace detail
}  // namespace openstudio

#endif  // UTILITIES_IDF_IDFOBJECTDIFF_IMPL_HPP
