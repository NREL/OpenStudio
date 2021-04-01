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
#include "CoilHeatingDXVariableSpeed.hpp"
#include "CoilHeatingDXVariableSpeed_Impl.hpp"
#include "CoilCoolingDXVariableSpeed.hpp"
#include "CoilCoolingDXVariableSpeed_Impl.hpp"
#include "CoilWaterHeatingAirToWaterHeatPumpVariableSpeed.hpp"
#include "CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl.hpp"
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
#include "WaterHeaterHeatPump.hpp"
#include "WaterHeaterHeatPump_Impl.hpp"

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
      static const std::vector<std::string> result{"Integrated Heat Pump Air Loop Mass Flow Rate",
                                                   "Integrated Heat Pump Condenser Water Mass Flow Rate",
                                                   "Integrated Heat Pump Air Total Cooling Rate",
                                                   "Integrated Heat Pump Air Heating Rate",
                                                   "Integrated Heat Pump Water Heating Rate",
                                                   "Integrated Heat Pump Electricity Rate",
                                                   "Integrated Heat Pump Air Latent Cooling Rate",
                                                   "Integrated Heat Pump Source Heat Transfer Rate",
                                                   "Integrated Heat Pump COP",
                                                   "Integrated Heat Pump Electricity Energy",
                                                   "Integrated Heat Pump Air Total Cooling Energy",
                                                   "Integrated Heat Pump Air Heating Energy",
                                                   "Integrated Heat Pump Water Heating Energy",
                                                   "Integrated Heat Pump Air Latent Cooling Energy",
                                                   "Integrated Heat Pump Source Heat Transfer Energy"};

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

      // Using the optional version so that if remove() is called when an assignment fails in Ctor, then it doesn't crash
      straightComponent = optionalSpaceCoolingCoil();
      if (straightComponent) {
        result.push_back(straightComponent.get());
      }

      straightComponent = optionalSpaceHeatingCoil();
      if (straightComponent) {
        result.push_back(straightComponent.get());
      }

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

      straightComponent = enhancedDehumidificationCoolingCoil();
      if (straightComponent) {
        result.push_back(straightComponent.get());
      }

      straightComponent = gridResponseCoolingCoil();
      if (straightComponent) {
        result.push_back(straightComponent.get());
      }

      straightComponent = gridResponseHeatingCoil();
      if (straightComponent) {
        result.push_back(straightComponent.get());
      }

      straightComponent = chillerCoil();
      if (straightComponent) {
        result.push_back(straightComponent.get());
      }

      hvacComponent = supplementalChillerCoil();
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

      if (boost::optional<StraightComponent> enhancedDehumidificationCoolingCoil = this->enhancedDehumidificationCoolingCoil()) {
        StraightComponent enhancedDehumidificationCoolingCoilClone = enhancedDehumidificationCoolingCoil->clone(model).cast<StraightComponent>();
        newCoilSystem.setEnhancedDehumidificationCoolingCoil(enhancedDehumidificationCoolingCoilClone);
      }

      if (boost::optional<StraightComponent> gridResponseCoolingCoil = this->gridResponseCoolingCoil()) {
        StraightComponent gridResponseCoolingCoilClone = gridResponseCoolingCoil->clone(model).cast<StraightComponent>();
        newCoilSystem.setGridResponseCoolingCoil(gridResponseCoolingCoilClone);
      }

      if (boost::optional<StraightComponent> gridResponseHeatingCoil = this->gridResponseHeatingCoil()) {
        StraightComponent gridResponseHeatingCoilClone = gridResponseHeatingCoil->clone(model).cast<StraightComponent>();
        newCoilSystem.setGridResponseHeatingCoil(gridResponseHeatingCoilClone);
      }

      if (boost::optional<StraightComponent> chillerCoil = this->chillerCoil()) {
        StraightComponent chillerCoilClone = chillerCoil->clone(model).cast<StraightComponent>();
        newCoilSystem.setChillerCoil(chillerCoilClone);
      }

      if (boost::optional<HVACComponent> supplementalChillerCoil = this->supplementalChillerCoil()) {
        HVACComponent supplementalChillerCoilClone = supplementalChillerCoil->clone(model).cast<HVACComponent>();
        newCoilSystem.setSupplementalChillerCoil(supplementalChillerCoilClone);
      }

      if (boost::optional<StraightComponent> storageTank = this->storageTank()) {
        StraightComponent storageTankClone = storageTank->clone(model).cast<StraightComponent>();
        newCoilSystem.setStorageTank(storageTankClone);
      }

      return newCoilSystem;
    }

    boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitaryHeatPumpAirToAir

      auto airLoopHVACUnitaryHeatPumpAirToAirs = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();
      for (const auto& airLoopHVACUnitaryHeatPumpAirToAir : airLoopHVACUnitaryHeatPumpAirToAirs) {
        if (airLoopHVACUnitaryHeatPumpAirToAir.coolingCoil().handle() == this->handle()) {
          return airLoopHVACUnitaryHeatPumpAirToAir;
        } else if (airLoopHVACUnitaryHeatPumpAirToAir.heatingCoil().handle() == this->handle()) {
          return airLoopHVACUnitaryHeatPumpAirToAir;
        }
      }

      auto waterHeaterHeatPumps = this->model().getConcreteModelObjects<WaterHeaterHeatPump>();
      for (const auto& waterHeaterHeatPump : waterHeaterHeatPumps) {
        if (waterHeaterHeatPump.dXCoil().handle() == this->handle()) {
          return waterHeaterHeatPump;
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

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::enhancedDehumidificationCoolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(
        OS_CoilSystem_IntegratedHeatPump_AirSourceFields::EnhancedDehumidificationCoolingCoil);
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::gridResponseCoolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(
        OS_CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseCoolingCoil);
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::gridResponseHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(
        OS_CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseHeatingCoil);
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

    double CoilSystemIntegratedHeatPumpAirSource_Impl::sizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil() const {
      boost::optional<double> value =
        getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::sizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil() const {
      boost::optional<double> value =
        getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil, true);
      OS_ASSERT(value);
      return value.get();
    }

    double
      CoilSystemIntegratedHeatPumpAirSource_Impl::sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil() const {
      boost::optional<double> value = getDouble(
        OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil,
        true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::
      sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil() const {
      boost::optional<double> value =
        getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::
                    SizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil,
                  true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::
      sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil() const {
      boost::optional<double> value =
        getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::
                    SizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil,
                  true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::
      sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil() const {
      boost::optional<double> value =
        getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::
                    SizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil,
                  true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::
      sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil() const {
      boost::optional<double> value =
        getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::
                    SizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil,
                  true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::sizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil() const {
      boost::optional<double> value =
        getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::sizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil() const {
      boost::optional<double> value =
        getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::sizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil() const {
      boost::optional<double> value =
        getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::chillerCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoil);
    }

    std::string CoilSystemIntegratedHeatPumpAirSource_Impl::chillerCoilBelongstoaSingleorSeparateUnit() const {
      boost::optional<std::string> value =
        getString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilBelongstoaSingleorSeparateUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    int CoilSystemIntegratedHeatPumpAirSource_Impl::chillerCoilCompressorRunSpeed() const {
      boost::optional<int> value = getInt(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilCompressorRunSpeed, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::sizingRatioofChillerCoiltoSpaceCoolingCoil() const {
      boost::optional<double> value = getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofChillerCoiltoSpaceCoolingCoil, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::supplementalChillerCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::WaterCoil);
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::airFlowRatioofWaterCoiltotheSpaceCoolingCoil() const {
      boost::optional<double> value = getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::AirFlowRatioofWaterCoiltotheSpaceCoolingCoil, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::waterFlowRatioofWaterCoiltotheChillerCoil() const {
      boost::optional<double> value = getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::WaterFlowRatioofWaterCoiltotheChillerCoil, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::storageTank() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::Tank);
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::iceFractionBelowWhichChargingStarts() const {
      boost::optional<double> value = getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::IceFractionBelowWhichChargingStarts, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilSystemIntegratedHeatPumpAirSource_Impl::chillerEnteringTemperatureatZeroTankFraction() const {
      boost::optional<double> value = getDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerEnteringTemperatureatZeroTankFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> CoilSystemIntegratedHeatPumpAirSource_Impl::temperatureDeviationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::TemperatureDeviationCurve);
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

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setEnhancedDehumidificationCoolingCoil(
      const boost::optional<StraightComponent>& enhancedDehumidificationCoolingCoil) {
      bool result(false);
      if (enhancedDehumidificationCoolingCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::EnhancedDehumidificationCoolingCoil,
                            enhancedDehumidificationCoolingCoil.get().handle());
      } else {
        resetEnhancedDehumidificationCoolingCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetEnhancedDehumidificationCoolingCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::EnhancedDehumidificationCoolingCoil, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setGridResponseCoolingCoil(const boost::optional<StraightComponent>& gridResponseCoolingCoil) {
      bool result(false);
      if (gridResponseCoolingCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseCoolingCoil, gridResponseCoolingCoil.get().handle());
      } else {
        resetGridResponseCoolingCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetGridResponseCoolingCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseCoolingCoil, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setGridResponseHeatingCoil(const boost::optional<StraightComponent>& gridResponseHeatingCoil) {
      bool result(false);
      if (gridResponseHeatingCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseHeatingCoil, gridResponseHeatingCoil.get().handle());
      } else {
        resetGridResponseHeatingCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetGridResponseHeatingCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseHeatingCoil, "");
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

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil(
      double sizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil,
                              sizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil(
      double sizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil,
                              sizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil(
      double sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil) {
      bool result = setDouble(
        OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil,
        sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::
      setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil(
        double sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::
                                SizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil,
                              sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::
      setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil(
        double sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::
                                SizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil,
                              sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::
      setSizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil(
        double sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::
                                SizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil,
                              sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::
      setSizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil(
        double sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::
                                SizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil,
                              sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil(
      double sizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil,
                              sizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil(
      double sizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil,
                              sizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil(
      double sizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil,
                              sizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setChillerCoil(const boost::optional<StraightComponent>& chillerCoil) {
      bool result(false);
      if (chillerCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoil, chillerCoil.get().handle());
      } else {
        resetChillerCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetChillerCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoil, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setChillerCoilBelongstoaSingleorSeparateUnit(
      std::string chillerCoilBelongstoaSingleorSeparateUnit) {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilBelongstoaSingleorSeparateUnit,
                              chillerCoilBelongstoaSingleorSeparateUnit);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setChillerCoilCompressorRunSpeed(int chillerCoilCompressorRunSpeed) {
      bool result = setInt(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilCompressorRunSpeed, chillerCoilCompressorRunSpeed);
      OS_ASSERT(result);
      return result;
    }

    bool
      CoilSystemIntegratedHeatPumpAirSource_Impl::setSizingRatioofChillerCoiltoSpaceCoolingCoil(double sizingRatioofChillerCoiltoSpaceCoolingCoil) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofChillerCoiltoSpaceCoolingCoil,
                              sizingRatioofChillerCoiltoSpaceCoolingCoil);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSupplementalChillerCoil(const boost::optional<HVACComponent>& supplementalChillerCoil) {
      bool result(false);
      if (supplementalChillerCoil) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::WaterCoil, supplementalChillerCoil.get().handle());
      } else {
        resetSupplementalChillerCoil();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetSupplementalChillerCoil() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::WaterCoil, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setAirFlowRatioofWaterCoiltotheSpaceCoolingCoil(
      double airFlowRatioofWaterCoiltotheSpaceCoolingCoil) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::AirFlowRatioofWaterCoiltotheSpaceCoolingCoil,
                              airFlowRatioofWaterCoiltotheSpaceCoolingCoil);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setWaterFlowRatioofWaterCoiltotheChillerCoil(double waterFlowRatioofWaterCoiltotheChillerCoil) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::WaterFlowRatioofWaterCoiltotheChillerCoil,
                              waterFlowRatioofWaterCoiltotheChillerCoil);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setStorageTank(const boost::optional<StraightComponent>& storageTank) {
      bool result(false);
      if (storageTank) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::Tank, storageTank.get().handle());
      } else {
        resetStorageTank();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetStorageTank() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::Tank, "");
      OS_ASSERT(result);
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setIceFractionBelowWhichChargingStarts(double iceFractionBelowWhichChargingStarts) {
      bool result =
        setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::IceFractionBelowWhichChargingStarts, iceFractionBelowWhichChargingStarts);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setChillerEnteringTemperatureatZeroTankFraction(
      double chillerEnteringTemperatureatZeroTankFraction) {
      bool result = setDouble(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerEnteringTemperatureatZeroTankFraction,
                              chillerEnteringTemperatureatZeroTankFraction);
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setTemperatureDeviationCurve(const boost::optional<Curve>& temperatureDeviationCurve) {
      bool result(false);
      if (temperatureDeviationCurve) {
        result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::TemperatureDeviationCurve, temperatureDeviationCurve.get().handle());
      } else {
        resetTemperatureDeviationCurve();
        result = true;
      }
      return result;
    }

    void CoilSystemIntegratedHeatPumpAirSource_Impl::resetTemperatureDeviationCurve() {
      bool result = setString(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::TemperatureDeviationCurve, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  CoilSystemIntegratedHeatPumpAirSource::CoilSystemIntegratedHeatPumpAirSource(const Model& model)
    : StraightComponent(CoilSystemIntegratedHeatPumpAirSource::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>());

    CoilCoolingDXVariableSpeed spaceCoolingCoil(model);
    CoilHeatingDXVariableSpeed spaceHeatingCoil(model);
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed dedicatedWaterHeatingCoil(model);
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scwhCoil(model);
    CoilCoolingDXVariableSpeed scdwhCoolingCoil(model);
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scdwhWaterHeatingCoil(model);
    CoilHeatingDXVariableSpeed shdwhHeatingCoil(model);
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed shdwhWaterHeatingCoil(model);

    bool ok = setSpaceCoolingCoil(spaceCoolingCoil);
    OS_ASSERT(ok);
    ok = setSpaceHeatingCoil(spaceHeatingCoil);
    OS_ASSERT(ok);
    ok = setDedicatedWaterHeatingCoil(dedicatedWaterHeatingCoil);
    OS_ASSERT(ok);
    ok = setSCWHCoil(scwhCoil);
    OS_ASSERT(ok);
    ok = setSCDWHCoolingCoil(scdwhCoolingCoil);
    OS_ASSERT(ok);
    ok = setSCDWHWaterHeatingCoil(scdwhWaterHeatingCoil);
    OS_ASSERT(ok);
    ok = setSHDWHHeatingCoil(shdwhHeatingCoil);
    OS_ASSERT(ok);
    ok = setSHDWHWaterHeatingCoil(shdwhWaterHeatingCoil);
    OS_ASSERT(ok);
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

  CoilSystemIntegratedHeatPumpAirSource::CoilSystemIntegratedHeatPumpAirSource(const Model& model, const StraightComponent& spaceCoolingCoil,
                                                                               const StraightComponent& spaceHeatingCoil)
    : StraightComponent(CoilSystemIntegratedHeatPumpAirSource::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>());

    bool ok = setSpaceCoolingCoil(spaceCoolingCoil);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Space Cooling Coil to " << spaceCoolingCoil.briefDescription() << ".");
    }

    ok = setSpaceHeatingCoil(spaceHeatingCoil);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Space Heating Coil to " << spaceHeatingCoil.briefDescription() << ".");
    }

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
    setSizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil(1.0);
    setSizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil(1.0);
    setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil(1.0);
    setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil(1.0);
    setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil(0.15);
    setSizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil(1.0);
    setSizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil(0.1);
    setSizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil(0.9);
    setSizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil(0.9);
    setSizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil(0.9);
    setChillerCoilBelongstoaSingleorSeparateUnit("Single");
    setChillerCoilCompressorRunSpeed(1);
    setSizingRatioofChillerCoiltoSpaceCoolingCoil(1.0);
    setAirFlowRatioofWaterCoiltotheSpaceCoolingCoil(1.0);
    setWaterFlowRatioofWaterCoiltotheChillerCoil(1.0);
    setIceFractionBelowWhichChargingStarts(0.9);
    setChillerEnteringTemperatureatZeroTankFraction(-0.5);
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

  boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource::enhancedDehumidificationCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->enhancedDehumidificationCoolingCoil();
  }

  boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource::gridResponseCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->gridResponseCoolingCoil();
  }

  boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource::gridResponseHeatingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->gridResponseHeatingCoil();
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

  double CoilSystemIntegratedHeatPumpAirSource::sizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->sizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil();
  }

  double CoilSystemIntegratedHeatPumpAirSource::sizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->sizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil();
  }

  double CoilSystemIntegratedHeatPumpAirSource::sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()
      ->sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil();
  }

  double
    CoilSystemIntegratedHeatPumpAirSource::sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil()
      const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()
      ->sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil();
  }

  double CoilSystemIntegratedHeatPumpAirSource::
    sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()
      ->sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil();
  }

  double CoilSystemIntegratedHeatPumpAirSource::
    sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()
      ->sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil();
  }

  double CoilSystemIntegratedHeatPumpAirSource::
    sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()
      ->sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil();
  }

  double CoilSystemIntegratedHeatPumpAirSource::sizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->sizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil();
  }

  double CoilSystemIntegratedHeatPumpAirSource::sizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->sizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil();
  }

  double CoilSystemIntegratedHeatPumpAirSource::sizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->sizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil();
  }

  boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource::chillerCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->chillerCoil();
  }

  std::string CoilSystemIntegratedHeatPumpAirSource::chillerCoilBelongstoaSingleorSeparateUnit() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->chillerCoilBelongstoaSingleorSeparateUnit();
  }

  int CoilSystemIntegratedHeatPumpAirSource::chillerCoilCompressorRunSpeed() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->chillerCoilCompressorRunSpeed();
  }

  double CoilSystemIntegratedHeatPumpAirSource::sizingRatioofChillerCoiltoSpaceCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->sizingRatioofChillerCoiltoSpaceCoolingCoil();
  }

  boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource::supplementalChillerCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->supplementalChillerCoil();
  }

  double CoilSystemIntegratedHeatPumpAirSource::airFlowRatioofWaterCoiltotheSpaceCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->airFlowRatioofWaterCoiltotheSpaceCoolingCoil();
  }

  double CoilSystemIntegratedHeatPumpAirSource::waterFlowRatioofWaterCoiltotheChillerCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->waterFlowRatioofWaterCoiltotheChillerCoil();
  }

  boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource::storageTank() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->storageTank();
  }

  double CoilSystemIntegratedHeatPumpAirSource::iceFractionBelowWhichChargingStarts() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->iceFractionBelowWhichChargingStarts();
  }

  double CoilSystemIntegratedHeatPumpAirSource::chillerEnteringTemperatureatZeroTankFraction() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->chillerEnteringTemperatureatZeroTankFraction();
  }

  boost::optional<Curve> CoilSystemIntegratedHeatPumpAirSource::temperatureDeviationCurve() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->temperatureDeviationCurve();
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

  bool CoilSystemIntegratedHeatPumpAirSource::setSizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil(
    double sizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil(
      sizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil(
    double sizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil(
      sizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil(
    double sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()
      ->setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil(
        sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil);
  }

  bool
    CoilSystemIntegratedHeatPumpAirSource::setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil(
      double sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()
      ->setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil(
        sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::
    setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil(
      double sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()
      ->setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil(
        sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::
    setSizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil(
      double sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()
      ->setSizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil(
        sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::
    setSizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil(
      double sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()
      ->setSizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil(
        sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil(
    double sizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil(
      sizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil(
    double sizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil(
      sizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil(
    double sizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil(
      sizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setChillerCoil(const StraightComponent& chillerCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setChillerCoil(chillerCoil);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetChillerCoil() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetChillerCoil();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setChillerCoilBelongstoaSingleorSeparateUnit(std::string chillerCoilBelongstoaSingleorSeparateUnit) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setChillerCoilBelongstoaSingleorSeparateUnit(
      chillerCoilBelongstoaSingleorSeparateUnit);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setChillerCoilCompressorRunSpeed(int chillerCoilCompressorRunSpeed) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setChillerCoilCompressorRunSpeed(chillerCoilCompressorRunSpeed);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSizingRatioofChillerCoiltoSpaceCoolingCoil(double sizingRatioofChillerCoiltoSpaceCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSizingRatioofChillerCoiltoSpaceCoolingCoil(
      sizingRatioofChillerCoiltoSpaceCoolingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSupplementalChillerCoil(const HVACComponent& supplementalChillerCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSupplementalChillerCoil(supplementalChillerCoil);
  }

  void CoilSystemIntegratedHeatPumpAirSource::resetSupplementalChillerCoil() {
    getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->resetSupplementalChillerCoil();
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setAirFlowRatioofWaterCoiltotheSpaceCoolingCoil(double airFlowRatioofWaterCoiltotheSpaceCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setAirFlowRatioofWaterCoiltotheSpaceCoolingCoil(
      airFlowRatioofWaterCoiltotheSpaceCoolingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setWaterFlowRatioofWaterCoiltotheChillerCoil(double waterFlowRatioofWaterCoiltotheChillerCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setWaterFlowRatioofWaterCoiltotheChillerCoil(
      waterFlowRatioofWaterCoiltotheChillerCoil);
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

  bool CoilSystemIntegratedHeatPumpAirSource::setChillerEnteringTemperatureatZeroTankFraction(double chillerEnteringTemperatureatZeroTankFraction) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setChillerEnteringTemperatureatZeroTankFraction(
      chillerEnteringTemperatureatZeroTankFraction);
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
