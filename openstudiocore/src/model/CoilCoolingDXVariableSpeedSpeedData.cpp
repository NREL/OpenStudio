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

#include "CoilCoolingDXVariableSpeedSpeedData.hpp"
#include "CoilCoolingDXVariableSpeedSpeedData_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_Coil_Cooling_DX_VariableSpeed_SpeedData_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingDXVariableSpeedSpeedData_Impl::CoilCoolingDXVariableSpeedSpeedData_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXVariableSpeedSpeedData::iddObjectType());
  }

  CoilCoolingDXVariableSpeedSpeedData_Impl::CoilCoolingDXVariableSpeedSpeedData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingDXVariableSpeedSpeedData::iddObjectType());
  }

  CoilCoolingDXVariableSpeedSpeedData_Impl::CoilCoolingDXVariableSpeedSpeedData_Impl(const CoilCoolingDXVariableSpeedSpeedData_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingDXVariableSpeedSpeedData_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilCoolingDXVariableSpeedSpeedData_Impl::iddObjectType() const {
    return CoilCoolingDXVariableSpeedSpeedData::iddObjectType();
  }

  double CoilCoolingDXVariableSpeedSpeedData_Impl::referenceUnitGrossRatedTotalCoolingCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedTotalCoolingCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXVariableSpeedSpeedData_Impl::referenceUnitGrossRatedSensibleHeatRatio() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedSensibleHeatRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXVariableSpeedSpeedData_Impl::referenceUnitGrossRatedCoolingCOP() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedCoolingCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXVariableSpeedSpeedData_Impl::referenceUnitRatedAirFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedAirFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilCoolingDXVariableSpeedSpeedData_Impl::referenceUnitRatedCondenserAirFlowRate() const {
    return getDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedCondenserAirFlowRate,true);
  }

  boost::optional<double> CoilCoolingDXVariableSpeedSpeedData_Impl::referenceUnitRatedPadEffectivenessofEvapPrecooling() const {
    return getDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedPadEffectivenessofEvapPrecooling,true);
  }

  Curve CoilCoolingDXVariableSpeedSpeedData_Impl::totalCoolingCapacityFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalTotalCoolingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingDXVariableSpeedSpeedData_Impl::totalCoolingCapacityFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalTotalCoolingCapacityFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingDXVariableSpeedSpeedData_Impl::energyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingDXVariableSpeedSpeedData_Impl::energyInputRatioFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity) {
    bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedTotalCoolingCapacity, referenceUnitGrossRatedTotalCoolingCapacity);
    return result;
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio) {
    bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedSensibleHeatRatio, referenceUnitGrossRatedSensibleHeatRatio);
    return result;
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP) {
    bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedCoolingCOP, referenceUnitGrossRatedCoolingCOP);
    return result;
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate) {
    bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedAirFlowRate, referenceUnitRatedAirFlowRate);
    return result;
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setReferenceUnitRatedCondenserAirFlowRate(boost::optional<double> referenceUnitRatedCondenserAirFlowRate) {
    bool result(false);
    if (referenceUnitRatedCondenserAirFlowRate) {
      result = setDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedCondenserAirFlowRate, referenceUnitRatedCondenserAirFlowRate.get());
    }
    else {
      resetReferenceUnitRatedCondenserAirFlowRate();
      result = true;
    }
    return result;
  }

  void CoilCoolingDXVariableSpeedSpeedData_Impl::resetReferenceUnitRatedCondenserAirFlowRate() {
    bool result = setString(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedCondenserAirFlowRate, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setReferenceUnitRatedPadEffectivenessofEvapPrecooling(boost::optional<double> referenceUnitRatedPadEffectivenessofEvapPrecooling) {
    bool result(false);
    if (referenceUnitRatedPadEffectivenessofEvapPrecooling) {
      result = setDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedPadEffectivenessofEvapPrecooling, referenceUnitRatedPadEffectivenessofEvapPrecooling.get());
    }
    else {
      resetReferenceUnitRatedPadEffectivenessofEvapPrecooling();
      result = true;
    }
    return result;
  }

  void CoilCoolingDXVariableSpeedSpeedData_Impl::resetReferenceUnitRatedPadEffectivenessofEvapPrecooling() {
    bool result = setString(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedPadEffectivenessofEvapPrecooling, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::TotalCoolingCapacityFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::TotalCoolingCapacityFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  boost::optional<Curve> CoilCoolingDXVariableSpeedSpeedData_Impl::optionalTotalCoolingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::TotalCoolingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilCoolingDXVariableSpeedSpeedData_Impl::optionalTotalCoolingCapacityFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::TotalCoolingCapacityFunctionofAirFlowFractionCurveName);
  }

  boost::optional<Curve> CoilCoolingDXVariableSpeedSpeedData_Impl::optionalEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilCoolingDXVariableSpeedSpeedData_Impl::optionalEnergyInputRatioFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName);
  }

} // detail

