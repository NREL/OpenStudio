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

#ifndef MODEL_WATERUSEEQUIPMENTDEFINITION_HPP
#define MODEL_WATERUSEEQUIPMENTDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

namespace openstudio {

  class Quantity;
  class OSOptionalQuantity;

namespace model {

class Schedule;

namespace detail {

  class WaterUseEquipmentDefinition_Impl;

} // detail

/** WaterUseEquipmentDefinition is a SpaceLoadDefinition that wraps the OpenStudio IDD object 'OS:WaterUse:Equipment:Definition'. */
class MODEL_API WaterUseEquipmentDefinition : public SpaceLoadDefinition {
  public:

  explicit WaterUseEquipmentDefinition(const Model& model);

  virtual ~WaterUseEquipmentDefinition() {}

  static const IddObjectType& iddObjectType();

  std::string endUseSubcategory() const;
  bool isEndUseSubcategoryDefaulted() const;
  void setEndUseSubcategory(std::string endUseSubcategory);
  void resetEndUseSubcategory();

  double peakFlowRate() const;
  Quantity getPeakFlowRate(bool returnIP=false) const;
  bool setPeakFlowRate(double peakFlowRate);
  bool setPeakFlowRate(const Quantity& peakFlowRate);

  boost::optional<Schedule> targetTemperatureSchedule() const;
  bool setTargetTemperatureSchedule(const Schedule& targetTemperatureSchedule);
  void resetTargetTemperatureSchedule();

  boost::optional<Schedule> sensibleFractionSchedule() const;
  bool setSensibleFractionSchedule(const Schedule& sensibleFractionSchedule);
  void resetSensibleFractionSchedule();

  boost::optional<Schedule> latentFractionSchedule() const;
  bool setLatentFractionSchedule(const Schedule& latentFractionSchedule);
  void resetLatentFractionSchedule();

  protected:

  typedef detail::WaterUseEquipmentDefinition_Impl ImplType;

  explicit WaterUseEquipmentDefinition(std::shared_ptr<detail::WaterUseEquipmentDefinition_Impl> impl);

  friend class detail::WaterUseEquipmentDefinition_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  private:

  REGISTER_LOGGER("openstudio.model.WaterUseEquipmentDefinition");
};

typedef boost::optional<WaterUseEquipmentDefinition> OptionalWaterUseEquipmentDefinition;

typedef std::vector<WaterUseEquipmentDefinition> WaterUseEquipmentDefinitionVector;

} // model
} // openstudio

#endif // MODEL_WATERUSEEQUIPMENTDEFINITION_HPP

