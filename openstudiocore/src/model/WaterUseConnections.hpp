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

#ifndef MODEL_WATERUSECONNECTIONS_HPP
#define MODEL_WATERUSECONNECTIONS_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;
class WaterUseEquipment;

namespace detail {

class WaterUseConnections_Impl;

} // detail

/** WaterUseConnections is a StraightComponent that wraps the OpenStudio IDD object
 *  'OS:WaterUse:Connections'. */
class MODEL_API WaterUseConnections : public StraightComponent {

  public:

  explicit WaterUseConnections(const Model& model);

  virtual ~WaterUseConnections() {}

  static IddObjectType iddObjectType();

  boost::optional<Schedule> hotWaterSupplyTemperatureSchedule() const;

  bool setHotWaterSupplyTemperatureSchedule(Schedule& hotWaterSupplyTemperatureSchedule);

  void resetHotWaterSupplyTemperatureSchedule();

  boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

  bool setColdWaterSupplyTemperatureSchedule(Schedule& coldWaterSupplyTemperatureSchedule);

  void resetColdWaterSupplyTemperatureSchedule();

  std::vector<WaterUseEquipment> waterUseEquipment() const;

  bool addWaterUseEquipment(const WaterUseEquipment & waterUseEquipment);

  // Detaches waterUseEquipment from this object but does not remove it from the model
  bool removeWaterUseEquipment(WaterUseEquipment & waterUseEquipment);

  unsigned inletPort();

  unsigned outletPort();

  protected:

  /// @cond
  typedef detail::WaterUseConnections_Impl ImplType;

  explicit WaterUseConnections(std::shared_ptr<detail::WaterUseConnections_Impl> impl);

  friend class detail::WaterUseConnections_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

  private:

  REGISTER_LOGGER("openstudio.model.WaterUseConnections");
};

/** \relates WaterUseConnections*/
typedef boost::optional<WaterUseConnections> OptionalWaterUseConnections;

/** \relates WaterUseConnections*/
typedef std::vector<WaterUseConnections> WaterUseConnectionsVector;

} // model
} // openstudio

#endif // MODEL_WATERUSECONNECTIONS_HPP

