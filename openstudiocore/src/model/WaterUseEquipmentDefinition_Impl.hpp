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

#ifndef MODEL_WATERUSEEQUIPMENTDEFINITION_IMPL_HPP
#define MODEL_WATERUSEEQUIPMENTDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"

#include "../utilities/units/Quantity.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

/** WaterUseEquipmentDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for WaterUseEquipmentDefinition.*/
class MODEL_API WaterUseEquipmentDefinition_Impl : public SpaceLoadDefinition_Impl {
  Q_OBJECT;

  Q_PROPERTY(std::string endUseSubcategory READ endUseSubcategory WRITE setEndUseSubcategory RESET resetEndUseSubcategory);
  Q_PROPERTY(bool isEndUseSubcategoryDefaulted READ isEndUseSubcategoryDefaulted);

  Q_PROPERTY(double peakFlowRate READ peakFlowRate WRITE setPeakFlowRate);
  Q_PROPERTY(openstudio::Quantity peakFlowRate_SI READ peakFlowRate_SI WRITE setPeakFlowRate);
  Q_PROPERTY(openstudio::Quantity peakFlowRate_IP READ peakFlowRate_IP WRITE setPeakFlowRate);

  Q_PROPERTY(boost::optional<openstudio::model::ModelObject> targetTemperatureSchedule READ targetTemperatureScheduleAsModelObject WRITE setTargetTemperatureScheduleAsModelObject RESET resetTargetTemperatureSchedule);
  Q_PROPERTY(boost::optional<openstudio::model::ModelObject> sensibleFractionSchedule READ sensibleFractionScheduleAsModelObject WRITE setSensibleFractionScheduleAsModelObject RESET resetSensibleFractionSchedule);
  Q_PROPERTY(boost::optional<openstudio::model::ModelObject> latentFractionSchedule READ latentFractionScheduleAsModelObject WRITE setLatentFractionScheduleAsModelObject RESET resetLatentFractionSchedule);

  public:

  WaterUseEquipmentDefinition_Impl(const IdfObject& idfObject,
                                   Model_Impl* model,
                                   bool keepHandle);

  WaterUseEquipmentDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

  WaterUseEquipmentDefinition_Impl(const WaterUseEquipmentDefinition_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

  virtual ~WaterUseEquipmentDefinition_Impl() {}

  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const;

  std::string endUseSubcategory() const;
  bool isEndUseSubcategoryDefaulted() const;
  void setEndUseSubcategory(std::string endUseSubcategory);
  void resetEndUseSubcategory();

  double peakFlowRate() const;
  Quantity getPeakFlowRate(bool returnIP=false) const;
  bool setPeakFlowRate(double peakFlowRate);
  bool setPeakFlowRate(const Quantity& peakFlowRate);

  boost::optional<Schedule> targetTemperatureSchedule() const;
  bool setTargetTemperatureSchedule(const boost::optional<Schedule>& targetTemperatureSchedule);
  void resetTargetTemperatureSchedule();

  boost::optional<Schedule> sensibleFractionSchedule() const;
  bool setSensibleFractionSchedule(const boost::optional<Schedule>& sensibleFractionSchedule);
  void resetSensibleFractionSchedule();

  boost::optional<Schedule> latentFractionSchedule() const;
  bool setLatentFractionSchedule(const boost::optional<Schedule>& latentFractionSchedule);
  void resetLatentFractionSchedule();

  private:

  REGISTER_LOGGER("openstudio.model.WaterUseEquipmentDefinition");

  openstudio::Quantity peakFlowRate_SI() const;
  openstudio::Quantity peakFlowRate_IP() const;

  boost::optional<ModelObject> targetTemperatureScheduleAsModelObject() const;
  bool setTargetTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

  boost::optional<ModelObject> sensibleFractionScheduleAsModelObject() const;
  bool setSensibleFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

  boost::optional<ModelObject> latentFractionScheduleAsModelObject() const;
  bool setLatentFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
};

} // detail

} // model
} // openstudio

#endif // MODEL_WATERUSEEQUIPMENTDEFINITION_IMPL_HPP

