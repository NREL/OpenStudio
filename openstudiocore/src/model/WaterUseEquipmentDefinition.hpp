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

  static IddObjectType iddObjectType();

  std::string endUseSubcategory() const;
  bool isEndUseSubcategoryDefaulted() const;
  bool setEndUseSubcategory(std::string endUseSubcategory);
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

