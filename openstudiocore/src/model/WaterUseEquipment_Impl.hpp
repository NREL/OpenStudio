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

#ifndef MODEL_WATERUSEEQUIPMENT_IMPL_HPP
#define MODEL_WATERUSEEQUIPMENT_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance_Impl.hpp"
#include "../utilities/units/Quantity.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ThermalZone;
class WaterUseConnections;
class WaterUseEquipmentDefinition;

namespace detail {

class MODEL_API WaterUseEquipment_Impl : public SpaceLoadInstance_Impl {
  Q_OBJECT;

  Q_PROPERTY(boost::optional<openstudio::model::ModelObject> flowRateFractionSchedule READ flowRateFractionScheduleAsModelObject WRITE setFlowRateFractionScheduleAsModelObject RESET resetFlowRateFractionSchedule);

  public:


  WaterUseEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  WaterUseEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
              Model_Impl* model,
              bool keepHandle);

  WaterUseEquipment_Impl(const WaterUseEquipment_Impl& other,
              Model_Impl* model,
              bool keepHandle);

  virtual ~WaterUseEquipment_Impl() {}

  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const;

  virtual bool hardSize();

  virtual bool hardApplySchedules();

  virtual double multiplier() const;

  virtual bool isMultiplierDefaulted() const;

  virtual bool setDefinition(const SpaceLoadDefinition& definition);

  virtual std::vector<IdfObject> remove();

  std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

  boost::optional<Schedule> flowRateFractionSchedule() const;
  bool setFlowRateFractionSchedule(Schedule & flowRateFractionSchedule);
  void resetFlowRateFractionSchedule();

  boost::optional<WaterUseConnections> waterUseConnections() const;

  WaterUseEquipmentDefinition waterUseEquipmentDefinition() const;
  bool setWaterUseEquipmentDefinition(const WaterUseEquipmentDefinition & definition);

  protected:

  // index of the space name
  virtual int spaceIndex() const;

  // index of the definition name
  virtual int definitionIndex() const;

  private:

  REGISTER_LOGGER("openstudio.model.WaterUseEquipment");

  openstudio::Quantity peakFlowRate_SI() const;
  openstudio::Quantity peakFlowRate_IP() const;

  boost::optional<ModelObject> flowRateFractionScheduleAsModelObject() const;

  bool setFlowRateFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
};

} // detail

} // model
} // openstudio

#endif // MODEL_WATERUSEEQUIPMENT_IMPL_HPP
