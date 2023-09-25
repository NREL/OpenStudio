/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::addToNode(Node& /*node*/) {
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

      hvacComponent = optionalDedicatedWaterHeatingCoil();
      if (hvacComponent) {
        result.push_back(hvacComponent.get());
      }

      hvacComponent = optionalScwhCoil();
      if (hvacComponent) {
        result.push_back(hvacComponent.get());
      }

      straightComponent = optionalScdwhCoolingCoil();
      if (straightComponent) {
        result.push_back(straightComponent.get());
      }

      hvacComponent = optionalScdwhWaterHeatingCoil();
      if (hvacComponent) {
        result.push_back(hvacComponent.get());
      }

      straightComponent = optionalShdwhHeatingCoil();
      if (straightComponent) {
        result.push_back(straightComponent.get());
      }

      hvacComponent = optionalShdwhWaterHeatingCoil();
      if (hvacComponent) {
        result.push_back(hvacComponent.get());
      }

      return result;
    }

    ModelObject CoilSystemIntegratedHeatPumpAirSource_Impl::clone(Model model) const {
      auto newCoilSystem = StraightComponent_Impl::clone(model).cast<CoilSystemIntegratedHeatPumpAirSource>();

      auto spaceCoolingCoilClone = this->spaceCoolingCoil().clone(model).cast<StraightComponent>();
      newCoilSystem.setSpaceCoolingCoil(spaceCoolingCoilClone);

      auto spaceHeatingCoilClone = this->spaceHeatingCoil().clone(model).cast<StraightComponent>();
      newCoilSystem.setSpaceHeatingCoil(spaceHeatingCoilClone);

      auto dedicatedWaterHeatingCoilClone = this->dedicatedWaterHeatingCoil().clone(model).cast<HVACComponent>();
      newCoilSystem.setDedicatedWaterHeatingCoil(dedicatedWaterHeatingCoilClone);

      auto scwhCoilClone = this->scwhCoil().clone(model).cast<HVACComponent>();
      newCoilSystem.setSCWHCoil(scwhCoilClone);

      auto scdwhCoolingCoilClone = this->scdwhCoolingCoil().clone(model).cast<StraightComponent>();
      newCoilSystem.setSCDWHCoolingCoil(scdwhCoolingCoilClone);

      auto scdwhWaterHeatingCoilClone = this->scdwhWaterHeatingCoil().clone(model).cast<HVACComponent>();
      newCoilSystem.setSCDWHWaterHeatingCoil(scdwhWaterHeatingCoilClone);

      auto shdwhHeatingCoilClone = this->shdwhHeatingCoil().clone(model).cast<StraightComponent>();
      newCoilSystem.setSHDWHHeatingCoil(shdwhHeatingCoilClone);

      auto shdwhWaterHeatingCoilClone = this->shdwhWaterHeatingCoil().clone(model).cast<HVACComponent>();
      newCoilSystem.setSHDWHWaterHeatingCoil(shdwhWaterHeatingCoilClone);

      return std::move(newCoilSystem);
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

    HVACComponent CoilSystemIntegratedHeatPumpAirSource_Impl::dedicatedWaterHeatingCoil() const {
      auto value = optionalDedicatedWaterHeatingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Dedicated Water Heating Coil attached.");
      }
      return value.get();
    }

    boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::optionalDedicatedWaterHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(
        OS_CoilSystem_IntegratedHeatPump_AirSourceFields::DedicatedWaterHeatingCoil);
    }

    HVACComponent CoilSystemIntegratedHeatPumpAirSource_Impl::scwhCoil() const {
      auto value = optionalScwhCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a SCWH Mode Coil attached.");
      }
      return value.get();
    }

    boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::optionalScwhCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCWHCoil);
    }

    StraightComponent CoilSystemIntegratedHeatPumpAirSource_Impl::scdwhCoolingCoil() const {
      auto value = optionalScdwhCoolingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a SCDWH Mode Cooling Coil attached.");
      }
      return value.get();
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::optionalScdwhCoolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHCoolingCoil);
    }

    HVACComponent CoilSystemIntegratedHeatPumpAirSource_Impl::scdwhWaterHeatingCoil() const {
      auto value = optionalScdwhWaterHeatingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a SCDWH Mode Water Heating Coil attached.");
      }
      return value.get();
    }

    boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::optionalScdwhWaterHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHWaterHeatingCoil);
    }

    StraightComponent CoilSystemIntegratedHeatPumpAirSource_Impl::shdwhHeatingCoil() const {
      auto value = optionalShdwhHeatingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a SHDWH Mode Heating Coil Name attached.");
      }
      return value.get();
    }

    boost::optional<StraightComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::optionalShdwhHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHHeatingCoil);
    }

    HVACComponent CoilSystemIntegratedHeatPumpAirSource_Impl::shdwhWaterHeatingCoil() const {
      auto value = optionalShdwhWaterHeatingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a SHDWH Mode Water Heating Coil attached.");
      }
      return value.get();
    }

    boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::optionalShdwhWaterHeatingCoil() const {
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

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setDedicatedWaterHeatingCoil(const HVACComponent& dedicatedWaterHeatingCoil) {
      bool result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::DedicatedWaterHeatingCoil, dedicatedWaterHeatingCoil.handle());
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSCWHCoil(const HVACComponent& scwhCoil) {
      bool result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCWHCoil, scwhCoil.handle());
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSCDWHCoolingCoil(const StraightComponent& scdwhCoolingCoil) {
      bool result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHCoolingCoil, scdwhCoolingCoil.handle());
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSCDWHWaterHeatingCoil(const HVACComponent& scdwhWaterHeatingCoil) {
      bool result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHWaterHeatingCoil, scdwhWaterHeatingCoil.handle());
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSHDWHHeatingCoil(const StraightComponent& shdwhHeatingCoil) {
      bool result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHHeatingCoil, shdwhHeatingCoil.handle());
      return result;
    }

    bool CoilSystemIntegratedHeatPumpAirSource_Impl::setSHDWHWaterHeatingCoil(const HVACComponent& shdwhWaterHeatingCoil) {
      bool result = setPointer(OS_CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHWaterHeatingCoil, shdwhWaterHeatingCoil.handle());
      return result;
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

    ComponentType CoilSystemIntegratedHeatPumpAirSource_Impl::componentType() const {
      return ComponentType::Both;
    }

    std::vector<FuelType> CoilSystemIntegratedHeatPumpAirSource_Impl::coolingFuelTypes() const {
      std::set<FuelType> result;

      for (auto ft : spaceCoolingCoil().coolingFuelTypes()) {
        result.insert(ft);
      }

      for (auto ft : scwhCoil().coolingFuelTypes()) {
        result.insert(ft);
      }
      for (auto ft : scdwhCoolingCoil().coolingFuelTypes()) {
        result.insert(ft);
      }
      for (auto ft : scdwhWaterHeatingCoil().coolingFuelTypes()) {
        result.insert(ft);
      }

      return {result.begin(), result.end()};
    }

    std::vector<FuelType> CoilSystemIntegratedHeatPumpAirSource_Impl::heatingFuelTypes() const {
      std::set<FuelType> result;

      for (auto ft : spaceHeatingCoil().heatingFuelTypes()) {
        result.insert(ft);
      }
      // TODO: include dedicatedWaterHeatingCoil? The combined space cooling + water heating ones too?
      for (auto ft : shdwhHeatingCoil().heatingFuelTypes()) {
        result.insert(ft);
      }
      for (auto ft : shdwhWaterHeatingCoil().heatingFuelTypes()) {
        result.insert(ft);
      }

      return {result.begin(), result.end()};
    }

    std::vector<AppGFuelType> CoilSystemIntegratedHeatPumpAirSource_Impl::appGHeatingFuelTypes() const {
      std::set<AppGFuelType> result;

      for (auto ft : spaceHeatingCoil().appGHeatingFuelTypes()) {
        result.insert(ft);
      }
      // TODO: include dedicatedWaterHeatingCoil? The combined space cooling + water heating ones too?
      for (auto ft : shdwhHeatingCoil().appGHeatingFuelTypes()) {
        result.insert(ft);
      }
      for (auto ft : shdwhWaterHeatingCoil().appGHeatingFuelTypes()) {
        result.insert(ft);
      }

      return {result.begin(), result.end()};
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

  CoilSystemIntegratedHeatPumpAirSource::CoilSystemIntegratedHeatPumpAirSource(
    const Model& model, const StraightComponent& spaceCoolingCoil, const StraightComponent& spaceHeatingCoil,
    const HVACComponent& dedicatedWaterHeatingCoil, const HVACComponent& scwhCoil, const StraightComponent& scdwhCoolingCoil,
    const HVACComponent& scdwhWaterHeatingCoil, const StraightComponent& shdwhHeatingCoil, const HVACComponent& shdwhWaterHeatingCoil)
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

    ok = setDedicatedWaterHeatingCoil(dedicatedWaterHeatingCoil);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Dedicated Water Heating Coil to " << dedicatedWaterHeatingCoil.briefDescription()
                                     << ".");
    }

    ok = setSCWHCoil(scwhCoil);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s SCWH Coil to " << scwhCoil.briefDescription() << ".");
    }

    ok = setSCDWHCoolingCoil(scdwhCoolingCoil);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s SCDWH Cooling Coil to " << scdwhCoolingCoil.briefDescription() << ".");
    }

    ok = setSCDWHWaterHeatingCoil(scdwhWaterHeatingCoil);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s SCDWH Water Heating Coil to " << scdwhWaterHeatingCoil.briefDescription() << ".");
    }

    ok = setSHDWHHeatingCoil(shdwhHeatingCoil);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s SHDWH Heating Coil to " << shdwhHeatingCoil.briefDescription() << ".");
    }

    ok = setSHDWHWaterHeatingCoil(shdwhWaterHeatingCoil);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s SHDWH Water Heating Coil to " << shdwhWaterHeatingCoil.briefDescription() << ".");
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
  }

  IddObjectType CoilSystemIntegratedHeatPumpAirSource::iddObjectType() {
    return {IddObjectType::OS_CoilSystem_IntegratedHeatPump_AirSource};
  }

  StraightComponent CoilSystemIntegratedHeatPumpAirSource::spaceCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->spaceCoolingCoil();
  }

  StraightComponent CoilSystemIntegratedHeatPumpAirSource::spaceHeatingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->spaceHeatingCoil();
  }

  HVACComponent CoilSystemIntegratedHeatPumpAirSource::dedicatedWaterHeatingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->dedicatedWaterHeatingCoil();
  }

  HVACComponent CoilSystemIntegratedHeatPumpAirSource::scwhCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->scwhCoil();
  }

  StraightComponent CoilSystemIntegratedHeatPumpAirSource::scdwhCoolingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->scdwhCoolingCoil();
  }

  HVACComponent CoilSystemIntegratedHeatPumpAirSource::scdwhWaterHeatingCoil() const {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->scdwhWaterHeatingCoil();
  }

  StraightComponent CoilSystemIntegratedHeatPumpAirSource::shdwhHeatingCoil() const {

    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->shdwhHeatingCoil();
  }

  HVACComponent CoilSystemIntegratedHeatPumpAirSource::shdwhWaterHeatingCoil() const {
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

  bool CoilSystemIntegratedHeatPumpAirSource::setSCWHCoil(const HVACComponent& scwhCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSCWHCoil(scwhCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSCDWHCoolingCoil(const StraightComponent& scdwhCoolingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSCDWHCoolingCoil(scdwhCoolingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSCDWHWaterHeatingCoil(const HVACComponent& scdwhWaterHeatingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSCDWHWaterHeatingCoil(scdwhWaterHeatingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSHDWHHeatingCoil(const StraightComponent& shdwhHeatingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSHDWHHeatingCoil(shdwhHeatingCoil);
  }

  bool CoilSystemIntegratedHeatPumpAirSource::setSHDWHWaterHeatingCoil(const HVACComponent& shdwhWaterHeatingCoil) {
    return getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>()->setSHDWHWaterHeatingCoil(shdwhWaterHeatingCoil);
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
