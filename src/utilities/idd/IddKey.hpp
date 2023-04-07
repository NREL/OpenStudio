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

#ifndef UTILITIES_IDD_IDDKEY_HPP
#define UTILITIES_IDD_IDDKEY_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"

#include <boost/optional.hpp>

#include <string>
#include <vector>
#include <ostream>

namespace openstudio {

struct IddKeyProperties;

namespace detail {
  class IddKey_Impl;
}

/** IddKey represents an enumeration value for an IDD field of type choice. */
class UTILITIES_API IddKey
{
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor */
  IddKey();

  /** Copy constructor shares implementation. */
  IddKey(const IddKey& other);

  //@}
  /** @name Getters */
  //@{

  /** Returns the key name */
  std::string name() const;

  /** Returns the key properties. */
  const IddKeyProperties& properties() const;

  //@}
  /** @name Queries */
  //@{

  /** Returns true if all data is exactly equal. */
  bool operator==(const IddKey& other) const;

  /** Negation of operator==. */
  bool operator!=(const IddKey& other) const;

  //@}
  /** @name Serialization */
  //@{

  /** Load from text. */
  static boost::optional<IddKey> load(const std::string& name, const std::string& text);

  /** Print to os in standard IDD format */
  std::ostream& print(std::ostream& os) const;

  //@}
 private:
  ///@cond
  // pointer to implementation
  std::shared_ptr<detail::IddKey_Impl> m_impl;

  // construct from impl
  IddKey(const std::shared_ptr<detail::IddKey_Impl>& impl);
  ///@endcond

  // configure logging
  REGISTER_LOGGER("openstudio.idd.IddKey");
};

/** \relates IddKey */
using IddKeyVector = std::vector<IddKey>;

/** \relates IddKey */
using OptionalIddKey = boost::optional<IddKey>;

/** \relates IddKey */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const IddKey& iddKey);

}  // namespace openstudio

#endif  // UTILITIES_IDD_IDDKEY_HPP
