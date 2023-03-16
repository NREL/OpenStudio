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

#include "CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ZoneHVACWaterToAirHeatPump.hpp"
#include "ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "CurveQuadLinear.hpp"
#include "CurveQuadLinear_Impl.hpp"

#include <utilities/idd/OS_Coil_Heating_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    //constructor
    CoilHeatingWaterToAirHeatPumpEquationFit_Impl::CoilHeatingWaterToAirHeatPumpEquationFit_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                 bool keepHandle)
      : WaterToAirComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilHeatingWaterToAirHeatPumpEquationFit::iddObjectType());
    }

    CoilHeatingWaterToAirHeatPumpEquationFit_Impl::CoilHeatingWaterToAirHeatPumpEquationFit_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToAirComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilHeatingWaterToAirHeatPumpEquationFit::iddObjectType());
    }

    CoilHeatingWaterToAirHeatPumpEquationFit_Impl::CoilHeatingWaterToAirHeatPumpEquationFit_Impl(
      const CoilHeatingWaterToAirHeatPumpEquationFit_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToAirComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilHeatingWaterToAirHeatPumpEquationFit_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Heating Coil Electricity Rate",
                                                   "Heating Coil Heating Rate",
                                                   "Heating Coil Sensible Heating Rate",
                                                   "Heating Coil Source Side Heat Transfer Rate",
                                                   "Heating Coil Part Load Ratio",
                                                   "Heating Coil Runtime Fraction",
                                                   "Heating Coil Air Mass Flow Rate",
                                                   "Heating Coil Air Inlet Temperature",
                                                   "Heating Coil Air Inlet Humidity Ratio",
                                                   "Heating Coil Air Outlet Temperature",
                                                   "Heating Coil Air Outlet Humidity Ratio",
                                                   "Heating Coil Source Side Mass Flow Rate",
                                                   "Heating Coil Source Side Inlet Temperature",
                                                   "Heating Coil Source Side Outlet Temperature",
                                                   "Heating Coil Electricity Energy",
                                                   "Heating Coil Heating Energy",
                                                   "Heating Coil Source Side Heat Transfer Energy"};
      return result;
    }

    IddObjectType CoilHeatingWaterToAirHeatPumpEquationFit_Impl::iddObjectType() const {
      return CoilHeatingWaterToAirHeatPumpEquationFit::iddObjectType();
    }

    unsigned CoilHeatingWaterToAirHeatPumpEquationFit_Impl::airInletPort() const {
      return OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirInletNodeName;
    }

    unsigned CoilHeatingWaterToAirHeatPumpEquationFit_Impl::airOutletPort() const {
      return OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName;
    }

    unsigned CoilHeatingWaterToAirHeatPumpEquationFit_Impl::waterInletPort() const {
      return OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName;
    }

    unsigned CoilHeatingWaterToAirHeatPumpEquationFit_Impl::waterOutletPort() const {
      return OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName;
    }

    boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit_Impl::ratedAirFlowRate() const {
      return getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, true);
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::isRatedAirFlowRateDefaulted() const {
      return isEmpty(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate);
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::isRatedAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit_Impl::ratedWaterFlowRate() const {
      return getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, true);
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::isRatedWaterFlowRateDefaulted() const {
      return isEmpty(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate);
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::isRatedWaterFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit_Impl::ratedHeatingCapacity() const {
      return getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCapacity, true);
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::isRatedHeatingCapacityDefaulted() const {
      return isEmpty(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCapacity);
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::isRatedHeatingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::ratedHeatingCoefficientofPerformance() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCoefficientofPerformance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::isRatedHeatingCoefficientofPerformanceDefaulted() const {
      return isEmpty(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCoefficientofPerformance);
    }

    double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::ratedEnteringWaterTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedEnteringWaterTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::ratedEnteringAirDryBulbTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirDryBulbTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::ratioofRatedHeatingCapacitytoRatedCoolingCapacity() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatioofRatedHeatingCapacitytoRatedCoolingCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate) {
      bool result(false);
      if (ratedAirFlowRate) {
        result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, ratedAirFlowRate.get());
      } else {
        resetRatedAirFlowRate();
        result = true;
      }
      return result;
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setRatedAirFlowRate(double ratedAirFlowRate) {
      return setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, ratedAirFlowRate);
    }

    void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::resetRatedAirFlowRate() {
      bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, "");
      OS_ASSERT(result);
    }

    void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::autosizeRatedAirFlowRate() {
      bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setRatedWaterFlowRate(boost::optional<double> ratedWaterFlowRate) {
      bool result(false);
      if (ratedWaterFlowRate) {
        result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, ratedWaterFlowRate.get());
      } else {
        resetRatedWaterFlowRate();
        result = true;
      }
      return result;
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setRatedWaterFlowRate(double ratedWaterFlowRate) {
      return setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, ratedWaterFlowRate);
    }

    void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::resetRatedWaterFlowRate() {
      bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, "");
      OS_ASSERT(result);
    }

    void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::autosizeRatedWaterFlowRate() {
      bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setRatedHeatingCapacity(boost::optional<double> ratedHeatingCapacity) {
      bool result(false);
      if (ratedHeatingCapacity) {
        result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCapacity, ratedHeatingCapacity.get());
      } else {
        resetRatedHeatingCapacity();
        result = true;
      }
      return result;
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setRatedHeatingCapacity(double ratedHeatingCapacity) {
      return setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCapacity, ratedHeatingCapacity);
    }

    void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::resetRatedHeatingCapacity() {
      bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCapacity, "");
      OS_ASSERT(result);
    }

    void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::autosizeRatedHeatingCapacity() {
      bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCapacity, "Autosize");
      OS_ASSERT(result);
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setRatedHeatingCoefficientofPerformance(double ratedHeatingCoefficientofPerformance) {
      bool result =
        setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCoefficientofPerformance, ratedHeatingCoefficientofPerformance);
      return result;
    }

    void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::resetRatedHeatingCoefficientofPerformance() {
      bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCoefficientofPerformance, "");
      OS_ASSERT(result);
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setRatedEnteringWaterTemperature(double ratedEnteringWaterTemperature) {
      bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedEnteringWaterTemperature, ratedEnteringWaterTemperature);
      return result;
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setRatedEnteringAirDryBulbTemperature(double ratedEnteringAirDryBulbTemperature) {
      bool result =
        setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirDryBulbTemperature, ratedEnteringAirDryBulbTemperature);
      return result;
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setRatioofRatedHeatingCapacitytoRatedCoolingCapacity(
      double ratioofRatedHeatingCapacitytoRatedCoolingCapacity) {
      bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatioofRatedHeatingCapacitytoRatedCoolingCapacity,
                              ratioofRatedHeatingCapacitytoRatedCoolingCapacity);
      return result;
    }

    boost::optional<HVACComponent> CoilHeatingWaterToAirHeatPumpEquationFit_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitarySystem
      std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

      for (const auto& airLoopHVACUnitarySystem : airLoopHVACUnitarySystems) {
        if (boost::optional<HVACComponent> heatingCoil = airLoopHVACUnitarySystem.heatingCoil()) {
          if (heatingCoil->handle() == this->handle()) {
            return airLoopHVACUnitarySystem;
          }
        }
      }
      return boost::none;
    }

    boost::optional<ZoneHVACComponent> CoilHeatingWaterToAirHeatPumpEquationFit_Impl::containingZoneHVACComponent() const {
      // ZoneHVACWaterToAirHeatPump

      std::vector<ZoneHVACWaterToAirHeatPump> zoneHVACWaterToAirHeatPumps;

      zoneHVACWaterToAirHeatPumps = this->model().getConcreteModelObjects<ZoneHVACWaterToAirHeatPump>();

      for (const auto& zoneHVACWaterToAirHeatPump : zoneHVACWaterToAirHeatPumps) {
        if (boost::optional<HVACComponent> coil = zoneHVACWaterToAirHeatPump.heatingCoil()) {
          if (coil->handle() == this->handle()) {
            return zoneHVACWaterToAirHeatPump;
          }
        }
      }

      return boost::none;
    }

    CurveQuadLinear CoilHeatingWaterToAirHeatPumpEquationFit_Impl::heatingCapacityCurve() const {
      WorkspaceObject wo = getTarget(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCurveName).get();
      return wo.cast<CurveQuadLinear>();
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setHeatingCapacityCurve(const CurveQuadLinear& heatingCapacityCurve) {
      bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCurveName, heatingCapacityCurve.handle());
      return result;
    }

    CurveQuadLinear CoilHeatingWaterToAirHeatPumpEquationFit_Impl::heatingPowerConsumptionCurve() const {
      WorkspaceObject wo = getTarget(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCurveName).get();
      return wo.cast<CurveQuadLinear>();
    }

    bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setHeatingPowerConsumptionCurve(const CurveQuadLinear& heatingPowerConsumptionCurve) {
      bool result =
        setPointer(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCurveName, heatingPowerConsumptionCurve.handle());
      return result;
    }

    boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit_Impl::autosizedRatedAirFlowRate() const {
      // EPLUS-SQL-INCONSISTENCY
      return getAutosizedValue("Design Size Rated Air Flow Rate", "m3/s", "COIL:HEATING:WATERTOAIRHEATPUMP:EQUATIONFIT");
    }

    boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit_Impl::autosizedRatedWaterFlowRate() const {
      // EPLUS-SQL-INCONSISTENCY
      return getAutosizedValue("Design Size Rated Water Flow Rate", "m3/s", "COIL:HEATING:WATERTOAIRHEATPUMP:EQUATIONFIT");
    }

    boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit_Impl::autosizedRatedHeatingCapacity() const {
      // EPLUS-SQL-INCONSISTENCY
      return getAutosizedValue("Design Size Rated Heating Capacity", "W", "COIL:HEATING:WATERTOAIRHEATPUMP:EQUATIONFIT");
    }

    void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::autosize() {
      autosizeRatedAirFlowRate();
      autosizeRatedWaterFlowRate();
      autosizeRatedHeatingCapacity();
    }

    void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedRatedAirFlowRate();
      if (val) {
        setRatedAirFlowRate(val.get());
      }

      val = autosizedRatedWaterFlowRate();
      if (val) {
        setRatedWaterFlowRate(val.get());
      }

      val = autosizedRatedHeatingCapacity();
      if (val) {
        setRatedHeatingCapacity(val.get());
      }
    }

  }  // namespace detail

  // create a new CoilHeatingWaterToAirHeatPumpEquationFit object in the model's workspace
  CoilHeatingWaterToAirHeatPumpEquationFit::CoilHeatingWaterToAirHeatPumpEquationFit(const Model& model, const CurveQuadLinear& heatingCapacityCurve,
                                                                                     const CurveQuadLinear& heatingPowerConsumptionCurve)
    : WaterToAirComponent(CoilHeatingWaterToAirHeatPumpEquationFit::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>());

    bool ok = setRatedEnteringWaterTemperature(20);
    OS_ASSERT(ok);

    ok = setRatedEnteringAirDryBulbTemperature(20);
    OS_ASSERT(ok);

    ok = setRatioofRatedHeatingCapacitytoRatedCoolingCapacity(1.0);
    OS_ASSERT(ok);

    ok = setHeatingCapacityCurve(heatingCapacityCurve);
    OS_ASSERT(ok);

    ok = setHeatingPowerConsumptionCurve(heatingPowerConsumptionCurve);
    OS_ASSERT(ok);
  }

  // create a new CoilHeatingWaterToAirHeatPumpEquationFit object in the model's workspace
  CoilHeatingWaterToAirHeatPumpEquationFit::CoilHeatingWaterToAirHeatPumpEquationFit(const Model& model)
    : WaterToAirComponent(CoilHeatingWaterToAirHeatPumpEquationFit::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>());

    bool ok = setRatedEnteringWaterTemperature(20);
    OS_ASSERT(ok);

    ok = setRatedEnteringAirDryBulbTemperature(20);
    OS_ASSERT(ok);

    ok = setRatioofRatedHeatingCapacitytoRatedCoolingCapacity(1.0);
    OS_ASSERT(ok);

    CurveQuadLinear heatingCapacityCurve(model);
    heatingCapacityCurve.setCoefficient1Constant(-5.50102734);
    heatingCapacityCurve.setCoefficient2w(-0.96688754);
    heatingCapacityCurve.setCoefficient3x(7.70755007);
    heatingCapacityCurve.setCoefficient4y(0.031928881);
    heatingCapacityCurve.setCoefficient5z(0.028112522);

    ok = setHeatingCapacityCurve(heatingCapacityCurve);
    OS_ASSERT(ok);

    CurveQuadLinear heatingPowerConsumptionCurve(model);
    heatingPowerConsumptionCurve.setCoefficient1Constant(-7.47517858);
    heatingPowerConsumptionCurve.setCoefficient2w(6.40876653);
    heatingPowerConsumptionCurve.setCoefficient3x(1.99711665);
    heatingPowerConsumptionCurve.setCoefficient4y(-0.050682973);
    heatingPowerConsumptionCurve.setCoefficient5z(0.011385145);
    ok = setHeatingPowerConsumptionCurve(heatingPowerConsumptionCurve);
    OS_ASSERT(ok);
  }

  IddObjectType CoilHeatingWaterToAirHeatPumpEquationFit::iddObjectType() {
    return {IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_EquationFit};
  }

  boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit::ratedAirFlowRate() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->ratedAirFlowRate();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::isRatedAirFlowRateDefaulted() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->isRatedAirFlowRateDefaulted();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::isRatedAirFlowRateAutosized() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->isRatedAirFlowRateAutosized();
  }

  boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit::ratedWaterFlowRate() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->ratedWaterFlowRate();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::isRatedWaterFlowRateDefaulted() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->isRatedWaterFlowRateDefaulted();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::isRatedWaterFlowRateAutosized() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->isRatedWaterFlowRateAutosized();
  }

  boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit::ratedHeatingCapacity() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->ratedHeatingCapacity();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::isRatedHeatingCapacityDefaulted() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->isRatedHeatingCapacityDefaulted();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::isRatedHeatingCapacityAutosized() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->isRatedHeatingCapacityAutosized();
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit::ratedHeatingCoefficientofPerformance() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->ratedHeatingCoefficientofPerformance();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::isRatedHeatingCoefficientofPerformanceDefaulted() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->isRatedHeatingCoefficientofPerformanceDefaulted();
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit::ratedEnteringWaterTemperature() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->ratedEnteringWaterTemperature();
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit::ratedEnteringAirDryBulbTemperature() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->ratedEnteringAirDryBulbTemperature();
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit::ratioofRatedHeatingCapacitytoRatedCoolingCapacity() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->ratioofRatedHeatingCapacitytoRatedCoolingCapacity();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setRatedAirFlowRate(OptionalDouble ratedAirFlowRate) {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setRatedAirFlowRate(double ratedAirFlowRate) {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit::resetRatedAirFlowRate() {
    getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedAirFlowRate();
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit::autosizeRatedAirFlowRate() {
    getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->autosizeRatedAirFlowRate();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setRatedWaterFlowRate(OptionalDouble ratedWaterFlowRate) {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setRatedWaterFlowRate(ratedWaterFlowRate);
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setRatedWaterFlowRate(double ratedWaterFlowRate) {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setRatedWaterFlowRate(ratedWaterFlowRate);
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit::resetRatedWaterFlowRate() {
    getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedWaterFlowRate();
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit::autosizeRatedWaterFlowRate() {
    getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->autosizeRatedWaterFlowRate();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setRatedHeatingCapacity(OptionalDouble ratedHeatingCapacity) {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setRatedHeatingCapacity(ratedHeatingCapacity);
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setRatedHeatingCapacity(double ratedHeatingCapacity) {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setRatedHeatingCapacity(ratedHeatingCapacity);
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit::resetRatedHeatingCapacity() {
    getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedHeatingCapacity();
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit::autosizeRatedHeatingCapacity() {
    getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->autosizeRatedHeatingCapacity();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setRatedHeatingCoefficientofPerformance(double ratedHeatingCoefficientofPerformance) {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setRatedHeatingCoefficientofPerformance(
      ratedHeatingCoefficientofPerformance);
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit::resetRatedHeatingCoefficientofPerformance() {
    getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedHeatingCoefficientofPerformance();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setRatedEnteringWaterTemperature(double ratedEnteringWaterTemperature) {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setRatedEnteringWaterTemperature(ratedEnteringWaterTemperature);
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setRatedEnteringAirDryBulbTemperature(double ratedEnteringAirDryBulbTemperature) {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setRatedEnteringAirDryBulbTemperature(
      ratedEnteringAirDryBulbTemperature);
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setRatioofRatedHeatingCapacitytoRatedCoolingCapacity(
    double ratioofRatedHeatingCapacitytoRatedCoolingCapacity) {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setRatioofRatedHeatingCapacitytoRatedCoolingCapacity(
      ratioofRatedHeatingCapacitytoRatedCoolingCapacity);
  }

  boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit::autosizedRatedAirFlowRate() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->autosizedRatedAirFlowRate();
  }

  boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit::autosizedRatedWaterFlowRate() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->autosizedRatedWaterFlowRate();
  }

  boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit::autosizedRatedHeatingCapacity() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->autosizedRatedHeatingCapacity();
  }

  CurveQuadLinear CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCurve() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCurve();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingCapacityCurve(const CurveQuadLinear& heatingCapacityCurve) {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setHeatingCapacityCurve(heatingCapacityCurve);
  }

  CurveQuadLinear CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCurve() const {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCurve();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingPowerConsumptionCurve(const CurveQuadLinear& heatingPowerConsumptionCurve) {
    return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setHeatingPowerConsumptionCurve(heatingPowerConsumptionCurve);
  }

  /// @cond
  CoilHeatingWaterToAirHeatPumpEquationFit::CoilHeatingWaterToAirHeatPumpEquationFit(
    std::shared_ptr<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl> impl)
    : WaterToAirComponent(std::move(impl)) {}

  // DEPRECATED
  double CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCoefficient1() const {
    LOG(Warn, "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCurve().coefficient1Constant() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCurve();
    return curve.coefficient1Constant();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1) {
    LOG(Warn,
        "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCurve().setCoefficient1Constant(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient1Constant(heatingCapacityCoefficient1);
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCoefficient2() const {
    LOG(Warn, "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCurve().coefficient2w() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCurve();
    return curve.coefficient2w();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2) {
    LOG(Warn, "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCurve().setCoefficient2w(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient2w(heatingCapacityCoefficient2);
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCoefficient3() const {
    LOG(Warn, "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCurve().coefficient3x() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCurve();
    return curve.coefficient3x();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3) {
    LOG(Warn, "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCurve().setCoefficient3x(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient3x(heatingCapacityCoefficient3);
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCoefficient4() const {
    LOG(Warn, "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCurve().coefficient4y() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCurve();
    return curve.coefficient4y();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4) {
    LOG(Warn, "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCurve().setCoefficient4y(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient4y(heatingCapacityCoefficient4);
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCoefficient5() const {
    LOG(Warn, "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCurve().coefficient5z() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCurve();
    return curve.coefficient5z();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5) {
    LOG(Warn, "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCurve().setCoefficient5z(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient5z(heatingCapacityCoefficient5);
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCoefficient1() const {
    LOG(Warn,
        "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCurve().coefficient1Constant() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCurve();
    return curve.coefficient1Constant();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingPowerConsumptionCoefficient1(double heatingPowerConsumptionCoefficient1) {
    LOG(Warn, "This method is deprecated. Use "
              "CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCurve().setCoefficient1Constant(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCurve();
    return curve.setCoefficient1Constant(heatingPowerConsumptionCoefficient1);
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCoefficient2() const {
    LOG(Warn, "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCurve().coefficient2w() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCurve();
    return curve.coefficient2w();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingPowerConsumptionCoefficient2(double heatingPowerConsumptionCoefficient2) {
    LOG(Warn,
        "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCurve().setCoefficient2w(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCurve();
    return curve.setCoefficient2w(heatingPowerConsumptionCoefficient2);
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCoefficient3() const {
    LOG(Warn, "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCurve().coefficient3x() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCurve();
    return curve.coefficient3x();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingPowerConsumptionCoefficient3(double heatingPowerConsumptionCoefficient3) {
    LOG(Warn,
        "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCurve().setCoefficient3x(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCurve();
    return curve.setCoefficient3x(heatingPowerConsumptionCoefficient3);
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCoefficient4() const {
    LOG(Warn, "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCurve().coefficient4y() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCurve();
    return curve.coefficient4y();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingPowerConsumptionCoefficient4(double heatingPowerConsumptionCoefficient4) {
    LOG(Warn,
        "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCurve().setCoefficient4y(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCurve();
    return curve.setCoefficient4y(heatingPowerConsumptionCoefficient4);
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCoefficient5() const {
    LOG(Warn, "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCurve().coefficient5z() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCurve();
    return curve.coefficient5z();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingPowerConsumptionCoefficient5(double heatingPowerConsumptionCoefficient5) {
    LOG(Warn,
        "This method is deprecated. Use CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCurve().setCoefficient5z(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCurve();
    return curve.setCoefficient5z(heatingPowerConsumptionCoefficient5);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
