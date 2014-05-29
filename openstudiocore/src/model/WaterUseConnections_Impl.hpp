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

#ifndef MODEL_WATERUSECONNECTIONS_IMPL_HPP
#define MODEL_WATERUSECONNECTIONS_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

#include "../utilities/units/Quantity.hpp"

namespace openstudio {

namespace model {

class Schedule;
class WaterUseEquipment;

namespace detail {

/** WaterUseConnections_Impl is a StraightComponent_Impl that is the implementation class for WaterUseConnections.*/
class MODEL_API WaterUseConnections_Impl : public StraightComponent_Impl {
  Q_OBJECT;

  Q_PROPERTY(boost::optional<openstudio::model::ModelObject> hotWaterSupplyTemperatureSchedule READ hotWaterSupplyTemperatureScheduleAsModelObject WRITE setHotWaterSupplyTemperatureScheduleAsModelObject RESET resetHotWaterSupplyTemperatureSchedule);
  Q_PROPERTY(boost::optional<openstudio::model::ModelObject> coldWaterSupplyTemperatureSchedule READ coldWaterSupplyTemperatureScheduleAsModelObject WRITE setColdWaterSupplyTemperatureScheduleAsModelObject RESET resetColdWaterSupplyTemperatureSchedule);

  public:

  WaterUseConnections_Impl(const IdfObject& idfObject,
                           Model_Impl* model,
                           bool keepHandle);

  WaterUseConnections_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

  WaterUseConnections_Impl(const WaterUseConnections_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

  virtual ~WaterUseConnections_Impl() {}

  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const;

  std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

  boost::optional<Schedule> hotWaterSupplyTemperatureSchedule() const;

  boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

  bool setHotWaterSupplyTemperatureSchedule(Schedule & hotWaterSupplyTemperatureSchedule);

  void resetHotWaterSupplyTemperatureSchedule();

  bool setColdWaterSupplyTemperatureSchedule(Schedule & coldWaterSupplyTemperatureSchedule);

  void resetColdWaterSupplyTemperatureSchedule();

  unsigned inletPort();

  unsigned outletPort();

  std::vector<WaterUseEquipment> waterUseEquipment() const;

  bool addWaterUseEquipment(const WaterUseEquipment & waterUseEquipment);

  bool removeWaterUseEquipment(WaterUseEquipment & waterUseEquipment);

  bool addToNode(Node & node);

  protected:

  private:

  REGISTER_LOGGER("openstudio.model.WaterUseConnections");

  boost::optional<ModelObject> hotWaterSupplyTemperatureScheduleAsModelObject() const;
  boost::optional<ModelObject> coldWaterSupplyTemperatureScheduleAsModelObject() const;

  bool setHotWaterSupplyTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  bool setColdWaterSupplyTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
};

} // detail

} // model
} // openstudio

#endif // MODEL_WATERUSECONNECTIONS_IMPL_HPP

