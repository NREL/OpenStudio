/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "CoilHeatingDXVariableSpeedSpeedData.hpp"
#include "CoilHeatingDXVariableSpeedSpeedData_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveQuadratic.hpp"

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

  Curve CoilHeatingDXVariableSpeedSpeedData_Impl::heatingCapacityFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingDXVariableSpeedSpeedData_Impl::totalHeatingCapacityFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalTotalHeatingCapacityFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Heating Capacity Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingDXVariableSpeedSpeedData_Impl::energyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingDXVariableSpeedSpeedData_Impl::energyInputRatioFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofAirFlowFractionCurve();
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

  bool CoilHeatingDXVariableSpeedSpeedData_Impl::setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::HeatingCapacityFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingDXVariableSpeedSpeedData_Impl::setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::TotalHeatingCapacityFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingDXVariableSpeedSpeedData_Impl::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingDXVariableSpeedSpeedData_Impl::setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  boost::optional<Curve> CoilHeatingDXVariableSpeedSpeedData_Impl::optionalHeatingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::HeatingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilHeatingDXVariableSpeedSpeedData_Impl::optionalTotalHeatingCapacityFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::TotalHeatingCapacityFunctionofAirFlowFractionCurveName);
  }

  boost::optional<Curve> CoilHeatingDXVariableSpeedSpeedData_Impl::optionalEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilHeatingDXVariableSpeedSpeedData_Impl::optionalEnergyInputRatioFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName);
  }

  ModelObject CoilHeatingDXVariableSpeedSpeedData_Impl::clone(Model model) const {
    auto t_clone = ParentObject_Impl::clone(model).cast<CoilHeatingDXVariableSpeedSpeedData>();

    return t_clone;
  }

  std::vector<ModelObject> CoilHeatingDXVariableSpeedSpeedData_Impl::children() const {
    std::vector<ModelObject> children;
    children.push_back( heatingCapacityFunctionofTemperatureCurve() );
    children.push_back( totalHeatingCapacityFunctionofAirFlowFractionCurve() );
    children.push_back( energyInputRatioFunctionofTemperatureCurve() );
    children.push_back( energyInputRatioFunctionofAirFlowFractionCurve() );
    return children;
  }

} // detail

CoilHeatingDXVariableSpeedSpeedData::CoilHeatingDXVariableSpeedSpeedData(const Model& model)
  : ParentObject(CoilHeatingDXVariableSpeedSpeedData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>());

  CurveBiquadratic heating_curve_1(model);
  heating_curve_1.setCoefficient1Constant(0.8406506803);
  heating_curve_1.setCoefficient2x(-0.000083433);
  heating_curve_1.setCoefficient3xPOW2(-0.0000683822);
  heating_curve_1.setCoefficient4y(0.0234244981);
  heating_curve_1.setCoefficient5yPOW2(0.000176366);
  heating_curve_1.setCoefficient6xTIMESY(-0.0000901777);
  heating_curve_1.setMinimumValueofx(17.78);
  heating_curve_1.setMaximumValueofx(23.33);
  heating_curve_1.setMinimumValueofy(-30.56);
  heating_curve_1.setMaximumValueofy(17.22);

  CurveQuadratic heating_curve_2(model);
  heating_curve_2.setCoefficient1Constant(1.0);
  heating_curve_2.setCoefficient2x(0.0);
  heating_curve_2.setCoefficient3xPOW2(0.0);
  heating_curve_2.setMinimumValueofx(0.5);
  heating_curve_2.setMaximumValueofx(1.5);

  CurveBiquadratic heating_curve_3(model);
  heating_curve_3.setCoefficient1Constant(0.9172082199);
  heating_curve_3.setCoefficient2x(-0.0116036212);
  heating_curve_3.setCoefficient3xPOW2(0.0010074804);
  heating_curve_3.setCoefficient4y(-0.0089288423);
  heating_curve_3.setCoefficient5yPOW2(0.0009349133);
  heating_curve_3.setCoefficient6xTIMESY(-0.0009326244);
  heating_curve_3.setMinimumValueofx(17.78);
  heating_curve_3.setMaximumValueofx(23.33);
  heating_curve_3.setMinimumValueofy(-30.56);
  heating_curve_3.setMaximumValueofy(17.22);

  CurveQuadratic heating_curve_4(model);
  heating_curve_4.setCoefficient1Constant(1.0);
  heating_curve_4.setCoefficient2x(0.0);
  heating_curve_4.setCoefficient3xPOW2(0.0);
  heating_curve_4.setMinimumValueofx(0.5);
  heating_curve_4.setMaximumValueofx(1.5);

  bool ok = true;
  ok = setReferenceUnitGrossRatedHeatingCapacity(9124.853458);
  OS_ASSERT(ok);
  ok = setReferenceUnitGrossRatedHeatingCOP(4.29731);
  OS_ASSERT(ok);
  ok = setReferenceUnitRatedAirFlowRate(0.653);
  OS_ASSERT(ok);
  ok = setHeatingCapacityFunctionofTemperatureCurve(heating_curve_1);
  OS_ASSERT(ok);
  ok = setTotalHeatingCapacityFunctionofAirFlowFractionCurve(heating_curve_2);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofTemperatureCurve(heating_curve_3);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofAirFlowFractionCurve(heating_curve_4);
  OS_ASSERT(ok);

}

