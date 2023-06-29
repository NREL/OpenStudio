/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CoilHeatingWaterBaseboardRadiant.hpp"
#include "CoilHeatingWaterBaseboardRadiant_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "ZoneHVACBaseboardRadiantConvectiveWater.hpp"
#include "ZoneHVACBaseboardRadiantConvectiveWater_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_Heating_Water_Baseboard_Radiant_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilHeatingWaterBaseboardRadiant_Impl::CoilHeatingWaterBaseboardRadiant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilHeatingWaterBaseboardRadiant::iddObjectType());
    }

    CoilHeatingWaterBaseboardRadiant_Impl::CoilHeatingWaterBaseboardRadiant_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilHeatingWaterBaseboardRadiant::iddObjectType());
    }

    CoilHeatingWaterBaseboardRadiant_Impl::CoilHeatingWaterBaseboardRadiant_Impl(const CoilHeatingWaterBaseboardRadiant_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilHeatingWaterBaseboardRadiant_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType CoilHeatingWaterBaseboardRadiant_Impl::iddObjectType() const {
      return CoilHeatingWaterBaseboardRadiant::iddObjectType();
    }

    unsigned CoilHeatingWaterBaseboardRadiant_Impl::inletPort() const {
      return OS_Coil_Heating_Water_Baseboard_RadiantFields::InletNodeName;
    }

    unsigned CoilHeatingWaterBaseboardRadiant_Impl::outletPort() const {
      return OS_Coil_Heating_Water_Baseboard_RadiantFields::OutletNodeName;
    }

    boost::optional<ZoneHVACComponent> CoilHeatingWaterBaseboardRadiant_Impl::containingZoneHVACComponent() const {
      // this coil can only be found in a ZoneHVACBaseboardRadiantConvectiveWater
      // check all ZoneHVACBaseboardRadiantConvectiveWaters in the model, seeing if this coil
      // is inside of one of them.  Return the one it is inside of

      auto const zoneHVACBaseboardRadiantConvectiveWaters = this->model().getConcreteModelObjects<ZoneHVACBaseboardRadiantConvectiveWater>();
      // loop through each one, seeing if the coil is contained by the zonehvacbaseboard
      for (const auto& zoneHVACBaseboardRadiantConvectiveWater : zoneHVACBaseboardRadiantConvectiveWaters) {
        if (boost::optional<HVACComponent> coil = zoneHVACBaseboardRadiantConvectiveWater.heatingCoil()) {
          if (coil->handle() == this->handle()) {  //if the handles match, this coil is inside of a zonehvacbaseboard
            return zoneHVACBaseboardRadiantConvectiveWater;
          }
        }
      }

      // if the coil isn't inside any zonehvacbaseboards (which currently should never happen), return nothing
      return boost::none;
    }

    bool CoilHeatingWaterBaseboardRadiant_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->demandComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    double CoilHeatingWaterBaseboardRadiant_Impl::ratedAverageWaterTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::RatedAverageWaterTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilHeatingWaterBaseboardRadiant_Impl::ratedWaterMassFlowRate() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::RatedWaterMassFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string CoilHeatingWaterBaseboardRadiant_Impl::heatingDesignCapacityMethod() const {
      boost::optional<std::string> value = getString(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacityMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilHeatingWaterBaseboardRadiant_Impl::heatingDesignCapacity() const {
      return getDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacity, true);
    }

    bool CoilHeatingWaterBaseboardRadiant_Impl::isHeatingDesignCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilHeatingWaterBaseboardRadiant_Impl::heatingDesignCapacityPerFloorArea() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacityPerFloorArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilHeatingWaterBaseboardRadiant_Impl::fractionofAutosizedHeatingDesignCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::FractionofAutosizedHeatingDesignCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilHeatingWaterBaseboardRadiant_Impl::maximumWaterFlowRate() const {
      return getDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::MaximumWaterFlowRate, true);
    }

    bool CoilHeatingWaterBaseboardRadiant_Impl::isMaximumWaterFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_Water_Baseboard_RadiantFields::MaximumWaterFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilHeatingWaterBaseboardRadiant_Impl::convergenceTolerance() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::ConvergenceTolerance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingWaterBaseboardRadiant_Impl::setRatedAverageWaterTemperature(double ratedAverageWaterTemperature) {
      bool result = setDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::RatedAverageWaterTemperature, ratedAverageWaterTemperature);
      return result;
    }

    bool CoilHeatingWaterBaseboardRadiant_Impl::setRatedWaterMassFlowRate(double ratedWaterMassFlowRate) {
      bool result = setDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::RatedWaterMassFlowRate, ratedWaterMassFlowRate);
      return result;
    }

    bool CoilHeatingWaterBaseboardRadiant_Impl::setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod) {
      bool result = setString(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacityMethod, heatingDesignCapacityMethod);
      return result;
    }

    bool CoilHeatingWaterBaseboardRadiant_Impl::setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity) {
      bool result(false);
      if (heatingDesignCapacity) {
        result = setDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacity, heatingDesignCapacity.get());
      }
      return result;
    }

    void CoilHeatingWaterBaseboardRadiant_Impl::autosizeHeatingDesignCapacity() {
      bool result = setString(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool CoilHeatingWaterBaseboardRadiant_Impl::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
      bool result = setDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacityPerFloorArea, heatingDesignCapacityPerFloorArea);
      return result;
    }

    bool CoilHeatingWaterBaseboardRadiant_Impl::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
      bool result =
        setDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::FractionofAutosizedHeatingDesignCapacity, fractionofAutosizedHeatingDesignCapacity);
      return result;
    }

    bool CoilHeatingWaterBaseboardRadiant_Impl::setMaximumWaterFlowRate(boost::optional<double> maximumWaterFlowRate) {
      bool result(false);
      if (maximumWaterFlowRate) {
        result = setDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::MaximumWaterFlowRate, maximumWaterFlowRate.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void CoilHeatingWaterBaseboardRadiant_Impl::autosizeMaximumWaterFlowRate() {
      bool result = setString(OS_Coil_Heating_Water_Baseboard_RadiantFields::MaximumWaterFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool CoilHeatingWaterBaseboardRadiant_Impl::setConvergenceTolerance(double convergenceTolerance) {
      bool result = setDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::ConvergenceTolerance, convergenceTolerance);
      return result;
    }

    boost::optional<double> CoilHeatingWaterBaseboardRadiant_Impl::autosizedHeatingDesignCapacity() const {
      boost::optional<double> result;
      // Get the containing ZoneHVAC equipment and get its autosized value
      auto parentHVAC = containingZoneHVACComponent();
      if (!parentHVAC) {
        return result;
      }
      return parentHVAC->getAutosizedValue("Design Size Heating Design Capacity", "W");
    }

    boost::optional<double> CoilHeatingWaterBaseboardRadiant_Impl::autosizedMaximumWaterFlowRate() const {
      boost::optional<double> result;
      // Get the containing ZoneHVAC equipment and get its autosized value
      auto parentHVAC = containingZoneHVACComponent();
      if (!parentHVAC) {
        return result;
      }
      return parentHVAC->getAutosizedValue("Design Size Maximum Water Flow Rate", "m3/s");
    }

    void CoilHeatingWaterBaseboardRadiant_Impl::autosize() {
      autosizeHeatingDesignCapacity();
      autosizeMaximumWaterFlowRate();
    }

    void CoilHeatingWaterBaseboardRadiant_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedHeatingDesignCapacity();
      if (val) {
        setHeatingDesignCapacity(val.get());
      }

      val = autosizedMaximumWaterFlowRate();
      if (val) {
        setMaximumWaterFlowRate(val.get());
      }
    }

    ComponentType CoilHeatingWaterBaseboardRadiant_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> CoilHeatingWaterBaseboardRadiant_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> CoilHeatingWaterBaseboardRadiant_Impl::heatingFuelTypes() const {
      if (auto p_ = plantLoop()) {
        return p_->heatingFuelTypes();
      }
      return {};
    }

    std::vector<AppGFuelType> CoilHeatingWaterBaseboardRadiant_Impl::appGHeatingFuelTypes() const {
      if (auto p_ = plantLoop()) {
        return p_->appGHeatingFuelTypes();
      }
      return {};
    }

  }  // namespace detail

  CoilHeatingWaterBaseboardRadiant::CoilHeatingWaterBaseboardRadiant(const Model& model)
    : StraightComponent(CoilHeatingWaterBaseboardRadiant::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>());

    bool ok = true;
    ok = setRatedAverageWaterTemperature(87.78);
    OS_ASSERT(ok);
    ok = setRatedWaterMassFlowRate(0.063);
    OS_ASSERT(ok);
    ok = setHeatingDesignCapacityMethod("HeatingDesignCapacity");
    OS_ASSERT(ok);
    autosizeHeatingDesignCapacity();
    ok = setHeatingDesignCapacityPerFloorArea(0);
    OS_ASSERT(ok);
    ok = setFractionofAutosizedHeatingDesignCapacity(1.0);
    OS_ASSERT(ok);
    autosizeMaximumWaterFlowRate();
    ok = setConvergenceTolerance(0.001);
    OS_ASSERT(ok);
  }

  IddObjectType CoilHeatingWaterBaseboardRadiant::iddObjectType() {
    return {IddObjectType::OS_Coil_Heating_Water_Baseboard_Radiant};
  }

  std::vector<std::string> CoilHeatingWaterBaseboardRadiant::heatingDesignCapacityMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacityMethod);
  }

  double CoilHeatingWaterBaseboardRadiant::ratedAverageWaterTemperature() const {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->ratedAverageWaterTemperature();
  }

  double CoilHeatingWaterBaseboardRadiant::ratedWaterMassFlowRate() const {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->ratedWaterMassFlowRate();
  }

  std::string CoilHeatingWaterBaseboardRadiant::heatingDesignCapacityMethod() const {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->heatingDesignCapacityMethod();
  }

  boost::optional<double> CoilHeatingWaterBaseboardRadiant::heatingDesignCapacity() const {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->heatingDesignCapacity();
  }

  bool CoilHeatingWaterBaseboardRadiant::isHeatingDesignCapacityAutosized() const {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->isHeatingDesignCapacityAutosized();
  }

  double CoilHeatingWaterBaseboardRadiant::heatingDesignCapacityPerFloorArea() const {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->heatingDesignCapacityPerFloorArea();
  }

  double CoilHeatingWaterBaseboardRadiant::fractionofAutosizedHeatingDesignCapacity() const {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->fractionofAutosizedHeatingDesignCapacity();
  }

  boost::optional<double> CoilHeatingWaterBaseboardRadiant::maximumWaterFlowRate() const {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->maximumWaterFlowRate();
  }

  bool CoilHeatingWaterBaseboardRadiant::isMaximumWaterFlowRateAutosized() const {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->isMaximumWaterFlowRateAutosized();
  }

  double CoilHeatingWaterBaseboardRadiant::convergenceTolerance() const {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->convergenceTolerance();
  }

  bool CoilHeatingWaterBaseboardRadiant::setRatedAverageWaterTemperature(double ratedAverageWaterTemperature) {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->setRatedAverageWaterTemperature(ratedAverageWaterTemperature);
  }

  bool CoilHeatingWaterBaseboardRadiant::setRatedWaterMassFlowRate(double ratedWaterMassFlowRate) {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->setRatedWaterMassFlowRate(ratedWaterMassFlowRate);
  }

  bool CoilHeatingWaterBaseboardRadiant::setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod) {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->setHeatingDesignCapacityMethod(heatingDesignCapacityMethod);
  }

  bool CoilHeatingWaterBaseboardRadiant::setHeatingDesignCapacity(double heatingDesignCapacity) {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->setHeatingDesignCapacity(heatingDesignCapacity);
  }

  void CoilHeatingWaterBaseboardRadiant::autosizeHeatingDesignCapacity() {
    getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->autosizeHeatingDesignCapacity();
  }

  bool CoilHeatingWaterBaseboardRadiant::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->setHeatingDesignCapacityPerFloorArea(heatingDesignCapacityPerFloorArea);
  }

  bool CoilHeatingWaterBaseboardRadiant::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->setFractionofAutosizedHeatingDesignCapacity(
      fractionofAutosizedHeatingDesignCapacity);
  }

  bool CoilHeatingWaterBaseboardRadiant::setMaximumWaterFlowRate(double maximumWaterFlowRate) {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->setMaximumWaterFlowRate(maximumWaterFlowRate);
  }

  void CoilHeatingWaterBaseboardRadiant::autosizeMaximumWaterFlowRate() {
    getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->autosizeMaximumWaterFlowRate();
  }

  bool CoilHeatingWaterBaseboardRadiant::setConvergenceTolerance(double convergenceTolerance) {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->setConvergenceTolerance(convergenceTolerance);
  }

  /// @cond
  CoilHeatingWaterBaseboardRadiant::CoilHeatingWaterBaseboardRadiant(std::shared_ptr<detail::CoilHeatingWaterBaseboardRadiant_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> CoilHeatingWaterBaseboardRadiant::autosizedHeatingDesignCapacity() const {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->autosizedHeatingDesignCapacity();
  }

  boost::optional<double> CoilHeatingWaterBaseboardRadiant::autosizedMaximumWaterFlowRate() const {
    return getImpl<detail::CoilHeatingWaterBaseboardRadiant_Impl>()->autosizedMaximumWaterFlowRate();
  }

}  // namespace model
}  // namespace openstudio
