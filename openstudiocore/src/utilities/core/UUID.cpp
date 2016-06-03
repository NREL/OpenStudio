/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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
