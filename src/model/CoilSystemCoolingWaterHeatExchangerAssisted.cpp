/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CoilSystemCoolingWaterHeatExchangerAssisted.hpp"
#include "CoilSystemCoolingWaterHeatExchangerAssisted_Impl.hpp"
#include "AirToAirComponent.hpp"
#include "AirToAirComponent_Impl.hpp"
#include "ControllerWaterCoil.hpp"
#include "ControllerWaterCoil_Impl.hpp"
#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "CoilCoolingWater.hpp"
#include "CoilCoolingWater_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"

#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "ZoneHVACFourPipeFanCoil.hpp"
#include "ZoneHVACFourPipeFanCoil_Impl.hpp"
#include "ZoneHVACUnitVentilator.hpp"
#include "ZoneHVACUnitVentilator_Impl.hpp"

#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "AirLoopHVACDedicatedOutdoorAirSystem.hpp"
#include "AirLoopHVACDedicatedOutdoorAirSystem_Impl.hpp"

#include <utilities/idd/OS_CoilSystem_Cooling_Water_HeatExchangerAssisted_FieldEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilSystemCoolingWaterHeatExchangerAssisted_Impl::CoilSystemCoolingWaterHeatExchangerAssisted_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                       bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilSystemCoolingWaterHeatExchangerAssisted::iddObjectType());
    }

    CoilSystemCoolingWaterHeatExchangerAssisted_Impl::CoilSystemCoolingWaterHeatExchangerAssisted_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilSystemCoolingWaterHeatExchangerAssisted::iddObjectType());
    }

    CoilSystemCoolingWaterHeatExchangerAssisted_Impl::CoilSystemCoolingWaterHeatExchangerAssisted_Impl(
      const CoilSystemCoolingWaterHeatExchangerAssisted_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilSystemCoolingWaterHeatExchangerAssisted_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not appropriate: no specific output, there are at the coil and HX level
      return result;
    }

    IddObjectType CoilSystemCoolingWaterHeatExchangerAssisted_Impl::iddObjectType() const {
      return CoilSystemCoolingWaterHeatExchangerAssisted::iddObjectType();
    }

    std::vector<ModelObject> CoilSystemCoolingWaterHeatExchangerAssisted_Impl::children() const {
      std::vector<ModelObject> result;

      result.push_back(coolingCoil());
      result.push_back(heatExchanger());

      return result;
    }

    ModelObject CoilSystemCoolingWaterHeatExchangerAssisted_Impl::clone(Model model) const {
      auto newCoilSystem = StraightComponent_Impl::clone(model).cast<CoilSystemCoolingWaterHeatExchangerAssisted>();

      {
        auto mo = coolingCoil().clone(model).cast<WaterToAirComponent>();
        newCoilSystem.setCoolingCoil(mo);
      }

      {
        auto mo = heatExchanger().clone(model).cast<AirToAirComponent>();
        newCoilSystem.setHeatExchanger(mo);
      }

      return std::move(newCoilSystem);
    }

    boost::optional<HVACComponent> CoilSystemCoolingWaterHeatExchangerAssisted_Impl::containingHVACComponent() const {
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

    boost::optional<ZoneHVACComponent> CoilSystemCoolingWaterHeatExchangerAssisted_Impl::containingZoneHVACComponent() const {

      // ZoneHVACFourPipeFanCoil
      std::vector<ZoneHVACFourPipeFanCoil> zoneHVACFourPipeFanCoils;

      zoneHVACFourPipeFanCoils = this->model().getConcreteModelObjects<ZoneHVACFourPipeFanCoil>();

      for (const auto& zoneHVACFourPipeFanCoil : zoneHVACFourPipeFanCoils) {
        if (boost::optional<HVACComponent> coil = zoneHVACFourPipeFanCoil.coolingCoil()) {
          if (coil->handle() == this->handle()) {
            return zoneHVACFourPipeFanCoil;
          }
        }
      }

      // ZoneHVACUnitVentilator
      std::vector<ZoneHVACUnitVentilator> zoneHVACUnitVentilators;

      zoneHVACUnitVentilators = this->model().getConcreteModelObjects<ZoneHVACUnitVentilator>();

      for (const auto& zoneHVACUnitVentilator : zoneHVACUnitVentilators) {
        if (boost::optional<HVACComponent> coil = zoneHVACUnitVentilator.coolingCoil()) {
          if (coil->handle() == this->handle()) {
            return zoneHVACUnitVentilator;
          }
        }
      }

      // ZoneHVAC:WindowAirConditioner not wrapped

      return boost::none;
    }

    AirToAirComponent CoilSystemCoolingWaterHeatExchangerAssisted_Impl::heatExchanger() const {
      boost::optional<AirToAirComponent> value = optionalHeatExchanger();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Heat Exchanger attached.");
      }
      return value.get();
    }

    WaterToAirComponent CoilSystemCoolingWaterHeatExchangerAssisted_Impl::coolingCoil() const {
      auto value = optionalCoolingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Cooling Coil attached.");
      }
      return value.get();
    }

    bool CoilSystemCoolingWaterHeatExchangerAssisted_Impl::setHeatExchanger(const AirToAirComponent& heatExchanger) {
      bool result = setPointer(OS_CoilSystem_Cooling_Water_HeatExchangerAssistedFields::HeatExchanger, heatExchanger.handle());
      return result;
    }

    bool CoilSystemCoolingWaterHeatExchangerAssisted_Impl::setCoolingCoil(const WaterToAirComponent& coolingCoil) {
      bool result = setPointer(OS_CoilSystem_Cooling_Water_HeatExchangerAssistedFields::CoolingCoil, coolingCoil.handle());
      return result;
    }

    boost::optional<AirToAirComponent> CoilSystemCoolingWaterHeatExchangerAssisted_Impl::optionalHeatExchanger() const {
      return getObject<ModelObject>().getModelObjectTarget<AirToAirComponent>(OS_CoilSystem_Cooling_Water_HeatExchangerAssistedFields::HeatExchanger);
    }

    boost::optional<WaterToAirComponent> CoilSystemCoolingWaterHeatExchangerAssisted_Impl::optionalCoolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<WaterToAirComponent>(OS_CoilSystem_Cooling_Water_HeatExchangerAssistedFields::CoolingCoil);
    }

    unsigned CoilSystemCoolingWaterHeatExchangerAssisted_Impl::inletPort() const {
      return OS_CoilSystem_Cooling_Water_HeatExchangerAssistedFields::AirInletNodeName;
    }

    unsigned CoilSystemCoolingWaterHeatExchangerAssisted_Impl::outletPort() const {
      return OS_CoilSystem_Cooling_Water_HeatExchangerAssistedFields::AirOutletNodeName;
    }

    bool CoilSystemCoolingWaterHeatExchangerAssisted_Impl::addToNode(Node& node) {
      bool result = false;

      if (boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC()) {
        if (!airLoop->demandComponent(node.handle())) {
          result = StraightComponent_Impl::addToNode(node);
          if (result) {
            auto t_coolingCoil = coolingCoil();
            if (auto waterInletModelObject = t_coolingCoil.waterInletModelObject()) {

              // TODO: why aren't we setting the water coil in this case? @kbenne thoughts please
              if (auto coilCoolingWater = t_coolingCoil.optionalCast<CoilCoolingWater>()) {
                if (auto oldController = coilCoolingWater->controllerWaterCoil()) {
                  oldController->remove();
                }
              }

              auto t_model = model();
              ControllerWaterCoil controller(t_model);

              auto coilWaterInletNode = waterInletModelObject->optionalCast<Node>();
              OS_ASSERT(coilWaterInletNode);
              controller.setActuatorNode(coilWaterInletNode.get());
              // sensor node will be established in translator since that node does not yet exist

              controller.setAction("Reverse");
            }
          }
        }
      } else if (boost::optional<AirLoopHVACOutdoorAirSystem> oas = node.airLoopHVACOutdoorAirSystem()) {
        if (oas->airLoopHVACDedicatedOutdoorAirSystem()) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return result;
    }

    ComponentType CoilSystemCoolingWaterHeatExchangerAssisted_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> CoilSystemCoolingWaterHeatExchangerAssisted_Impl::coolingFuelTypes() const {
      return coolingCoil().coolingFuelTypes();
    }

    std::vector<FuelType> CoilSystemCoolingWaterHeatExchangerAssisted_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> CoilSystemCoolingWaterHeatExchangerAssisted_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  CoilSystemCoolingWaterHeatExchangerAssisted::CoilSystemCoolingWaterHeatExchangerAssisted(const Model& model)
    : StraightComponent(CoilSystemCoolingWaterHeatExchangerAssisted::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl>());

    CoilCoolingWater coolingCoil(model);
    setCoolingCoil(coolingCoil);

    HeatExchangerAirToAirSensibleAndLatent heatExchanger(model);
    heatExchanger.setSupplyAirOutletTemperatureControl(false);
    setHeatExchanger(heatExchanger);
  }

  CoilSystemCoolingWaterHeatExchangerAssisted::CoilSystemCoolingWaterHeatExchangerAssisted(const Model& model, const AirToAirComponent& heatExchanger)
    : StraightComponent(CoilSystemCoolingWaterHeatExchangerAssisted::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl>());

    bool ok = setHeatExchanger(heatExchanger);
    if (!ok) {
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Heat Exchanger " << heatExchanger.briefDescription() << ".");
    }

    CoilCoolingWater coolingCoil(model);
    setCoolingCoil(coolingCoil);

    setHeatExchanger(heatExchanger);
  }

  IddObjectType CoilSystemCoolingWaterHeatExchangerAssisted::iddObjectType() {
    return {IddObjectType::OS_CoilSystem_Cooling_Water_HeatExchangerAssisted};
  }

  AirToAirComponent CoilSystemCoolingWaterHeatExchangerAssisted::heatExchanger() const {
    return getImpl<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl>()->heatExchanger();
  }

  WaterToAirComponent CoilSystemCoolingWaterHeatExchangerAssisted::coolingCoil() const {
    return getImpl<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl>()->coolingCoil();
  }

  bool CoilSystemCoolingWaterHeatExchangerAssisted::setHeatExchanger(const AirToAirComponent& heatExchanger) {
    return getImpl<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl>()->setHeatExchanger(heatExchanger);
  }

  bool CoilSystemCoolingWaterHeatExchangerAssisted::setCoolingCoil(const WaterToAirComponent& coolingCoil) {
    return getImpl<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl>()->setCoolingCoil(coolingCoil);
  }

  /// @cond
  CoilSystemCoolingWaterHeatExchangerAssisted::CoilSystemCoolingWaterHeatExchangerAssisted(
    std::shared_ptr<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
