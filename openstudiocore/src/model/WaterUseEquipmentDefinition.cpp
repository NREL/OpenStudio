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

#include "WaterUseEquipmentDefinition.hpp"
#include "WaterUseEquipmentDefinition_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include <utilities/idd/OS_WaterUse_Equipment_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

  WaterUseEquipmentDefinition_Impl::WaterUseEquipmentDefinition_Impl(const IdfObject& idfObject,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : SpaceLoadDefinition_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == WaterUseEquipmentDefinition::iddObjectType());
  }

  WaterUseEquipmentDefinition_Impl::WaterUseEquipmentDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == WaterUseEquipmentDefinition::iddObjectType());
  }

  WaterUseEquipmentDefinition_Impl::WaterUseEquipmentDefinition_Impl(const WaterUseEquipmentDefinition_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& WaterUseEquipmentDefinition_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType WaterUseEquipmentDefinition_Impl::iddObjectType() const {
    return WaterUseEquipmentDefinition::iddObjectType();
  }

  std::string WaterUseEquipmentDefinition_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_WaterUse_Equipment_DefinitionFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterUseEquipmentDefinition_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_WaterUse_Equipment_DefinitionFields::EndUseSubcategory);
  }

  double WaterUseEquipmentDefinition_Impl::peakFlowRate() const {
    boost::optional<double> value = getDouble(OS_WaterUse_Equipment_DefinitionFields::PeakFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity WaterUseEquipmentDefinition_Impl::getPeakFlowRate(bool returnIP) const {
    OptionalDouble value = peakFlowRate();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WaterUse_Equipment_DefinitionFields::PeakFlowRate, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  boost::optional<Schedule> WaterUseEquipmentDefinition_Impl::targetTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterUse_Equipment_DefinitionFields::TargetTemperatureScheduleName);
  }

  boost::optional<Schedule> WaterUseEquipmentDefinition_Impl::sensibleFractionSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterUse_Equipment_DefinitionFields::SensibleFractionScheduleName);
  }

  boost::optional<Schedule> WaterUseEquipmentDefinition_Impl::latentFractionSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterUse_Equipment_DefinitionFields::LatentFractionScheduleName);
  }

  void WaterUseEquipmentDefinition_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_WaterUse_Equipment_DefinitionFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void WaterUseEquipmentDefinition_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_WaterUse_Equipment_DefinitionFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  bool WaterUseEquipmentDefinition_Impl::setPeakFlowRate(double peakFlowRate) {
    bool result = setDouble(OS_WaterUse_Equipment_DefinitionFields::PeakFlowRate, peakFlowRate);
    return result;
  }

  bool WaterUseEquipmentDefinition_Impl::setPeakFlowRate(const Quantity& peakFlowRate) {
    OptionalDouble value = getDoubleFromQuantity(OS_WaterUse_Equipment_DefinitionFields::PeakFlowRate,peakFlowRate);
    if (!value) {
      return false;
    }
    return setPeakFlowRate(value.get());
  }

  bool WaterUseEquipmentDefinition_Impl::setTargetTemperatureSchedule(const boost::optional<Schedule>& targetTemperatureSchedule) {
    bool result = false;
    if (targetTemperatureSchedule) {
      result = setPointer(OS_WaterUse_Equipment_DefinitionFields::TargetTemperatureScheduleName, targetTemperatureSchedule.get().handle());
    } else {
      result = setString(OS_WaterUse_Equipment_DefinitionFields::TargetTemperatureScheduleName, "");
    }
    return result;
  }

  void WaterUseEquipmentDefinition_Impl::resetTargetTemperatureSchedule() {
    bool result = setString(OS_WaterUse_Equipment_DefinitionFields::TargetTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterUseEquipmentDefinition_Impl::setSensibleFractionSchedule(const boost::optional<Schedule>& sensibleFractionSchedule) {
    bool result = false;
    if (sensibleFractionSchedule) {
      result = setPointer(OS_WaterUse_Equipment_DefinitionFields::SensibleFractionScheduleName, sensibleFractionSchedule.get().handle());
    } else {
      result = setString(OS_WaterUse_Equipment_DefinitionFields::SensibleFractionScheduleName, "");
    }
    return result;
  }

  void WaterUseEquipmentDefinition_Impl::resetSensibleFractionSchedule() {
    bool result = setString(OS_WaterUse_Equipment_DefinitionFields::SensibleFractionScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterUseEquipmentDefinition_Impl::setLatentFractionSchedule(const boost::optional<Schedule>& latentFractionSchedule) {
    bool result = false;
    if (latentFractionSchedule) {
      result = setPointer(OS_WaterUse_Equipment_DefinitionFields::LatentFractionScheduleName, latentFractionSchedule.get().handle());
    } else {
      result = setString(OS_WaterUse_Equipment_DefinitionFields::LatentFractionScheduleName, "");
    }
    return result;
  }

  void WaterUseEquipmentDefinition_Impl::resetLatentFractionSchedule() {
    bool result = setString(OS_WaterUse_Equipment_DefinitionFields::LatentFractionScheduleName, "");
    OS_ASSERT(result);
  }

  openstudio::Quantity WaterUseEquipmentDefinition_Impl::peakFlowRate_SI() const {
    return getPeakFlowRate(false);
  }

  openstudio::Quantity WaterUseEquipmentDefinition_Impl::peakFlowRate_IP() const {
    return getPeakFlowRate(true);
  }

  boost::optional<ModelObject> WaterUseEquipmentDefinition_Impl::targetTemperatureScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = targetTemperatureSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> WaterUseEquipmentDefinition_Impl::sensibleFractionScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = sensibleFractionSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> WaterUseEquipmentDefinition_Impl::latentFractionScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = latentFractionSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool WaterUseEquipmentDefinition_Impl::setTargetTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        return setTargetTemperatureSchedule(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetTargetTemperatureSchedule();
    }
    return true;
  }

  bool WaterUseEquipmentDefinition_Impl::setSensibleFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        return setSensibleFractionSchedule(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetSensibleFractionSchedule();
    }
    return true;
  }

  bool WaterUseEquipmentDefinition_Impl::setLatentFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        return setLatentFractionSchedule(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetLatentFractionSchedule();
    }
    return true;
  }

} // detail

