/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ENERGYPLUS_MAPFIELDS_HPP
#define ENERGYPLUS_MAPFIELDS_HPP

#include "EnergyPlusAPI.hpp"

#include <utility>
#include <vector>

namespace openstudio {

class IdfObject;

namespace energyplus {

  // map fields directly from old object to new one, must have same exact fields
  ENERGYPLUS_API bool mapFields(const IdfObject& oldObject, IdfObject& newObject);

  // map fields directly from old object to new one
  ENERGYPLUS_API bool mapFields(const IdfObject& oldObject, IdfObject& newObject, const std::vector<std::pair<unsigned, unsigned>>& fieldMap);

}  // namespace energyplus
}  // namespace openstudio

#endif  // ENERGYPLUS_MAPFIELDS_HPP
