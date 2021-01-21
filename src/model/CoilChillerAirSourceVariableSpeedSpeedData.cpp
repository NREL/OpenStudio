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

#include "CoilChillerAirSourceVariableSpeedSpeedData.hpp"
#include "CoilChillerAirSourceVariableSpeedSpeedData_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveQuadratic.hpp"

#include <utilities/idd/OS_Coil_Chiller_AirSource_VariableSpeed_SpeedData_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilChillerAirSourceVariableSpeedSpeedData_Impl::CoilChillerAirSourceVariableSpeedSpeedData_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                     bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilChillerAirSourceVariableSpeedSpeedData::iddObjectType());
    }

    CoilChillerAirSourceVariableSpeedSpeedData_Impl::CoilChillerAirSourceVariableSpeedSpeedData_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilChillerAirSourceVariableSpeedSpeedData::iddObjectType());
    }

    CoilChillerAirSourceVariableSpeedSpeedData_Impl::CoilChillerAirSourceVariableSpeedSpeedData_Impl(
      const CoilChillerAirSourceVariableSpeedSpeedData_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilChillerAirSourceVariableSpeedSpeedData_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType CoilChillerAirSourceVariableSpeedSpeedData_Impl::iddObjectType() const {
      return CoilChillerAirSourceVariableSpeedSpeedData::iddObjectType();
    }

    ModelObject CoilChillerAirSourceVariableSpeedSpeedData_Impl::clone(Model model) const {
      auto t_clone = ParentObject_Impl::clone(model).cast<CoilChillerAirSourceVariableSpeedSpeedData>();

      return t_clone;
    }

    std::vector<ModelObject> CoilChillerAirSourceVariableSpeedSpeedData_Impl::children() const {
      std::vector<ModelObject> children;
      children.push_back(totalCoolingCapacityFunctionofTemperatureCurve());
      children.push_back(totalCoolingCapacityFunctionofWaterFlowFractionCurve());
      children.push_back(energyInputRatioFunctionofTemperatureCurve());
      children.push_back(energyInputRatioFunctionofWaterFlowFractionCurve());
      return children;
    }

    double CoilChillerAirSourceVariableSpeedSpeedData_Impl::ratedWaterCoolingCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::RatedWaterCoolingCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilChillerAirSourceVariableSpeedSpeedData_Impl::ratedWaterCoolingCOP() const {
      boost::optional<double> value = getDouble(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::RatedWaterCoolingCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilChillerAirSourceVariableSpeedSpeedData_Impl::isRatedWaterCoolingCOPDefaulted() const {
      return isEmpty(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::RatedWaterCoolingCOP);
    }

    double CoilChillerAirSourceVariableSpeedSpeedData_Impl::referenceUnitRatedAirFlowRate() const {
      boost::optional<double> value = getDouble(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::ReferenceUnitRatedAirFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilChillerAirSourceVariableSpeedSpeedData_Impl::referenceUnitRatedWaterFlowRate() const {
      boost::optional<double> value = getDouble(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::ReferenceUnitRatedWaterFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilChillerAirSourceVariableSpeedSpeedData_Impl::referenceUnitWaterPumpInputPowerAtRatedConditions() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::ReferenceUnitWaterPumpInputPowerAtRatedConditions, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve CoilChillerAirSourceVariableSpeedSpeedData_Impl::totalCoolingCapacityFunctionofTemperatureCurve() const {
      boost::optional<Curve> value = optionalTotalCoolingCapacityFunctionofTemperatureCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Function of Temperature Curve attached.");
      }
      return value.get();
    }

    Curve CoilChillerAirSourceVariableSpeedSpeedData_Impl::totalCoolingCapacityFunctionofWaterFlowFractionCurve() const {
      boost::optional<Curve> value = optionalTotalCoolingCapacityFunctionofWaterFlowFractionCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Function of Air Flow Fraction Curve attached.");
      }
      return value.get();
    }

    Curve CoilChillerAirSourceVariableSpeedSpeedData_Impl::energyInputRatioFunctionofTemperatureCurve() const {
      boost::optional<Curve> value = optionalEnergyInputRatioFunctionofTemperatureCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Function of Temperature Curve attached.");
      }
      return value.get();
    }

    Curve CoilChillerAirSourceVariableSpeedSpeedData_Impl::energyInputRatioFunctionofWaterFlowFractionCurve() const {
      boost::optional<Curve> value = optionalEnergyInputRatioFunctionofWaterFlowFractionCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Function of Air Flow Fraction Curve attached.");
      }
      return value.get();
    }

    bool CoilChillerAirSourceVariableSpeedSpeedData_Impl::setRatedWaterCoolingCapacity(double ratedWaterCoolingCapacity) {
      bool result = setDouble(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::RatedWaterCoolingCapacity, ratedWaterCoolingCapacity);
      return result;
    }

    bool CoilChillerAirSourceVariableSpeedSpeedData_Impl::setRatedWaterCoolingCOP(double ratedWaterCoolingCOP) {
      bool result = setDouble(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::RatedWaterCoolingCOP, ratedWaterCoolingCOP);
      return result;
    }

    void CoilChillerAirSourceVariableSpeedSpeedData_Impl::resetRatedWaterCoolingCOP() {
      bool result = setString(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::RatedWaterCoolingCOP, "");
      OS_ASSERT(result);
    }

    bool CoilChillerAirSourceVariableSpeedSpeedData_Impl::setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate) {
      bool result = setDouble(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::ReferenceUnitRatedAirFlowRate, referenceUnitRatedAirFlowRate);
      return result;
    }

    bool CoilChillerAirSourceVariableSpeedSpeedData_Impl::setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate) {
      bool result =
        setDouble(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::ReferenceUnitRatedWaterFlowRate, referenceUnitRatedWaterFlowRate);
      return result;
    }

    bool CoilChillerAirSourceVariableSpeedSpeedData_Impl::setReferenceUnitWaterPumpInputPowerAtRatedConditions(
      double referenceUnitWaterPumpInputPowerAtRatedConditions) {
      bool result = setDouble(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::ReferenceUnitWaterPumpInputPowerAtRatedConditions,
                              referenceUnitWaterPumpInputPowerAtRatedConditions);
      return result;
    }

    bool CoilChillerAirSourceVariableSpeedSpeedData_Impl::setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve) {
      bool result =
        setPointer(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::TotalCoolingCapacityFunctionofTemperatureCurveName, curve.handle());
      return result;
    }

    bool CoilChillerAirSourceVariableSpeedSpeedData_Impl::setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve) {
      bool result =
        setPointer(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::TotalCoolingCapacityFunctionofWaterFlowFractionCurveName, curve.handle());
      return result;
    }

    bool CoilChillerAirSourceVariableSpeedSpeedData_Impl::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
      bool result =
        setPointer(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName, curve.handle());
      return result;
    }

    bool CoilChillerAirSourceVariableSpeedSpeedData_Impl::setEnergyInputRatioFunctionofWaterFlowFractionCurve(const Curve& curve) {
      bool result =
        setPointer(OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofWaterFlowFractionCurveName, curve.handle());
      return result;
    }

    boost::optional<Curve> CoilChillerAirSourceVariableSpeedSpeedData_Impl::optionalTotalCoolingCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::TotalCoolingCapacityFunctionofTemperatureCurveName);
    }

    boost::optional<Curve> CoilChillerAirSourceVariableSpeedSpeedData_Impl::optionalTotalCoolingCapacityFunctionofWaterFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::TotalCoolingCapacityFunctionofWaterFlowFractionCurveName);
    }

    boost::optional<Curve> CoilChillerAirSourceVariableSpeedSpeedData_Impl::optionalEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName);
    }

    boost::optional<Curve> CoilChillerAirSourceVariableSpeedSpeedData_Impl::optionalEnergyInputRatioFunctionofWaterFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Chiller_AirSource_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofWaterFlowFractionCurveName);
    }

  }  // namespace detail

  CoilChillerAirSourceVariableSpeedSpeedData::CoilChillerAirSourceVariableSpeedSpeedData(const Model& model)
    : ParentObject(CoilChillerAirSourceVariableSpeedSpeedData::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>());

    CurveBiquadratic cooling_curve_1(model);
    cooling_curve_1.setCoefficient1Constant(1.6253449506);
    cooling_curve_1.setCoefficient2x(-0.0786550838);
    cooling_curve_1.setCoefficient3xPOW2(0.0030679776);
    cooling_curve_1.setCoefficient4y(0.0008002088);
    cooling_curve_1.setCoefficient5yPOW2(-0.0000354060);
    cooling_curve_1.setCoefficient6xTIMESY(-0.0003534409);
    cooling_curve_1.setMinimumValueofx(13.89);
    cooling_curve_1.setMaximumValueofx(22.22);
    cooling_curve_1.setMinimumValueofy(12.78);
    cooling_curve_1.setMaximumValueofy(51.67);

    CurveQuadratic cooling_curve_2(model);
    cooling_curve_2.setCoefficient1Constant(1.0);
    cooling_curve_2.setCoefficient2x(0.0);
    cooling_curve_2.setCoefficient3xPOW2(0.0);
    cooling_curve_2.setMinimumValueofx(0.5);
    cooling_curve_2.setMaximumValueofx(1.5);

    CurveBiquadratic cooling_curve_3(model);
    cooling_curve_3.setCoefficient1Constant(1.4240389306);
    cooling_curve_3.setCoefficient2x(-0.0593310687);
    cooling_curve_3.setCoefficient3xPOW2(0.0026068070);
    cooling_curve_3.setCoefficient4y(0.0008867551);
    cooling_curve_3.setCoefficient5yPOW2(-0.0000369191);
    cooling_curve_3.setCoefficient6xTIMESY(-0.0003552805);
    cooling_curve_3.setMinimumValueofx(13.89);
    cooling_curve_3.setMaximumValueofx(22.22);
    cooling_curve_3.setMinimumValueofy(12.78);
    cooling_curve_3.setMaximumValueofy(51.67);

    CurveQuadratic cooling_curve_4(model);
    cooling_curve_4.setCoefficient1Constant(1.0);
    cooling_curve_4.setCoefficient2x(0.0);
    cooling_curve_4.setCoefficient3xPOW2(0.0);
    cooling_curve_4.setMinimumValueofx(0.5);
    cooling_curve_4.setMaximumValueofx(1.5);

    bool ok = true;
    ok = setRatedWaterCoolingCapacity(9555.0);  // TODO
    OS_ASSERT(ok);
    ok = setReferenceUnitRatedAirFlowRate(1.58);  // TODO
    OS_ASSERT(ok);
    ok = setReferenceUnitRatedWaterFlowRate(0.00063);  // TODO
    OS_ASSERT(ok);
    ok = setReferenceUnitWaterPumpInputPowerAtRatedConditions(15.0);  // TODO
    OS_ASSERT(ok);
    ok = setTotalCoolingCapacityFunctionofTemperatureCurve(cooling_curve_1);  // TODO
    OS_ASSERT(ok);
    ok = setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(cooling_curve_2);  // TODO
    OS_ASSERT(ok);
    ok = setEnergyInputRatioFunctionofTemperatureCurve(cooling_curve_3);  // TODO
    OS_ASSERT(ok);
    ok = setEnergyInputRatioFunctionofWaterFlowFractionCurve(cooling_curve_4);  // TODO
    OS_ASSERT(ok);
  }

  IddObjectType CoilChillerAirSourceVariableSpeedSpeedData::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Coil_Chiller_AirSource_VariableSpeed_SpeedData);
  }

  double CoilChillerAirSourceVariableSpeedSpeedData::ratedWaterCoolingCapacity() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->ratedWaterCoolingCapacity();
  }

  double CoilChillerAirSourceVariableSpeedSpeedData::ratedWaterCoolingCOP() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->ratedWaterCoolingCOP();
  }

  bool CoilChillerAirSourceVariableSpeedSpeedData::isRatedWaterCoolingCOPDefaulted() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->isRatedWaterCoolingCOPDefaulted();
  }

  double CoilChillerAirSourceVariableSpeedSpeedData::referenceUnitRatedAirFlowRate() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->referenceUnitRatedAirFlowRate();
  }

  double CoilChillerAirSourceVariableSpeedSpeedData::referenceUnitRatedWaterFlowRate() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->referenceUnitRatedWaterFlowRate();
  }

  double CoilChillerAirSourceVariableSpeedSpeedData::referenceUnitWaterPumpInputPowerAtRatedConditions() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->referenceUnitWaterPumpInputPowerAtRatedConditions();
  }

  Curve CoilChillerAirSourceVariableSpeedSpeedData::totalCoolingCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->totalCoolingCapacityFunctionofTemperatureCurve();
  }

  Curve CoilChillerAirSourceVariableSpeedSpeedData::totalCoolingCapacityFunctionofWaterFlowFractionCurve() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->totalCoolingCapacityFunctionofWaterFlowFractionCurve();
  }

  Curve CoilChillerAirSourceVariableSpeedSpeedData::energyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->energyInputRatioFunctionofTemperatureCurve();
  }

  Curve CoilChillerAirSourceVariableSpeedSpeedData::energyInputRatioFunctionofWaterFlowFractionCurve() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->energyInputRatioFunctionofWaterFlowFractionCurve();
  }

  bool CoilChillerAirSourceVariableSpeedSpeedData::setRatedWaterCoolingCapacity(double ratedWaterCoolingCapacity) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->setRatedWaterCoolingCapacity(ratedWaterCoolingCapacity);
  }

  bool CoilChillerAirSourceVariableSpeedSpeedData::setRatedWaterCoolingCOP(double ratedWaterCoolingCOP) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->setRatedWaterCoolingCOP(ratedWaterCoolingCOP);
  }

  void CoilChillerAirSourceVariableSpeedSpeedData::resetRatedWaterCoolingCOP() {
    getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->resetRatedWaterCoolingCOP();
  }

  bool CoilChillerAirSourceVariableSpeedSpeedData::setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->setReferenceUnitRatedAirFlowRate(referenceUnitRatedAirFlowRate);
  }

  bool CoilChillerAirSourceVariableSpeedSpeedData::setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->setReferenceUnitRatedWaterFlowRate(referenceUnitRatedWaterFlowRate);
  }

  bool CoilChillerAirSourceVariableSpeedSpeedData::setReferenceUnitWaterPumpInputPowerAtRatedConditions(
    double referenceUnitWaterPumpInputPowerAtRatedConditions) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->setReferenceUnitWaterPumpInputPowerAtRatedConditions(
      referenceUnitWaterPumpInputPowerAtRatedConditions);
  }

  bool CoilChillerAirSourceVariableSpeedSpeedData::setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->setTotalCoolingCapacityFunctionofTemperatureCurve(curve);
  }

  bool CoilChillerAirSourceVariableSpeedSpeedData::setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(curve);
  }

  bool CoilChillerAirSourceVariableSpeedSpeedData::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve(curve);
  }

  bool CoilChillerAirSourceVariableSpeedSpeedData::setEnergyInputRatioFunctionofWaterFlowFractionCurve(const Curve& curve) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl>()->setEnergyInputRatioFunctionofWaterFlowFractionCurve(curve);
  }

  /// @cond
  CoilChillerAirSourceVariableSpeedSpeedData::CoilChillerAirSourceVariableSpeedSpeedData(
    std::shared_ptr<detail::CoilChillerAirSourceVariableSpeedSpeedData_Impl> impl)
    : ParentObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
