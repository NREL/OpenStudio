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

#include "MapFields.hpp"

#include "../utilities/idf/IdfObject.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace energyplus {

  bool mapFields(const IdfObject& oldObject, IdfObject& newObject)
  {
    bool result = true;
    unsigned numFields = oldObject.numFields();
    for (unsigned i=0; i < numFields; ++i){
      OptionalString oldStr = oldObject.getString(i);
      if (oldStr){
        result = result && newObject.setString(i, *oldStr);
      }
    }
    return result;
  }

  bool mapFields(const IdfObject& oldObject, IdfObject& newObject, std::vector<std::pair<unsigned, unsigned> > fieldMap)
  {
    bool result = true;
    typedef std::pair<unsigned, unsigned> PairType;
    for (PairType p : fieldMap){
      OptionalString oldStr = oldObject.getString(p.first);
      if (oldStr){
        result = result && newObject.setString(p.second, *oldStr);
      }
    }
    return result;
  }


} // energyplus
} // openstudio
