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

#include "CoilHeatingDXVariableSpeed.hpp"
#include "CoilHeatingDXVariableSpeed_Impl.hpp"

#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Coil_Heating_DX_VariableSpeed_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingDXVariableSpeed_Impl::CoilHeatingDXVariableSpeed_Impl(const IdfObject& idfObject,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingDXVariableSpeed::iddObjectType());
  }

  CoilHeatingDXVariableSpeed_Impl::CoilHeatingDXVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingDXVariableSpeed::iddObjectType());
  }

  CoilHeatingDXVariableSpeed_Impl::CoilHeatingDXVariableSpeed_Impl(const CoilHeatingDXVariableSpeed_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingDXVariableSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingDXVariableSpeed_Impl::iddObjectType() const {
    return CoilHeatingDXVariableSpeed::iddObjectType();
  }

  unsigned CoilHeatingDXVariableSpeed_Impl::inletPort() {
    return OS_Coil_Heating_DX_VariableSpeedFields::IndoorAirInletNodeName;
  }

  unsigned CoilHeatingDXVariableSpeed_Impl::outletPort() {
    return OS_Coil_Heating_DX_VariableSpeedFields::IndoorAirOutletNodeName;
  }

  int CoilHeatingDXVariableSpeed_Impl::nominalSpeedLevel() const {
    boost::optional<int> value = getInt(OS_Coil_Heating_DX_VariableSpeedFields::NominalSpeedLevel,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingDXVariableSpeed_Impl::ratedHeatingCapacityAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Heating_DX_VariableSpeedFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel,true);
  }

  bool CoilHeatingDXVariableSpeed_Impl::isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_VariableSpeedFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilHeatingDXVariableSpeed_Impl::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Heating_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel,true);
  }

  bool CoilHeatingDXVariableSpeed_Impl::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  CurveQuadratic CoilHeatingDXVariableSpeed_Impl::energyPartLoadFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalEnergyPartLoadFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Part Load Fraction Curve attached.");
    }
    return value.get();
  }

  boost::optional<CurveBiquadratic> CoilHeatingDXVariableSpeed_Impl::defrostEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_Heating_DX_VariableSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName);
  }

  double CoilHeatingDXVariableSpeed_Impl::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_VariableSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingDXVariableSpeed_Impl::outdoorDryBulbTemperaturetoTurnOnCompressor() const {
    return getDouble(OS_Coil_Heating_DX_VariableSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor,true);
  }

  double CoilHeatingDXVariableSpeed_Impl::maximumOutdoorDryBulbTemperatureforDefrostOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingDXVariableSpeed_Impl::crankcaseHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_VariableSpeedFields::CrankcaseHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingDXVariableSpeed_Impl::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string CoilHeatingDXVariableSpeed_Impl::defrostStrategy() const {
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_VariableSpeedFields::DefrostStrategy,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string CoilHeatingDXVariableSpeed_Impl::defrostControl() const {
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_VariableSpeedFields::DefrostControl,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingDXVariableSpeed_Impl::defrostTimePeriodFraction() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_VariableSpeedFields::DefrostTimePeriodFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingDXVariableSpeed_Impl::resistiveDefrostHeaterCapacity() const {
    return getDouble(OS_Coil_Heating_DX_VariableSpeedFields::ResistiveDefrostHeaterCapacity,true);
  }

  bool CoilHeatingDXVariableSpeed_Impl::isResistiveDefrostHeaterCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_VariableSpeedFields::ResistiveDefrostHeaterCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  void CoilHeatingDXVariableSpeed_Impl::setNominalSpeedLevel(int nominalSpeedLevel) {
    bool result = setInt(OS_Coil_Heating_DX_VariableSpeedFields::NominalSpeedLevel, nominalSpeedLevel);
    OS_ASSERT(result);
  }

  void CoilHeatingDXVariableSpeed_Impl::setRatedHeatingCapacityAtSelectedNominalSpeedLevel(boost::optional<double> ratedHeatingCapacityAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedHeatingCapacityAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel, ratedHeatingCapacityAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilHeatingDXVariableSpeed_Impl::autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  void CoilHeatingDXVariableSpeed_Impl::setRatedAirFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedAirFlowRateAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, ratedAirFlowRateAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilHeatingDXVariableSpeed_Impl::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXVariableSpeed_Impl::setEnergyPartLoadFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Heating_DX_VariableSpeedFields::EnergyPartLoadFractionCurveName, curveQuadratic.handle());
    return result;
  }

  bool CoilHeatingDXVariableSpeed_Impl::setDefrostEnergyInputRatioFunctionofTemperatureCurve(const boost::optional<CurveBiquadratic>& curveBiquadratic) {
    bool result(false);
    if (curveBiquadratic) {
      result = setPointer(OS_Coil_Heating_DX_VariableSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName, curveBiquadratic.get().handle());
    }
    else {
      resetDefrostEnergyInputRatioFunctionofTemperatureCurve();
      result = true;
    }
    return result;
  }

  void CoilHeatingDXVariableSpeed_Impl::resetDefrostEnergyInputRatioFunctionofTemperatureCurve() {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXVariableSpeed_Impl::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
    bool result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, minimumOutdoorDryBulbTemperatureforCompressorOperation);
    return result;
  }

  void CoilHeatingDXVariableSpeed_Impl::setOutdoorDryBulbTemperaturetoTurnOnCompressor(boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor) {
    bool result(false);
    if (outdoorDryBulbTemperaturetoTurnOnCompressor) {
      result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor, outdoorDryBulbTemperaturetoTurnOnCompressor.get());
    }
    else {
      resetOutdoorDryBulbTemperaturetoTurnOnCompressor();
      result = true;
    }
    OS_ASSERT(result);
  }

  void CoilHeatingDXVariableSpeed_Impl::resetOutdoorDryBulbTemperaturetoTurnOnCompressor() {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXVariableSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation) {
    bool result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation, maximumOutdoorDryBulbTemperatureforDefrostOperation);
    return result;
  }

  bool CoilHeatingDXVariableSpeed_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    bool result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
    return result;
  }

  bool CoilHeatingDXVariableSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
    bool result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
    return result;
  }

  bool CoilHeatingDXVariableSpeed_Impl::setDefrostStrategy(std::string defrostStrategy) {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::DefrostStrategy, defrostStrategy);
    return result;
  }

  bool CoilHeatingDXVariableSpeed_Impl::setDefrostControl(std::string defrostControl) {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::DefrostControl, defrostControl);
    return result;
  }

  bool CoilHeatingDXVariableSpeed_Impl::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
    bool result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::DefrostTimePeriodFraction, defrostTimePeriodFraction);
    return result;
  }

  bool CoilHeatingDXVariableSpeed_Impl::setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity) {
    bool result(false);
    if (resistiveDefrostHeaterCapacity) {
      result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::ResistiveDefrostHeaterCapacity, resistiveDefrostHeaterCapacity.get());
    }
    return result;
  }

  void CoilHeatingDXVariableSpeed_Impl::autosizeResistiveDefrostHeaterCapacity() {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::ResistiveDefrostHeaterCapacity, "autosize");
    OS_ASSERT(result);
  }

  boost::optional<CurveQuadratic> CoilHeatingDXVariableSpeed_Impl::optionalEnergyPartLoadFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Heating_DX_VariableSpeedFields::EnergyPartLoadFractionCurveName);
  }

} // detail

