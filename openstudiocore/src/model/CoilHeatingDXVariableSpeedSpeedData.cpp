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

#include "CoilHeatingDXVariableSpeedSpeedData.hpp"
#include "CoilHeatingDXVariableSpeedSpeedData_Impl.hpp"

#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"

#include <utilities/idd/OS_Coil_Heating_DX_VariableSpeed_SpeedData_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingDXVariableSpeedSpeedData_Impl::CoilHeatingDXVariableSpeedSpeedData_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingDXVariableSpeedSpeedData::iddObjectType());
  }

  CoilHeatingDXVariableSpeedSpeedData_Impl::CoilHeatingDXVariableSpeedSpeedData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingDXVariableSpeedSpeedData::iddObjectType());
  }

  CoilHeatingDXVariableSpeedSpeedData_Impl::CoilHeatingDXVariableSpeedSpeedData_Impl(const CoilHeatingDXVariableSpeedSpeedData_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingDXVariableSpeedSpeedData_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingDXVariableSpeedSpeedData_Impl::iddObjectType() const {
    return CoilHeatingDXVariableSpeedSpeedData::iddObjectType();
  }

  double CoilHeatingDXVariableSpeedSpeedData_Impl::referenceUnitGrossRatedHeatingCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedHeatingCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingDXVariableSpeedSpeedData_Impl::referenceUnitGrossRatedHeatingCOP() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedHeatingCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingDXVariableSpeedSpeedData_Impl::referenceUnitRatedAirFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedAirFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  CurveBiquadratic CoilHeatingDXVariableSpeedSpeedData_Impl::heatingCapacityFunctionofTemperatureCurve() const {
    boost::optional<CurveBiquadratic> value = optionalHeatingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilHeatingDXVariableSpeedSpeedData_Impl::totalHeatingCapacityFunctionofAirFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalTotalHeatingCapacityFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Heating Capacity Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  CurveBiquadratic CoilHeatingDXVariableSpeedSpeedData_Impl::energyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<CurveBiquadratic> value = optionalEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilHeatingDXVariableSpeedSpeedData_Impl::energyInputRatioFunctionofAirFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalEnergyInputRatioFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  bool CoilHeatingDXVariableSpeedSpeedData_Impl::setReferenceUnitGrossRatedHeatingCapacity(double referenceUnitGrossRatedHeatingCapacity) {
    bool result = setDouble(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedHeatingCapacity, referenceUnitGrossRatedHeatingCapacity);
    return result;
  }

  bool CoilHeatingDXVariableSpeedSpeedData_Impl::setReferenceUnitGrossRatedHeatingCOP(double referenceUnitGrossRatedHeatingCOP) {
    bool result = setDouble(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedHeatingCOP, referenceUnitGrossRatedHeatingCOP);
    return result;
  }

  bool CoilHeatingDXVariableSpeedSpeedData_Impl::setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate) {
    bool result = setDouble(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedAirFlowRate, referenceUnitRatedAirFlowRate);
    return result;
  }

  bool CoilHeatingDXVariableSpeedSpeedData_Impl::setHeatingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
    bool result = setPointer(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::HeatingCapacityFunctionofTemperatureCurveName, curveBiquadratic.handle());
    return result;
  }

  bool CoilHeatingDXVariableSpeedSpeedData_Impl::setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::TotalHeatingCapacityFunctionofAirFlowFractionCurveName, curveQuadratic.handle());
    return result;
  }

  bool CoilHeatingDXVariableSpeedSpeedData_Impl::setEnergyInputRatioFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
    bool result = setPointer(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName, curveBiquadratic.handle());
    return result;
  }

  bool CoilHeatingDXVariableSpeedSpeedData_Impl::setEnergyInputRatioFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName, curveQuadratic.handle());
    return result;
  }

  boost::optional<CurveBiquadratic> CoilHeatingDXVariableSpeedSpeedData_Impl::optionalHeatingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::HeatingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<CurveQuadratic> CoilHeatingDXVariableSpeedSpeedData_Impl::optionalTotalHeatingCapacityFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::TotalHeatingCapacityFunctionofAirFlowFractionCurveName);
  }

  boost::optional<CurveBiquadratic> CoilHeatingDXVariableSpeedSpeedData_Impl::optionalEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<CurveQuadratic> CoilHeatingDXVariableSpeedSpeedData_Impl::optionalEnergyInputRatioFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName);
  }

} // detail

