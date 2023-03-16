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

#include "CoilCoolingWaterPanelRadiant.hpp"
#include "CoilCoolingWaterPanelRadiant_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "ZoneHVACCoolingPanelRadiantConvectiveWater.hpp"
#include "ZoneHVACCoolingPanelRadiantConvectiveWater_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

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

    CoilCoolingWaterPanelRadiant_Impl::CoilCoolingWaterPanelRadiant_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                         bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilCoolingWaterPanelRadiant::iddObjectType());
    }

    CoilCoolingWaterPanelRadiant_Impl::CoilCoolingWaterPanelRadiant_Impl(const CoilCoolingWaterPanelRadiant_Impl& other, Model_Impl* model,
                                                                         bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilCoolingWaterPanelRadiant_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType CoilCoolingWaterPanelRadiant_Impl::iddObjectType() const {
      return CoilCoolingWaterPanelRadiant::iddObjectType();
    }

    unsigned CoilCoolingWaterPanelRadiant_Impl::inletPort() const {
      return OS_Coil_Cooling_Water_Panel_RadiantFields::WaterInletNodeName;
    }

    unsigned CoilCoolingWaterPanelRadiant_Impl::outletPort() const {
      return OS_Coil_Cooling_Water_Panel_RadiantFields::WaterOutletNodeName;
    }

    boost::optional<ZoneHVACComponent> CoilCoolingWaterPanelRadiant_Impl::containingZoneHVACComponent() const {
      // this coil can only be found in a ZoneHVACCoolingPanelRadiantConvectiveWater
      // check all ZoneHVACCoolingPanelRadiantConvectiveWaters in the model, seeing if this coil
      // is inside of one of them.  Return the one it is inside of

      auto const zoneHVACCoolingPanelRadiantConvectiveWaters = this->model().getConcreteModelObjects<ZoneHVACCoolingPanelRadiantConvectiveWater>();
      // loop through each one, seeing if the coil is contained by the zonehvaccoolingpanel
      for (const auto& zoneHVACCoolingPanelRadiantConvectiveWater : zoneHVACCoolingPanelRadiantConvectiveWaters) {
        if (boost::optional<HVACComponent> coil = zoneHVACCoolingPanelRadiantConvectiveWater.coolingCoil()) {
          if (coil->handle() == this->handle()) {  //if the handles match, this coil is inside of a zonehvaccoolingpanel
            return zoneHVACCoolingPanelRadiantConvectiveWater;
          }
        }
      }

      // if the coil isn't inside any zonehvaccoolingpanels (which currently should never happen), return nothing
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

    double CoilCoolingWaterPanelRadiant_Impl::ratedInletWaterTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::RatedInletWaterTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingWaterPanelRadiant_Impl::ratedInletSpaceTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::RatedInletSpaceTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingWaterPanelRadiant_Impl::ratedWaterMassFlowRate() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::RatedWaterMassFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string CoilCoolingWaterPanelRadiant_Impl::coolingDesignCapacityMethod() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_Water_Panel_RadiantFields::CoolingDesignCapacityMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingWaterPanelRadiant_Impl::coolingDesignCapacity() const {
      return getDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::CoolingDesignCapacity, true);
    }

    bool CoilCoolingWaterPanelRadiant_Impl::isCoolingDesignCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_Water_Panel_RadiantFields::CoolingDesignCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilCoolingWaterPanelRadiant_Impl::coolingDesignCapacityPerFloorArea() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::CoolingDesignCapacityPerFloorArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingWaterPanelRadiant_Impl::fractionofAutosizedCoolingDesignCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::FractionofAutosizedCoolingDesignCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingWaterPanelRadiant_Impl::maximumChilledWaterFlowRate() const {
      return getDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::MaximumChilledWaterFlowRate, true);
    }

    bool CoilCoolingWaterPanelRadiant_Impl::isMaximumChilledWaterFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_Water_Panel_RadiantFields::MaximumChilledWaterFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    std::string CoilCoolingWaterPanelRadiant_Impl::controlType() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_Water_Panel_RadiantFields::ControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingWaterPanelRadiant_Impl::coolingControlThrottlingRange() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::CoolingControlThrottlingRange, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> CoilCoolingWaterPanelRadiant_Impl::coolingControlTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_Coil_Cooling_Water_Panel_RadiantFields::CoolingControlTemperatureScheduleName);
    }

    std::string CoilCoolingWaterPanelRadiant_Impl::condensationControlType() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_Water_Panel_RadiantFields::CondensationControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingWaterPanelRadiant_Impl::condensationControlDewpointOffset() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::CondensationControlDewpointOffset, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setRatedInletWaterTemperature(double ratedInletWaterTemperature) {
      bool result = setDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::RatedInletWaterTemperature, ratedInletWaterTemperature);
      return result;
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setRatedInletSpaceTemperature(double ratedInletSpaceTemperature) {
      bool result = setDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::RatedInletSpaceTemperature, ratedInletSpaceTemperature);
      return result;
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setRatedWaterMassFlowRate(double ratedWaterMassFlowRate) {
      bool result = setDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::RatedWaterMassFlowRate, ratedWaterMassFlowRate);
      return result;
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setCoolingDesignCapacityMethod(const std::string& coolingDesignCapacityMethod) {
      bool result = setString(OS_Coil_Cooling_Water_Panel_RadiantFields::CoolingDesignCapacityMethod, coolingDesignCapacityMethod);
      return result;
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setCoolingDesignCapacity(boost::optional<double> coolingDesignCapacity) {
      bool result(false);
      if (coolingDesignCapacity) {
        result = setDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::CoolingDesignCapacity, coolingDesignCapacity.get());
      }
      return result;
    }

    void CoilCoolingWaterPanelRadiant_Impl::autosizeCoolingDesignCapacity() {
      bool result = setString(OS_Coil_Cooling_Water_Panel_RadiantFields::CoolingDesignCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea) {
      bool result = setDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::CoolingDesignCapacityPerFloorArea, coolingDesignCapacityPerFloorArea);
      return result;
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity) {
      bool result =
        setDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::FractionofAutosizedCoolingDesignCapacity, fractionofAutosizedCoolingDesignCapacity);
      return result;
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setMaximumChilledWaterFlowRate(boost::optional<double> maximumChilledWaterFlowRate) {
      bool result(false);
      if (maximumChilledWaterFlowRate) {
        result = setDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::MaximumChilledWaterFlowRate, maximumChilledWaterFlowRate.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingWaterPanelRadiant_Impl::autosizeMaximumChilledWaterFlowRate() {
      bool result = setString(OS_Coil_Cooling_Water_Panel_RadiantFields::MaximumChilledWaterFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setControlType(const std::string& controlType) {
      bool result = setString(OS_Coil_Cooling_Water_Panel_RadiantFields::ControlType, controlType);
      return result;
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setCoolingControlThrottlingRange(double coolingControlThrottlingRange) {
      bool result = setDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::CoolingControlThrottlingRange, coolingControlThrottlingRange);
      return result;
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setCoolingControlTemperatureSchedule(Schedule& coolingControlTemperatureSchedule) {
      bool result = setSchedule(OS_Coil_Cooling_Water_Panel_RadiantFields::CoolingControlTemperatureScheduleName, "CoilCoolingWaterPanelRadiant",
                                "Cooling Control Temperature", coolingControlTemperatureSchedule);
      return result;
    }

    void CoilCoolingWaterPanelRadiant_Impl::resetCoolingControlTemperatureSchedule() {
      bool result = setString(OS_Coil_Cooling_Water_Panel_RadiantFields::CoolingControlTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setCondensationControlType(const std::string& condensationControlType) {
      bool result = setString(OS_Coil_Cooling_Water_Panel_RadiantFields::CondensationControlType, condensationControlType);
      return result;
    }

    bool CoilCoolingWaterPanelRadiant_Impl::setCondensationControlDewpointOffset(double condensationControlDewpointOffset) {
      bool result = setDouble(OS_Coil_Cooling_Water_Panel_RadiantFields::CondensationControlDewpointOffset, condensationControlDewpointOffset);
      return result;
    }

    boost::optional<double> CoilCoolingWaterPanelRadiant_Impl::autosizedCoolingDesignCapacity() const {
      boost::optional<double> result;
      // Get the containing ZoneHVAC equipment and get its autosized value
      auto parentHVAC = containingZoneHVACComponent();
      if (!parentHVAC) {
        return result;
      }
      return parentHVAC->getAutosizedValue("Design Size Cooling Design Capacity", "W");
    }

    boost::optional<double> CoilCoolingWaterPanelRadiant_Impl::autosizedMaximumChilledWaterFlowRate() const {
      boost::optional<double> result;
      // Get the containing ZoneHVAC equipment and get its autosized value
      auto parentHVAC = containingZoneHVACComponent();
      if (!parentHVAC) {
        return result;
      }
      return parentHVAC->getAutosizedValue("Design Size Maximum Cold Water Flow", "m3/s");
    }

    void CoilCoolingWaterPanelRadiant_Impl::autosize() {
      autosizeCoolingDesignCapacity();
      autosizeMaximumChilledWaterFlowRate();
    }

    void CoilCoolingWaterPanelRadiant_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedCoolingDesignCapacity();
      if (val) {
        setCoolingDesignCapacity(val.get());
      }

      val = autosizedMaximumChilledWaterFlowRate();
      if (val) {
        setMaximumChilledWaterFlowRate(val.get());
      }
    }

  }  // namespace detail

  CoilCoolingWaterPanelRadiant::CoilCoolingWaterPanelRadiant(const Model& model)
    : StraightComponent(CoilCoolingWaterPanelRadiant::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>());

    bool ok = true;
    ok = setRatedInletWaterTemperature(5.0);
    OS_ASSERT(ok);
    ok = setRatedInletSpaceTemperature(24.0);
    OS_ASSERT(ok);
    ok = setRatedWaterMassFlowRate(0.063);
    OS_ASSERT(ok);
    ok = setCoolingDesignCapacityMethod("CoolingDesignCapacity");
    OS_ASSERT(ok);
    autosizeCoolingDesignCapacity();
    ok = setCoolingDesignCapacityPerFloorArea(0);
    OS_ASSERT(ok);
    ok = setFractionofAutosizedCoolingDesignCapacity(1.0);
    OS_ASSERT(ok);
    autosizeMaximumChilledWaterFlowRate();
    ok = setControlType("MeanAirTemperature");
    OS_ASSERT(ok);
    ok = setCoolingControlThrottlingRange(0.5);
    OS_ASSERT(ok);
    ok = setCondensationControlType("SimpleOff");
    OS_ASSERT(ok);
    ok = setCondensationControlDewpointOffset(1.0);
    OS_ASSERT(ok);
  }

  IddObjectType CoilCoolingWaterPanelRadiant::iddObjectType() {
    return {IddObjectType::OS_Coil_Cooling_Water_Panel_Radiant};
  }

  std::vector<std::string> CoilCoolingWaterPanelRadiant::coolingDesignCapacityMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Coil_Cooling_Water_Panel_RadiantFields::CoolingDesignCapacityMethod);
  }

  double CoilCoolingWaterPanelRadiant::ratedInletWaterTemperature() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->ratedInletWaterTemperature();
  }

  double CoilCoolingWaterPanelRadiant::ratedInletSpaceTemperature() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->ratedInletSpaceTemperature();
  }

  double CoilCoolingWaterPanelRadiant::ratedWaterMassFlowRate() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->ratedWaterMassFlowRate();
  }

  std::string CoilCoolingWaterPanelRadiant::coolingDesignCapacityMethod() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->coolingDesignCapacityMethod();
  }

  boost::optional<double> CoilCoolingWaterPanelRadiant::coolingDesignCapacity() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->coolingDesignCapacity();
  }

  bool CoilCoolingWaterPanelRadiant::isCoolingDesignCapacityAutosized() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->isCoolingDesignCapacityAutosized();
  }

  double CoilCoolingWaterPanelRadiant::coolingDesignCapacityPerFloorArea() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->coolingDesignCapacityPerFloorArea();
  }

  double CoilCoolingWaterPanelRadiant::fractionofAutosizedCoolingDesignCapacity() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->fractionofAutosizedCoolingDesignCapacity();
  }

  boost::optional<double> CoilCoolingWaterPanelRadiant::maximumChilledWaterFlowRate() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->maximumChilledWaterFlowRate();
  }

  bool CoilCoolingWaterPanelRadiant::isMaximumChilledWaterFlowRateAutosized() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->isMaximumChilledWaterFlowRateAutosized();
  }

  std::string CoilCoolingWaterPanelRadiant::controlType() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->controlType();
  }

  double CoilCoolingWaterPanelRadiant::coolingControlThrottlingRange() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->coolingControlThrottlingRange();
  }

  boost::optional<Schedule> CoilCoolingWaterPanelRadiant::coolingControlTemperatureSchedule() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->coolingControlTemperatureSchedule();
  }

  std::string CoilCoolingWaterPanelRadiant::condensationControlType() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->condensationControlType();
  }

  double CoilCoolingWaterPanelRadiant::condensationControlDewpointOffset() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->condensationControlDewpointOffset();
  }

  bool CoilCoolingWaterPanelRadiant::setRatedInletWaterTemperature(double ratedInletWaterTemperature) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setRatedInletWaterTemperature(ratedInletWaterTemperature);
  }

  bool CoilCoolingWaterPanelRadiant::setRatedInletSpaceTemperature(double ratedInletSpaceTemperature) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setRatedInletSpaceTemperature(ratedInletSpaceTemperature);
  }

  bool CoilCoolingWaterPanelRadiant::setRatedWaterMassFlowRate(double ratedWaterMassFlowRate) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setRatedWaterMassFlowRate(ratedWaterMassFlowRate);
  }

  bool CoilCoolingWaterPanelRadiant::setCoolingDesignCapacityMethod(const std::string& coolingDesignCapacityMethod) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setCoolingDesignCapacityMethod(coolingDesignCapacityMethod);
  }

  bool CoilCoolingWaterPanelRadiant::setCoolingDesignCapacity(double coolingDesignCapacity) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setCoolingDesignCapacity(coolingDesignCapacity);
  }

  void CoilCoolingWaterPanelRadiant::autosizeCoolingDesignCapacity() {
    getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->autosizeCoolingDesignCapacity();
  }

  bool CoilCoolingWaterPanelRadiant::setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setCoolingDesignCapacityPerFloorArea(coolingDesignCapacityPerFloorArea);
  }

  bool CoilCoolingWaterPanelRadiant::setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setFractionofAutosizedCoolingDesignCapacity(
      fractionofAutosizedCoolingDesignCapacity);
  }

  bool CoilCoolingWaterPanelRadiant::setMaximumChilledWaterFlowRate(double maximumChilledWaterFlowRate) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setMaximumChilledWaterFlowRate(maximumChilledWaterFlowRate);
  }

  void CoilCoolingWaterPanelRadiant::autosizeMaximumChilledWaterFlowRate() {
    getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->autosizeMaximumChilledWaterFlowRate();
  }

  bool CoilCoolingWaterPanelRadiant::setControlType(const std::string& controlType) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setControlType(controlType);
  }

  bool CoilCoolingWaterPanelRadiant::setCoolingControlThrottlingRange(double coolingControlThrottlingRange) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setCoolingControlThrottlingRange(coolingControlThrottlingRange);
  }

  bool CoilCoolingWaterPanelRadiant::setCoolingControlTemperatureSchedule(Schedule& coolingControlTemperatureSchedule) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setCoolingControlTemperatureSchedule(coolingControlTemperatureSchedule);
  }

  void CoilCoolingWaterPanelRadiant::resetCoolingControlTemperatureSchedule() {
    getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->resetCoolingControlTemperatureSchedule();
  }

  bool CoilCoolingWaterPanelRadiant::setCondensationControlType(const std::string& condensationControlType) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setCondensationControlType(condensationControlType);
  }

  bool CoilCoolingWaterPanelRadiant::setCondensationControlDewpointOffset(double condensationControlDewpointOffset) {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->setCondensationControlDewpointOffset(condensationControlDewpointOffset);
  }

  /// @cond
  CoilCoolingWaterPanelRadiant::CoilCoolingWaterPanelRadiant(std::shared_ptr<detail::CoilCoolingWaterPanelRadiant_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> CoilCoolingWaterPanelRadiant::autosizedCoolingDesignCapacity() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->autosizedCoolingDesignCapacity();
  }

  boost::optional<double> CoilCoolingWaterPanelRadiant::autosizedMaximumChilledWaterFlowRate() const {
    return getImpl<detail::CoilCoolingWaterPanelRadiant_Impl>()->autosizedMaximumChilledWaterFlowRate();
  }

}  // namespace model
}  // namespace openstudio
