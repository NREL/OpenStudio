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

#ifndef UTILITIES_CORE_OPTIONAL_HPP
#define UTILITIES_CORE_OPTIONAL_HPP

#include "../UtilitiesAPI.hpp"

#include <boost/optional.hpp>

#include <QVariant>
#include <QMetaType>
#include <QUrl>

#include <string>
#include <ostream>

namespace openstudio {
  // note: boost::optional<bool> is a bad idea, don't do it
  typedef boost::optional<double> OptionalDouble;
  typedef boost::optional<unsigned> OptionalUnsigned;
  typedef boost::optional<int> OptionalInt;
  typedef boost::optional<std::string> OptionalString;

  UTILITIES_API std::ostream& operator<<(std::ostream& os, const boost::optional<double>& x);
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const boost::optional<unsigned>& x);
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const boost::optional<int>& x);
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const boost::optional<std::string>& x);
}

// declare these types so we can use them as properties
Q_DECLARE_METATYPE(boost::optional<double>);
Q_DECLARE_METATYPE(boost::optional<unsigned>);
Q_DECLARE_METATYPE(boost::optional<int>);
Q_DECLARE_METATYPE(boost::optional<std::string>);

namespace openstudio {
namespace detail {

  // register meta datatypes
  struct OptionalMetaTypeInitializer{
    OptionalMetaTypeInitializer();
  };

}
}

#endif // UTILITIES_CORE_OPTIONAL_HPP