CoilHeatingDXVariableSpeed::CoilHeatingDXVariableSpeed(const Model& model)
  : StraightComponent(CoilHeatingDXVariableSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingDXVariableSpeed_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Coil_Heating_DX_VariableSpeedFields::IndoorAirInletNodeName
  //     OS_Coil_Heating_DX_VariableSpeedFields::IndoorAirOutletNodeName
  //     OS_Coil_Heating_DX_VariableSpeedFields::EnergyPartLoadFractionCurveName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setIndoorAirInletNode();
  OS_ASSERT(ok);
  // ok = setIndoorAirOutletNode();
  OS_ASSERT(ok);
  // setNominalSpeedLevel();
  // setRatedHeatingCapacityAtSelectedNominalSpeedLevel();
  // setRatedAirFlowRateAtSelectedNominalSpeedLevel();
  // ok = setEnergyPartLoadFractionCurve();
  OS_ASSERT(ok);
  // ok = setMinimumOutdoorDryBulbTemperatureforCompressorOperation();
  OS_ASSERT(ok);
  // ok = setMaximumOutdoorDryBulbTemperatureforDefrostOperation();
  OS_ASSERT(ok);
  // ok = setCrankcaseHeaterCapacity();
  OS_ASSERT(ok);
  // ok = setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();
  OS_ASSERT(ok);
  // ok = setDefrostStrategy();
  OS_ASSERT(ok);
  // ok = setDefrostControl();
  OS_ASSERT(ok);
  // ok = setDefrostTimePeriodFraction();
  OS_ASSERT(ok);
  // ok = setResistiveDefrostHeaterCapacity();
  OS_ASSERT(ok);
}

IddObjectType CoilHeatingDXVariableSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_DX_VariableSpeed);
}

std::vector<std::string> CoilHeatingDXVariableSpeed::defrostStrategyValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Heating_DX_VariableSpeedFields::DefrostStrategy);
}

std::vector<std::string> CoilHeatingDXVariableSpeed::defrostControlValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Heating_DX_VariableSpeedFields::DefrostControl);
}

int CoilHeatingDXVariableSpeed::nominalSpeedLevel() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->nominalSpeedLevel();
}

boost::optional<double> CoilHeatingDXVariableSpeed::ratedHeatingCapacityAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->ratedHeatingCapacityAtSelectedNominalSpeedLevel();
}

bool CoilHeatingDXVariableSpeed::isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized();
}

