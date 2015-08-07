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

#include "Curve.hpp"
#include "Curve_Impl.hpp"

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

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::heatingCapacityFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::totalHeatingCapacityFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalTotalHeatingCapacityFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Heating Capacity Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::heatingCapacityFunctionofWaterFlowFractionCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityFunctionofWaterFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Water Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofWaterFlowFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofWaterFlowFractionCurve();
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

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::wasteHeatFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalWasteHeatFunctionofTemperatureCurve();
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

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::HeatingCapacityFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalHeatingCapacityFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::HeatingCapacityFunctionofWaterFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofWaterFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofWaterFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions, referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setWasteHeatFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::WasteHeatFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalHeatingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::HeatingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalTotalHeatingCapacityFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalHeatingCapacityFunctionofAirFlowFractionCurveName);
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalHeatingCapacityFunctionofWaterFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::HeatingCapacityFunctionofWaterFlowFractionCurveName);
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName);
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofWaterFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofWaterFlowFractionCurveName);
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalWasteHeatFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::WasteHeatFunctionofTemperatureCurveName);
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

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::heatingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->heatingCapacityFunctionofTemperatureCurve();
}

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::totalHeatingCapacityFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->totalHeatingCapacityFunctionofAirFlowFractionCurve();
}

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::heatingCapacityFunctionofWaterFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->heatingCapacityFunctionofWaterFlowFractionCurve();
}

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofTemperatureCurve();
}

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofAirFlowFractionCurve();
}

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofWaterFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofWaterFlowFractionCurve();
}

double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitWasteHeatFractionofInputPowerAtRatedConditions();
}

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::wasteHeatFunctionofTemperatureCurve() const {
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

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setHeatingCapacityFunctionofTemperatureCurve(curve);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setTotalHeatingCapacityFunctionofAirFlowFractionCurve(curve);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setHeatingCapacityFunctionofWaterFlowFractionCurve(curve);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve(curve);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofAirFlowFractionCurve(curve);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofWaterFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofWaterFlowFractionCurve(curve);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setWasteHeatFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setWasteHeatFunctionofTemperatureCurve(curve);
}

/// @cond
CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(std::shared_ptr<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl> impl)
  : ParentObject(impl)
{}
/// @endcond

} // model
} // openstudio

