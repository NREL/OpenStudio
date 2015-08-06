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

#include "CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl.hpp"

#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"

#include <utilities/idd/OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const IdfObject& idfObject,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : WaterToAirComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType());
  }

  CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType());
  }

  CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::iddObjectType() const {
    return CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType();
  }

  unsigned CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::airInletPort() {
    return OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirInletNodeName;
  }

  unsigned CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::airOutletPort() {
    return OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirOutletNodeName;
  }

  unsigned CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::waterInletPort() {
    return OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterInletNodeName;
  }

  unsigned CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::waterOutletPort() {
    return OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterOutletNodeName;
  }

  int CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::nominalSpeedLevel() const {
    boost::optional<int> value = getInt(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalSpeedLevel,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::ratedHeatingCapacityAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel,true);
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel,true);
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::ratedWaterFlowRateAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel,true);
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  CurveQuadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::energyPartLoadFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalEnergyPartLoadFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Part Load Fraction Curve attached.");
    }
    return value.get();
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setNominalSpeedLevel(int nominalSpeedLevel) {
    bool result = setInt(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalSpeedLevel, nominalSpeedLevel);
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setRatedHeatingCapacityAtSelectedNominalSpeedLevel(boost::optional<double> ratedHeatingCapacityAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedHeatingCapacityAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel, ratedHeatingCapacityAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setRatedAirFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedAirFlowRateAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, ratedAirFlowRateAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setRatedWaterFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedWaterFlowRateAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedWaterFlowRateAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel, ratedWaterFlowRateAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setEnergyPartLoadFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::EnergyPartLoadFractionCurveName, curveQuadratic.handle());
    return result;
  }

  boost::optional<CurveQuadratic> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::optionalEnergyPartLoadFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::EnergyPartLoadFractionCurveName);
  }

} // detail

CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit(const Model& model)
  : WaterToAirComponent(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterInletNodeName
  //     OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterOutletNodeName
  //     OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirInletNodeName
  //     OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirOutletNodeName
  //     OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::EnergyPartLoadFractionCurveName
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
  // setRatedHeatingCapacityAtSelectedNominalSpeedLevel();
  // setRatedAirFlowRateAtSelectedNominalSpeedLevel();
  // setRatedWaterFlowRateAtSelectedNominalSpeedLevel();
  // ok = setEnergyPartLoadFractionCurve();
  OS_ASSERT(ok);
}

IddObjectType CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit);
}

int CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::nominalSpeedLevel() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->nominalSpeedLevel();
}

boost::optional<double> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::ratedHeatingCapacityAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->ratedHeatingCapacityAtSelectedNominalSpeedLevel();
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized();
}

boost::optional<double> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->ratedAirFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized();
}

boost::optional<double> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::ratedWaterFlowRateAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->ratedWaterFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized();
}

CurveQuadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::energyPartLoadFractionCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->energyPartLoadFractionCurve();
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::setNominalSpeedLevel(int nominalSpeedLevel) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setNominalSpeedLevel(nominalSpeedLevel);
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::setRatedHeatingCapacityAtSelectedNominalSpeedLevel(double ratedHeatingCapacityAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setRatedHeatingCapacityAtSelectedNominalSpeedLevel(ratedHeatingCapacityAtSelectedNominalSpeedLevel);
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setRatedAirFlowRateAtSelectedNominalSpeedLevel(ratedAirFlowRateAtSelectedNominalSpeedLevel);
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::setRatedWaterFlowRateAtSelectedNominalSpeedLevel(double ratedWaterFlowRateAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setRatedWaterFlowRateAtSelectedNominalSpeedLevel(ratedWaterFlowRateAtSelectedNominalSpeedLevel);
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::setEnergyPartLoadFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setEnergyPartLoadFractionCurve(curveQuadratic);
}

/// @cond
CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit(std::shared_ptr<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl> impl)
  : WaterToAirComponent(impl)
{}
/// @endcond

} // model
} // openstudio

