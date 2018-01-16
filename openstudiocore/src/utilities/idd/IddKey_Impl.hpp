/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef UTILITIES_IDD_IDDKEY_IMPL_HPP
#define UTILITIES_IDD_IDDKEY_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "IddKeyProperties.hpp"

#include "../core/Logger.hpp"

#include <string>
#include <ostream>

namespace openstudio {

// private namespace
namespace detail {

  /** Implementation class for IddKey. */
  class UTILITIES_API IddKey_Impl {
   public:

    /// default constructor for serialization
    IddKey_Impl();

    /// equality operator
    bool operator==(const IddKey_Impl& other) const;

    /// get name
    std::string name() const;

    /// get properties
    const IddKeyProperties& properties() const;

    /// load by parsing text
    static std::shared_ptr<IddKey_Impl> load(const std::string& name, const std::string& text);

    /// print idd
    std::ostream& print(std::ostream& os) const;

   private:

    /// partial constructor used by load
    IddKey_Impl(const std::string& name);

    // parse
    void parse(const std::string& text);

    // name
    std::string m_name;

    // properties
    IddKeyProperties m_properties;

    // configure logging
    REGISTER_LOGGER("utilities.idd.IddKey");
  };

} // detail

} // openstudio

#endif // UTILITIES_IDD_IDDKEY_IMPL_HPP
