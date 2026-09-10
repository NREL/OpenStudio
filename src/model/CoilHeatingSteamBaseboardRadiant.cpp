/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CoilHeatingSteamBaseboardRadiant.hpp"
#include "CoilHeatingSteamBaseboardRadiant_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "ZoneHVACBaseboardRadiantConvectiveSteam.hpp"
#include "ZoneHVACBaseboardRadiantConvectiveSteam_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_Heating_Steam_Baseboard_Radiant_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilHeatingSteamBaseboardRadiant_Impl::CoilHeatingSteamBaseboardRadiant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilHeatingSteamBaseboardRadiant::iddObjectType());
    }

    CoilHeatingSteamBaseboardRadiant_Impl::CoilHeatingSteamBaseboardRadiant_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilHeatingSteamBaseboardRadiant::iddObjectType());
    }

    CoilHeatingSteamBaseboardRadiant_Impl::CoilHeatingSteamBaseboardRadiant_Impl(const CoilHeatingSteamBaseboardRadiant_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilHeatingSteamBaseboardRadiant_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType CoilHeatingSteamBaseboardRadiant_Impl::iddObjectType() const {
      return CoilHeatingSteamBaseboardRadiant::iddObjectType();
    }

    unsigned CoilHeatingSteamBaseboardRadiant_Impl::inletPort() const {
      return OS_Coil_Heating_Steam_Baseboard_RadiantFields::InletNodeName;
    }

    unsigned CoilHeatingSteamBaseboardRadiant_Impl::outletPort() const {
      return OS_Coil_Heating_Steam_Baseboard_RadiantFields::OutletNodeName;
    }

    boost::optional<ZoneHVACComponent> CoilHeatingSteamBaseboardRadiant_Impl::containingZoneHVACComponent() const {
      // this coil can only be found in a ZoneHVACBaseboardRadiantConvectiveSteam
      // check all ZoneHVACBaseboardRadiantConvectiveSteams in the model, seeing if this coil
      // is inside of one of them.  Return the one it is inside of

      auto const zoneHVACBaseboardRadiantConvectiveSteams = this->model().getConcreteModelObjects<ZoneHVACBaseboardRadiantConvectiveSteam>();
      // loop through each one, seeing if the coil is contained by the zonehvacbaseboard
      for (const auto& zoneHVACBaseboardRadiantConvectiveSteam : zoneHVACBaseboardRadiantConvectiveSteams) {
        if (boost::optional<HVACComponent> coil = zoneHVACBaseboardRadiantConvectiveSteam.heatingCoil()) {
          if (coil->handle() == this->handle()) {  //if the handles match, this coil is inside of a zonehvacbaseboard
            return zoneHVACBaseboardRadiantConvectiveSteam;
          }
        }
      }

      // if the coil isn't inside any zonehvacbaseboards (which currently should never happen), return nothing
      return boost::none;
    }

    bool CoilHeatingSteamBaseboardRadiant_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->demandComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    std::string CoilHeatingSteamBaseboardRadiant_Impl::heatingDesignCapacityMethod() const {
      boost::optional<std::string> value = getString(OS_Coil_Heating_Steam_Baseboard_RadiantFields::HeatingDesignCapacityMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilHeatingSteamBaseboardRadiant_Impl::heatingDesignCapacity() const {
      return getDouble(OS_Coil_Heating_Steam_Baseboard_RadiantFields::HeatingDesignCapacity, true);
    }

    bool CoilHeatingSteamBaseboardRadiant_Impl::isHeatingDesignCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_Steam_Baseboard_RadiantFields::HeatingDesignCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilHeatingSteamBaseboardRadiant_Impl::heatingDesignCapacityPerFloorArea() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Steam_Baseboard_RadiantFields::HeatingDesignCapacityPerFloorArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilHeatingSteamBaseboardRadiant_Impl::fractionofAutosizedHeatingDesignCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Steam_Baseboard_RadiantFields::FractionofAutosizedHeatingDesignCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilHeatingSteamBaseboardRadiant_Impl::degreeofSubCooling() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Steam_Baseboard_RadiantFields::DegreeofSubCooling, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilHeatingSteamBaseboardRadiant_Impl::maximumSteamFlowRate() const {
      return getDouble(OS_Coil_Heating_Steam_Baseboard_RadiantFields::MaximumSteamFlowRate, true);
    }

    bool CoilHeatingSteamBaseboardRadiant_Impl::isMaximumSteamFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_Steam_Baseboard_RadiantFields::MaximumSteamFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilHeatingSteamBaseboardRadiant_Impl::convergenceTolerance() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Steam_Baseboard_RadiantFields::ConvergenceTolerance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingSteamBaseboardRadiant_Impl::setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod) {
      bool result = setString(OS_Coil_Heating_Steam_Baseboard_RadiantFields::HeatingDesignCapacityMethod, heatingDesignCapacityMethod);
      return result;
    }

    bool CoilHeatingSteamBaseboardRadiant_Impl::setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity) {
      bool result(false);
      if (heatingDesignCapacity) {
        result = setDouble(OS_Coil_Heating_Steam_Baseboard_RadiantFields::HeatingDesignCapacity, heatingDesignCapacity.get());
      }
      return result;
    }

    void CoilHeatingSteamBaseboardRadiant_Impl::autosizeHeatingDesignCapacity() {
      bool result = setString(OS_Coil_Heating_Steam_Baseboard_RadiantFields::HeatingDesignCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool CoilHeatingSteamBaseboardRadiant_Impl::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
      bool result = setDouble(OS_Coil_Heating_Steam_Baseboard_RadiantFields::HeatingDesignCapacityPerFloorArea, heatingDesignCapacityPerFloorArea);
      return result;
    }

    bool CoilHeatingSteamBaseboardRadiant_Impl::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
      bool result =
        setDouble(OS_Coil_Heating_Steam_Baseboard_RadiantFields::FractionofAutosizedHeatingDesignCapacity, fractionofAutosizedHeatingDesignCapacity);
      return result;
    }

    bool CoilHeatingSteamBaseboardRadiant_Impl::setDegreeofSubCooling(double degreeofSubCooling) {
      bool result = setDouble(OS_Coil_Heating_Steam_Baseboard_RadiantFields::DegreeofSubCooling, degreeofSubCooling);
      OS_ASSERT(result);
      return result;
    }

    bool CoilHeatingSteamBaseboardRadiant_Impl::setMaximumSteamFlowRate(boost::optional<double> maximumSteamFlowRate) {
      bool result(false);
      if (maximumSteamFlowRate) {
        result = setDouble(OS_Coil_Heating_Steam_Baseboard_RadiantFields::MaximumSteamFlowRate, maximumSteamFlowRate.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void CoilHeatingSteamBaseboardRadiant_Impl::autosizeMaximumSteamFlowRate() {
      bool result = setString(OS_Coil_Heating_Steam_Baseboard_RadiantFields::MaximumSteamFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool CoilHeatingSteamBaseboardRadiant_Impl::setConvergenceTolerance(double convergenceTolerance) {
      bool result = setDouble(OS_Coil_Heating_Steam_Baseboard_RadiantFields::ConvergenceTolerance, convergenceTolerance);
      return result;
    }

    boost::optional<double> CoilHeatingSteamBaseboardRadiant_Impl::autosizedHeatingDesignCapacity() const {
      boost::optional<double> result;
      // Get the containing ZoneHVAC equipment and get its autosized value
      auto parentHVAC = containingZoneHVACComponent();
      if (!parentHVAC) {
        return result;
      }
      return parentHVAC->getAutosizedValue("Design Size Heating Design Capacity", "W");
    }

    boost::optional<double> CoilHeatingSteamBaseboardRadiant_Impl::autosizedMaximumSteamFlowRate() const {
      boost::optional<double> result;
      // Get the containing ZoneHVAC equipment and get its autosized value
      auto parentHVAC = containingZoneHVACComponent();
      if (!parentHVAC) {
        return result;
      }
      return parentHVAC->getAutosizedValue("Design Size Maximum Steam Flow Rate", "m3/s");
    }

    void CoilHeatingSteamBaseboardRadiant_Impl::autosize() {
      autosizeHeatingDesignCapacity();
      autosizeMaximumSteamFlowRate();
    }

    void CoilHeatingSteamBaseboardRadiant_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedHeatingDesignCapacity();
      if (val) {
        setHeatingDesignCapacity(val.get());
      }

      val = autosizedMaximumSteamFlowRate();
      if (val) {
        setMaximumSteamFlowRate(val.get());
      }
    }

    ComponentType CoilHeatingSteamBaseboardRadiant_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> CoilHeatingSteamBaseboardRadiant_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> CoilHeatingSteamBaseboardRadiant_Impl::heatingFuelTypes() const {
      if (auto p_ = plantLoop()) {
        return p_->heatingFuelTypes();
      }
      return {};
    }

    std::vector<AppGFuelType> CoilHeatingSteamBaseboardRadiant_Impl::appGHeatingFuelTypes() const {
      if (auto p_ = plantLoop()) {
        return p_->appGHeatingFuelTypes();
      }
      return {};
    }

  }  // namespace detail

  CoilHeatingSteamBaseboardRadiant::CoilHeatingSteamBaseboardRadiant(const Model& model)
    : StraightComponent(CoilHeatingSteamBaseboardRadiant::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>());

    bool ok = true;
    ok = setHeatingDesignCapacityMethod("HeatingDesignCapacity");
    OS_ASSERT(ok);
    autosizeHeatingDesignCapacity();
    ok = setHeatingDesignCapacityPerFloorArea(0);
    OS_ASSERT(ok);
    ok = setFractionofAutosizedHeatingDesignCapacity(1.0);
    OS_ASSERT(ok);
    ok = setDegreeofSubCooling(5.0);
    autosizeMaximumSteamFlowRate();
    ok = setConvergenceTolerance(0.001);
    OS_ASSERT(ok);
  }

  IddObjectType CoilHeatingSteamBaseboardRadiant::iddObjectType() {
    return {IddObjectType::OS_Coil_Heating_Steam_Baseboard_Radiant};
  }

  std::vector<std::string> CoilHeatingSteamBaseboardRadiant::heatingDesignCapacityMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Coil_Heating_Steam_Baseboard_RadiantFields::HeatingDesignCapacityMethod);
  }

  std::string CoilHeatingSteamBaseboardRadiant::heatingDesignCapacityMethod() const {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->heatingDesignCapacityMethod();
  }

  boost::optional<double> CoilHeatingSteamBaseboardRadiant::heatingDesignCapacity() const {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->heatingDesignCapacity();
  }

  bool CoilHeatingSteamBaseboardRadiant::isHeatingDesignCapacityAutosized() const {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->isHeatingDesignCapacityAutosized();
  }

  double CoilHeatingSteamBaseboardRadiant::heatingDesignCapacityPerFloorArea() const {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->heatingDesignCapacityPerFloorArea();
  }

  double CoilHeatingSteamBaseboardRadiant::fractionofAutosizedHeatingDesignCapacity() const {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->fractionofAutosizedHeatingDesignCapacity();
  }

  double CoilHeatingSteamBaseboardRadiant::degreeofSubCooling() const {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->degreeofSubCooling();
  }

  boost::optional<double> CoilHeatingSteamBaseboardRadiant::maximumSteamFlowRate() const {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->maximumSteamFlowRate();
  }

  bool CoilHeatingSteamBaseboardRadiant::isMaximumSteamFlowRateAutosized() const {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->isMaximumSteamFlowRateAutosized();
  }

  double CoilHeatingSteamBaseboardRadiant::convergenceTolerance() const {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->convergenceTolerance();
  }

  bool CoilHeatingSteamBaseboardRadiant::setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod) {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->setHeatingDesignCapacityMethod(heatingDesignCapacityMethod);
  }

  bool CoilHeatingSteamBaseboardRadiant::setHeatingDesignCapacity(double heatingDesignCapacity) {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->setHeatingDesignCapacity(heatingDesignCapacity);
  }

  void CoilHeatingSteamBaseboardRadiant::autosizeHeatingDesignCapacity() {
    getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->autosizeHeatingDesignCapacity();
  }

  bool CoilHeatingSteamBaseboardRadiant::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->setHeatingDesignCapacityPerFloorArea(heatingDesignCapacityPerFloorArea);
  }

  bool CoilHeatingSteamBaseboardRadiant::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->setFractionofAutosizedHeatingDesignCapacity(
      fractionofAutosizedHeatingDesignCapacity);
  }

  bool CoilHeatingSteamBaseboardRadiant::setDegreeofSubCooling(double degreeofSubCooling) {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->setDegreeofSubCooling(degreeofSubCooling);
  }

  bool CoilHeatingSteamBaseboardRadiant::setMaximumSteamFlowRate(double maximumSteamFlowRate) {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->setMaximumSteamFlowRate(maximumSteamFlowRate);
  }

  void CoilHeatingSteamBaseboardRadiant::autosizeMaximumSteamFlowRate() {
    getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->autosizeMaximumSteamFlowRate();
  }

  bool CoilHeatingSteamBaseboardRadiant::setConvergenceTolerance(double convergenceTolerance) {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->setConvergenceTolerance(convergenceTolerance);
  }

  /// @cond
  CoilHeatingSteamBaseboardRadiant::CoilHeatingSteamBaseboardRadiant(std::shared_ptr<detail::CoilHeatingSteamBaseboardRadiant_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> CoilHeatingSteamBaseboardRadiant::autosizedHeatingDesignCapacity() const {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->autosizedHeatingDesignCapacity();
  }

  boost::optional<double> CoilHeatingSteamBaseboardRadiant::autosizedMaximumSteamFlowRate() const {
    return getImpl<detail::CoilHeatingSteamBaseboardRadiant_Impl>()->autosizedMaximumSteamFlowRate();
  }

}  // namespace model
}  // namespace openstudio
