/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

  // TODO: remove (unused)
  virtual const std::vector<std::string>& outputVariableNames() const override;

  virtual IddObjectType iddObjectType() const override;

  std::string endUseSubcategory() const;
  bool isEndUseSubcategoryDefaulted() const;
  bool setEndUseSubcategory(std::string endUseSubcategory);
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

