/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "ThermalStorageHeatingPair.hpp"
#include "ThermalStorageHeatingPair_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ThermalStorage_Heating_Pair_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ThermalStorageHeatingPair_Impl::ThermalStorageHeatingPair_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ThermalStorageHeatingPair::iddObjectType());
  }

  ThermalStorageHeatingPair_Impl::ThermalStorageHeatingPair_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ThermalStorageHeatingPair::iddObjectType());
  }

  ThermalStorageHeatingPair_Impl::ThermalStorageHeatingPair_Impl(const ThermalStorageHeatingPair_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ThermalStorageHeatingPair_Impl::outputVariableNames() const
  {
    static const std::vector<std::string> result{
      // TODO
    };
    return result;
  }

  IddObjectType ThermalStorageHeatingPair_Impl::iddObjectType() const {
    return ThermalStorageHeatingPair::iddObjectType();
  }

  boost::optional<HVACComponent> ThermalStorageHeatingPair_Impl::optionalHeatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ThermalStorage_Heating_PairFields::HeatingCoilName);
  }

  HVACComponent ThermalStorageHeatingPair_Impl::heatingCoil() const{
    boost::optional<HVACComponent> value = optionalHeatingCoil();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have a Heating Coil attached.");
    }
    return value.get();
  }

  boost::optional<HVACComponent> ThermalStorageHeatingPair_Impl::optionalTank() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ThermalStorage_Heating_PairFields::TankName);
  }

  HVACComponent ThermalStorageHeatingPair_Impl::tank() const {
    boost::optional<HVACComponent> value = optionalTank();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have a Tank attached.");
    }
    return value.get();
  }

  double ThermalStorageHeatingPair_Impl::maximumPeakOperationHours() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_Heating_PairFields::MaximumPeakOperationHours, true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageHeatingPair_Impl::temperatureChangeInTankThroughOperation() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_Heating_PairFields::TemperatureChangeInTankThroughOperation, true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<HVACComponent> ThermalStorageHeatingPair_Impl::optionalRecoveryUnit() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ThermalStorage_Heating_PairFields::RecoveryUnitName);
  }

  HVACComponent ThermalStorageHeatingPair_Impl::recoveryUnit() const {
    boost::optional<HVACComponent> value = optionalRecoveryUnit();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have a Recovery Unit attached.");
    }
    return value.get();
  }

  double ThermalStorageHeatingPair_Impl::capacityRatioOfRecoveryUnitToMainCoolingCoil() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_Heating_PairFields::CapacityRatioOfRecoveryUnitToMainCoolingCoil, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ThermalStorageHeatingPair_Impl::isCapacityRatioOfRecoveryUnitToMainCoolingCoilDefaulted() const {
    return isEmpty(OS_ThermalStorage_Heating_PairFields::CapacityRatioOfRecoveryUnitToMainCoolingCoil);
  }

  bool ThermalStorageHeatingPair_Impl::setHeatingCoil(const HVACComponent& heatingCoil) {
    bool result = setPointer(OS_ThermalStorage_Heating_PairFields::HeatingCoilName, heatingCoil.handle());
    return result;
  }

  bool ThermalStorageHeatingPair_Impl::setTank(const HVACComponent& tank) {
    bool result = setPointer(OS_ThermalStorage_Heating_PairFields::TankName, tank.handle());
    return result;
  }

  bool ThermalStorageHeatingPair_Impl::setMaximumPeakOperationHours(double maximumPeakOperationHours) {
    bool result = setDouble(OS_ThermalStorage_Heating_PairFields::MaximumPeakOperationHours, maximumPeakOperationHours);
    OS_ASSERT(result);
    return result;
  }

  bool ThermalStorageHeatingPair_Impl::setTemperatureChangeInTankThroughOperation(double temperatureChangeInTankThroughOperation) {
    bool result = setDouble(OS_ThermalStorage_Heating_PairFields::TemperatureChangeInTankThroughOperation, temperatureChangeInTankThroughOperation);
    OS_ASSERT(result);
    return result;
  }

  bool ThermalStorageHeatingPair_Impl::setRecoveryUnit(const HVACComponent& recoveryUnit) {
    bool result = setPointer(OS_ThermalStorage_Heating_PairFields::RecoveryUnitName, recoveryUnit.handle());
    return result;
  }

  bool ThermalStorageHeatingPair_Impl::setCapacityRatioOfRecoveryUnitToMainCoolingCoil(double capacityRatioOfRecoveryUnitToMainCoolingCoil) {
    bool result = setDouble(OS_ThermalStorage_Heating_PairFields::CapacityRatioOfRecoveryUnitToMainCoolingCoil, capacityRatioOfRecoveryUnitToMainCoolingCoil);
    OS_ASSERT(result);
    return result;
  }

  void ThermalStorageHeatingPair_Impl::resetCapacityRatioOfRecoveryUnitToMainCoolingCoil() {
    bool result = setString(OS_ThermalStorage_Heating_PairFields::CapacityRatioOfRecoveryUnitToMainCoolingCoil, "");
    OS_ASSERT(result);
  }

} // detail

