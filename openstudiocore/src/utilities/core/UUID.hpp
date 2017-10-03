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

#ifndef UTILITIES_CORE_UUID_HPP
#define UTILITIES_CORE_UUID_HPP

#include "../UtilitiesAPI.hpp"
#include <boost/regex.hpp>

#include <QMetaType>
#include <boost/optional.hpp>
#include <boost/uuid/uuid.hpp>
#include <vector>
#include <ostream>
#include <string>

namespace openstudio {
  class UTILITIES_API UUID;

  /// create a UUID
  UTILITIES_API UUID createUUID();

  /// create a UUID from a std::string, does not throw, may return a null UUID
  UTILITIES_API UUID toUUID(const std::string& str);

  /// create a UUID from a std::string, does not throw, may return a null UUID
  UTILITIES_API UUID toUUID(const QString& str);

  /// create a std::string from a UUID
  UTILITIES_API std::string toString(const UUID& uuid);

  /// create a QString from a UUID
  UTILITIES_API QString toQString(const UUID& uuid);

  /// create a unique name, prefix << " " << UUID.
  UTILITIES_API std::string createUniqueName(const std::string& prefix);

  /// create a std::string without curly brackets from a UUID
  UTILITIES_API std::string removeBraces(const UUID& uuid);

  UTILITIES_API std::ostream& operator<<(std::ostream& os, const UUID& uuid);

  UTILITIES_API bool operator!= (const UUID & lhs, const UUID & rhs);
  UTILITIES_API bool operator< (const UUID & lhs, const UUID & rhs);
  UTILITIES_API bool operator== (const UUID & lhs, const UUID & rhs);
  UTILITIES_API bool operator> (const UUID & lhs, const UUID & rhs);

  /// Universally Unique Identifier
  class UTILITIES_API UUID : public boost::uuids::uuid
  {
  public:
    UUID();

    bool isNull() const {
      return is_nil();
    }

    using boost::uuids::uuid::iterator;
    using boost::uuids::uuid::const_iterator;
    using boost::uuids::uuid::begin;
    using boost::uuids::uuid::end;

  private:
    explicit UUID(const boost::uuids::uuid &);

    UTILITIES_API friend UUID openstudio::createUUID();
    UTILITIES_API friend UUID openstudio::toUUID(const std::string& str);
    UTILITIES_API friend UUID openstudio::toUUID(const QString& str);
    UTILITIES_API friend std::string openstudio::toString(const UUID& uuid);
    UTILITIES_API friend QString openstudio::toQString(const UUID& uuid);
    UTILITIES_API friend std::string openstudio::createUniqueName(const std::string& prefix);
    UTILITIES_API friend std::string openstudio::removeBraces(const UUID& uuid);
    UTILITIES_API friend bool openstudio::operator!= (const UUID & lhs, const UUID & rhs);
    UTILITIES_API friend bool openstudio::operator< (const UUID & lhs, const UUID & rhs);
    UTILITIES_API friend bool openstudio::operator== (const UUID & lhs, const UUID & rhs);
    UTILITIES_API friend bool openstudio::operator> (const UUID & lhs, const UUID & rhs);

    static UUID random_generate();
    static UUID string_generate(const std::string &);
  };

  /// Find version 4 UUIDs in a string.
  UTILITIES_API boost::regex &uuidInString();

  /// create a unique name, prefix << " " << UUID.
  UTILITIES_API std::string createUniqueName(const std::string& prefix);

  /// optional UUID
  typedef boost::optional<UUID> OptionalUUID;

  /// vector of UUID
  typedef std::vector<UUID> UUIDVector;


} // openstudio

Q_DECLARE_METATYPE(openstudio::UUID);


#endif // UTILITIES_CORE_UUID_HPP
