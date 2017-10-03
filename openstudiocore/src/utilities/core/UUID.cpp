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

#include "UUID.hpp"
#include "String.hpp"
#include "Checksum.hpp"
#include "StaticInitializer.hpp"

#include <sstream>

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/thread/tss.hpp>

#ifdef __APPLE__

#include <QtGlobal>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>


#endif  // __APPLE__

int _uuid_id = qRegisterMetaType<openstudio::UUID>("openstudio::UUID");

namespace openstudio {

#ifdef __APPLE__

  namespace detail {

    // if you do not initialize random number generator on Mac you will generate same sequence of UUIDs
    // this issue is addressed in Qt 4.7 by using /dev/urandom
    struct RandomNumberInitializer
    {
      RandomNumberInitializer(){
        FILE *urandom;
        unsigned int seed;
        urandom = fopen ("/dev/urandom", "r");
        fread(&seed, sizeof (seed), 1, urandom);
        fclose(urandom);
        qsrand(seed);
      }
    };

    RandomNumberInitializer _randomNumberInitializer;

  }

#endif  // __APPLE__

  namespace detail {
    struct BoostGeneratorsInitializer : StaticInitializer<BoostGeneratorsInitializer>
    {
      static void initialize()
      {
        createUUID();
        toUUID(std::string("00000000-0000-0000-0000-000000000000"));
      }
    };
    struct MakeSureBoostGeneratorsInitializerIsInitialized
    {
      MakeSureBoostGeneratorsInitializerIsInitialized()
      {}

      BoostGeneratorsInitializer m_i;
    };

  }


UUID::UUID()
  : boost::uuids::uuid(boost::uuids::nil_uuid())
{
}

UUID::UUID(const boost::uuids::uuid &t_other)
  : boost::uuids::uuid(t_other)
{
}

UUID UUID::random_generate()
{
  static boost::thread_specific_ptr<boost::uuids::random_generator> gen;

  if (gen.get() == nullptr) {
    gen.reset(new boost::uuids::random_generator);
  }

  return UUID((*gen)());
}

UUID UUID::string_generate(const std::string &t_str)
{
  static boost::thread_specific_ptr<boost::uuids::string_generator> gen;

  if (gen.get() == nullptr) {
    gen.reset(new boost::uuids::string_generator);
  }

  return UUID((*gen)(t_str));
}


UUID createUUID()
{
  return UUID::random_generate();
}

UUID toUUID(const QString &str)
{
  return toUUID(toString(str));
}

UUID toUUID(const std::string& str)
{
  try {
    return UUID::string_generate(str);
  } catch (...) {
    return UUID();
  }
}

// Finds Version 4 uuid in a string including {}
boost::regex &uuidInString() {
  static boost::regex result("(\\{[a-f0-9]{8}-[a-f0-9]{4}-4[a-f0-9]{3}-[89aAbB][a-f0-9]{3}-[a-f0-9]{12}\\})");
  return result;
}

bool operator!= ( const UUID & lhs, const UUID & rhs ) {
  return static_cast<const boost::uuids::uuid&>(lhs) != static_cast<const boost::uuids::uuid&>(rhs);
}

bool operator< ( const UUID & lhs, const UUID & rhs ) {
  return static_cast<const boost::uuids::uuid&>(lhs) < static_cast<const boost::uuids::uuid&>(rhs);
}

bool operator== ( const UUID & lhs, const UUID & rhs ) {
  return static_cast<const boost::uuids::uuid&>(lhs) == static_cast<const boost::uuids::uuid&>(rhs);
}

bool operator> ( const UUID & lhs, const UUID & rhs ) {
  return static_cast<const boost::uuids::uuid&>(lhs) > static_cast<const boost::uuids::uuid&>(rhs);
}

std::string toString(const UUID& uuid)
{
  std::stringstream ss;
  ss << '{';
  boost::uuids::operator<<(ss, uuid);
  ss << '}';
  return ss.str();
}

QString toQString(const UUID& uuid)
{
  return toQString(toString(uuid));
}


std::string createUniqueName(const std::string& prefix) {
  std::stringstream ss;
  if (!prefix.empty()) {
    ss << prefix << " ";
  }
  ss << toString(createUUID());
  return ss.str();
}

std::string removeBraces(const UUID& uuid) {
  std::stringstream ss;
  boost::uuids::operator<<(ss, uuid);
  return ss.str();
}

std::ostream& operator<<(std::ostream& os,const UUID& uuid) {
  os << toString(uuid);
  return os;
}


} // openstudio