CoilHeatingDXVariableSpeedSpeedData::CoilHeatingDXVariableSpeedSpeedData(const Model& model)
  : ParentObject(CoilHeatingDXVariableSpeedSpeedData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::HeatingCapacityFunctionofTemperatureCurveName
  //     OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::TotalHeatingCapacityFunctionofAirFlowFractionCurveName
  //     OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName
  //     OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setReferenceUnitGrossRatedHeatingCapacity();
  OS_ASSERT(ok);
  // ok = setReferenceUnitGrossRatedHeatingCOP();
  OS_ASSERT(ok);
  // ok = setReferenceUnitRatedAirFlowRate();
  OS_ASSERT(ok);
  // ok = setHeatingCapacityFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setTotalHeatingCapacityFunctionofAirFlowFractionCurve();
  OS_ASSERT(ok);
  // ok = setEnergyInputRatioFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setEnergyInputRatioFunctionofAirFlowFractionCurve();
  OS_ASSERT(ok);
}

IddObjectType CoilHeatingDXVariableSpeedSpeedData::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_DX_VariableSpeed_SpeedData);
}

double CoilHeatingDXVariableSpeedSpeedData::referenceUnitGrossRatedHeatingCapacity() const {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->referenceUnitGrossRatedHeatingCapacity();
}

double CoilHeatingDXVariableSpeedSpeedData::referenceUnitGrossRatedHeatingCOP() const {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->referenceUnitGrossRatedHeatingCOP();
}

double CoilHeatingDXVariableSpeedSpeedData::referenceUnitRatedAirFlowRate() const {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->referenceUnitRatedAirFlowRate();
}

CurveBiquadratic CoilHeatingDXVariableSpeedSpeedData::heatingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->heatingCapacityFunctionofTemperatureCurve();
}

CurveQuadratic CoilHeatingDXVariableSpeedSpeedData::totalHeatingCapacityFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->totalHeatingCapacityFunctionofAirFlowFractionCurve();
}

CurveBiquadratic CoilHeatingDXVariableSpeedSpeedData::energyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->energyInputRatioFunctionofTemperatureCurve();
}

CurveQuadratic CoilHeatingDXVariableSpeedSpeedData::energyInputRatioFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->energyInputRatioFunctionofAirFlowFractionCurve();
}

bool CoilHeatingDXVariableSpeedSpeedData::setReferenceUnitGrossRatedHeatingCapacity(double referenceUnitGrossRatedHeatingCapacity) {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitGrossRatedHeatingCapacity(referenceUnitGrossRatedHeatingCapacity);
}

bool CoilHeatingDXVariableSpeedSpeedData::setReferenceUnitGrossRatedHeatingCOP(double referenceUnitGrossRatedHeatingCOP) {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitGrossRatedHeatingCOP(referenceUnitGrossRatedHeatingCOP);
}

bool CoilHeatingDXVariableSpeedSpeedData::setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate) {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitRatedAirFlowRate(referenceUnitRatedAirFlowRate);
}

bool CoilHeatingDXVariableSpeedSpeedData::setHeatingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->setHeatingCapacityFunctionofTemperatureCurve(curveBiquadratic);
}

bool CoilHeatingDXVariableSpeedSpeedData::setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->setTotalHeatingCapacityFunctionofAirFlowFractionCurve(curveQuadratic);
}

bool CoilHeatingDXVariableSpeedSpeedData::setEnergyInputRatioFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve(curveBiquadratic);
}

bool CoilHeatingDXVariableSpeedSpeedData::setEnergyInputRatioFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->setEnergyInputRatioFunctionofAirFlowFractionCurve(curveQuadratic);
}

/// @cond
CoilHeatingDXVariableSpeedSpeedData::CoilHeatingDXVariableSpeedSpeedData(std::shared_ptr<detail::CoilHeatingDXVariableSpeedSpeedData_Impl> impl)
  : ParentObject(impl)
{}
/// @endcond

} // model
} // openstudio

