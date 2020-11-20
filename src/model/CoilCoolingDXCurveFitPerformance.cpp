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

#include "CoilCoolingDXCurveFitPerformance.hpp"
#include "CoilCoolingDXCurveFitPerformance_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "CoilCoolingDX.hpp"
#include "CoilCoolingDX_Impl.hpp"
#include "CoilCoolingDXCurveFitOperatingMode.hpp"
#include "CoilCoolingDXCurveFitOperatingMode_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_Cooling_DX_CurveFit_Performance_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilCoolingDXCurveFitPerformance_Impl::CoilCoolingDXCurveFitPerformance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXCurveFitPerformance::iddObjectType());
    }

    CoilCoolingDXCurveFitPerformance_Impl::CoilCoolingDXCurveFitPerformance_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilCoolingDXCurveFitPerformance::iddObjectType());
    }

    CoilCoolingDXCurveFitPerformance_Impl::CoilCoolingDXCurveFitPerformance_Impl(const CoilCoolingDXCurveFitPerformance_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilCoolingDXCurveFitPerformance_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType CoilCoolingDXCurveFitPerformance_Impl::iddObjectType() const {
      return CoilCoolingDXCurveFitPerformance::iddObjectType();
    }

    std::vector<ScheduleTypeKey> CoilCoolingDXCurveFitPerformance_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterOperatingSchedule) != e) {
        result.push_back(ScheduleTypeKey("CoilCoolingDXCurveFitPerformance", "Evaporative Condenser Basin Heater Operating Schedule"));
      }
      return result;
    }

    ModelObject CoilCoolingDXCurveFitPerformance_Impl::clone(Model model) const {
      // clone the operating modes is already handle in ModelObject_Impl::clone since they are ResourceObjects
      // We don't do ParentObject_Impl::clone since it'll also CLONE the children...
      return ModelObject_Impl::clone(model);
    }

    std::vector<ModelObject> CoilCoolingDXCurveFitPerformance_Impl::children() const {
      std::vector<ModelObject> result;

      // These are ResourceObjects, so they shouldn't really be children except for OS App / IG
      result.push_back(baseOperatingMode());
      if (auto _mode = alternativeOperatingMode1()) {
        result.push_back(_mode.get());
      }
      if (auto _mode = alternativeOperatingMode2()) {
        result.push_back(_mode.get());
      }

      return result;
    }

    std::vector<IdfObject> CoilCoolingDXCurveFitPerformance_Impl::remove() {
      if (!coilCoolingDXs().empty()) {
        LOG(Warn, "Cannot remove object because it is used by at least one CoilCoolingDX as a required field");
        return std::vector<IdfObject>();
      }
      return ResourceObject_Impl::remove();
    }

    double CoilCoolingDXCurveFitPerformance_Impl::crankcaseHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::CrankcaseHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXCurveFitPerformance_Impl::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXCurveFitPerformance_Impl::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXCurveFitPerformance_Impl::unitInternalStaticAirPressure() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::UnitInternalStaticAirPressure, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string CoilCoolingDXCurveFitPerformance_Impl::capacityControlMethod() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::CapacityControlMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXCurveFitPerformance_Impl::evaporativeCondenserBasinHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXCurveFitPerformance_Impl::evaporativeCondenserBasinHeaterSetpointTemperature() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    Schedule CoilCoolingDXCurveFitPerformance_Impl::evaporativeCondenserBasinHeaterOperatingSchedule() const {
      boost::optional<Schedule> schedule = getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterOperatingSchedule);
      OS_ASSERT(schedule);
      return schedule.get();
    }

    std::string CoilCoolingDXCurveFitPerformance_Impl::compressorFuelType() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::CompressorFuelType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<CoilCoolingDXCurveFitOperatingMode> CoilCoolingDXCurveFitPerformance_Impl::optionalBaseOperatingMode() const {
      return getObject<ModelObject>().getModelObjectTarget<CoilCoolingDXCurveFitOperatingMode>(
        OS_Coil_Cooling_DX_CurveFit_PerformanceFields::BaseOperatingMode);
    }

    CoilCoolingDXCurveFitOperatingMode CoilCoolingDXCurveFitPerformance_Impl::baseOperatingMode() const {
      boost::optional<CoilCoolingDXCurveFitOperatingMode> value = optionalBaseOperatingMode();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Base Operating Mode attached.");
      }
      return value.get();
    }

    boost::optional<CoilCoolingDXCurveFitOperatingMode> CoilCoolingDXCurveFitPerformance_Impl::alternativeOperatingMode1() const {
      return getObject<ModelObject>().getModelObjectTarget<CoilCoolingDXCurveFitOperatingMode>(
        OS_Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode1);
    }

    boost::optional<CoilCoolingDXCurveFitOperatingMode> CoilCoolingDXCurveFitPerformance_Impl::alternativeOperatingMode2() const {
      return getObject<ModelObject>().getModelObjectTarget<CoilCoolingDXCurveFitOperatingMode>(
        OS_Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode2);
    }

    std::vector<CoilCoolingDX> CoilCoolingDXCurveFitPerformance_Impl::coilCoolingDXs() const {
      return getObject<ModelObject>().getModelObjectSources<CoilCoolingDX>(CoilCoolingDX::iddObjectType());
    }

    bool CoilCoolingDXCurveFitPerformance_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
      bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
      return result;
    }

    bool CoilCoolingDXCurveFitPerformance_Impl::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
      double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
      bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,
                              minimumOutdoorDryBulbTemperatureforCompressorOperation);
      return result;
    }

    bool CoilCoolingDXCurveFitPerformance_Impl::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(
      double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
      bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,
                              maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
      return result;
    }

    bool CoilCoolingDXCurveFitPerformance_Impl::setUnitInternalStaticAirPressure(double unitInternalStaticAirPressure) {
      bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::UnitInternalStaticAirPressure, unitInternalStaticAirPressure);
      return result;
    }

    bool CoilCoolingDXCurveFitPerformance_Impl::setCapacityControlMethod(const std::string& capacityControlMethod) {
      bool result = setString(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::CapacityControlMethod, capacityControlMethod);
      return result;
    }

    bool CoilCoolingDXCurveFitPerformance_Impl::setEvaporativeCondenserBasinHeaterCapacity(double evaporativeCondenserBasinHeaterCapacity) {
      bool result =
        setDouble(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterCapacity, evaporativeCondenserBasinHeaterCapacity);
      return result;
    }

    bool CoilCoolingDXCurveFitPerformance_Impl::setEvaporativeCondenserBasinHeaterSetpointTemperature(
      double evaporativeCondenserBasinHeaterSetpointTemperature) {
      bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterSetpointTemperature,
                              evaporativeCondenserBasinHeaterSetpointTemperature);
      return result;
    }

    bool CoilCoolingDXCurveFitPerformance_Impl::setEvaporativeCondenserBasinHeaterOperatingSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterOperatingSchedule,
                                "CoilCoolingDXCurveFitPerformance", "Evaporative Condenser Basin Heater Operating Schedule", schedule);
      return result;
    }

    bool CoilCoolingDXCurveFitPerformance_Impl::setCompressorFuelType(const std::string& compressorFuelType) {
      bool result = setString(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::CompressorFuelType, compressorFuelType);
      return result;
    }

    bool CoilCoolingDXCurveFitPerformance_Impl::setBaseOperatingMode(const CoilCoolingDXCurveFitOperatingMode& baseOperatingMode) {
      bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::BaseOperatingMode, baseOperatingMode.handle());
      return result;
    }

    bool CoilCoolingDXCurveFitPerformance_Impl::setAlternativeOperatingMode1(const CoilCoolingDXCurveFitOperatingMode& alternativeOperatingMode1) {
      bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode1, alternativeOperatingMode1.handle());
      return result;
    }

    void CoilCoolingDXCurveFitPerformance_Impl::resetAlternativeOperatingMode1() {
      bool result = setString(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode1, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXCurveFitPerformance_Impl::setAlternativeOperatingMode2(const CoilCoolingDXCurveFitOperatingMode& alternativeOperatingMode2) {
      bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode2, alternativeOperatingMode2.handle());
      return result;
    }

    void CoilCoolingDXCurveFitPerformance_Impl::resetAlternativeOperatingMode2() {
      bool result = setString(OS_Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode2, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  CoilCoolingDXCurveFitPerformance::CoilCoolingDXCurveFitPerformance(const Model& model, const CoilCoolingDXCurveFitOperatingMode& baseOperatingMode)
    : ResourceObject(CoilCoolingDXCurveFitPerformance::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>());

    bool ok = setCrankcaseHeaterCapacity(0.0);
    OS_ASSERT(ok);
    ok = setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-25.0);
    OS_ASSERT(ok);
    ok = setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(10.0);
    OS_ASSERT(ok);
    ok = setUnitInternalStaticAirPressure(773.3);
    OS_ASSERT(ok);
    ok = setCapacityControlMethod("Discrete");
    OS_ASSERT(ok);
    ok = setEvaporativeCondenserBasinHeaterCapacity(0.0);
    OS_ASSERT(ok);
    ok = setEvaporativeCondenserBasinHeaterSetpointTemperature(2.0);
    OS_ASSERT(ok);
    {
      auto schedule = model.alwaysOnDiscreteSchedule();
      setEvaporativeCondenserBasinHeaterOperatingSchedule(schedule);
    }
    setCompressorFuelType("Electricity");

    ok = setBaseOperatingMode(baseOperatingMode);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s baseOperatingMode to " << baseOperatingMode.briefDescription() << ".");
    }
  }

  IddObjectType CoilCoolingDXCurveFitPerformance::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Coil_Cooling_DX_CurveFit_Performance);
  }

  std::vector<std::string> CoilCoolingDXCurveFitPerformance::capacityControlMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Coil_Cooling_DX_CurveFit_PerformanceFields::CapacityControlMethod);
  }

  std::vector<std::string> CoilCoolingDXCurveFitPerformance::validCapacityControlMethodValues() {
    return capacityControlMethodValues();
  }

  std::vector<std::string> CoilCoolingDXCurveFitPerformance::compressorFuelTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Coil_Cooling_DX_CurveFit_PerformanceFields::CompressorFuelType);
  }

  std::vector<std::string> CoilCoolingDXCurveFitPerformance::validCompressorFuelTypeValues() {
    return compressorFuelTypeValues();
  }

  double CoilCoolingDXCurveFitPerformance::crankcaseHeaterCapacity() const {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->crankcaseHeaterCapacity();
  }

  double CoilCoolingDXCurveFitPerformance::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->minimumOutdoorDryBulbTemperatureforCompressorOperation();
  }

  double CoilCoolingDXCurveFitPerformance::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();
  }

  double CoilCoolingDXCurveFitPerformance::unitInternalStaticAirPressure() const {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->unitInternalStaticAirPressure();
  }

  std::string CoilCoolingDXCurveFitPerformance::capacityControlMethod() const {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->capacityControlMethod();
  }

  double CoilCoolingDXCurveFitPerformance::evaporativeCondenserBasinHeaterCapacity() const {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->evaporativeCondenserBasinHeaterCapacity();
  }

  double CoilCoolingDXCurveFitPerformance::evaporativeCondenserBasinHeaterSetpointTemperature() const {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->evaporativeCondenserBasinHeaterSetpointTemperature();
  }

  Schedule CoilCoolingDXCurveFitPerformance::evaporativeCondenserBasinHeaterOperatingSchedule() const {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->evaporativeCondenserBasinHeaterOperatingSchedule();
  }

  std::string CoilCoolingDXCurveFitPerformance::compressorFuelType() const {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->compressorFuelType();
  }

  CoilCoolingDXCurveFitOperatingMode CoilCoolingDXCurveFitPerformance::baseOperatingMode() const {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->baseOperatingMode();
  }

  boost::optional<CoilCoolingDXCurveFitOperatingMode> CoilCoolingDXCurveFitPerformance::alternativeOperatingMode1() const {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->alternativeOperatingMode1();
  }

  boost::optional<CoilCoolingDXCurveFitOperatingMode> CoilCoolingDXCurveFitPerformance::alternativeOperatingMode2() const {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->alternativeOperatingMode2();
  }

  std::vector<CoilCoolingDX> CoilCoolingDXCurveFitPerformance::coilCoolingDXs() const {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->coilCoolingDXs();
  }

  bool CoilCoolingDXCurveFitPerformance::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
  }

  bool CoilCoolingDXCurveFitPerformance::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
    double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
      minimumOutdoorDryBulbTemperatureforCompressorOperation);
  }

  bool CoilCoolingDXCurveFitPerformance::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(
    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(
      maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
  }

  bool CoilCoolingDXCurveFitPerformance::setUnitInternalStaticAirPressure(double unitInternalStaticAirPressure) {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->setUnitInternalStaticAirPressure(unitInternalStaticAirPressure);
  }

  bool CoilCoolingDXCurveFitPerformance::setCapacityControlMethod(const std::string& capacityControlMethod) {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->setCapacityControlMethod(capacityControlMethod);
  }

  bool CoilCoolingDXCurveFitPerformance::setEvaporativeCondenserBasinHeaterCapacity(double evaporativeCondenserBasinHeaterCapacity) {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->setEvaporativeCondenserBasinHeaterCapacity(
      evaporativeCondenserBasinHeaterCapacity);
  }

  bool CoilCoolingDXCurveFitPerformance::setEvaporativeCondenserBasinHeaterSetpointTemperature(
    double evaporativeCondenserBasinHeaterSetpointTemperature) {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->setEvaporativeCondenserBasinHeaterSetpointTemperature(
      evaporativeCondenserBasinHeaterSetpointTemperature);
  }

  bool CoilCoolingDXCurveFitPerformance::setEvaporativeCondenserBasinHeaterOperatingSchedule(Schedule& schedule) {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->setEvaporativeCondenserBasinHeaterOperatingSchedule(schedule);
  }

  bool CoilCoolingDXCurveFitPerformance::setCompressorFuelType(const std::string& compressorFuelType) {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->setCompressorFuelType(compressorFuelType);
  }

  bool CoilCoolingDXCurveFitPerformance::setBaseOperatingMode(const CoilCoolingDXCurveFitOperatingMode& baseOperatingMode) {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->setBaseOperatingMode(baseOperatingMode);
  }

  bool CoilCoolingDXCurveFitPerformance::setAlternativeOperatingMode1(const CoilCoolingDXCurveFitOperatingMode& alternativeOperatingMode1) {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->setAlternativeOperatingMode1(alternativeOperatingMode1);
  }

  void CoilCoolingDXCurveFitPerformance::resetAlternativeOperatingMode1() {
    getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->resetAlternativeOperatingMode1();
  }

  bool CoilCoolingDXCurveFitPerformance::setAlternativeOperatingMode2(const CoilCoolingDXCurveFitOperatingMode& alternativeOperatingMode2) {
    return getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->setAlternativeOperatingMode2(alternativeOperatingMode2);
  }

  void CoilCoolingDXCurveFitPerformance::resetAlternativeOperatingMode2() {
    getImpl<detail::CoilCoolingDXCurveFitPerformance_Impl>()->resetAlternativeOperatingMode2();
  }

  /// @cond
  CoilCoolingDXCurveFitPerformance::CoilCoolingDXCurveFitPerformance(std::shared_ptr<detail::CoilCoolingDXCurveFitPerformance_Impl> impl)
    : ResourceObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
