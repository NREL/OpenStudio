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

#include "ThermalStorageCoolingPair.hpp"
#include "ThermalStorageCoolingPair_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ThermalStorage_Cooling_Pair_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ThermalStorageCoolingPair_Impl::ThermalStorageCoolingPair_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ThermalStorageCoolingPair::iddObjectType());
  }

  ThermalStorageCoolingPair_Impl::ThermalStorageCoolingPair_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ThermalStorageCoolingPair::iddObjectType());
  }

  ThermalStorageCoolingPair_Impl::ThermalStorageCoolingPair_Impl(const ThermalStorageCoolingPair_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ThermalStorageCoolingPair_Impl::outputVariableNames() const
  {
    static const std::vector<std::string> result{
      // TODO
    };
    return result;
  }

  IddObjectType ThermalStorageCoolingPair_Impl::iddObjectType() const {
    return ThermalStorageCoolingPair::iddObjectType();
  }

  boost::optional<HVACComponent> ThermalStorageCoolingPair_Impl::optionalCoolingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ThermalStorage_Cooling_PairFields::CoolingCoilName);
  }

  HVACComponent ThermalStorageCoolingPair_Impl::coolingCoil() const{
    boost::optional<HVACComponent> value = optionalCoolingCoil();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have a Cooling Coil attached.");
    }
    return value.get();
  }

  boost::optional<HVACComponent> ThermalStorageCoolingPair_Impl::optionalTank() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ThermalStorage_Cooling_PairFields::TankName);
  }

  HVACComponent ThermalStorageCoolingPair_Impl::tank() const {
    boost::optional<HVACComponent> value = optionalTank();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have a Tank attached.");
    }
    return value.get();
  }

  double ThermalStorageCoolingPair_Impl::maximumPeakOperationHours() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_Cooling_PairFields::MaximumPeakOperationHours, true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageCoolingPair_Impl::temperatureOrConcentrationChangeInTankThroughOperation() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_Cooling_PairFields::TemperatureOrConcentrationChangeInTankThroughOperation, true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ThermalStorageCoolingPair_Impl::loadType() const {
    boost::optional<std::string> value = getString(OS_ThermalStorage_Cooling_PairFields::LoadType, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ThermalStorageCoolingPair_Impl::isLoadTypeDefaulted() const {
    return isEmpty(OS_ThermalStorage_Cooling_PairFields::LoadType);
  }

  boost::optional<HVACComponent> ThermalStorageCoolingPair_Impl::optionalRecoveryUnit() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ThermalStorage_Cooling_PairFields::RecoveryUnitName);
  }

  HVACComponent ThermalStorageCoolingPair_Impl::recoveryUnit() const {
    boost::optional<HVACComponent> value = optionalRecoveryUnit();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have a Recovery Unit attached.");
    }
    return value.get();
  }

  double ThermalStorageCoolingPair_Impl::capacityRatioOfRecoveryUnitToMainCoolingCoil() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_Cooling_PairFields::CapacityRatioOfRecoveryUnitToMainCoolingCoil, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ThermalStorageCoolingPair_Impl::isCapacityRatioOfRecoveryUnitToMainCoolingCoilDefaulted() const {
    return isEmpty(OS_ThermalStorage_Cooling_PairFields::CapacityRatioOfRecoveryUnitToMainCoolingCoil);
  }

  bool ThermalStorageCoolingPair_Impl::setCoolingCoil(const HVACComponent& coolingCoil) {
    bool result = setPointer(OS_ThermalStorage_Cooling_PairFields::CoolingCoilName, coolingCoil.handle());
    return result;
  }

  bool ThermalStorageCoolingPair_Impl::setTank(const HVACComponent& tank) {
    bool result = setPointer(OS_ThermalStorage_Cooling_PairFields::TankName, tank.handle());
    return result;
  }

  bool ThermalStorageCoolingPair_Impl::setMaximumPeakOperationHours(double maximumPeakOperationHours) {
    bool result = setDouble(OS_ThermalStorage_Cooling_PairFields::MaximumPeakOperationHours, maximumPeakOperationHours);
    OS_ASSERT(result);
    return result;
  }

  bool ThermalStorageCoolingPair_Impl::setTemperatureOrConcentrationChangeInTankThroughOperation(double temperatureOrConcentrationChangeInTankThroughOperation) {
    bool result = setDouble(OS_ThermalStorage_Cooling_PairFields::TemperatureOrConcentrationChangeInTankThroughOperation, temperatureOrConcentrationChangeInTankThroughOperation);
    OS_ASSERT(result);
    return result;
  }

  bool ThermalStorageCoolingPair_Impl::setLoadType(std::string loadType) {
    bool result = setString(OS_ThermalStorage_Cooling_PairFields::LoadType, loadType);
    OS_ASSERT(result);
    return result;
  }

  void ThermalStorageCoolingPair_Impl::resetLoadType() {
    bool result = setString(OS_ThermalStorage_Cooling_PairFields::LoadType, "");
    OS_ASSERT(result);
  }

  bool ThermalStorageCoolingPair_Impl::setRecoveryUnit(const HVACComponent& recoveryUnit) {
    bool result = setPointer(OS_ThermalStorage_Cooling_PairFields::RecoveryUnitName, recoveryUnit.handle());
    return result;
  }

  bool ThermalStorageCoolingPair_Impl::setCapacityRatioOfRecoveryUnitToMainCoolingCoil(double capacityRatioOfRecoveryUnitToMainCoolingCoil) {
    bool result = setDouble(OS_ThermalStorage_Cooling_PairFields::CapacityRatioOfRecoveryUnitToMainCoolingCoil, capacityRatioOfRecoveryUnitToMainCoolingCoil);
    OS_ASSERT(result);
    return result;
  }

  void ThermalStorageCoolingPair_Impl::resetCapacityRatioOfRecoveryUnitToMainCoolingCoil() {
    bool result = setString(OS_ThermalStorage_Cooling_PairFields::CapacityRatioOfRecoveryUnitToMainCoolingCoil, "");
    OS_ASSERT(result);
  }

} // detail

