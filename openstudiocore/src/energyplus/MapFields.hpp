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

#ifndef ENERGYPLUS_MAPFIELDS_HPP
#define ENERGYPLUS_MAPFIELDS_HPP

#include "EnergyPlusAPI.hpp"

#include <utility>
#include <vector>

namespace openstudio {

  class IdfObject;

namespace energyplus {

  // map fields directly from old object to new one, must have same exact fields
  ENERGYPLUS_API  bool mapFields(const IdfObject& oldObject, IdfObject& newObject);

  // map fields directly from old object to new one
  ENERGYPLUS_API  bool mapFields(const IdfObject& oldObject, IdfObject& newObject, std::vector<std::pair<unsigned, unsigned> > fieldMap);


} // energyplus
} // openstudio

#endif // ENERGYPLUS_MAPFIELDS_HPP