CoilHeatingDXVariableSpeedSpeedData::CoilHeatingDXVariableSpeedSpeedData(const Model& model,
                                                                         const Curve& heatingCapacityFunctionofTemperatureCurve,
                                                                         const Curve& totalHeatingCapacityFunctionofAirFlowFractionCurve,
                                                                         const Curve& energyInputRatioFunctionofTemperatureCurve,
                                                                         const Curve& energyInputRatioFunctionofAirFlowFractionCurve)
  : ParentObject(CoilHeatingDXVariableSpeedSpeedData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>());

  bool ok = true;
  ok = setReferenceUnitGrossRatedHeatingCapacity(9124.853458);
  OS_ASSERT(ok);
  ok = setReferenceUnitGrossRatedHeatingCOP(4.29731);
  OS_ASSERT(ok);
  ok = setReferenceUnitRatedAirFlowRate(0.653);
  OS_ASSERT(ok);
  ok = setHeatingCapacityFunctionofTemperatureCurve(heatingCapacityFunctionofTemperatureCurve);
  OS_ASSERT(ok);
  ok = setTotalHeatingCapacityFunctionofAirFlowFractionCurve(totalHeatingCapacityFunctionofAirFlowFractionCurve);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofTemperatureCurve(energyInputRatioFunctionofTemperatureCurve);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofAirFlowFractionCurve(energyInputRatioFunctionofAirFlowFractionCurve);
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

Curve CoilHeatingDXVariableSpeedSpeedData::heatingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->heatingCapacityFunctionofTemperatureCurve();
}

Curve CoilHeatingDXVariableSpeedSpeedData::totalHeatingCapacityFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->totalHeatingCapacityFunctionofAirFlowFractionCurve();
}

Curve CoilHeatingDXVariableSpeedSpeedData::energyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->energyInputRatioFunctionofTemperatureCurve();
}

Curve CoilHeatingDXVariableSpeedSpeedData::energyInputRatioFunctionofAirFlowFractionCurve() const {
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

bool CoilHeatingDXVariableSpeedSpeedData::setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->setHeatingCapacityFunctionofTemperatureCurve(curve);
}

bool CoilHeatingDXVariableSpeedSpeedData::setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->setTotalHeatingCapacityFunctionofAirFlowFractionCurve(curve);
}

bool CoilHeatingDXVariableSpeedSpeedData::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve(curve);
}

bool CoilHeatingDXVariableSpeedSpeedData::setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXVariableSpeedSpeedData_Impl>()->setEnergyInputRatioFunctionofAirFlowFractionCurve(curve);
}

/// @cond
CoilHeatingDXVariableSpeedSpeedData::CoilHeatingDXVariableSpeedSpeedData(std::shared_ptr<detail::CoilHeatingDXVariableSpeedSpeedData_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

