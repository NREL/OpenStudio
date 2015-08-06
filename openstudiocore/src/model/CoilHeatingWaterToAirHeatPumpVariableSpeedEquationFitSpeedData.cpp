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

#include "CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl.hpp"

#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"

#include <utilities/idd/OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedData_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const IdfObject& idfObject,
                                                                                                                                           Model_Impl* model,
                                                                                                                                           bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType());
  }

  CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                                           Model_Impl* model,
                                                                                                                                           bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType());
  }

  CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl& other,
                                                                                                                                           Model_Impl* model,
                                                                                                                                           bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::iddObjectType() const {
    return CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType();
  }

  double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitGrossRatedHeatingCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedHeatingCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitGrossRatedHeatingCOP() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedHeatingCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitRatedAirFlow() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedAirFlow,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitRatedWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  CurveBiquadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::heatingCapacityFunctionofTemperatureCurve() const {
    boost::optional<CurveBiquadratic> value = optionalHeatingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::totalHeatingCapacityFunctionofAirFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalTotalHeatingCapacityFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Heating Capacity Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::heatingCapacityFunctionofWaterFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalHeatingCapacityFunctionofWaterFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Water Flow Fraction Curve attached.");
    }
    return value.get();
  }

  CurveBiquadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<CurveBiquadratic> value = optionalEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofAirFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalEnergyInputRatioFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofWaterFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalEnergyInputRatioFunctionofWaterFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Water Flow Fraction Curve attached.");
    }
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions,true);
    OS_ASSERT(value);
    return value.get();
  }

  CurveBiquadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::wasteHeatFunctionofTemperatureCurve() const {
    boost::optional<CurveBiquadratic> value = optionalWasteHeatFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Waste Heat Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitGrossRatedHeatingCapacity(double referenceUnitGrossRatedHeatingCapacity) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedHeatingCapacity, referenceUnitGrossRatedHeatingCapacity);
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitGrossRatedHeatingCOP(double referenceUnitGrossRatedHeatingCOP) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedHeatingCOP, referenceUnitGrossRatedHeatingCOP);
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitRatedAirFlow(double referenceUnitRatedAirFlow) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedAirFlow, referenceUnitRatedAirFlow);
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedWaterFlowRate, referenceUnitRatedWaterFlowRate);
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setHeatingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::HeatingCapacityFunctionofTemperatureCurveName, curveBiquadratic.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalHeatingCapacityFunctionofAirFlowFractionCurveName, curveQuadratic.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setHeatingCapacityFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::HeatingCapacityFunctionofWaterFlowFractionCurveName, curveQuadratic.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName, curveBiquadratic.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName, curveQuadratic.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofWaterFlowFractionCurveName, curveQuadratic.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions, referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setWasteHeatFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::WasteHeatFunctionofTemperatureCurveName, curveBiquadratic.handle());
    return result;
  }

  boost::optional<CurveBiquadratic> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalHeatingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::HeatingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<CurveQuadratic> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalTotalHeatingCapacityFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalHeatingCapacityFunctionofAirFlowFractionCurveName);
  }

  boost::optional<CurveQuadratic> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalHeatingCapacityFunctionofWaterFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::HeatingCapacityFunctionofWaterFlowFractionCurveName);
  }

  boost::optional<CurveBiquadratic> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<CurveQuadratic> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName);
  }

  boost::optional<CurveQuadratic> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofWaterFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofWaterFlowFractionCurveName);
  }

  boost::optional<CurveBiquadratic> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalWasteHeatFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::WasteHeatFunctionofTemperatureCurveName);
  }

} // detail

CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(const Model& model)
  : ParentObject(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::HeatingCapacityFunctionofTemperatureCurveName
  //     OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalHeatingCapacityFunctionofAirFlowFractionCurveName
  //     OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::HeatingCapacityFunctionofWaterFlowFractionCurveName
  //     OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName
  //     OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName
  //     OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofWaterFlowFractionCurveName
  //     OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::WasteHeatFunctionofTemperatureCurveName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setReferenceUnitGrossRatedHeatingCapacity();
  OS_ASSERT(ok);
  // ok = setReferenceUnitGrossRatedHeatingCOP();
  OS_ASSERT(ok);
  // ok = setReferenceUnitRatedAirFlow();
  OS_ASSERT(ok);
  // ok = setReferenceUnitRatedWaterFlowRate();
  OS_ASSERT(ok);
  // ok = setHeatingCapacityFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setTotalHeatingCapacityFunctionofAirFlowFractionCurve();
  OS_ASSERT(ok);
  // ok = setHeatingCapacityFunctionofWaterFlowFractionCurve();
  OS_ASSERT(ok);
  // ok = setEnergyInputRatioFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setEnergyInputRatioFunctionofAirFlowFractionCurve();
  OS_ASSERT(ok);
  // ok = setEnergyInputRatioFunctionofWaterFlowFractionCurve();
  OS_ASSERT(ok);
  // ok = setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions();
  OS_ASSERT(ok);
  // ok = setWasteHeatFunctionofTemperatureCurve();
  OS_ASSERT(ok);
}

IddObjectType CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedData);
}

double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitGrossRatedHeatingCapacity() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitGrossRatedHeatingCapacity();
}

double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitGrossRatedHeatingCOP() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitGrossRatedHeatingCOP();
}

double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitRatedAirFlow() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitRatedAirFlow();
}

double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitRatedWaterFlowRate() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitRatedWaterFlowRate();
}

CurveBiquadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::heatingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->heatingCapacityFunctionofTemperatureCurve();
}

CurveQuadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::totalHeatingCapacityFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->totalHeatingCapacityFunctionofAirFlowFractionCurve();
}

CurveQuadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::heatingCapacityFunctionofWaterFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->heatingCapacityFunctionofWaterFlowFractionCurve();
}

CurveBiquadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofTemperatureCurve();
}

CurveQuadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofAirFlowFractionCurve();
}

CurveQuadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofWaterFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofWaterFlowFractionCurve();
}

double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitWasteHeatFractionofInputPowerAtRatedConditions();
}

CurveBiquadratic CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::wasteHeatFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->wasteHeatFunctionofTemperatureCurve();
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitGrossRatedHeatingCapacity(double referenceUnitGrossRatedHeatingCapacity) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitGrossRatedHeatingCapacity(referenceUnitGrossRatedHeatingCapacity);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitGrossRatedHeatingCOP(double referenceUnitGrossRatedHeatingCOP) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitGrossRatedHeatingCOP(referenceUnitGrossRatedHeatingCOP);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitRatedAirFlow(double referenceUnitRatedAirFlow) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitRatedAirFlow(referenceUnitRatedAirFlow);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitRatedWaterFlowRate(referenceUnitRatedWaterFlowRate);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setHeatingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setHeatingCapacityFunctionofTemperatureCurve(curveBiquadratic);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setTotalHeatingCapacityFunctionofAirFlowFractionCurve(curveQuadratic);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setHeatingCapacityFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setHeatingCapacityFunctionofWaterFlowFractionCurve(curveQuadratic);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve(curveBiquadratic);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofAirFlowFractionCurve(curveQuadratic);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofWaterFlowFractionCurve(curveQuadratic);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setWasteHeatFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setWasteHeatFunctionofTemperatureCurve(curveBiquadratic);
}

/// @cond
CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(std::shared_ptr<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl> impl)
  : ParentObject(impl)
{}
/// @endcond

} // model
} // openstudio

