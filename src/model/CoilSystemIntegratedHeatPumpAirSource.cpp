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
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"

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

      result.push_back(spaceCoolingCoil());

      result.push_back(spaceHeatingCoil());

      hvacComponent = dedicatedWaterHeatingCoil();
      if (hvacComponent) {
        result.push_back(hvacComponent.get());
      }

      hvacComponent = scwhCoil();
      if (hvacComponent) {
        result.push_back(hvacComponent.get());
      }

      straightComponent = scdwhCoolingCoil();
      if (straightComponent) {
        result.push_back(straightComponent.get());
      }

      hvacComponent = scdwhWaterHeatingCoil();
      if (hvacComponent) {
        result.push_back(hvacComponent.get());
      }

      straightComponent = shdwhHeatingCoil();
      if (straightComponent) {
        result.push_back(straightComponent.get());
      }

      hvacComponent = shdwhWaterHeatingCoil();
      if (hvacComponent) {
        result.push_back(hvacComponent.get());
      }

      return result;
    }

    ModelObject CoilSystemIntegratedHeatPumpAirSource_Impl::clone(Model model) const {
      auto newCoilSystem = StraightComponent_Impl::clone(model).cast<CoilSystemIntegratedHeatPumpAirSource>();

      StraightComponent spaceCoolingCoilClone = this->spaceCoolingCoil().clone(model).cast<StraightComponent>();
      newCoilSystem.setSpaceCoolingCoil(spaceCoolingCoilClone);

      StraightComponent spaceHeatingCoilClone = this->spaceHeatingCoil().clone(model).cast<StraightComponent>();
      newCoilSystem.setSpaceHeatingCoil(spaceHeatingCoilClone);

      if (boost::optional<HVACComponent> dedicatedWaterHeatingCoil = this->dedicatedWaterHeatingCoil()) {
        HVACComponent dedicatedWaterHeatingCoilClone = dedicatedWaterHeatingCoil->clone(model).cast<HVACComponent>();
        newCoilSystem.setDedicatedWaterHeatingCoil(dedicatedWaterHeatingCoilClone);
      }

      if (boost::optional<HVACComponent> scwhCoil = this->scwhCoil()) {
        HVACComponent scwhCoilClone = scwhCoil->clone(model).cast<HVACComponent>();
        newCoilSystem.setSCWHCoil(scwhCoilClone);
      }

      if (boost::optional<StraightComponent> scdwhCoolingCoil = this->scdwhCoolingCoil()) {
        StraightComponent scdwhCoolingCoilClone = scdwhCoolingCoil->clone(model).cast<StraightComponent>();
        newCoilSystem.setSCDWHCoolingCoil(scdwhCoolingCoilClone);
      }

      if (boost::optional<HVACComponent> scdwhWaterHeatingCoil = this->scdwhWaterHeatingCoil()) {
        HVACComponent scdwhWaterHeatingCoilClone = scdwhWaterHeatingCoil->clone(model).cast<HVACComponent>();
        newCoilSystem.setSCDWHWaterHeatingCoil(scdwhWaterHeatingCoilClone);
      }

      if (boost::optional<StraightComponent> shdwhHeatingCoil = this->shdwhHeatingCoil()) {
        StraightComponent shdwhHeatingCoilClone = shdwhHeatingCoil->clone(model).cast<StraightComponent>();
        newCoilSystem.setSHDWHHeatingCoil(shdwhHeatingCoilClone);
      }

      if (boost::optional<HVACComponent> shdwhWaterHeatingCoil = this->shdwhWaterHeatingCoil()) {
        HVACComponent shdwhWaterHeatingCoilClone = shdwhWaterHeatingCoil->clone(model).cast<HVACComponent>();
        newCoilSystem.setSHDWHWaterHeatingCoil(shdwhWaterHeatingCoilClone);
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

    StraightComponent CoilSystemIntegratedHeatPumpAirSource_Impl::spaceCoolingCoil() const {
      boost::optional<StraightComponent> value = optionalSpaceCoolingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Space Cooling Coil attached.");
      }
      return value.get();
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::optionalSpaceCoolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceCoolingCoil);
    }

    StraightComponent CoilSystemIntegratedHeatPumpAirSource_Impl::spaceHeatingCoil() const {
      boost::optional<StraightComponent> value = optionalSpaceHeatingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Space Heating Coil attached.");
      }
      return value.get();
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::optionalSpaceHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceHeatingCoil);
    }

    boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::dedicatedWaterHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(
        OS_CoilSystem_IntegratedHeatPump_AirSourceFields::DedicatedWaterHeatingCoil);
    }

    boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::scwhCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCWHCoil);
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::scdwhCoolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHCoolingCoil);
    }

    boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::scdwhWaterHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHWaterHeatingCoil);
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::shdwhHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHHeatingCoil);
    }

    boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::shdwhWaterHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHWaterHeatingCoil);
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::indoorTemperatureLimitForSCWHMode() const {
      boost::optional<double> value = getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::IndoorTemperatureLimitForSCWHMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::ambientTemperatureLimitForSCWHMode() const {
      boost::optional<double> value = getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::AmbientTemperatureLimitForSCWHMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::indoorTemperatureAboveWhichWHHasHigherPriority() const {
      boost::optional<double> value =
        getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::IndoorTemperatureAboveWhichWHHasHigherPriority, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::ambientTemperatureAboveWhichWHHasHigherPriority() const {
      boost::optional<double> value =
        getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::AmbientTemperatureAboveWhichWHHasHigherPriority, true);
      OS_ASSERT(value);
      return value.get();
    }

    int CoilSystemIntegratedHeatPumpAirSource_Impl::flagtoIndicateLoadControlInSCWHMode() const {
      boost::optional<int> value = getInt(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::FlagtoIndicateLoadControlInSCWHMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    int CoilSystemIntegratedHeatPumpAirSource_Impl::minimumSpeedLevelForSCWHMode() const {
      boost::optional<int> value = getInt(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelForSCWHMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode() const {
      boost::optional<double> value =
        getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::MaximumWaterFlowVolumeBeforeSwitchingFromSCDWHToSCWHMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    int CoilSystemIntegratedHeatPumpAirSource_Impl::minimumSpeedLevelForSCDWHMode() const {
      boost::optional<int> value = getInt(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelForSCDWHMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode() const {
      boost::optional<double> value =
        getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::MaximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    int CoilSystemIntegratedHeatPumpAirSource_Impl::minimumSpeedLevelForSHDWHMode() const {
      boost::optional<int> value = getInt(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelForSHDWHMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSpaceCoolingCoil(const StraightComponent& spaceCoolingCoil) {
      bool result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceCoolingCoil, spaceCoolingCoil.handle());
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSpaceHeatingCoil(const StraightComponent& spaceHeatingCoil) {
      bool result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceHeatingCoil, spaceHeatingCoil.handle());
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setDedicatedWaterHeatingCoil(const boost::optional<HVACComponent>& dedicatedWaterHeatingCoil) {
      bool result(false);
      if (dedicatedWaterHeatingCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::DedicatedWaterHeatingCoil, dedicatedWaterHeatingCoil.get().handle());
      } else {
        resetDedicatedWaterHeatingCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetDedicatedWaterHeatingCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::DedicatedWaterHeatingCoil, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSCWHCoil(const boost::optional<HVACComponent>& scwhCoil) {
      bool result(false);
      if (scwhCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCWHCoil, scwhCoil.get().handle());
      } else {
        resetSCWHCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetSCWHCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCWHCoil, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSCDWHCoolingCoil(const boost::optional<StraightComponent>& scdwhCoolingCoil) {
      bool result(false);
      if (scdwhCoolingCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHCoolingCoil, scdwhCoolingCoil.get().handle());
      } else {
        resetSCDWHCoolingCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetSCDWHCoolingCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHCoolingCoil, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSCDWHWaterHeatingCoil(const boost::optional<HVACComponent>& scdwhWaterHeatingCoil) {
      bool result(false);
      if (scdwhWaterHeatingCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHWaterHeatingCoil, scdwhWaterHeatingCoil.get().handle());
      } else {
        resetSCDWHWaterHeatingCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetSCDWHWaterHeatingCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHWaterHeatingCoil, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSHDWHHeatingCoil(const boost::optional<StraightComponent>& shdwhHeatingCoil) {
      bool result(false);
      if (shdwhHeatingCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHHeatingCoil, shdwhHeatingCoil.get().handle());
      } else {
        resetSHDWHHeatingCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetSHDWHHeatingCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHHeatingCoil, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSHDWHWaterHeatingCoil(const boost::optional<HVACComponent>& shdwhWaterHeatingCoil) {
      bool result(false);
      if (shdwhWaterHeatingCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHWaterHeatingCoil, shdwhWaterHeatingCoil.get().handle());
      } else {
        resetSHDWHWaterHeatingCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetSHDWHWaterHeatingCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHWaterHeatingCoil, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setIndoorTemperatureLimitForSCWHMode(double indoorTemperatureLimitForSCWHMode) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::IndoorTemperatureLimitForSCWHMode, indoorTemperatureLimitForSCWHMode);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setAmbientTemperatureLimitForSCWHMode(double ambientTemperatureLimitForSCWHMode) {
      bool result =
        setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::AmbientTemperatureLimitForSCWHMode, ambientTemperatureLimitForSCWHMode);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setIndoorTemperatureAboveWhichWHHasHigherPriority(
      double indoorTemperatureAboveWhichWHHasHigherPriority) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::IndoorTemperatureAboveWhichWHHasHigherPriority,
                              indoorTemperatureAboveWhichWHHasHigherPriority);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setAmbientTemperatureAboveWhichWHHasHigherPriority(
      double ambientTemperatureAboveWhichWHHasHigherPriority) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::AmbientTemperatureAboveWhichWHHasHigherPriority,
                              ambientTemperatureAboveWhichWHHasHigherPriority);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setFlagtoIndicateLoadControlInSCWHMode(int flagtoIndicateLoadControlInSCWHMode) {
      bool result =
        setInt(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::FlagtoIndicateLoadControlInSCWHMode, flagtoIndicateLoadControlInSCWHMode);
      OS_ASSERT(result);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setMinimumSpeedLevelForSCWHMode(int minimumSpeedLevelForSCWHMode) {
      bool result = setInt(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelForSCWHMode, minimumSpeedLevelForSCWHMode);
      OS_ASSERT(result);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setMaximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode(
      double maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::MaximumWaterFlowVolumeBeforeSwitchingFromSCDWHToSCWHMode,
                              maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setMinimumSpeedLevelForSCDWHMode(int minimumSpeedLevelForSCDWHMode) {
      bool result = setInt(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelForSCDWHMode, minimumSpeedLevelForSCDWHMode);
      OS_ASSERT(result);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setMaximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode(
      double maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode) {
      bool result =
        setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::MaximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode,
                  maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setMinimumSpeedLevelForSHDWHMode(int minimumSpeedLevelForSHDWHMode) {
      bool result = setInt(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelForSHDWHMode, minimumSpeedLevelForSHDWHMode);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  CoilSystemIntegratedHeatPumpAirSource::CoilSystemIntegratedHeatPumpAirSource(const Model& model, const StraightComponent& spaceCoolingCoil,
                                                                               const StraightComponent& spaceHeatingCoil)
    : StraightComponent(CoilSystemIntegratedHeatPumpAirSource::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>());

    setSpaceCoolingCoil(spaceCoolingCoil);
    setSpaceHeatingCoil(spaceHeatingCoil);
    setIndoorTemperatureLimitForSCWHMode(20.0);
    setAmbientTemperatureLimitForSCWHMode(27.0);
    setIndoorTemperatureAboveWhichWHHasHigherPriority(20.0);
    setAmbientTemperatureAboveWhichWHHasHigherPriority(20.0);
    setFlagtoIndicateLoadControlInSCWHMode(0);
    setMinimumSpeedLevelForSCWHMode(1);
    setMaximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode(0.0);
    setMinimumSpeedLevelForSCDWHMode(1);
    setMaximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode(360.0);
    setMinimumSpeedLevelForSHDWHMode(1);
  }

  IddObjectType CoilSystemIntegratedHeatPumpAirSource::iddObjectType() {
    return IddObjectType(IddObjectType::OS_CoilSystem_IntegratedHeatPump_AirSource);
  }

  StraightComponent CoilSystemIntegratedHeatPumpAirSource::spaceCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->spaceCoolingCoil();
  }

  StraightComponent CoilSystemIntegratedHeatPumpAirSource::spaceHeatingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->spaceHeatingCoil();
  }

  boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource::dedicatedWaterHeatingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->dedicatedWaterHeatingCoil();
  }

  boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource::scwhCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->scwhCoil();
  }

  boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource::scdwhCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->scdwhCoolingCoil();
  }

  boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource::scdwhWaterHeatingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->scdwhWaterHeatingCoil();
  }

  boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource::shdwhHeatingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->shdwhHeatingCoil();
  }

  boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource::shdwhWaterHeatingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->shdwhWaterHeatingCoil();
  }

  double CoilSystemIntegratedHeatPumpAirSource::indoorTemperatureLimitForSCWHMode() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->indoorTemperatureLimitForSCWHMode();
  }

  double CoilSystemIntegratedHeatPumpAirSource::ambientTemperatureLimitForSCWHMode() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->ambientTemperatureLimitForSCWHMode();
  }

  double CoilSystemIntegratedHeatPumpAirSource::indoorTemperatureAboveWhichWHHasHigherPriority() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->indoorTemperatureAboveWhichWHHasHigherPriority();
  }

  double CoilSystemIntegratedHeatPumpAirSource::ambientTemperatureAboveWhichWHHasHigherPriority() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->ambientTemperatureAboveWhichWHHasHigherPriority();
  }

  int CoilSystemIntegratedHeatPumpAirSource::flagtoIndicateLoadControlInSCWHMode() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->flagtoIndicateLoadControlInSCWHMode();
  }

  int CoilSystemIntegratedHeatPumpAirSource::minimumSpeedLevelForSCWHMode() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->minimumSpeedLevelForSCWHMode();
  }

  double CoilSystemIntegratedHeatPumpAirSource::maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode();
  }

  int CoilSystemIntegratedHeatPumpAirSource::minimumSpeedLevelForSCDWHMode() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->minimumSpeedLevelForSCDWHMode();
  }

  double CoilSystemIntegratedHeatPumpAirSource::maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode();
  }

  int CoilSystemIntegratedHeatPumpAirSource::minimumSpeedLevelForSHDWHMode() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->minimumSpeedLevelForSHDWHMode();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSpaceCoolingCoil(const StraightComponent& spaceCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSpaceCoolingCoil(spaceCoolingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSpaceHeatingCoil(const StraightComponent& spaceHeatingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSpaceHeatingCoil(spaceHeatingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setDedicatedWaterHeatingCoil(const HVACComponent& dedicatedWaterHeatingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setDedicatedWaterHeatingCoil(dedicatedWaterHeatingCoil);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetDedicatedWaterHeatingCoil() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetDedicatedWaterHeatingCoil();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSCWHCoil(const HVACComponent& scwhCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSCWHCoil(scwhCoil);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetSCWHCoil() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetSCWHCoil();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSCDWHCoolingCoil(const StraightComponent& scdwhCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSCDWHCoolingCoil(scdwhCoolingCoil);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetSCDWHCoolingCoil() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetSCDWHCoolingCoil();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSCDWHWaterHeatingCoil(const HVACComponent& scdwhWaterHeatingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSCDWHWaterHeatingCoil(scdwhWaterHeatingCoil);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetSCDWHWaterHeatingCoil() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetSCDWHWaterHeatingCoil();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSHDWHHeatingCoil(const StraightComponent& shdwhHeatingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSHDWHHeatingCoil(shdwhHeatingCoil);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetSHDWHHeatingCoil() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetSHDWHHeatingCoil();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSHDWHWaterHeatingCoil(const HVACComponent& shdwhWaterHeatingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSHDWHWaterHeatingCoil(shdwhWaterHeatingCoil);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetSHDWHWaterHeatingCoil() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetSHDWHWaterHeatingCoil();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setIndoorTemperatureLimitForSCWHMode(double indoorTemperatureLimitForSCWHMode) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setIndoorTemperatureLimitForSCWHMode(indoorTemperatureLimitForSCWHMode);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setAmbientTemperatureLimitForSCWHMode(double ambientTemperatureLimitForSCWHMode) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setAmbientTemperatureLimitForSCWHMode(ambientTemperatureLimitForSCWHMode);
  }

  bool
    CoilSystemIntegratedHeatPumpAirSource::setIndoorTemperatureAboveWhichWHHasHigherPriority(double indoorTemperatureAboveWhichWHHasHigherPriority) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setIndoorTemperatureAboveWhichWHHasHigherPriority(
      indoorTemperatureAboveWhichWHHasHigherPriority);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setAmbientTemperatureAboveWhichWHHasHigherPriority(
    double ambientTemperatureAboveWhichWHHasHigherPriority) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setAmbientTemperatureAboveWhichWHHasHigherPriority(
      ambientTemperatureAboveWhichWHHasHigherPriority);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setFlagtoIndicateLoadControlInSCWHMode(int flagtoIndicateLoadControlInSCWHMode) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setFlagtoIndicateLoadControlInSCWHMode(flagtoIndicateLoadControlInSCWHMode);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setMinimumSpeedLevelForSCWHMode(int minimumSpeedLevelForSCWHMode) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setMinimumSpeedLevelForSCWHMode(minimumSpeedLevelForSCWHMode);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setMaximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode(
    double maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setMaximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode(
      maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setMinimumSpeedLevelForSCDWHMode(int minimumSpeedLevelForSCDWHMode) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setMinimumSpeedLevelForSCDWHMode(minimumSpeedLevelForSCDWHMode);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setMaximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode(
    double maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setMaximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode(
      maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setMinimumSpeedLevelForSHDWHMode(int minimumSpeedLevelForSHDWHMode) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setMinimumSpeedLevelForSHDWHMode(minimumSpeedLevelForSHDWHMode);
  }

  /// @cond
  CoilSystemIntegratedHeatPumpAirSource::CoilSystemIntegratedHeatPumpAirSource(
    std::shared_ptr<detail::CoilSystemIntegratedHeatPumpAirSource_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
