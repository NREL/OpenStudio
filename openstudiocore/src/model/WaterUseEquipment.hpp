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

#ifndef MODEL_WATERUSEEQUIPMENT_HPP
#define MODEL_WATERUSEEQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;
class ThermalZone;
class WaterUseConnections;
class WaterUseEquipmentDefinition;

namespace detail {

class WaterUseEquipment_Impl;

} // detail

/** WaterUseEquipment is a ModelObject that wraps the OpenStudio IDD object 'OS:WaterUse:Equipment'. */
class MODEL_API WaterUseEquipment : public SpaceLoadInstance {
  public:

  explicit WaterUseEquipment(const WaterUseEquipmentDefinition& waterUseEquipmentDefinition);

  virtual ~WaterUseEquipment() {}

  static const IddObjectType& iddObjectType();

  boost::optional<Schedule> flowRateFractionSchedule() const;
  bool setFlowRateFractionSchedule(Schedule& flowRateFractionSchedule);
  void resetFlowRateFractionSchedule();

  WaterUseEquipmentDefinition waterUseEquipmentDefinition() const;
  bool setWaterUseEquipmentDefinition(const WaterUseEquipmentDefinition & definition);

  boost::optional<WaterUseConnections> waterUseConnections() const;

  protected:

  /// @cond

  typedef detail::WaterUseEquipment_Impl ImplType;

  explicit WaterUseEquipment(std::shared_ptr<detail::WaterUseEquipment_Impl> impl);

  friend class detail::WaterUseEquipment_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

  private:

  REGISTER_LOGGER("openstudio.model.WaterUseEquipment");
};

/** \relates WaterUseEquipment*/
typedef boost::optional<WaterUseEquipment> OptionalWaterUseEquipment;

/** \relates WaterUseEquipment*/
typedef std::vector<WaterUseEquipment> WaterUseEquipmentVector;

} // model
} // openstudio

#endif // MODEL_WATERUSEEQUIPMENT_HPP