WaterUseEquipmentDefinition::WaterUseEquipmentDefinition(const Model& model)
  : SpaceLoadDefinition(WaterUseEquipmentDefinition::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::WaterUseEquipmentDefinition_Impl>());

  setPeakFlowRate(0.0);
}

IddObjectType WaterUseEquipmentDefinition::iddObjectType() {
  return IddObjectType(IddObjectType::OS_WaterUse_Equipment_Definition);
}

std::string WaterUseEquipmentDefinition::endUseSubcategory() const {
  return getImpl<detail::WaterUseEquipmentDefinition_Impl>()->endUseSubcategory();
}

bool WaterUseEquipmentDefinition::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::WaterUseEquipmentDefinition_Impl>()->isEndUseSubcategoryDefaulted();
}

double WaterUseEquipmentDefinition::peakFlowRate() const {
  return getImpl<detail::WaterUseEquipmentDefinition_Impl>()->peakFlowRate();
}

Quantity WaterUseEquipmentDefinition::getPeakFlowRate(bool returnIP) const {
  return getImpl<detail::WaterUseEquipmentDefinition_Impl>()->getPeakFlowRate(returnIP);
}

boost::optional<Schedule> WaterUseEquipmentDefinition::targetTemperatureSchedule() const {
  return getImpl<detail::WaterUseEquipmentDefinition_Impl>()->targetTemperatureSchedule();
}

boost::optional<Schedule> WaterUseEquipmentDefinition::sensibleFractionSchedule() const {
  return getImpl<detail::WaterUseEquipmentDefinition_Impl>()->sensibleFractionSchedule();
}

boost::optional<Schedule> WaterUseEquipmentDefinition::latentFractionSchedule() const {
  return getImpl<detail::WaterUseEquipmentDefinition_Impl>()->latentFractionSchedule();
}

void WaterUseEquipmentDefinition::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::WaterUseEquipmentDefinition_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void WaterUseEquipmentDefinition::resetEndUseSubcategory() {
  getImpl<detail::WaterUseEquipmentDefinition_Impl>()->resetEndUseSubcategory();
}

bool WaterUseEquipmentDefinition::setPeakFlowRate(double peakFlowRate) {
  return getImpl<detail::WaterUseEquipmentDefinition_Impl>()->setPeakFlowRate(peakFlowRate);
}

bool WaterUseEquipmentDefinition::setPeakFlowRate(const Quantity& peakFlowRate) {
  return getImpl<detail::WaterUseEquipmentDefinition_Impl>()->setPeakFlowRate(peakFlowRate);
}

bool WaterUseEquipmentDefinition::setTargetTemperatureSchedule(const Schedule& targetTemperatureSchedule) {
  return getImpl<detail::WaterUseEquipmentDefinition_Impl>()->setTargetTemperatureSchedule(targetTemperatureSchedule);
}

void WaterUseEquipmentDefinition::resetTargetTemperatureSchedule() {
  getImpl<detail::WaterUseEquipmentDefinition_Impl>()->resetTargetTemperatureSchedule();
}

bool WaterUseEquipmentDefinition::setSensibleFractionSchedule(const Schedule& sensibleFractionSchedule) {
  return getImpl<detail::WaterUseEquipmentDefinition_Impl>()->setSensibleFractionSchedule(sensibleFractionSchedule);
}

void WaterUseEquipmentDefinition::resetSensibleFractionSchedule() {
  getImpl<detail::WaterUseEquipmentDefinition_Impl>()->resetSensibleFractionSchedule();
}

bool WaterUseEquipmentDefinition::setLatentFractionSchedule(const Schedule& latentFractionSchedule) {
  return getImpl<detail::WaterUseEquipmentDefinition_Impl>()->setLatentFractionSchedule(latentFractionSchedule);
}

void WaterUseEquipmentDefinition::resetLatentFractionSchedule() {
  getImpl<detail::WaterUseEquipmentDefinition_Impl>()->resetLatentFractionSchedule();
}

/// @cond
WaterUseEquipmentDefinition::WaterUseEquipmentDefinition(std::shared_ptr<detail::WaterUseEquipmentDefinition_Impl> impl)
  : SpaceLoadDefinition(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

