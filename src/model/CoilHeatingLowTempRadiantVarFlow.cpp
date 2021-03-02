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

#include "CoilHeatingLowTempRadiantVarFlow.hpp"
#include "CoilHeatingLowTempRadiantVarFlow_Impl.hpp"

#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ZoneHVACLowTempRadiantVarFlow.hpp"
#include "ZoneHVACLowTempRadiantVarFlow_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Coil_Heating_LowTemperatureRadiant_VariableFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilHeatingLowTempRadiantVarFlow_Impl::CoilHeatingLowTempRadiantVarFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilHeatingLowTempRadiantVarFlow::iddObjectType());
    }

    CoilHeatingLowTempRadiantVarFlow_Impl::CoilHeatingLowTempRadiantVarFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilHeatingLowTempRadiantVarFlow::iddObjectType());
    }

    CoilHeatingLowTempRadiantVarFlow_Impl::CoilHeatingLowTempRadiantVarFlow_Impl(const CoilHeatingLowTempRadiantVarFlow_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    //ModelObject CoilHeatingLowTempRadiantVarFlow_Impl::clone(Model model) const
    //{
    //  CoilHeatingLowTempRadiantVarFlow newCoil = WaterToAirComponent_Impl::clone(model).optionalCast<CoilHeatingLowTempRadiantVarFlow>().get();

    //  return newCoil;
    //}

    const std::vector<std::string>& CoilHeatingLowTempRadiantVarFlow_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType CoilHeatingLowTempRadiantVarFlow_Impl::iddObjectType() const {
      return CoilHeatingLowTempRadiantVarFlow::iddObjectType();
    }

    std::vector<ScheduleTypeKey> CoilHeatingLowTempRadiantVarFlow_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());

      if (std::find(b, e, OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("CoilHeatingLowTempRadiantVarFlow", "Heating Control Temperature"));
      }
      return result;
    }

    unsigned CoilHeatingLowTempRadiantVarFlow_Impl::inletPort() const {
      return OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingWaterInletNodeName;
    }

    unsigned CoilHeatingLowTempRadiantVarFlow_Impl::outletPort() const {
      return OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingWaterOutletNodeName;
    }

    boost::optional<ZoneHVACComponent> CoilHeatingLowTempRadiantVarFlow_Impl::containingZoneHVACComponent() const {
      std::vector<ZoneHVACLowTempRadiantVarFlow> zoneHVACLowTempRadiantVarFlows;

      zoneHVACLowTempRadiantVarFlows = this->model().getConcreteModelObjects<ZoneHVACLowTempRadiantVarFlow>();

      for (const auto& zoneHVACLowTempRadiantVarFlow : zoneHVACLowTempRadiantVarFlows) {
        if (boost::optional<HVACComponent> coil = zoneHVACLowTempRadiantVarFlow.heatingCoil()) {
          if (coil->handle() == this->handle()) {
            return zoneHVACLowTempRadiantVarFlow;
          }
        }
      }
      return boost::none;
    }

    boost::optional<double> CoilHeatingLowTempRadiantVarFlow_Impl::maximumHotWaterFlow() const {
      return getDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow, true);
    }

    bool CoilHeatingLowTempRadiantVarFlow_Impl::isMaximumHotWaterFlowDefaulted() const {
      return isEmpty(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow);
    }

    bool CoilHeatingLowTempRadiantVarFlow_Impl::isMaximumHotWaterFlowAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilHeatingLowTempRadiantVarFlow_Impl::heatingControlThrottlingRange() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlThrottlingRange, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingLowTempRadiantVarFlow_Impl::isHeatingControlThrottlingRangeDefaulted() const {
      return isEmpty(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlThrottlingRange);
    }

    boost::optional<Schedule> CoilHeatingLowTempRadiantVarFlow_Impl::heatingControlTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlTemperatureScheduleName);
    }

    bool CoilHeatingLowTempRadiantVarFlow_Impl::setMaximumHotWaterFlow(boost::optional<double> maximumHotWaterFlow) {
      bool result(false);
      if (maximumHotWaterFlow) {
        result = setDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow, maximumHotWaterFlow.get());
      } else {
        resetMaximumHotWaterFlow();
        result = true;
      }
      return result;
    }

    void CoilHeatingLowTempRadiantVarFlow_Impl::resetMaximumHotWaterFlow() {
      bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow, "");
      OS_ASSERT(result);
    }

    void CoilHeatingLowTempRadiantVarFlow_Impl::autosizeMaximumHotWaterFlow() {
      bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow, "autosize");
      OS_ASSERT(result);
    }

    bool CoilHeatingLowTempRadiantVarFlow_Impl::setHeatingControlThrottlingRange(double heatingControlThrottlingRange) {
      bool result = setDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlThrottlingRange, heatingControlThrottlingRange);
      return result;
    }

    void CoilHeatingLowTempRadiantVarFlow_Impl::resetHeatingControlThrottlingRange() {
      bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlThrottlingRange, "");
      OS_ASSERT(result);
    }

    bool CoilHeatingLowTempRadiantVarFlow_Impl::setHeatingControlTemperatureSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlTemperatureScheduleName,
                                "CoilHeatingLowTempRadiantVarFlow", "Heating Control Temperature", schedule);
      return result;
    }

    void CoilHeatingLowTempRadiantVarFlow_Impl::resetHeatingControlTemperatureSchedule() {
      bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<ModelObject> CoilHeatingLowTempRadiantVarFlow_Impl::heatingControlTemperatureScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = heatingControlTemperatureSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool CoilHeatingLowTempRadiantVarFlow_Impl::setHeatingControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setHeatingControlTemperatureSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetHeatingControlTemperatureSchedule();
      }
      return true;
    }

    bool CoilHeatingLowTempRadiantVarFlow_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->demandComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    boost::optional<ZoneHVACLowTempRadiantVarFlow> CoilHeatingLowTempRadiantVarFlow_Impl::parentZoneHVAC() const {

      boost::optional<ZoneHVACLowTempRadiantVarFlow> result;

      // This coil performance object can only be found in a ZoneHVACLowTempRadiantVarFlow
      // Check all ZoneHVACLowTempRadiantVarFlow in the model, seeing if this is inside of one of them.
      boost::optional<ZoneHVACLowTempRadiantVarFlow> parentCoil;
      auto zoneHVACs = this->model().getConcreteModelObjects<ZoneHVACLowTempRadiantVarFlow>();
      for (const auto& zoneHVAC : zoneHVACs) {
        if (auto hc = zoneHVAC.heatingCoil()) {
          if (hc->handle() == this->handle()) {
            result = zoneHVAC;
            break;
          }
        }
      }

      // Warn if this coil was not found inside a ZoneHVACLowTempRadiantVarFlow
      if (!result) {
        LOG(Warn, name().get() + " was not found inside a ZoneHVACLowTempRadiantVarFlow in the model, cannot retrieve the autosized value.");
        return result;
      }

      return result;
    }

    std::string CoilHeatingLowTempRadiantVarFlow_Impl::heatingDesignCapacityMethod() const {
      boost::optional<std::string> value = getString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacityMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingLowTempRadiantVarFlow_Impl::setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod) {
      bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacityMethod, heatingDesignCapacityMethod);
      return result;
    }

    boost::optional<double> CoilHeatingLowTempRadiantVarFlow_Impl::heatingDesignCapacity() const {
      return getDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacity, true);
    }

    bool CoilHeatingLowTempRadiantVarFlow_Impl::isHeatingDesignCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    bool CoilHeatingLowTempRadiantVarFlow_Impl::setHeatingDesignCapacity(double heatingDesignCapacity) {
      bool result(false);
      if (heatingDesignCapacity) {
        result = setDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacity, heatingDesignCapacity);
      }
      return result;
    }

    void CoilHeatingLowTempRadiantVarFlow_Impl::autosizeHeatingDesignCapacity() {
      bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacity, "Autosize");
      OS_ASSERT(result);
    }

    double CoilHeatingLowTempRadiantVarFlow_Impl::heatingDesignCapacityPerFloorArea() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacityPerFloorArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingLowTempRadiantVarFlow_Impl::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
      bool result =
        setDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacityPerFloorArea, heatingDesignCapacityPerFloorArea);
      return result;
    }

    double CoilHeatingLowTempRadiantVarFlow_Impl::fractionofAutosizedHeatingDesignCapacity() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::FractionofAutosizedHeatingDesignCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingLowTempRadiantVarFlow_Impl::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
      bool result = setDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::FractionofAutosizedHeatingDesignCapacity,
                              fractionofAutosizedHeatingDesignCapacity);
      return result;
    }

    boost::optional<double> CoilHeatingLowTempRadiantVarFlow_Impl::autosizedHeatingDesignCapacity() const {

      // TODO!!!
      return boost::none;
    }

    boost::optional<double> CoilHeatingLowTempRadiantVarFlow_Impl::autosizedMaximumHotWaterFlow() const {
      boost::optional<ZoneHVACLowTempRadiantVarFlow> zoneHVAC = parentZoneHVAC();
      boost::optional<double> result;
      if (!zoneHVAC) {
        return result;
      }

      return zoneHVAC->getAutosizedValue("Design Size Maximum Hot Water Flow", "m3/s");
    }

    void CoilHeatingLowTempRadiantVarFlow_Impl::autosize() {
      autosizeMaximumHotWaterFlow();
      autosizeHeatingDesignCapacity();
    }

    void CoilHeatingLowTempRadiantVarFlow_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedMaximumHotWaterFlow();
      if (val) {
        setMaximumHotWaterFlow(val.get());
      }
      val = autosizedHeatingDesignCapacity();
      if (val) {
        setHeatingDesignCapacity(val.get());
      }
    }

  }  // namespace detail

  CoilHeatingLowTempRadiantVarFlow::CoilHeatingLowTempRadiantVarFlow(const Model& model, Schedule& heatingControlTemperature)

    : StraightComponent(CoilHeatingLowTempRadiantVarFlow::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>());

    bool ok = setHeatingControlTemperatureSchedule(heatingControlTemperature);
    OS_ASSERT(ok);

    ok = setHeatingDesignCapacityMethod("HeatingDesignCapacity");
    OS_ASSERT(ok);
    autosizeHeatingDesignCapacity();
    ok = setHeatingDesignCapacityPerFloorArea(0);
    OS_ASSERT(ok);
    ok = setFractionofAutosizedHeatingDesignCapacity(1.0);
    OS_ASSERT(ok);
  }

  std::vector<std::string> CoilHeatingLowTempRadiantVarFlow::heatingDesignCapacityMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacityMethod);
  }

  IddObjectType CoilHeatingLowTempRadiantVarFlow::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_VariableFlow);
  }

  boost::optional<double> CoilHeatingLowTempRadiantVarFlow::maximumHotWaterFlow() const {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->maximumHotWaterFlow();
  }

  bool CoilHeatingLowTempRadiantVarFlow::isMaximumHotWaterFlowDefaulted() const {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->isMaximumHotWaterFlowDefaulted();
  }

  bool CoilHeatingLowTempRadiantVarFlow::isMaximumHotWaterFlowAutosized() const {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->isMaximumHotWaterFlowAutosized();
  }

  double CoilHeatingLowTempRadiantVarFlow::heatingControlThrottlingRange() const {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->heatingControlThrottlingRange();
  }

  bool CoilHeatingLowTempRadiantVarFlow::isHeatingControlThrottlingRangeDefaulted() const {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->isHeatingControlThrottlingRangeDefaulted();
  }

  boost::optional<Schedule> CoilHeatingLowTempRadiantVarFlow::heatingControlTemperatureSchedule() const {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->heatingControlTemperatureSchedule();
  }

  bool CoilHeatingLowTempRadiantVarFlow::setMaximumHotWaterFlow(double maximumHotWaterFlow) {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->setMaximumHotWaterFlow(maximumHotWaterFlow);
  }

  void CoilHeatingLowTempRadiantVarFlow::resetMaximumHotWaterFlow() {
    getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->resetMaximumHotWaterFlow();
  }

  void CoilHeatingLowTempRadiantVarFlow::autosizeMaximumHotWaterFlow() {
    getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->autosizeMaximumHotWaterFlow();
  }

  bool CoilHeatingLowTempRadiantVarFlow::setHeatingControlThrottlingRange(double heatingControlThrottlingRange) {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->setHeatingControlThrottlingRange(heatingControlThrottlingRange);
  }

  void CoilHeatingLowTempRadiantVarFlow::resetHeatingControlThrottlingRange() {
    getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->resetHeatingControlThrottlingRange();
  }

  bool CoilHeatingLowTempRadiantVarFlow::setHeatingControlTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->setHeatingControlTemperatureSchedule(schedule);
  }

  void CoilHeatingLowTempRadiantVarFlow::resetHeatingControlTemperatureSchedule() {
    getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->resetHeatingControlTemperatureSchedule();
  }

  boost::optional<double> CoilHeatingLowTempRadiantVarFlow::autosizedMaximumHotWaterFlow() const {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->autosizedMaximumHotWaterFlow();
  }

  boost::optional<ZoneHVACLowTempRadiantVarFlow> CoilHeatingLowTempRadiantVarFlow::parentZoneHVAC() const {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->parentZoneHVAC();
  }

  std::string CoilHeatingLowTempRadiantVarFlow::heatingDesignCapacityMethod() const {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->heatingDesignCapacityMethod();
  }

  bool CoilHeatingLowTempRadiantVarFlow::setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod) {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->setHeatingDesignCapacityMethod(heatingDesignCapacityMethod);
  }

  boost::optional<double> CoilHeatingLowTempRadiantVarFlow::heatingDesignCapacity() const {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->heatingDesignCapacity();
  }

  bool CoilHeatingLowTempRadiantVarFlow::isHeatingDesignCapacityAutosized() const {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->isHeatingDesignCapacityAutosized();
  }

  bool CoilHeatingLowTempRadiantVarFlow::setHeatingDesignCapacity(double heatingDesignCapacity) {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->setHeatingDesignCapacity(heatingDesignCapacity);
  }

  void CoilHeatingLowTempRadiantVarFlow::autosizeHeatingDesignCapacity() {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->autosizeHeatingDesignCapacity();
  }

  boost::optional<double> CoilHeatingLowTempRadiantVarFlow::autosizedHeatingDesignCapacity() const {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->autosizedHeatingDesignCapacity();
  }

  double CoilHeatingLowTempRadiantVarFlow::heatingDesignCapacityPerFloorArea() const {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->heatingDesignCapacityPerFloorArea();
  }

  bool CoilHeatingLowTempRadiantVarFlow::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->setHeatingDesignCapacityPerFloorArea(heatingDesignCapacityPerFloorArea);
  }

  double CoilHeatingLowTempRadiantVarFlow::fractionofAutosizedHeatingDesignCapacity() const {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->fractionofAutosizedHeatingDesignCapacity();
  }

  bool CoilHeatingLowTempRadiantVarFlow::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
    return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->setFractionofAutosizedHeatingDesignCapacity(
      fractionofAutosizedHeatingDesignCapacity);
  }

  /// @cond
  CoilHeatingLowTempRadiantVarFlow::CoilHeatingLowTempRadiantVarFlow(std::shared_ptr<detail::CoilHeatingLowTempRadiantVarFlow_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
