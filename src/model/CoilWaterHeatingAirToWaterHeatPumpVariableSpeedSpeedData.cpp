/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData.hpp"
#include "CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveCubic.hpp"

#include <utilities/idd/OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedData_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::iddObjectType());
    }

    CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::iddObjectType());
    }

    CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl(
      const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::iddObjectType() const {
      return CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::iddObjectType();
    }

    ModelObject CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::clone(Model model) const {
      auto t_clone = ParentObject_Impl::clone(model).cast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData>();

      return std::move(t_clone);
    }

    std::vector<ModelObject> CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::children() const {
      std::vector<ModelObject> children;
      children.push_back(totalWaterHeatingCapacityFunctionofTemperatureCurve());
      children.push_back(totalWaterHeatingCapacityFunctionofAirFlowFractionCurve());
      children.push_back(totalWaterHeatingCapacityFunctionofWaterFlowFractionCurve());
      children.push_back(copFunctionofTemperatureCurve());
      children.push_back(copFunctionofAirFlowFractionCurve());
      children.push_back(copFunctionofWaterFlowFractionCurve());
      return children;
    }

    double CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::ratedWaterHeatingCapacity() const {
      boost::optional<double> value =
        getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::RatedWaterHeatingCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::ratedWaterHeatingCOP() const {
      boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::RatedWaterHeatingCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::ratedSensibleHeatRatio() const {
      boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::RatedSensibleHeatRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::referenceUnitRatedAirFlowRate() const {
      boost::optional<double> value =
        getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::ReferenceUnitRatedAirFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::referenceUnitRatedWaterFlowRate() const {
      boost::optional<double> value =
        getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::ReferenceUnitRatedWaterFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::referenceUnitWaterPumpInputPowerAtRatedConditions() const {
      boost::optional<double> value =
        getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::ReferenceUnitWaterPumpInputPowerAtRatedConditions, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::totalWaterHeatingCapacityFunctionofTemperatureCurve() const {
      boost::optional<Curve> value = optionalTotalWaterHeatingCapacityFunctionofTemperatureCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Total Water Heating Capacity Function of Temperature Curve attached.");
      }
      return value.get();
    }

    Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::totalWaterHeatingCapacityFunctionofAirFlowFractionCurve() const {
      boost::optional<Curve> value = optionalTotalWaterHeatingCapacityFunctionofAirFlowFractionCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Total Water Heating Capacity Function of Air Flow Fraction Curve attached.");
      }
      return value.get();
    }

    Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::totalWaterHeatingCapacityFunctionofWaterFlowFractionCurve() const {
      boost::optional<Curve> value = optionalTotalWaterHeatingCapacityFunctionofWaterFlowFractionCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Total Water Heating Capacity Function of Water Flow Fraction Curve attached.");
      }
      return value.get();
    }

    Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::copFunctionofTemperatureCurve() const {
      boost::optional<Curve> value = optionalCOPFunctionofTemperatureCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a COP Function of Temperature Curve attached.");
      }
      return value.get();
    }

    Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::copFunctionofAirFlowFractionCurve() const {
      boost::optional<Curve> value = optionalCOPFunctionofAirFlowFractionCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a COP Function of Air Flow Fraction Curve attached.");
      }
      return value.get();
    }

    Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::copFunctionofWaterFlowFractionCurve() const {
      boost::optional<Curve> value = optionalCOPFunctionofWaterFlowFractionCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a COP Function of Water Flow Fraction Curve attached.");
      }
      return value.get();
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::setRatedWaterHeatingCapacity(double ratedWaterHeatingCapacity) {
      bool result =
        setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::RatedWaterHeatingCapacity, ratedWaterHeatingCapacity);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::setRatedWaterHeatingCOP(double ratedWaterHeatingCOP) {
      bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::RatedWaterHeatingCOP, ratedWaterHeatingCOP);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::setRatedSensibleHeatRatio(double ratedSensibleHeatRatio) {
      bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::RatedSensibleHeatRatio, ratedSensibleHeatRatio);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate) {
      bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::ReferenceUnitRatedAirFlowRate,
                              referenceUnitRatedAirFlowRate);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate) {
      bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::ReferenceUnitRatedWaterFlowRate,
                              referenceUnitRatedWaterFlowRate);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::setReferenceUnitWaterPumpInputPowerAtRatedConditions(
      double referenceUnitWaterPumpInputPowerAtRatedConditions) {
      bool result =
        setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::ReferenceUnitWaterPumpInputPowerAtRatedConditions,
                  referenceUnitWaterPumpInputPowerAtRatedConditions);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::setTotalWaterHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
      bool result =
        setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::TotalWaterHeatingCapacityFunctionofTemperatureCurveName,
                   curve.handle());
      return result;
    }

    bool
      CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::setTotalWaterHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
      bool result =
        setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::TotalWaterHeatingCapacityFunctionofAirFlowFractionCurveName,
                   curve.handle());
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::setTotalWaterHeatingCapacityFunctionofWaterFlowFractionCurve(
      const Curve& curve) {
      bool result = setPointer(
        OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::TotalWaterHeatingCapacityFunctionofWaterFlowFractionCurveName,
        curve.handle());
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::setCOPFunctionofTemperatureCurve(const Curve& curve) {
      bool result =
        setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::COPFunctionofTemperatureCurveName, curve.handle());
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::setCOPFunctionofAirFlowFractionCurve(const Curve& curve) {
      bool result =
        setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::COPFunctionofAirFlowFractionCurveName, curve.handle());
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::setCOPFunctionofWaterFlowFractionCurve(const Curve& curve) {
      bool result =
        setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::COPFunctionofWaterFlowFractionCurveName, curve.handle());
      return result;
    }

    boost::optional<Curve>
      CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::optionalTotalWaterHeatingCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::TotalWaterHeatingCapacityFunctionofTemperatureCurveName);
    }

    boost::optional<Curve>
      CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::optionalTotalWaterHeatingCapacityFunctionofAirFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::TotalWaterHeatingCapacityFunctionofAirFlowFractionCurveName);
    }

    boost::optional<Curve>
      CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::optionalTotalWaterHeatingCapacityFunctionofWaterFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::TotalWaterHeatingCapacityFunctionofWaterFlowFractionCurveName);
    }

    boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::optionalCOPFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::COPFunctionofTemperatureCurveName);
    }

    boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::optionalCOPFunctionofAirFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::COPFunctionofAirFlowFractionCurveName);
    }

    boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl::optionalCOPFunctionofWaterFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedDataFields::COPFunctionofWaterFlowFractionCurveName);
    }

  }  // namespace detail

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData(const Model& model)
    : ParentObject(CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>());

    CurveBiquadratic water_heating_cap(model);
    water_heating_cap.setCoefficient1Constant(0.369827);
    water_heating_cap.setCoefficient2x(0.043341);
    water_heating_cap.setCoefficient3xPOW2(-0.00023);
    water_heating_cap.setCoefficient4y(0.000466);
    water_heating_cap.setCoefficient5yPOW2(0.000026);
    water_heating_cap.setCoefficient6xTIMESY(-0.00027);
    water_heating_cap.setMinimumValueofx(0.0);
    water_heating_cap.setMaximumValueofx(40.0);
    water_heating_cap.setMinimumValueofy(20.0);
    water_heating_cap.setMaximumValueofy(90.0);
    water_heating_cap.setInputUnitTypeforX("Temperature");
    water_heating_cap.setInputUnitTypeforY("Temperature");

    CurveCubic constant_cubic(model);
    constant_cubic.setCoefficient1Constant(1.0);
    constant_cubic.setCoefficient2x(0.0);
    constant_cubic.setCoefficient3xPOW2(0.0);
    constant_cubic.setCoefficient4xPOW3(0.0);
    constant_cubic.setMinimumValueofx(-100.0);
    constant_cubic.setMaximumValueofx(100.0);

    CurveBiquadratic water_heating_cop(model);
    water_heating_cop.setCoefficient1Constant(1.19713);
    water_heating_cop.setCoefficient2x(0.077849);
    water_heating_cop.setCoefficient3xPOW2(-0.0000016);
    water_heating_cop.setCoefficient4y(-0.02675);
    water_heating_cop.setCoefficient5yPOW2(0.000296);
    water_heating_cop.setCoefficient6xTIMESY(-0.00112);
    water_heating_cop.setMinimumValueofx(0.0);
    water_heating_cop.setMaximumValueofx(40.0);
    water_heating_cop.setMinimumValueofy(20.0);
    water_heating_cop.setMaximumValueofy(90.0);
    water_heating_cop.setInputUnitTypeforX("Temperature");
    water_heating_cop.setInputUnitTypeforY("Temperature");

    bool ok = true;
    ok = setRatedWaterHeatingCapacity(400.0);  // ASIHPMixedTank.idf
    OS_ASSERT(ok);
    ok = setRatedWaterHeatingCOP(5.0);  // ASIHPMixedTank.idf, not 3.2 idd default
    OS_ASSERT(ok);
    ok = setRatedSensibleHeatRatio(0.8);  // ASIHPMixedTank.idf, not 0.85 idd default
    OS_ASSERT(ok);
    ok = setReferenceUnitRatedAirFlowRate(0.02014);  // ASIHPMixedTank.idf
    OS_ASSERT(ok);
    ok = setReferenceUnitRatedWaterFlowRate(0.000018);  // ASIHPMixedTank.idf
    OS_ASSERT(ok);
    ok = setReferenceUnitWaterPumpInputPowerAtRatedConditions(10.0);  // ASIHPMixedTank.idf
    OS_ASSERT(ok);
    ok = setTotalWaterHeatingCapacityFunctionofTemperatureCurve(water_heating_cap);  // ASIHPMixedTank.idf
    OS_ASSERT(ok);
    ok = setTotalWaterHeatingCapacityFunctionofAirFlowFractionCurve(constant_cubic);  // ASIHPMixedTank.idf
    OS_ASSERT(ok);
    ok = setTotalWaterHeatingCapacityFunctionofWaterFlowFractionCurve(constant_cubic);  // ASIHPMixedTank.idf
    OS_ASSERT(ok);
    ok = setCOPFunctionofTemperatureCurve(water_heating_cop);  // ASIHPMixedTank.idf
    OS_ASSERT(ok);
    ok = setCOPFunctionofAirFlowFractionCurve(constant_cubic);  // ASIHPMixedTank.idf
    OS_ASSERT(ok);
    ok = setCOPFunctionofWaterFlowFractionCurve(constant_cubic);  // ASIHPMixedTank.idf
    OS_ASSERT(ok);
  }

  IddObjectType CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::iddObjectType() {
    return {IddObjectType::OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_SpeedData};
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::ratedWaterHeatingCapacity() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->ratedWaterHeatingCapacity();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::ratedWaterHeatingCOP() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->ratedWaterHeatingCOP();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::ratedSensibleHeatRatio() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->ratedSensibleHeatRatio();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::referenceUnitRatedAirFlowRate() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->referenceUnitRatedAirFlowRate();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::referenceUnitRatedWaterFlowRate() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->referenceUnitRatedWaterFlowRate();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::referenceUnitWaterPumpInputPowerAtRatedConditions() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->referenceUnitWaterPumpInputPowerAtRatedConditions();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::totalWaterHeatingCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->totalWaterHeatingCapacityFunctionofTemperatureCurve();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::totalWaterHeatingCapacityFunctionofAirFlowFractionCurve() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()
      ->totalWaterHeatingCapacityFunctionofAirFlowFractionCurve();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::totalWaterHeatingCapacityFunctionofWaterFlowFractionCurve() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()
      ->totalWaterHeatingCapacityFunctionofWaterFlowFractionCurve();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::copFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->copFunctionofTemperatureCurve();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::copFunctionofAirFlowFractionCurve() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->copFunctionofAirFlowFractionCurve();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::copFunctionofWaterFlowFractionCurve() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->copFunctionofWaterFlowFractionCurve();
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::setRatedWaterHeatingCapacity(double ratedWaterHeatingCapacity) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->setRatedWaterHeatingCapacity(ratedWaterHeatingCapacity);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::setRatedWaterHeatingCOP(double ratedWaterHeatingCOP) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->setRatedWaterHeatingCOP(ratedWaterHeatingCOP);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::setRatedSensibleHeatRatio(double ratedSensibleHeatRatio) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->setRatedSensibleHeatRatio(ratedSensibleHeatRatio);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->setReferenceUnitRatedAirFlowRate(
      referenceUnitRatedAirFlowRate);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->setReferenceUnitRatedWaterFlowRate(
      referenceUnitRatedWaterFlowRate);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::setReferenceUnitWaterPumpInputPowerAtRatedConditions(
    double referenceUnitWaterPumpInputPowerAtRatedConditions) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->setReferenceUnitWaterPumpInputPowerAtRatedConditions(
      referenceUnitWaterPumpInputPowerAtRatedConditions);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::setTotalWaterHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->setTotalWaterHeatingCapacityFunctionofTemperatureCurve(
      curve);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::setTotalWaterHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()
      ->setTotalWaterHeatingCapacityFunctionofAirFlowFractionCurve(curve);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::setTotalWaterHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()
      ->setTotalWaterHeatingCapacityFunctionofWaterFlowFractionCurve(curve);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::setCOPFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->setCOPFunctionofTemperatureCurve(curve);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::setCOPFunctionofAirFlowFractionCurve(const Curve& curve) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->setCOPFunctionofAirFlowFractionCurve(curve);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::setCOPFunctionofWaterFlowFractionCurve(const Curve& curve) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl>()->setCOPFunctionofWaterFlowFractionCurve(curve);
  }

  /// @cond
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData(
    std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl> impl)
    : ParentObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
