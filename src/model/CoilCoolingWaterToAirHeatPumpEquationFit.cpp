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

#include "CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"

#include "ZoneHVACWaterToAirHeatPump.hpp"
#include "ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "CurveQuadLinear.hpp"
#include "CurveQuadLinear_Impl.hpp"
#include "CurveQuintLinear.hpp"
#include "CurveQuintLinear_Impl.hpp"

#include <utilities/idd/OS_Coil_Cooling_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilCoolingWaterToAirHeatPumpEquationFit_Impl::CoilCoolingWaterToAirHeatPumpEquationFit_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                 bool keepHandle)
      : WaterToAirComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilCoolingWaterToAirHeatPumpEquationFit::iddObjectType());
    }

    CoilCoolingWaterToAirHeatPumpEquationFit_Impl::CoilCoolingWaterToAirHeatPumpEquationFit_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToAirComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilCoolingWaterToAirHeatPumpEquationFit::iddObjectType());
    }

    CoilCoolingWaterToAirHeatPumpEquationFit_Impl::CoilCoolingWaterToAirHeatPumpEquationFit_Impl(
      const CoilCoolingWaterToAirHeatPumpEquationFit_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToAirComponent_Impl(other, model, keepHandle) {}

    ModelObject CoilCoolingWaterToAirHeatPumpEquationFit_Impl::clone(Model model) const {
      return WaterToAirComponent_Impl::clone(model);
    }

    const std::vector<std::string>& CoilCoolingWaterToAirHeatPumpEquationFit_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Cooling Coil Electricity Rate",
                                                   "Cooling Coil Total Cooling Rate",
                                                   "Cooling Coil Sensible Cooling Rate",
                                                   "Cooling Coil Source Side Heat Transfer Rate",
                                                   "Cooling Coil Part Load Ratio",
                                                   "Cooling Coil Air Mass Flow Rate",
                                                   "Cooling Coil Air Inlet Temperature",
                                                   "Cooling Coil Air Inlet Humidity Ratio",
                                                   "Cooling Coil Air Outlet Temperature",
                                                   "Cooling Coil Air Outlet Humidity Ratio",
                                                   "Cooling Coil Source Side Mass Flow Rate",
                                                   "Cooling Coil Source Side Inlet Temperature",
                                                   "Cooling Coil Source Side Outlet Temperature",
                                                   "Cooling Coil Electricity Energy",
                                                   "Cooling Coil Total Cooling Energy",
                                                   "Cooling Coil Sensible Cooling Energy",
                                                   "Cooling Coil Latent Cooling Energy",
                                                   "Cooling Coil Source Side Heat Transfer Energy"};
      return result;
    }

    IddObjectType CoilCoolingWaterToAirHeatPumpEquationFit_Impl::iddObjectType() const {
      return CoilCoolingWaterToAirHeatPumpEquationFit::iddObjectType();
    }

    std::vector<openstudio::IdfObject> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::remove() {
      if (isRemovable()) {
        return WaterToAirComponent_Impl::remove();
      }

      return {};
    }

    boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratedAirFlowRate() const {
      return getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, true);
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedAirFlowRateDefaulted() const {
      return isEmpty(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate);
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratedWaterFlowRate() const {
      return getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, true);
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedWaterFlowRateDefaulted() const {
      return isEmpty(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate);
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedWaterFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratedTotalCoolingCapacity() const {
      return getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedTotalCoolingCapacity, true);
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedTotalCoolingCapacityDefaulted() const {
      return isEmpty(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedTotalCoolingCapacity);
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedTotalCoolingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedTotalCoolingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratedSensibleCoolingCapacity() const {
      return getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedSensibleCoolingCapacity, true);
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedSensibleCoolingCapacityDefaulted() const {
      return isEmpty(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedSensibleCoolingCapacity);
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedSensibleCoolingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedSensibleCoolingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratedCoolingCoefficientofPerformance() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedCoolingCoefficientofPerformance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedCoolingCoefficientofPerformanceDefaulted() const {
      return isEmpty(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedCoolingCoefficientofPerformance);
    }

    double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratedEnteringWaterTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringWaterTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratedEnteringAirDryBulbTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirDryBulbTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratedEnteringAirWetBulbTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirWetBulbTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    CurveQuadLinear CoilCoolingWaterToAirHeatPumpEquationFit_Impl::totalCoolingCapacityCurve() const {
      WorkspaceObject wo = getTarget(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCurveName).get();
      return wo.cast<CurveQuadLinear>();
    }

    CurveQuintLinear CoilCoolingWaterToAirHeatPumpEquationFit_Impl::sensibleCoolingCapacityCurve() const {
      WorkspaceObject wo = getTarget(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCurveName).get();
      return wo.cast<CurveQuintLinear>();
    }

    CurveQuadLinear CoilCoolingWaterToAirHeatPumpEquationFit_Impl::coolingPowerConsumptionCurve() const {
      WorkspaceObject wo = getTarget(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCurveName).get();
      return wo.cast<CurveQuadLinear>();
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setTotalCoolingCapacityCurve(const CurveQuadLinear& totalCoolingCapacityCurve) {
      bool result =
        setPointer(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCurveName, totalCoolingCapacityCurve.handle());
      return result;
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setSensibleCoolingCapacityCurve(const CurveQuintLinear& sensibleCoolingCapacityCurve) {
      bool result =
        setPointer(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCurveName, sensibleCoolingCapacityCurve.handle());
      return result;
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setCoolingPowerConsumptionCurve(const CurveQuadLinear& coolingPowerConsumptionCurve) {
      bool result =
        setPointer(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCurveName, coolingPowerConsumptionCurve.handle());
      return result;
    }

    double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::nominalTimeforCondensateRemovaltoBegin() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::NominalTimeforCondensateRemovaltoBegin, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isNominalTimeforCondensateRemovaltoBeginDefaulted() const {
      return isEmpty(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::NominalTimeforCondensateRemovaltoBegin);
    }

    double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacityDefaulted() const {
      return isEmpty(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate) {
      bool result(false);
      if (ratedAirFlowRate) {
        result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, ratedAirFlowRate.get());
      } else {
        resetRatedAirFlowRate();
        result = true;
      }
      return result;
    }

    void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::resetRatedAirFlowRate() {
      bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, "");
      OS_ASSERT(result);
    }

    void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::autosizeRatedAirFlowRate() {
      bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatedWaterFlowRate(boost::optional<double> ratedWaterFlowRate) {
      bool result(false);
      if (ratedWaterFlowRate) {
        result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, ratedWaterFlowRate.get());
      } else {
        resetRatedWaterFlowRate();
        result = true;
      }
      return result;
    }

    void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::resetRatedWaterFlowRate() {
      bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, "");
      OS_ASSERT(result);
    }

    void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::autosizeRatedWaterFlowRate() {
      bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatedTotalCoolingCapacity(boost::optional<double> ratedTotalCoolingCapacity) {
      bool result(false);
      if (ratedTotalCoolingCapacity) {
        result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedTotalCoolingCapacity, ratedTotalCoolingCapacity.get());
      } else {
        resetRatedTotalCoolingCapacity();
        result = true;
      }
      return result;
    }

    unsigned CoilCoolingWaterToAirHeatPumpEquationFit_Impl::airInletPort() const {
      return OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirInletNodeName;
    }

    unsigned CoilCoolingWaterToAirHeatPumpEquationFit_Impl::airOutletPort() const {
      return OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName;
    }

    unsigned CoilCoolingWaterToAirHeatPumpEquationFit_Impl::waterInletPort() const {
      return OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName;
    }

    unsigned CoilCoolingWaterToAirHeatPumpEquationFit_Impl::waterOutletPort() const {
      return OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName;
    }

    void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::resetRatedTotalCoolingCapacity() {
      bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedTotalCoolingCapacity, "");
      OS_ASSERT(result);
    }

    void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::autosizeRatedTotalCoolingCapacity() {
      bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedTotalCoolingCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatedSensibleCoolingCapacity(boost::optional<double> ratedSensibleCoolingCapacity) {
      bool result(false);
      if (ratedSensibleCoolingCapacity) {
        result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedSensibleCoolingCapacity, ratedSensibleCoolingCapacity.get());
      } else {
        resetRatedSensibleCoolingCapacity();
        result = true;
      }
      return result;
    }

    void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::resetRatedSensibleCoolingCapacity() {
      bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedSensibleCoolingCapacity, "");
      OS_ASSERT(result);
    }

    void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::autosizeRatedSensibleCoolingCapacity() {
      bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedSensibleCoolingCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatedCoolingCoefficientofPerformance(double ratedCoolingCoefficientofPerformance) {
      bool result =
        setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedCoolingCoefficientofPerformance, ratedCoolingCoefficientofPerformance);
      return result;
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatedEnteringWaterTemperature(double ratedEnteringWaterTemperature) {
      bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringWaterTemperature, ratedEnteringWaterTemperature);
      return result;
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatedEnteringAirDryBulbTemperature(double ratedEnteringAirDryBulbTemperature) {
      bool result =
        setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirDryBulbTemperature, ratedEnteringAirDryBulbTemperature);
      return result;
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatedEnteringAirWetBulbTemperature(double ratedEnteringAirWetBulbTemperature) {
      bool result =
        setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedEnteringAirWetBulbTemperature, ratedEnteringAirWetBulbTemperature);
      return result;
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin) {
      bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::NominalTimeforCondensateRemovaltoBegin,
                              nominalTimeforCondensateRemovaltoBegin);
      return result;
    }

    void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::resetNominalTimeforCondensateRemovaltoBegin() {
      bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::NominalTimeforCondensateRemovaltoBegin, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(
      double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity) {
      bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,
                              ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
      return result;
    }

    void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::resetRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() {
      bool result =
        setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity, "");
      OS_ASSERT(result);
    }

    boost::optional<HVACComponent> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitarySystem
      std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

      for (const auto& airLoopHVACUnitarySystem : airLoopHVACUnitarySystems) {
        if (boost::optional<HVACComponent> coolingCoil = airLoopHVACUnitarySystem.coolingCoil()) {
          if (coolingCoil->handle() == this->handle()) {
            return airLoopHVACUnitarySystem;
          }
        }
      }
      return boost::none;
    }

    boost::optional<ZoneHVACComponent> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::containingZoneHVACComponent() const {
      // ZoneHVACWaterToAirHeatPump

      std::vector<ZoneHVACWaterToAirHeatPump> zoneHVACWaterToAirHeatPumps;

      zoneHVACWaterToAirHeatPumps = this->model().getConcreteModelObjects<ZoneHVACWaterToAirHeatPump>();

      for (const auto& zoneHVACWaterToAirHeatPump : zoneHVACWaterToAirHeatPumps) {
        if (boost::optional<HVACComponent> coil = zoneHVACWaterToAirHeatPump.coolingCoil()) {
          if (coil->handle() == this->handle()) {
            return zoneHVACWaterToAirHeatPump;
          }
        }
      }

      return boost::none;
    }
    boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::autosizedRatedAirFlowRate() const {
      // EPLUS-SQL-INCONSISTENCY
      return getAutosizedValue("Design Size Rated Air Flow Rate", "m3/s", "COIL:COOLING:WATERTOAIRHEATPUMP:EQUATIONFIT");
    }

    boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::autosizedRatedWaterFlowRate() const {
      // EPLUS-SQL-INCONSISTENCY
      return getAutosizedValue("Design Size Rated Water Flow Rate", "m3/s", "COIL:COOLING:WATERTOAIRHEATPUMP:EQUATIONFIT");
    }

    boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::autosizedRatedTotalCoolingCapacity() const {
      // EPLUS-SQL-INCONSISTENCY
      return getAutosizedValue("Design Size Rated Total Cooling Capacity", "W", "COIL:COOLING:WATERTOAIRHEATPUMP:EQUATIONFIT");
    }

    boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::autosizedRatedSensibleCoolingCapacity() const {
      // EPLUS-SQL-INCONSISTENCY
      return getAutosizedValue("Design Size Rated Sensible Cooling Capacity", "W", "COIL:COOLING:WATERTOAIRHEATPUMP:EQUATIONFIT");
    }

    void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::autosize() {
      autosizeRatedAirFlowRate();
      autosizeRatedWaterFlowRate();
      autosizeRatedTotalCoolingCapacity();
      autosizeRatedSensibleCoolingCapacity();
    }

    void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedRatedAirFlowRate();
      if (val) {
        setRatedAirFlowRate(val.get());
      }

      val = autosizedRatedWaterFlowRate();
      if (val) {
        setRatedWaterFlowRate(val.get());
      }

      val = autosizedRatedTotalCoolingCapacity();
      if (val) {
        setRatedTotalCoolingCapacity(val.get());
      }

      val = autosizedRatedSensibleCoolingCapacity();
      if (val) {
        setRatedSensibleCoolingCapacity(val.get());
      }
    }

  }  // namespace detail

  // Ctors
  CoilCoolingWaterToAirHeatPumpEquationFit::CoilCoolingWaterToAirHeatPumpEquationFit(const Model& model,
                                                                                     const CurveQuadLinear& totalCoolingCapacityCurve,
                                                                                     const CurveQuintLinear& sensibleCoolingCapacityCurve,
                                                                                     const CurveQuadLinear& coolingPowerConsumptionCurve)
    : WaterToAirComponent(CoilCoolingWaterToAirHeatPumpEquationFit::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>());

    bool ok = setRatedEnteringWaterTemperature(30);
    OS_ASSERT(ok);

    ok = setRatedEnteringAirDryBulbTemperature(27);
    OS_ASSERT(ok);

    ok = setRatedEnteringAirWetBulbTemperature(19.0);
    OS_ASSERT(ok);

    ok = setTotalCoolingCapacityCurve(totalCoolingCapacityCurve);
    OS_ASSERT(ok);

    ok = setSensibleCoolingCapacityCurve(sensibleCoolingCapacityCurve);
    OS_ASSERT(ok);

    ok = setCoolingPowerConsumptionCurve(coolingPowerConsumptionCurve);
    OS_ASSERT(ok);
  }

  CoilCoolingWaterToAirHeatPumpEquationFit::CoilCoolingWaterToAirHeatPumpEquationFit(const Model& model)
    : WaterToAirComponent(CoilCoolingWaterToAirHeatPumpEquationFit::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>());

    bool ok = setRatedEnteringWaterTemperature(30);
    OS_ASSERT(ok);

    ok = setRatedEnteringAirDryBulbTemperature(27);
    OS_ASSERT(ok);

    ok = setRatedEnteringAirWetBulbTemperature(19.0);
    OS_ASSERT(ok);

    CurveQuadLinear totalCoolingCapacityCurve(model);
    totalCoolingCapacityCurve.setName("TotCoolCapCurve");
    totalCoolingCapacityCurve.setCoefficient1Constant(-0.68126221);
    totalCoolingCapacityCurve.setCoefficient2w(1.99529297);
    totalCoolingCapacityCurve.setCoefficient3x(-0.93611888);
    totalCoolingCapacityCurve.setCoefficient4y(0.02081177);
    totalCoolingCapacityCurve.setCoefficient5z(0.008438868);
    ok = setTotalCoolingCapacityCurve(totalCoolingCapacityCurve);
    OS_ASSERT(ok);

    CurveQuintLinear sensibleCoolingCapacityCurve(model);
    sensibleCoolingCapacityCurve.setCoefficient1Constant(2.24209455);
    sensibleCoolingCapacityCurve.setCoefficient2v(7.28913391);
    sensibleCoolingCapacityCurve.setCoefficient3w(-9.06079896);
    sensibleCoolingCapacityCurve.setCoefficient4x(-0.36729404);
    sensibleCoolingCapacityCurve.setCoefficient5y(0.218826161);
    sensibleCoolingCapacityCurve.setCoefficient6z(0.00901534);
    ok = setSensibleCoolingCapacityCurve(sensibleCoolingCapacityCurve);
    OS_ASSERT(ok);

    CurveQuadLinear coolingPowerConsumptionCurve(model);
    coolingPowerConsumptionCurve.setCoefficient1Constant(-3.20456384);
    coolingPowerConsumptionCurve.setCoefficient2w(0.47656454);
    coolingPowerConsumptionCurve.setCoefficient3x(3.16734236);
    coolingPowerConsumptionCurve.setCoefficient4y(0.10244637);
    coolingPowerConsumptionCurve.setCoefficient5z(-0.038132556);
    ok = setCoolingPowerConsumptionCurve(coolingPowerConsumptionCurve);
    OS_ASSERT(ok);
  }

  IddObjectType CoilCoolingWaterToAirHeatPumpEquationFit::iddObjectType() {
    return {IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_EquationFit};
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit::ratedAirFlowRate() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratedAirFlowRate();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedAirFlowRateDefaulted() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedAirFlowRateDefaulted();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedAirFlowRateAutosized() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedAirFlowRateAutosized();
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit::ratedWaterFlowRate() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratedWaterFlowRate();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedWaterFlowRateDefaulted() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedWaterFlowRateDefaulted();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedWaterFlowRateAutosized() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedWaterFlowRateAutosized();
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit::ratedTotalCoolingCapacity() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratedTotalCoolingCapacity();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedTotalCoolingCapacityDefaulted() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedTotalCoolingCapacityDefaulted();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedTotalCoolingCapacityAutosized() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedTotalCoolingCapacityAutosized();
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit::ratedSensibleCoolingCapacity() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratedSensibleCoolingCapacity();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedSensibleCoolingCapacityDefaulted() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedSensibleCoolingCapacityDefaulted();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedSensibleCoolingCapacityAutosized() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedSensibleCoolingCapacityAutosized();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::ratedCoolingCoefficientofPerformance() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratedCoolingCoefficientofPerformance();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedCoolingCoefficientofPerformanceDefaulted() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedCoolingCoefficientofPerformanceDefaulted();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::ratedEnteringWaterTemperature() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratedEnteringWaterTemperature();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::ratedEnteringAirDryBulbTemperature() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratedEnteringAirDryBulbTemperature();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::ratedEnteringAirWetBulbTemperature() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratedEnteringAirWetBulbTemperature();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::nominalTimeforCondensateRemovaltoBegin() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->nominalTimeforCondensateRemovaltoBegin();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isNominalTimeforCondensateRemovaltoBeginDefaulted() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isNominalTimeforCondensateRemovaltoBeginDefaulted();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacityDefaulted() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()
      ->isRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacityDefaulted();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatedAirFlowRate(double ratedAirFlowRate) {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::resetRatedAirFlowRate() {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedAirFlowRate();
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::autosizeRatedAirFlowRate() {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->autosizeRatedAirFlowRate();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatedWaterFlowRate(double ratedWaterFlowRate) {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatedWaterFlowRate(ratedWaterFlowRate);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::resetRatedWaterFlowRate() {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedWaterFlowRate();
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::autosizeRatedWaterFlowRate() {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->autosizeRatedWaterFlowRate();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity) {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatedTotalCoolingCapacity(ratedTotalCoolingCapacity);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::resetRatedTotalCoolingCapacity() {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedTotalCoolingCapacity();
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::autosizeRatedTotalCoolingCapacity() {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->autosizeRatedTotalCoolingCapacity();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatedSensibleCoolingCapacity(double ratedSensibleCoolingCapacity) {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatedSensibleCoolingCapacity(ratedSensibleCoolingCapacity);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::resetRatedSensibleCoolingCapacity() {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedSensibleCoolingCapacity();
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::autosizeRatedSensibleCoolingCapacity() {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->autosizeRatedSensibleCoolingCapacity();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatedCoolingCoefficientofPerformance(double ratedCoolingCoefficientofPerformance) {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatedCoolingCoefficientofPerformance(
      ratedCoolingCoefficientofPerformance);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatedEnteringWaterTemperature(double ratedEnteringAirDryBulbTemperature) {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatedEnteringWaterTemperature(ratedEnteringAirDryBulbTemperature);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatedEnteringAirDryBulbTemperature(double ratedEnteringAirDryBulbTemperature) {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatedEnteringAirDryBulbTemperature(
      ratedEnteringAirDryBulbTemperature);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatedEnteringAirWetBulbTemperature(double ratedEnteringAirWetBulbTemperature) {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatedEnteringAirWetBulbTemperature(
      ratedEnteringAirWetBulbTemperature);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin) {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setNominalTimeforCondensateRemovaltoBegin(
      nominalTimeforCondensateRemovaltoBegin);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::resetNominalTimeforCondensateRemovaltoBegin() {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->resetNominalTimeforCondensateRemovaltoBegin();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(
    double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity) {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(
      ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::resetRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->resetRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit::autosizedRatedAirFlowRate() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->autosizedRatedAirFlowRate();
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit::autosizedRatedWaterFlowRate() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->autosizedRatedWaterFlowRate();
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit::autosizedRatedTotalCoolingCapacity() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->autosizedRatedTotalCoolingCapacity();
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit::autosizedRatedSensibleCoolingCapacity() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->autosizedRatedSensibleCoolingCapacity();
  }

  CurveQuadLinear CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCurve() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCurve();
  }

  CurveQuintLinear CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCurve() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCurve();
  }

  CurveQuadLinear CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCurve() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCurve();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setTotalCoolingCapacityCurve(const CurveQuadLinear& totalCoolingCapacityCurve) {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setTotalCoolingCapacityCurve(totalCoolingCapacityCurve);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setSensibleCoolingCapacityCurve(const CurveQuintLinear& sensibleCoolingCapacityCurve) {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setSensibleCoolingCapacityCurve(sensibleCoolingCapacityCurve);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setCoolingPowerConsumptionCurve(const CurveQuadLinear& coolingPowerConsumptionCurve) {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setCoolingPowerConsumptionCurve(coolingPowerConsumptionCurve);
  }

  /// @cond
  CoilCoolingWaterToAirHeatPumpEquationFit::CoilCoolingWaterToAirHeatPumpEquationFit(
    std::shared_ptr<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl> impl)
    : WaterToAirComponent(std::move(impl)) {}

  // DEPRECATED

  double CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCoefficient1() const {
    LOG(Warn, "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCurve().coefficient1Constant() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCurve();
    return curve.coefficient1Constant();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setTotalCoolingCapacityCoefficient1(double totalCoolingCapacityCoefficient1) {
    LOG(Warn, "This method is deprecated. Use "
              "CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCurve().setCoefficient1Constant(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCurve();
    return curve.setCoefficient1Constant(totalCoolingCapacityCoefficient1);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCoefficient2() const {
    LOG(Warn, "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCurve().coefficient2w() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCurve();
    return curve.coefficient2w();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setTotalCoolingCapacityCoefficient2(double totalCoolingCapacityCoefficient2) {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCurve().setCoefficient2w(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCurve();
    return curve.setCoefficient2w(totalCoolingCapacityCoefficient2);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCoefficient3() const {
    LOG(Warn, "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCurve().coefficient3x() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCurve();
    return curve.coefficient3x();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setTotalCoolingCapacityCoefficient3(double totalCoolingCapacityCoefficient3) {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCurve().setCoefficient3x(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCurve();
    return curve.setCoefficient3x(totalCoolingCapacityCoefficient3);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCoefficient4() const {
    LOG(Warn, "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCurve().coefficient4y() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCurve();
    return curve.coefficient4y();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setTotalCoolingCapacityCoefficient4(double totalCoolingCapacityCoefficient4) {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCurve().setCoefficient4y(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCurve();
    return curve.setCoefficient4y(totalCoolingCapacityCoefficient4);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCoefficient5() const {
    LOG(Warn, "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCurve().coefficient5z() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCurve();
    return curve.coefficient5z();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setTotalCoolingCapacityCoefficient5(double totalCoolingCapacityCoefficient5) {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCurve().setCoefficient5z(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCurve();
    return curve.setCoefficient5z(totalCoolingCapacityCoefficient5);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCoefficient1() const {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCurve().coefficient1Constant() instead.")
    CurveQuintLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCurve();
    return curve.coefficient1Constant();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setSensibleCoolingCapacityCoefficient1(double sensibleCoolingCapacityCoefficient1) {
    LOG(Warn, "This method is deprecated. Use "
              "CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCurve().setCoefficient1Constant(double) instead.")
    CurveQuintLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCurve();
    return curve.setCoefficient1Constant(sensibleCoolingCapacityCoefficient1);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCoefficient2() const {
    LOG(Warn, "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCurve().coefficient2v() instead.")
    CurveQuintLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCurve();
    return curve.coefficient2v();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setSensibleCoolingCapacityCoefficient2(double sensibleCoolingCapacityCoefficient2) {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCurve().setCoefficient2v(double) instead.")
    CurveQuintLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCurve();
    return curve.setCoefficient2v(sensibleCoolingCapacityCoefficient2);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCoefficient3() const {
    LOG(Warn, "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCurve().coefficient3w() instead.")
    CurveQuintLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCurve();
    return curve.coefficient3w();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setSensibleCoolingCapacityCoefficient3(double sensibleCoolingCapacityCoefficient3) {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCurve().setCoefficient3w(double) instead.")
    CurveQuintLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCurve();
    return curve.setCoefficient3w(sensibleCoolingCapacityCoefficient3);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCoefficient4() const {
    LOG(Warn, "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCurve().coefficient4x() instead.")
    CurveQuintLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCurve();
    return curve.coefficient4x();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setSensibleCoolingCapacityCoefficient4(double sensibleCoolingCapacityCoefficient4) {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCurve().setCoefficient4x(double) instead.")
    CurveQuintLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCurve();
    return curve.setCoefficient4x(sensibleCoolingCapacityCoefficient4);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCoefficient5() const {
    LOG(Warn, "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCurve().coefficient5y() instead.")
    CurveQuintLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCurve();
    return curve.coefficient5y();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setSensibleCoolingCapacityCoefficient5(double sensibleCoolingCapacityCoefficient5) {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCurve().setCoefficient5y(double) instead.")
    CurveQuintLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCurve();
    return curve.setCoefficient5y(sensibleCoolingCapacityCoefficient5);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCoefficient6() const {
    LOG(Warn, "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCurve().coefficient6z() instead.")
    CurveQuintLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCurve();
    return curve.coefficient6z();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setSensibleCoolingCapacityCoefficient6(double sensibleCoolingCapacityCoefficient6) {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCurve().setCoefficient6z(double) instead.")
    CurveQuintLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCurve();
    return curve.setCoefficient6z(sensibleCoolingCapacityCoefficient6);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCoefficient1() const {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCurve().coefficient1Constant() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCurve();
    return curve.coefficient1Constant();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setCoolingPowerConsumptionCoefficient1(double coolingPowerConsumptionCoefficient1) {
    LOG(Warn, "This method is deprecated. Use "
              "CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCurve().setCoefficient1Constant(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCurve();
    return curve.setCoefficient1Constant(coolingPowerConsumptionCoefficient1);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCoefficient2() const {
    LOG(Warn, "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCurve().coefficient2w() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCurve();
    return curve.coefficient2w();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setCoolingPowerConsumptionCoefficient2(double coolingPowerConsumptionCoefficient2) {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCurve().setCoefficient2w(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCurve();
    return curve.setCoefficient2w(coolingPowerConsumptionCoefficient2);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCoefficient3() const {
    LOG(Warn, "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCurve().coefficient3x() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCurve();
    return curve.coefficient3x();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setCoolingPowerConsumptionCoefficient3(double coolingPowerConsumptionCoefficient3) {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCurve().setCoefficient3x(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCurve();
    return curve.setCoefficient3x(coolingPowerConsumptionCoefficient3);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCoefficient4() const {
    LOG(Warn, "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCurve().coefficient4y() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCurve();
    return curve.coefficient4y();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setCoolingPowerConsumptionCoefficient4(double coolingPowerConsumptionCoefficient4) {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCurve().setCoefficient4y(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCurve();
    return curve.setCoefficient4y(coolingPowerConsumptionCoefficient4);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCoefficient5() const {
    LOG(Warn, "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCurve().coefficient5z() instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCurve();
    return curve.coefficient5z();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setCoolingPowerConsumptionCoefficient5(double coolingPowerConsumptionCoefficient5) {
    LOG(Warn,
        "This method is deprecated. Use CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCurve().setCoefficient5z(double) instead.")
    CurveQuadLinear curve = getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCurve();
    return curve.setCoefficient5z(coolingPowerConsumptionCoefficient5);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