boost::optional<double> CoilHeatingDXVariableSpeed::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->ratedAirFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilHeatingDXVariableSpeed::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized();
}

CurveQuadratic CoilHeatingDXVariableSpeed::energyPartLoadFractionCurve() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->energyPartLoadFractionCurve();
}

boost::optional<CurveBiquadratic> CoilHeatingDXVariableSpeed::defrostEnergyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->defrostEnergyInputRatioFunctionofTemperatureCurve();
}

double CoilHeatingDXVariableSpeed::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->minimumOutdoorDryBulbTemperatureforCompressorOperation();
}

boost::optional<double> CoilHeatingDXVariableSpeed::outdoorDryBulbTemperaturetoTurnOnCompressor() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->outdoorDryBulbTemperaturetoTurnOnCompressor();
}

double CoilHeatingDXVariableSpeed::maximumOutdoorDryBulbTemperatureforDefrostOperation() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforDefrostOperation();
}

double CoilHeatingDXVariableSpeed::crankcaseHeaterCapacity() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->crankcaseHeaterCapacity();
}

double CoilHeatingDXVariableSpeed::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();
}

std::string CoilHeatingDXVariableSpeed::defrostStrategy() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->defrostStrategy();
}

std::string CoilHeatingDXVariableSpeed::defrostControl() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->defrostControl();
}

double CoilHeatingDXVariableSpeed::defrostTimePeriodFraction() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->defrostTimePeriodFraction();
}

boost::optional<double> CoilHeatingDXVariableSpeed::resistiveDefrostHeaterCapacity() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->resistiveDefrostHeaterCapacity();
}

bool CoilHeatingDXVariableSpeed::isResistiveDefrostHeaterCapacityAutosized() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->isResistiveDefrostHeaterCapacityAutosized();
}

void CoilHeatingDXVariableSpeed::setNominalSpeedLevel(int nominalSpeedLevel) {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setNominalSpeedLevel(nominalSpeedLevel);
}

void CoilHeatingDXVariableSpeed::setRatedHeatingCapacityAtSelectedNominalSpeedLevel(double ratedHeatingCapacityAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setRatedHeatingCapacityAtSelectedNominalSpeedLevel(ratedHeatingCapacityAtSelectedNominalSpeedLevel);
}

void CoilHeatingDXVariableSpeed::autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();
}

void CoilHeatingDXVariableSpeed::setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setRatedAirFlowRateAtSelectedNominalSpeedLevel(ratedAirFlowRateAtSelectedNominalSpeedLevel);
}

void CoilHeatingDXVariableSpeed::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilHeatingDXVariableSpeed::setEnergyPartLoadFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setEnergyPartLoadFractionCurve(curveQuadratic);
}

bool CoilHeatingDXVariableSpeed::setDefrostEnergyInputRatioFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setDefrostEnergyInputRatioFunctionofTemperatureCurve(curveBiquadratic);
}

void CoilHeatingDXVariableSpeed::resetDefrostEnergyInputRatioFunctionofTemperatureCurve() {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->resetDefrostEnergyInputRatioFunctionofTemperatureCurve();
}

bool CoilHeatingDXVariableSpeed::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setMinimumOutdoorDryBulbTemperatureforCompressorOperation(minimumOutdoorDryBulbTemperatureforCompressorOperation);
}

void CoilHeatingDXVariableSpeed::setOutdoorDryBulbTemperaturetoTurnOnCompressor(double outdoorDryBulbTemperaturetoTurnOnCompressor) {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setOutdoorDryBulbTemperaturetoTurnOnCompressor(outdoorDryBulbTemperaturetoTurnOnCompressor);
}

void CoilHeatingDXVariableSpeed::resetOutdoorDryBulbTemperaturetoTurnOnCompressor() {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->resetOutdoorDryBulbTemperaturetoTurnOnCompressor();
}

bool CoilHeatingDXVariableSpeed::setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforDefrostOperation(maximumOutdoorDryBulbTemperatureforDefrostOperation);
}

bool CoilHeatingDXVariableSpeed::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
}

bool CoilHeatingDXVariableSpeed::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
}

bool CoilHeatingDXVariableSpeed::setDefrostStrategy(std::string defrostStrategy) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setDefrostStrategy(defrostStrategy);
}

bool CoilHeatingDXVariableSpeed::setDefrostControl(std::string defrostControl) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setDefrostControl(defrostControl);
}

bool CoilHeatingDXVariableSpeed::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setDefrostTimePeriodFraction(defrostTimePeriodFraction);
}

bool CoilHeatingDXVariableSpeed::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setResistiveDefrostHeaterCapacity(resistiveDefrostHeaterCapacity);
}

void CoilHeatingDXVariableSpeed::autosizeResistiveDefrostHeaterCapacity() {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->autosizeResistiveDefrostHeaterCapacity();
}

/// @cond
CoilHeatingDXVariableSpeed::CoilHeatingDXVariableSpeed(std::shared_ptr<detail::CoilHeatingDXVariableSpeed_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

