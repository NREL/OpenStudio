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

#include "UUID.hpp"
#include "String.hpp"
#include "Checksum.hpp"
#include <sstream>

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

UUID createUUID() 
{
  return QUuid::createUuid();
}
  
UUID toUUID(const std::string& str)
{
  UUID uuid;
  try{
    uuid = UUID(toQString(str));
  }catch(...){
    try {
      uuid = UUID(toQString("{" + str + "}"));
    }
    catch(...) {}
  }
  return uuid;
}

std::string toString(const UUID& uuid)
{
  return toString(uuid.toString());
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
  return uuid.toString().replace("{", "").replace("}", "").toStdString();
}

std::ostream& operator<<(std::ostream& os,const UUID& uuid) {
  os << toString(uuid);
  return os;
}


} // openstudio
