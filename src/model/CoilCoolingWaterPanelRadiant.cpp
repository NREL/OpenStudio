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

#include "CoilCoolingWaterPanelRadiant.hpp"
#include "CoilCoolingWaterPanelRadiant_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "ZoneHVACPanelRadiantConvectiveWater.hpp"
#include "ZoneHVACPanelRadiantConvectiveWater_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_Cooling_Water_Panel_Radiant_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilCoolingWaterPanelRadiant_Impl::CoilCoolingWaterPanelRadiant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilCoolingWaterPanelRadiant::iddObjectType());
    }

    CoilCoolingWaterPanelRadiant_Impl::CoilCoolingWaterPanelRadiant_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilCoolingWaterPanelRadiant::iddObjectType());
    }

    CoilCoolingWaterPanelRadiant_Impl::CoilCoolingWaterPanelRadiant_Impl(const CoilCoolingWaterPanelRadiant_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilCoolingWaterPanelRadiant_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType CoilCoolingWaterPanelRadiant_Impl::iddObjectType() const {
      return CoilCoolingWaterPanelRadiant::iddObjectType();
    }

    unsigned CoilCoolingWaterPanelRadiant_Impl::inletPort() const {
      return OS_Coil_Heating_Water_Baseboard_RadiantFields::InletNodeName;
    }

    unsigned CoilCoolingWaterPanelRadiant_Impl::outletPort() const {
      return OS_Coil_Heating_Water_Baseboard_RadiantFields::OutletNodeName;
    }

    boost::optional<ZoneHVACComponent> CoilCoolingWaterPanelRadiant_Impl::containingZoneHVACComponent() const {
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

    bool CoilCoolingWaterPanelRadiant_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->demandComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    double CoilCoolingWaterPanelRadiant_Impl::ratedAverageWaterTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::RatedAverageWaterTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingWaterPanelRadiant_Impl::ratedWaterMassFlowRate() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::RatedWaterMassFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string CoilCoolingWaterPanelRadiant_Impl::heatingDesignCapacityMethod() const {
      boost::optional<std::string> value = getString(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacityMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingWaterPanelRadiant_Impl::heatingDesignCapacity() const {
      return getDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacity, true);
    }

    bool CoilCoolingWaterPanelRadiant_Impl::isHeatingDesignCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilCoolingWaterPanelRadiant_Impl::heatingDesignCapacityPerFloorArea() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacityPerFloorArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingWaterPanelRadiant_Impl::fractionofAutosizedHeatingDesignCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::FractionofAutosizedHeatingDesignCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingWaterPanelRadiant_Impl::maximumWaterFlowRate() const {
      return getDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::MaximumWaterFlowRate, true);
    }

    bool CoilCoolingWaterPanelRadiant_Impl::isMaximumWaterFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_Water_Baseboard_RadiantFields::MaximumWaterFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilCoolingWaterPanelRadiant_Impl::convergenceTolerance() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::ConvergenceTolerance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setRatedAverageWaterTemperature(double ratedAverageWaterTemperature) {
      bool result = setDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::RatedAverageWaterTemperature, ratedAverageWaterTemperature);
      return result;
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setRatedWaterMassFlowRate(double ratedWaterMassFlowRate) {
      bool result = setDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::RatedWaterMassFlowRate, ratedWaterMassFlowRate);
      return result;
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod) {
      bool result = setString(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacityMethod, heatingDesignCapacityMethod);
      return result;
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity) {
      bool result(false);
      if (heatingDesignCapacity) {
        result = setDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacity, heatingDesignCapacity.get());
      }
      return result;
    }

    void CoilCoolingWaterPanelRadiant_Impl::autosizeHeatingDesignCapacity() {
      bool result = setString(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
      bool result = setDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacityPerFloorArea, heatingDesignCapacityPerFloorArea);
      return result;
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
      bool result =
        setDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::FractionofAutosizedHeatingDesignCapacity, fractionofAutosizedHeatingDesignCapacity);
      return result;
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setMaximumWaterFlowRate(boost::optional<double> maximumWaterFlowRate) {
      bool result(false);
      if (maximumWaterFlowRate) {
        result = setDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::MaximumWaterFlowRate, maximumWaterFlowRate.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingWaterPanelRadiant_Impl::autosizeMaximumWaterFlowRate() {
      bool result = setString(OS_Coil_Heating_Water_Baseboard_RadiantFields::MaximumWaterFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setConvergenceTolerance(double convergenceTolerance) {
      bool result = setDouble(OS_Coil_Heating_Water_Baseboard_RadiantFields::ConvergenceTolerance, convergenceTolerance);
      return result;
    }

    boost::optional<double> CoilCoolingWaterPanelRadiant_Impl::autosizedHeatingDesignCapacity() const {
      boost::optional<double> result;
      // Get the containing ZoneHVAC equipment and get its autosized value
      auto parentHVAC = containingZoneHVACComponent();
      if (!parentHVAC) {
        return result;
      }
      return parentHVAC->getAutosizedValue("Design Size Heating Design Capacity", "W");
    }

    boost::optional<double> CoilCoolingWaterPanelRadiant_Impl::autosizedMaximumWaterFlowRate() const {
      boost::optional<double> result;
      // Get the containing ZoneHVAC equipment and get its autosized value
      auto parentHVAC = containingZoneHVACComponent();
      if (!parentHVAC) {
        return result;
      }
      return parentHVAC->getAutosizedValue("Design Size Maximum Water Flow Rate", "m3/s");
    }

    void CoilCoolingWaterPanelRadiant_Impl::autosize() {
      autosizeHeatingDesignCapacity();
      autosizeMaximumWaterFlowRate();
    }

    void CoilCoolingWaterPanelRadiant_Impl::applySizingValues() {
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

  }  // namespace detail

  CoilCoolingWaterPanelRadiant::CoilCoolingWaterPanelRadiant(const Model& model)
    : StraightComponent(CoilCoolingWaterPanelRadiant::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>());

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

  IddObjectType CoilCoolingWaterPanelRadiant::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Coil_Heating_Water_Baseboard_Radiant);
  }

  std::vector<std::string> CoilCoolingWaterPanelRadiant::heatingDesignCapacityMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Coil_Heating_Water_Baseboard_RadiantFields::HeatingDesignCapacityMethod);
  }

  double CoilCoolingWaterPanelRadiant::ratedAverageWaterTemperature() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->ratedAverageWaterTemperature();
  }

  double CoilCoolingWaterPanelRadiant::ratedWaterMassFlowRate() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->ratedWaterMassFlowRate();
  }

  std::string CoilCoolingWaterPanelRadiant::heatingDesignCapacityMethod() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->heatingDesignCapacityMethod();
  }

  boost::optional<double> CoilCoolingWaterPanelRadiant::heatingDesignCapacity() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->heatingDesignCapacity();
  }

  bool CoilCoolingWaterPanelRadiant::isHeatingDesignCapacityAutosized() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->isHeatingDesignCapacityAutosized();
  }

  double CoilCoolingWaterPanelRadiant::heatingDesignCapacityPerFloorArea() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->heatingDesignCapacityPerFloorArea();
  }

  double CoilCoolingWaterPanelRadiant::fractionofAutosizedHeatingDesignCapacity() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->fractionofAutosizedHeatingDesignCapacity();
  }

  boost::optional<double> CoilCoolingWaterPanelRadiant::maximumWaterFlowRate() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->maximumWaterFlowRate();
  }

  bool CoilCoolingWaterPanelRadiant::isMaximumWaterFlowRateAutosized() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->isMaximumWaterFlowRateAutosized();
  }

  double CoilCoolingWaterPanelRadiant::convergenceTolerance() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->convergenceTolerance();
  }

  bool CoilCoolingWaterPanelRadiant::setRatedAverageWaterTemperature(double ratedAverageWaterTemperature) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setRatedAverageWaterTemperature(ratedAverageWaterTemperature);
  }

  bool CoilCoolingWaterPanelRadiant::setRatedWaterMassFlowRate(double ratedWaterMassFlowRate) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setRatedWaterMassFlowRate(ratedWaterMassFlowRate);
  }

  bool CoilCoolingWaterPanelRadiant::setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setHeatingDesignCapacityMethod(heatingDesignCapacityMethod);
  }

  bool CoilCoolingWaterPanelRadiant::setHeatingDesignCapacity(double heatingDesignCapacity) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setHeatingDesignCapacity(heatingDesignCapacity);
  }

  void CoilCoolingWaterPanelRadiant::autosizeHeatingDesignCapacity() {
    getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->autosizeHeatingDesignCapacity();
  }

  bool CoilCoolingWaterPanelRadiant::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setHeatingDesignCapacityPerFloorArea(heatingDesignCapacityPerFloorArea);
  }

  bool CoilCoolingWaterPanelRadiant::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setFractionofAutosizedHeatingDesignCapacity(
      fractionofAutosizedHeatingDesignCapacity);
  }

  bool CoilCoolingWaterPanelRadiant::setMaximumWaterFlowRate(double maximumWaterFlowRate) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setMaximumWaterFlowRate(maximumWaterFlowRate);
  }

  void CoilCoolingWaterPanelRadiant::autosizeMaximumWaterFlowRate() {
    getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->autosizeMaximumWaterFlowRate();
  }

  bool CoilCoolingWaterPanelRadiant::setConvergenceTolerance(double convergenceTolerance) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setConvergenceTolerance(convergenceTolerance);
  }

  /// @cond
  CoilCoolingWaterPanelRadiant::CoilCoolingWaterPanelRadiant(std::shared_ptr<detail::CoilCoolingWaterPanelRadiant_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> CoilCoolingWaterPanelRadiant::autosizedHeatingDesignCapacity() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->autosizedHeatingDesignCapacity();
  }

  boost::optional<double> CoilCoolingWaterPanelRadiant::autosizedMaximumWaterFlowRate() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->autosizedMaximumWaterFlowRate();
  }

}  // namespace model
}  // namespace openstudio