ThermalStorageCoolingPair::ThermalStorageCoolingPair(const Model& model)
  : ModelObject(ThermalStorageCoolingPair::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ThermalStorageCoolingPair_Impl>());

  setMaximumPeakOperationHours(0.0); // TODO
  setTemperatureOrConcentrationChangeInTankThroughOperation(0.0); // TODO
}

IddObjectType ThermalStorageCoolingPair::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ThermalStorage_Cooling_Pair);
}

HVACComponent ThermalStorageCoolingPair::coolingCoil() const {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->coolingCoil();
}

HVACComponent ThermalStorageCoolingPair::tank() const {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->tank();
}

double ThermalStorageCoolingPair::maximumPeakOperationHours() const {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->maximumPeakOperationHours();
}

double ThermalStorageCoolingPair::temperatureOrConcentrationChangeInTankThroughOperation() const {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->temperatureOrConcentrationChangeInTankThroughOperation();
}

std::string ThermalStorageCoolingPair::loadType() const {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->loadType();
}

bool ThermalStorageCoolingPair::isLoadTypeDefaulted() const {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->isLoadTypeDefaulted();
}

HVACComponent ThermalStorageCoolingPair::recoveryUnit() const {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->recoveryUnit();
}

double ThermalStorageCoolingPair::capacityRatioOfRecoveryUnitToMainCoolingCoil() const {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->capacityRatioOfRecoveryUnitToMainCoolingCoil();
}

bool ThermalStorageCoolingPair::isCapacityRatioOfRecoveryUnitToMainCoolingCoilDefaulted() const {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->isCapacityRatioOfRecoveryUnitToMainCoolingCoilDefaulted();
}

bool ThermalStorageCoolingPair::setCoolingCoil(const HVACComponent& coolingCoil) {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->setCoolingCoil(coolingCoil);
}

bool ThermalStorageCoolingPair::setTank(const HVACComponent& tank) {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->setTank(tank);
}

bool ThermalStorageCoolingPair::setMaximumPeakOperationHours(double maximumPeakOperationHours) {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->setMaximumPeakOperationHours(maximumPeakOperationHours);
}

bool ThermalStorageCoolingPair::setTemperatureOrConcentrationChangeInTankThroughOperation(double temperatureOrConcentrationChangeInTankThroughOperation) {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->setTemperatureOrConcentrationChangeInTankThroughOperation(temperatureOrConcentrationChangeInTankThroughOperation);
}

bool ThermalStorageCoolingPair::setLoadType(std::string loadType) {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->setLoadType(loadType);
}

void ThermalStorageCoolingPair::resetLoadType() {
  getImpl<detail::ThermalStorageCoolingPair_Impl>()->resetLoadType();
}

bool ThermalStorageCoolingPair::setRecoveryUnit(const HVACComponent& recoveryUnit) {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->setRecoveryUnit(recoveryUnit);
}

bool ThermalStorageCoolingPair::setCapacityRatioOfRecoveryUnitToMainCoolingCoil(double capacityRatioOfRecoveryUnitToMainCoolingCoil) {
  return getImpl<detail::ThermalStorageCoolingPair_Impl>()->setCapacityRatioOfRecoveryUnitToMainCoolingCoil(capacityRatioOfRecoveryUnitToMainCoolingCoil);
}

void ThermalStorageCoolingPair::resetCapacityRatioOfRecoveryUnitToMainCoolingCoil() {
  getImpl<detail::ThermalStorageCoolingPair_Impl>()->resetCapacityRatioOfRecoveryUnitToMainCoolingCoil();
}

/// @cond
ThermalStorageCoolingPair::ThermalStorageCoolingPair(std::shared_ptr<detail::ThermalStorageCoolingPair_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