ThermalStorageHeatingPair::ThermalStorageHeatingPair(const Model& model)
  : ModelObject(ThermalStorageHeatingPair::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ThermalStorageHeatingPair_Impl>());

  setMaximumPeakOperationHours(0.0); // TODO
  setTemperatureChangeInTankThroughOperation(0.0); // TODO
}

IddObjectType ThermalStorageHeatingPair::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ThermalStorage_Heating_Pair);
}

HVACComponent ThermalStorageHeatingPair::heatingCoil() const {
  return getImpl<detail::ThermalStorageHeatingPair_Impl>()->heatingCoil();
}

HVACComponent ThermalStorageHeatingPair::tank() const {
  return getImpl<detail::ThermalStorageHeatingPair_Impl>()->tank();
}

double ThermalStorageHeatingPair::maximumPeakOperationHours() const {
  return getImpl<detail::ThermalStorageHeatingPair_Impl>()->maximumPeakOperationHours();
}

double ThermalStorageHeatingPair::temperatureChangeInTankThroughOperation() const {
  return getImpl<detail::ThermalStorageHeatingPair_Impl>()->temperatureChangeInTankThroughOperation();
}

HVACComponent ThermalStorageHeatingPair::recoveryUnit() const {
  return getImpl<detail::ThermalStorageHeatingPair_Impl>()->recoveryUnit();
}

double ThermalStorageHeatingPair::capacityRatioOfRecoveryUnitToMainCoolingCoil() const {
  return getImpl<detail::ThermalStorageHeatingPair_Impl>()->capacityRatioOfRecoveryUnitToMainCoolingCoil();
}

bool ThermalStorageHeatingPair::isCapacityRatioOfRecoveryUnitToMainCoolingCoilDefaulted() const {
  return getImpl<detail::ThermalStorageHeatingPair_Impl>()->isCapacityRatioOfRecoveryUnitToMainCoolingCoilDefaulted();
}

bool ThermalStorageHeatingPair::setHeatingCoil(const HVACComponent& heatingCoil) {
  return getImpl<detail::ThermalStorageHeatingPair_Impl>()->setHeatingCoil(heatingCoil);
}

bool ThermalStorageHeatingPair::setTank(const HVACComponent& tank) {
  return getImpl<detail::ThermalStorageHeatingPair_Impl>()->setTank(tank);
}

bool ThermalStorageHeatingPair::setMaximumPeakOperationHours(double maximumPeakOperationHours) {
  return getImpl<detail::ThermalStorageHeatingPair_Impl>()->setMaximumPeakOperationHours(maximumPeakOperationHours);
}

bool ThermalStorageHeatingPair::setTemperatureChangeInTankThroughOperation(double temperatureChangeInTankThroughOperation) {
  return getImpl<detail::ThermalStorageHeatingPair_Impl>()->setTemperatureChangeInTankThroughOperation(temperatureChangeInTankThroughOperation);
}

bool ThermalStorageHeatingPair::setRecoveryUnit(const HVACComponent& recoveryUnit) {
  return getImpl<detail::ThermalStorageHeatingPair_Impl>()->setRecoveryUnit(recoveryUnit);
}

bool ThermalStorageHeatingPair::setCapacityRatioOfRecoveryUnitToMainCoolingCoil(double capacityRatioOfRecoveryUnitToMainCoolingCoil) {
  return getImpl<detail::ThermalStorageHeatingPair_Impl>()->setCapacityRatioOfRecoveryUnitToMainCoolingCoil(capacityRatioOfRecoveryUnitToMainCoolingCoil);
}

void ThermalStorageHeatingPair::resetCapacityRatioOfRecoveryUnitToMainCoolingCoil() {
  getImpl<detail::ThermalStorageHeatingPair_Impl>()->resetCapacityRatioOfRecoveryUnitToMainCoolingCoil();
}

/// @cond
ThermalStorageHeatingPair::ThermalStorageHeatingPair(std::shared_ptr<detail::ThermalStorageHeatingPair_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