CoilCoolingDXVariableSpeedSpeedData::CoilCoolingDXVariableSpeedSpeedData(const Model& model)
  : ParentObject(CoilCoolingDXVariableSpeedSpeedData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::TotalCoolingCapacityFunctionofTemperatureCurveName
  //     OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::TotalCoolingCapacityFunctionofAirFlowFractionCurveName
  //     OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName
  //     OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName
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
  // ok = setTotalCoolingCapacityFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setTotalCoolingCapacityFunctionofAirFlowFractionCurve();
  OS_ASSERT(ok);
  // ok = setEnergyInputRatioFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setEnergyInputRatioFunctionofAirFlowFractionCurve();
  OS_ASSERT(ok);
}

IddObjectType CoilCoolingDXVariableSpeedSpeedData::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Cooling_DX_VariableSpeed_SpeedData);
}

double CoilCoolingDXVariableSpeedSpeedData::referenceUnitGrossRatedTotalCoolingCapacity() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->referenceUnitGrossRatedTotalCoolingCapacity();
}

double CoilCoolingDXVariableSpeedSpeedData::referenceUnitGrossRatedSensibleHeatRatio() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->referenceUnitGrossRatedSensibleHeatRatio();
}

double CoilCoolingDXVariableSpeedSpeedData::referenceUnitGrossRatedCoolingCOP() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->referenceUnitGrossRatedCoolingCOP();
}

double CoilCoolingDXVariableSpeedSpeedData::referenceUnitRatedAirFlowRate() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->referenceUnitRatedAirFlowRate();
}

boost::optional<double> CoilCoolingDXVariableSpeedSpeedData::referenceUnitRatedCondenserAirFlowRate() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->referenceUnitRatedCondenserAirFlowRate();
}

boost::optional<double> CoilCoolingDXVariableSpeedSpeedData::referenceUnitRatedPadEffectivenessofEvapPrecooling() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->referenceUnitRatedPadEffectivenessofEvapPrecooling();
}

Curve CoilCoolingDXVariableSpeedSpeedData::totalCoolingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->totalCoolingCapacityFunctionofTemperatureCurve();
}

Curve CoilCoolingDXVariableSpeedSpeedData::totalCoolingCapacityFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->totalCoolingCapacityFunctionofAirFlowFractionCurve();
}

Curve CoilCoolingDXVariableSpeedSpeedData::energyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->energyInputRatioFunctionofTemperatureCurve();
}

Curve CoilCoolingDXVariableSpeedSpeedData::energyInputRatioFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->energyInputRatioFunctionofAirFlowFractionCurve();
}

bool CoilCoolingDXVariableSpeedSpeedData::setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitGrossRatedTotalCoolingCapacity(referenceUnitGrossRatedTotalCoolingCapacity);
}

bool CoilCoolingDXVariableSpeedSpeedData::setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitGrossRatedSensibleHeatRatio(referenceUnitGrossRatedSensibleHeatRatio);
}

bool CoilCoolingDXVariableSpeedSpeedData::setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitGrossRatedCoolingCOP(referenceUnitGrossRatedCoolingCOP);
}

bool CoilCoolingDXVariableSpeedSpeedData::setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitRatedAirFlowRate(referenceUnitRatedAirFlowRate);
}

bool CoilCoolingDXVariableSpeedSpeedData::setReferenceUnitRatedCondenserAirFlowRate(double referenceUnitRatedCondenserAirFlowRate) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitRatedCondenserAirFlowRate(referenceUnitRatedCondenserAirFlowRate);
}

void CoilCoolingDXVariableSpeedSpeedData::resetReferenceUnitRatedCondenserAirFlowRate() {
  getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->resetReferenceUnitRatedCondenserAirFlowRate();
}

bool CoilCoolingDXVariableSpeedSpeedData::setReferenceUnitRatedPadEffectivenessofEvapPrecooling(double referenceUnitRatedPadEffectivenessofEvapPrecooling) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitRatedPadEffectivenessofEvapPrecooling(referenceUnitRatedPadEffectivenessofEvapPrecooling);
}

void CoilCoolingDXVariableSpeedSpeedData::resetReferenceUnitRatedPadEffectivenessofEvapPrecooling() {
  getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->resetReferenceUnitRatedPadEffectivenessofEvapPrecooling();
}

bool CoilCoolingDXVariableSpeedSpeedData::setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setTotalCoolingCapacityFunctionofTemperatureCurve(curve);
}

bool CoilCoolingDXVariableSpeedSpeedData::setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setTotalCoolingCapacityFunctionofAirFlowFractionCurve(curve);
}

bool CoilCoolingDXVariableSpeedSpeedData::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve(curve);
}

bool CoilCoolingDXVariableSpeedSpeedData::setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setEnergyInputRatioFunctionofAirFlowFractionCurve(curve);
}

/// @cond
CoilCoolingDXVariableSpeedSpeedData::CoilCoolingDXVariableSpeedSpeedData(std::shared_ptr<detail::CoilCoolingDXVariableSpeedSpeedData_Impl> impl)
  : ParentObject(impl)
{}
/// @endcond

} // model
} // openstudio

