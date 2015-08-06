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

#include "CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl.hpp"

#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"

#include <utilities/idd/OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedData_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const IdfObject& idfObject,
                                                                                                                                           Model_Impl* model,
                                                                                                                                           bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType());
  }

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                                           Model_Impl* model,
                                                                                                                                           bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType());
  }

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl& other,
                                                                                                                                           Model_Impl* model,
                                                                                                                                           bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::iddObjectType() const {
    return CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType();
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitGrossRatedTotalCoolingCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedTotalCoolingCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitGrossRatedSensibleHeatRatio() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedSensibleHeatRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitGrossRatedCoolingCOP() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedCoolingCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitRatedAirFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedAirFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitRatedWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  CurveBiquadratic CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::totalCoolingCapacityFunctionofTemperatureCurve() const {
    boost::optional<CurveBiquadratic> value = optionalTotalCoolingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::totalCoolingCapacityFunctionofAirFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalTotalCoolingCapacityFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::totalCoolingCapacityFunctionofWaterFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalTotalCoolingCapacityFunctionofWaterFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Functionof Water Flow Fraction Curve attached.");
    }
    return value.get();
  }

  CurveBiquadratic CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<CurveBiquadratic> value = optionalEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofAirFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalEnergyInputRatioFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofWaterFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalEnergyInputRatioFunctionofWaterFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Water Flow Fraction Curve attached.");
    }
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions,true);
    OS_ASSERT(value);
    return value.get();
  }

  CurveBiquadratic CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::wasteHeatFunctionofTemperatureCurve() const {
    boost::optional<CurveBiquadratic> value = optionalWasteHeatFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Waste Heat Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedTotalCoolingCapacity, referenceUnitGrossRatedTotalCoolingCapacity);
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedSensibleHeatRatio, referenceUnitGrossRatedSensibleHeatRatio);
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedCoolingCOP, referenceUnitGrossRatedCoolingCOP);
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedAirFlowRate, referenceUnitRatedAirFlowRate);
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedWaterFlowRate, referenceUnitRatedWaterFlowRate);
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setTotalCoolingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofTemperatureCurveName, curveBiquadratic.handle());
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofAirFlowFractionCurveName, curveQuadratic.handle());
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofWaterFlowFractionCurveName, curveQuadratic.handle());
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName, curveBiquadratic.handle());
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName, curveQuadratic.handle());
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofWaterFlowFractionCurveName, curveQuadratic.handle());
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions, referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setWasteHeatFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::WasteHeatFunctionofTemperatureCurveName, curveBiquadratic.handle());
    return result;
  }

  boost::optional<CurveBiquadratic> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalTotalCoolingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<CurveQuadratic> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalTotalCoolingCapacityFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofAirFlowFractionCurveName);
  }

  boost::optional<CurveQuadratic> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalTotalCoolingCapacityFunctionofWaterFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofWaterFlowFractionCurveName);
  }

  boost::optional<CurveBiquadratic> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<CurveQuadratic> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName);
  }

  boost::optional<CurveQuadratic> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofWaterFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofWaterFlowFractionCurveName);
  }

  boost::optional<CurveBiquadratic> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalWasteHeatFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::WasteHeatFunctionofTemperatureCurveName);
  }

} // detail

CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(const Model& model)
  : ParentObject(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofTemperatureCurveName
  //     OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofAirFlowFractionCurveName
  //     OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofWaterFlowFractionCurveName
  //     OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName
  //     OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName
  //     OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofWaterFlowFractionCurveName
  //     OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::WasteHeatFunctionofTemperatureCurveName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setReferenceUnitGrossRatedTotalCoolingCapacity();
  OS_ASSERT(ok);
  // ok = setReferenceUnitGrossRatedSensibleHeatRatio();
  OS_ASSERT(ok);
  // ok = setReferenceUnitGrossRatedCoolingCOP();
  OS_ASSERT(ok);
  // ok = setReferenceUnitRatedAirFlowRate();
  OS_ASSERT(ok);
  // ok = setReferenceUnitRatedWaterFlowRate();
  OS_ASSERT(ok);
  // ok = setTotalCoolingCapacityFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setTotalCoolingCapacityFunctionofAirFlowFractionCurve();
  OS_ASSERT(ok);
  // ok = setTotalCoolingCapacityFunctionofWaterFlowFractionCurve();
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

IddObjectType CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedData);
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitGrossRatedTotalCoolingCapacity() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitGrossRatedTotalCoolingCapacity();
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitGrossRatedSensibleHeatRatio() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitGrossRatedSensibleHeatRatio();
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitGrossRatedCoolingCOP() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitGrossRatedCoolingCOP();
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitRatedAirFlowRate() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitRatedAirFlowRate();
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitRatedWaterFlowRate() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitRatedWaterFlowRate();
}

CurveBiquadratic CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::totalCoolingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->totalCoolingCapacityFunctionofTemperatureCurve();
}

CurveQuadratic CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::totalCoolingCapacityFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->totalCoolingCapacityFunctionofAirFlowFractionCurve();
}

CurveQuadratic CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::totalCoolingCapacityFunctionofWaterFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->totalCoolingCapacityFunctionofWaterFlowFractionCurve();
}

CurveBiquadratic CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofTemperatureCurve();
}

CurveQuadratic CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofAirFlowFractionCurve();
}

CurveQuadratic CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofWaterFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofWaterFlowFractionCurve();
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitWasteHeatFractionofInputPowerAtRatedConditions();
}

CurveBiquadratic CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::wasteHeatFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->wasteHeatFunctionofTemperatureCurve();
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitGrossRatedTotalCoolingCapacity(referenceUnitGrossRatedTotalCoolingCapacity);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitGrossRatedSensibleHeatRatio(referenceUnitGrossRatedSensibleHeatRatio);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitGrossRatedCoolingCOP(referenceUnitGrossRatedCoolingCOP);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitRatedAirFlowRate(referenceUnitRatedAirFlowRate);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitRatedWaterFlowRate(referenceUnitRatedWaterFlowRate);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setTotalCoolingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setTotalCoolingCapacityFunctionofTemperatureCurve(curveBiquadratic);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setTotalCoolingCapacityFunctionofAirFlowFractionCurve(curveQuadratic);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(curveQuadratic);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve(curveBiquadratic);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofAirFlowFractionCurve(curveQuadratic);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofWaterFlowFractionCurve(curveQuadratic);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setWasteHeatFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setWasteHeatFunctionofTemperatureCurve(curveBiquadratic);
}

/// @cond
CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(std::shared_ptr<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl> impl)
  : ParentObject(impl)
{}
/// @endcond

} // model
} // openstudio

