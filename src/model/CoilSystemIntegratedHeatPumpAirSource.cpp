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

#include "CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "CoilSystemIntegratedHeatPumpAirSource_Impl.hpp"
#include "CoilCoolingDXVariableSpeed.hpp"
#include "CoilCoolingDXVariableSpeed_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"

#include <utilities/idd/OS_CoilSystem_IntegratedHeatPump_AirSource_FieldEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilSystemIntegratedHeatPumpAirSource_Impl::CoilSystemIntegratedHeatPumpAirSource_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                           bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilSystemIntegratedHeatPumpAirSource::iddObjectType());
    }

    CoilSystemIntegratedHeatPumpAirSource_Impl::CoilSystemIntegratedHeatPumpAirSource_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilSystemIntegratedHeatPumpAirSource::iddObjectType());
    }

    CoilSystemIntegratedHeatPumpAirSource_Impl::CoilSystemIntegratedHeatPumpAirSource_Impl(const CoilSystemIntegratedHeatPumpAirSource_Impl& other,
                                                                                           Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilSystemIntegratedHeatPumpAirSource_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;

      // TODO (not appropriate?)

      return result;
    }

    IddObjectType CoilSystemIntegratedHeatPumpAirSource_Impl::iddObjectType() const {
      return CoilSystemIntegratedHeatPumpAirSource::iddObjectType();
    }

    unsigned CoilSystemIntegratedHeatPumpAirSource_Impl::inletPort() const {
      return OS_CoilSystem_IntegratedHeatPump_AirSourceFields::AirInletNodeName;
    }

    unsigned CoilSystemIntegratedHeatPumpAirSource_Impl::outletPort() const {
      return OS_CoilSystem_IntegratedHeatPump_AirSourceFields::AirOutletNodeName;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::addToNode(Node& node) {
      return false;
    }

    std::vector<ModelObject> CoilSystemIntegratedHeatPumpAirSource_Impl::children() const {
      std::vector<ModelObject> result;

      boost::optional<StraightComponent> straightComponent;
      boost::optional<HVACComponent> hvacComponent;

      result.push_back(coolingCoil());

      straightComponent = heatingCoil();
      if (straightComponent) {
        result.push_back(straightComponent.get());
      }

      straightComponent = chillingCoil();
      if (straightComponent) {
        result.push_back(straightComponent.get());
      }

      hvacComponent = supplementalChillingCoil();
      if (hvacComponent) {
        result.push_back(hvacComponent.get());
      }

      straightComponent = storageTank();
      if (straightComponent) {
        result.push_back(straightComponent.get());
      }

      return result;
    }

    ModelObject CoilSystemIntegratedHeatPumpAirSource_Impl::clone(Model model) const {
      auto newCoilSystem = StraightComponent_Impl::clone(model).cast<CoilSystemIntegratedHeatPumpAirSource>();

      StraightComponent coilCoolingClone = this->coolingCoil().clone(model).cast<StraightComponent>();
      newCoilSystem.setCoolingCoil(coilCoolingClone);

      if (boost::optional<StraightComponent> heatingCoil = this->heatingCoil()) {
        StraightComponent heatingCoilClone = heatingCoil->clone(model).cast<StraightComponent>();
        newCoilSystem.setHeatingCoil(heatingCoilClone);
      }

      if (boost::optional<StraightComponent> chillingCoil = this->chillingCoil()) {
        StraightComponent chillingCoilClone = chillingCoil->clone(model).cast<StraightComponent>();
        newCoilSystem.setChillingCoil(chillingCoilClone);
      }

      if (boost::optional<HVACComponent> supplementalChillingCoil = this->supplementalChillingCoil()) {
        HVACComponent supplementalChillingCoilClone = supplementalChillingCoil->clone(model).cast<HVACComponent>();
        newCoilSystem.setSupplementalChillingCoil(supplementalChillingCoilClone);
      }

      if (boost::optional<StraightComponent> storageTank = this->storageTank()) {
        StraightComponent storageTankClone = storageTank->clone(model).cast<StraightComponent>();
        newCoilSystem.setStorageTank(storageTankClone);
      }

      return newCoilSystem;
    }

    boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitaryHeatPumpAirToAir

      std::vector<AirLoopHVACUnitaryHeatPumpAirToAir> airLoopHVACUnitaryHeatPumpAirToAirs =
        this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

      for (const auto& airLoopHVACUnitaryHeatPumpAirToAir : airLoopHVACUnitaryHeatPumpAirToAirs) {
        if (boost::optional<HVACComponent> coil = airLoopHVACUnitaryHeatPumpAirToAir.coolingCoil()) {
          if (coil->handle() == this->handle()) {
            return airLoopHVACUnitaryHeatPumpAirToAir;
          }
        } else if (boost::optional<HVACComponent> coil = airLoopHVACUnitaryHeatPumpAirToAir.heatingCoil()) {
          if (coil->handle() == this->handle()) {
            return airLoopHVACUnitaryHeatPumpAirToAir;
          }
        }
      }

      return boost::none;
    }

    StraightComponent CoilSystemIntegratedHeatPumpAirSource_Impl::coolingCoil() const {
      boost::optional<StraightComponent> value = optionalCoolingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Cooling Coil attached.");
      }
      return value.get();
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::optionalCoolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceCoolingCoil);
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::heatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceHeatingCoil);
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::enhancedDehumidificationCoolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(
        OS_CoilSystem_IntegratedHeatPump_AirSourceFields::EnhancedDehumidificationCoolingCoilName);
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::gridResponseCoolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(
        OS_CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseCoolingCoilName);
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::gridResponseHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(
        OS_CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseHeatingCoilName);
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::chillingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilName);
    }

    std::string CoilSystemIntegratedHeatPumpAirSource_Impl::chillingCoilBelongstoaSingleorSeparateUnit() const {
      boost::optional<std::string> value =
        getString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilBelongstoaSingleorSeparateUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::isChillingCoilBelongstoaSingleorSeprateUnitDefaulted() const {
      return isEmpty(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilBelongstoaSingleorSeparateUnit);
    }

    int CoilSystemIntegratedHeatPumpAirSource_Impl::chillingCoilCompressorRunSpeed() const {
      boost::optional<int> value = getInt(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilCompressorRunSpeed, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::isChillingCoilCompressorRunSpeedDefaulted() const {
      return isEmpty(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilCompressorRunSpeed);
    }

    boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::supplementalChillingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::WaterCoilName);
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::storageTank() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::TankName);
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::iceFractionBelowWhichChargingStarts() const {
      boost::optional<double> value = getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::IceFractionBelowWhichChargingStarts, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::isIceFractionBelowWhichChargingStartsDefaulted() const {
      return isEmpty(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::IceFractionBelowWhichChargingStarts);
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::chillerEnteringTemperatureatZeroTankFraction() const {
      boost::optional<double> value = getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerEnteringTemperatureatZeroTankFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::isChillerEnteringTemperatureatZeroTankFractionDefaulted() const {
      return isEmpty(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerEnteringTemperatureatZeroTankFraction);
    }

    boost::optional<Curve> CoilSystemIntegratedHeatPumpAirSource_Impl::temperatureDeviationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::TemperatureDeviationCurveName);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setCoolingCoil(const StraightComponent& coolingCoil) {
      bool result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceCoolingCoil, coolingCoil.handle());
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setHeatingCoil(const boost::optional<StraightComponent>& heatingCoil) {
      bool result(false);
      if (heatingCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceHeatingCoil, heatingCoil.get().handle());
      } else {
        resetHeatingCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetHeatingCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceHeatingCoil, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setEnhancedDehumidificationCoolingCoil(
      const boost::optional<StraightComponent>& enhancedDehumidificationCoolingCoil) {
      bool result(false);
      if (enhancedDehumidificationCoolingCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::EnhancedDehumidificationCoolingCoilName,
                            enhancedDehumidificationCoolingCoil.get().handle());
      } else {
        resetEnhancedDehumidificationCoolingCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetEnhancedDehumidificationCoolingCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::EnhancedDehumidificationCoolingCoilName, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setGridResponseCoolingCoil(const boost::optional<StraightComponent>& gridResponseCoolingCoil) {
      bool result(false);
      if (gridResponseCoolingCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseCoolingCoilName, gridResponseCoolingCoil.get().handle());
      } else {
        resetGridResponseCoolingCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetGridResponseCoolingCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseCoolingCoilName, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setGridResponseHeatingCoil(const boost::optional<StraightComponent>& gridResponseHeatingCoil) {
      bool result(false);
      if (gridResponseHeatingCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseHeatingCoilName, gridResponseHeatingCoil.get().handle());
      } else {
        resetGridResponseHeatingCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetGridResponseHeatingCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseHeatingCoilName, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setChillingCoil(const boost::optional<StraightComponent>& chillingCoil) {
      bool result(false);
      if (chillingCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilName, chillingCoil.get().handle());
      } else {
        resetChillingCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetChillingCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilName, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setChillingCoilBelongstoaSingleorSeparateUnit(
      std::string chillingCoilBelongstoaSingleorSeparateUnit) {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilBelongstoaSingleorSeparateUnit,
                              chillingCoilBelongstoaSingleorSeparateUnit);
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetChillingCoilBelongstoaSingleorSeparateUnit() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilBelongstoaSingleorSeparateUnit, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setChillingCoilCompressorRunSpeed(int chillingCoilCompressorRunSpeed) {
      bool result = setInt(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilCompressorRunSpeed, chillingCoilCompressorRunSpeed);
      OS_ASSERT(result);
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetChillingCoilCompressorRunSpeed() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilCompressorRunSpeed, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSupplementalChillingCoil(const boost::optional<HVACComponent>& supplementalChillingCoil) {
      bool result(false);
      if (supplementalChillingCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::WaterCoilName, supplementalChillingCoil.get().handle());
      } else {
        resetSupplementalChillingCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetSupplementalChillingCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::WaterCoilName, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setStorageTank(const boost::optional<StraightComponent>& storageTank) {
      bool result(false);
      if (storageTank) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::TankName, storageTank.get().handle());
      } else {
        resetStorageTank();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetStorageTank() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::TankName, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setIceFractionBelowWhichChargingStarts(double iceFractionBelowWhichChargingStarts) {
      bool result =
        setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::IceFractionBelowWhichChargingStarts, iceFractionBelowWhichChargingStarts);
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetIceFractionBelowWhichChargingStarts() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::IceFractionBelowWhichChargingStarts, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setChillerEnteringTemperatureatZeroTankFraction(
      double chillerEnteringTemperatureatZeroTankFraction) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerEnteringTemperatureatZeroTankFraction,
                              chillerEnteringTemperatureatZeroTankFraction);
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetChillerEnteringTemperatureatZeroTankFraction() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerEnteringTemperatureatZeroTankFraction, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setTemperatureDeviationCurve(const boost::optional<Curve>& temperatureDeviationCurve) {
      bool result(false);
      if (temperatureDeviationCurve) {
        result =
          setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::TemperatureDeviationCurveName, temperatureDeviationCurve.get().handle());
      } else {
        resetTemperatureDeviationCurve();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetTemperatureDeviationCurve() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::TemperatureDeviationCurveName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  CoilSystemIntegratedHeatPumpAirSource::CoilSystemIntegratedHeatPumpAirSource(const Model& model)
    : StraightComponent(CoilSystemIntegratedHeatPumpAirSource::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>());

    CoilCoolingDXVariableSpeed coolingCoil(model);
    setCoolingCoil(coolingCoil);
  }

  IddObjectType CoilSystemIntegratedHeatPumpAirSource::iddObjectType() {
    return IddObjectType(IddObjectType::OS_CoilSystem_IntegratedHeatPump_AirSource);
  }

  StraightComponent CoilSystemIntegratedHeatPumpAirSource::coolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->coolingCoil();
  }

  boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource::heatingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->heatingCoil();
  }

  boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource::enhancedDehumidificationCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->enhancedDehumidificationCoolingCoil();
  }

  boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource::gridResponseCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->gridResponseCoolingCoil();
  }

  boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource::gridResponseHeatingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->gridResponseHeatingCoil();
  }

  boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource::chillingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->chillingCoil();
  }

  std::string CoilSystemIntegratedHeatPumpAirSource::chillingCoilBelongstoaSingleorSeparateUnit() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->chillingCoilBelongstoaSingleorSeparateUnit();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::isChillingCoilBelongstoaSingleorSeprateUnitDefaulted() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->isChillingCoilBelongstoaSingleorSeprateUnitDefaulted();
  }

  int CoilSystemIntegratedHeatPumpAirSource::chillingCoilCompressorRunSpeed() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->chillingCoilCompressorRunSpeed();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::isChillingCoilCompressorRunSpeedDefaulted() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->isChillingCoilCompressorRunSpeedDefaulted();
  }

  boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource::supplementalChillingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->supplementalChillingCoil();
  }

  boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource::storageTank() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->storageTank();
  }

  double CoilSystemIntegratedHeatPumpAirSource::iceFractionBelowWhichChargingStarts() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->iceFractionBelowWhichChargingStarts();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::isIceFractionBelowWhichChargingStartsDefaulted() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->isIceFractionBelowWhichChargingStartsDefaulted();
  }

  double CoilSystemIntegratedHeatPumpAirSource::chillerEnteringTemperatureatZeroTankFraction() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->chillerEnteringTemperatureatZeroTankFraction();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::isChillerEnteringTemperatureatZeroTankFractionDefaulted() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->isChillerEnteringTemperatureatZeroTankFractionDefaulted();
  }

  boost::optional<Curve> CoilSystemIntegratedHeatPumpAirSource::temperatureDeviationCurve() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->temperatureDeviationCurve();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setCoolingCoil(const StraightComponent& coolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setCoolingCoil(coolingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setHeatingCoil(const StraightComponent& heatingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setHeatingCoil(heatingCoil);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetHeatingCoil() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetHeatingCoil();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setEnhancedDehumidificationCoolingCoil(const StraightComponent& enhancedDehumidificationCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setEnhancedDehumidificationCoolingCoil(enhancedDehumidificationCoolingCoil);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetEnhancedDehumidificationCoolingCoil() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetEnhancedDehumidificationCoolingCoil();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setGridResponseCoolingCoil(const StraightComponent& gridResponseCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setGridResponseCoolingCoil(gridResponseCoolingCoil);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetGridResponseCoolingCoil() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetGridResponseCoolingCoil();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setGridResponseHeatingCoil(const StraightComponent& gridResponseHeatingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setGridResponseHeatingCoil(gridResponseHeatingCoil);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetGridResponseHeatingCoil() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetGridResponseHeatingCoil();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setChillingCoil(const StraightComponent& chillingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setChillingCoil(chillingCoil);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetChillingCoil() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetChillingCoil();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setChillingCoilBelongstoaSingleorSeparateUnit(std::string chillingCoilBelongstoaSingleorSeparateUnit) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setChillingCoilBelongstoaSingleorSeparateUnit(
      chillingCoilBelongstoaSingleorSeparateUnit);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetChillingCoilBelongstoaSingleorSeparateUnit() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetChillingCoilBelongstoaSingleorSeparateUnit();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setChillingCoilCompressorRunSpeed(int chillingCoilCompressorRunSpeed) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setChillingCoilCompressorRunSpeed(chillingCoilCompressorRunSpeed);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetChillingCoilCompressorRunSpeed() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetChillingCoilCompressorRunSpeed();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSupplementalChillingCoil(const HVACComponent& supplementalChillingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSupplementalChillingCoil(supplementalChillingCoil);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetSupplementalChillingCoil() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetSupplementalChillingCoil();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setStorageTank(const StraightComponent& storageTank) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setStorageTank(storageTank);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetStorageTank() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetStorageTank();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setIceFractionBelowWhichChargingStarts(double iceFractionBelowWhichChargingStarts) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setIceFractionBelowWhichChargingStarts(iceFractionBelowWhichChargingStarts);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetIceFractionBelowWhichChargingStarts() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetIceFractionBelowWhichChargingStarts();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setChillerEnteringTemperatureatZeroTankFraction(double chillerEnteringTemperatureatZeroTankFraction) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setChillerEnteringTemperatureatZeroTankFraction(
      chillerEnteringTemperatureatZeroTankFraction);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetChillerEnteringTemperatureatZeroTankFraction() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetChillerEnteringTemperatureatZeroTankFraction();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setTemperatureDeviationCurve(const Curve& temperatureDeviationCurve) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setTemperatureDeviationCurve(temperatureDeviationCurve);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetTemperatureDeviationCurve() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetTemperatureDeviationCurve();
  }

  /// @cond
  CoilSystemIntegratedHeatPumpAirSource::CoilSystemIntegratedHeatPumpAirSource(
    std::shared_ptr<detail::CoilSystemIntegratedHeatPumpAirSource_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
