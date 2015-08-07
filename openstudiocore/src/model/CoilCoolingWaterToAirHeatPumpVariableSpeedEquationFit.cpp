/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"

#include <utilities/idd/OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const IdfObject& idfObject,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : WaterToAirComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType());
  }

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType());
  }

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::iddObjectType() const {
    return CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType();
  }

  unsigned CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::airInletPort() {
    return OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirInletNodeName;
  }

  unsigned CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::airOutletPort() {
    return OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirOutletNodeName;
  }

  unsigned CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::waterInletPort() {
    return OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterInletNodeName;
  }

  unsigned CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::waterOutletPort() {
    return OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterOutletNodeName;
  }

  int CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::nominalSpeedLevel() const {
    boost::optional<int> value = getInt(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalSpeedLevel,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel,true);
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel,true);
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::ratedWaterFlowRateAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel,true);
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::nominalTimeforCondensatetoBeginLeavingtheCoil() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalTimeforCondensatetoBeginLeavingtheCoil,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::InitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::useHotGasReheat() const {
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::UseHotGasReheat,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::energyPartLoadFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyPartLoadFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Part Load Fraction Curve attached.");
    }
    return value.get();
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setNominalSpeedLevel(int nominalSpeedLevel) {
    bool result = setInt(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalSpeedLevel, nominalSpeedLevel);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel, grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setRatedAirFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedAirFlowRateAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, ratedAirFlowRateAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setRatedWaterFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedWaterFlowRateAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedWaterFlowRateAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel, ratedWaterFlowRateAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalTimeforCondensatetoBeginLeavingtheCoil, nominalTimeforCondensatetoBeginLeavingtheCoil);
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::InitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity, initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);
    return result;
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setUseHotGasReheat(bool useHotGasReheat) {
    setBooleanFieldValue(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::UseHotGasReheat, useHotGasReheat);
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setEnergyPartLoadFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::EnergyPartLoadFractionCurveName, curve.handle());
    return result;
  }

  boost::optional<Curve> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::optionalEnergyPartLoadFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::EnergyPartLoadFractionCurveName);
  }

} // detail

CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(const Model& model)
  : WaterToAirComponent(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterInletNodeName
  //     OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterOutletNodeName
  //     OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirInletNodeName
  //     OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirOutletNodeName
  //     OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::EnergyPartLoadFractionCurveName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setWatertoRefrigerantHXWaterInletNode();
  OS_ASSERT(ok);
  // ok = setWatertoRefrigerantHXWaterOutletNode();
  OS_ASSERT(ok);
  // ok = setIndoorAirInletNode();
  OS_ASSERT(ok);
  // ok = setIndoorAirOutletNode();
  OS_ASSERT(ok);
  // setNominalSpeedLevel();
  // setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
  // setRatedAirFlowRateAtSelectedNominalSpeedLevel();
  // setRatedWaterFlowRateAtSelectedNominalSpeedLevel();
  // ok = setNominalTimeforCondensatetoBeginLeavingtheCoil();
  OS_ASSERT(ok);
  // ok = setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity();
  OS_ASSERT(ok);
  // setUseHotGasReheat();
  // ok = setEnergyPartLoadFractionCurve();
  OS_ASSERT(ok);
}

IddObjectType CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit);
}

int CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::nominalSpeedLevel() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->nominalSpeedLevel();
}

boost::optional<double> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized();
}

boost::optional<double> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->ratedAirFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized();
}

boost::optional<double> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::ratedWaterFlowRateAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->ratedWaterFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized();
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::nominalTimeforCondensatetoBeginLeavingtheCoil() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->nominalTimeforCondensatetoBeginLeavingtheCoil();
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity();
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::useHotGasReheat() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->useHotGasReheat();
}

Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::energyPartLoadFractionCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->energyPartLoadFractionCurve();
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setNominalSpeedLevel(int nominalSpeedLevel) {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setNominalSpeedLevel(nominalSpeedLevel);
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(double grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel);
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setRatedAirFlowRateAtSelectedNominalSpeedLevel(ratedAirFlowRateAtSelectedNominalSpeedLevel);
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setRatedWaterFlowRateAtSelectedNominalSpeedLevel(double ratedWaterFlowRateAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setRatedWaterFlowRateAtSelectedNominalSpeedLevel(ratedWaterFlowRateAtSelectedNominalSpeedLevel);
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setNominalTimeforCondensatetoBeginLeavingtheCoil(nominalTimeforCondensatetoBeginLeavingtheCoil);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setUseHotGasReheat(bool useHotGasReheat) {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setUseHotGasReheat(useHotGasReheat);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setEnergyPartLoadFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setEnergyPartLoadFractionCurve(curve);
}

/// @cond
CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(std::shared_ptr<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl> impl)
  : WaterToAirComponent(impl)
{}
/// @endcond

} // model
} // openstudio

