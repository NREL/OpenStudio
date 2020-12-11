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

#include "CoilCoolingLowTempRadiantVarFlow.hpp"
#include "CoilCoolingLowTempRadiantVarFlow_Impl.hpp"

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

#include <utilities/idd/OS_Coil_Cooling_LowTemperatureRadiant_VariableFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilCoolingLowTempRadiantVarFlow_Impl::CoilCoolingLowTempRadiantVarFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilCoolingLowTempRadiantVarFlow::iddObjectType());
    }

    CoilCoolingLowTempRadiantVarFlow_Impl::CoilCoolingLowTempRadiantVarFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilCoolingLowTempRadiantVarFlow::iddObjectType());
    }

    CoilCoolingLowTempRadiantVarFlow_Impl::CoilCoolingLowTempRadiantVarFlow_Impl(const CoilCoolingLowTempRadiantVarFlow_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilCoolingLowTempRadiantVarFlow_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType CoilCoolingLowTempRadiantVarFlow_Impl::iddObjectType() const {
      return CoilCoolingLowTempRadiantVarFlow::iddObjectType();
    }

    unsigned CoilCoolingLowTempRadiantVarFlow_Impl::inletPort() const {
      return OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingWaterInletNodeName;
    }

    unsigned CoilCoolingLowTempRadiantVarFlow_Impl::outletPort() const {
      return OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingWaterOutletNodeName;
    }

    std::vector<ScheduleTypeKey> CoilCoolingLowTempRadiantVarFlow_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("CoilCoolingLowTempRadiantVarFlow", "Cooling Control Temperature Schedule"));
      }
      return result;
    }

    boost::optional<ZoneHVACComponent> CoilCoolingLowTempRadiantVarFlow_Impl::containingZoneHVACComponent() const {

      std::vector<ZoneHVACLowTempRadiantVarFlow> zoneHVACLowTempRadiantVarFlows;

      zoneHVACLowTempRadiantVarFlows = this->model().getConcreteModelObjects<ZoneHVACLowTempRadiantVarFlow>();

      for (const auto& zoneHVACLowTempRadiantVarFlow : zoneHVACLowTempRadiantVarFlows) {
        if (boost::optional<HVACComponent> coil = zoneHVACLowTempRadiantVarFlow.coolingCoil()) {
          if (coil->handle() == this->handle()) {
            return zoneHVACLowTempRadiantVarFlow;
          }
        }
      }
      return boost::none;
    }

    boost::optional<Schedule> CoilCoolingLowTempRadiantVarFlow_Impl::coolingControlTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlTemperatureScheduleName);
    }

    boost::optional<double> CoilCoolingLowTempRadiantVarFlow_Impl::maximumColdWaterFlow() const {
      return getDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow, true);
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::isMaximumColdWaterFlowDefaulted() const {
      return isEmpty(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow);
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::isMaximumColdWaterFlowAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilCoolingLowTempRadiantVarFlow_Impl::coolingControlThrottlingRange() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlThrottlingRange, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::isCoolingControlThrottlingRangeDefaulted() const {
      return isEmpty(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlThrottlingRange);
    }

    std::string CoilCoolingLowTempRadiantVarFlow_Impl::condensationControlType() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::isCondensationControlTypeDefaulted() const {
      return isEmpty(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlType);
    }

    double CoilCoolingLowTempRadiantVarFlow_Impl::condensationControlDewpointOffset() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlDewpointOffset, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::isCondensationControlDewpointOffsetDefaulted() const {
      return isEmpty(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlDewpointOffset);
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::setCoolingControlTemperatureSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlTemperatureScheduleName,
                                "CoilCoolingLowTempRadiantVarFlow", "Cooling Control Temperature Schedule", schedule);
      return result;
    }

    void CoilCoolingLowTempRadiantVarFlow_Impl::resetCoolingControlTemperatureSchedule() {
      bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::setMaximumColdWaterFlow(boost::optional<double> maximumColdWaterFlow) {
      bool result(false);
      if (maximumColdWaterFlow) {
        result = setDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow, maximumColdWaterFlow.get());
      } else {
        resetMaximumColdWaterFlow();
        result = true;
      }
      return result;
    }

    void CoilCoolingLowTempRadiantVarFlow_Impl::resetMaximumColdWaterFlow() {
      bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow, "");
      OS_ASSERT(result);
    }

    void CoilCoolingLowTempRadiantVarFlow_Impl::autosizeMaximumColdWaterFlow() {
      bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::setCoolingControlThrottlingRange(double coolingControlThrottlingRange) {
      bool result = setDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlThrottlingRange, coolingControlThrottlingRange);
      return result;
    }

    void CoilCoolingLowTempRadiantVarFlow_Impl::resetCoolingControlThrottlingRange() {
      bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlThrottlingRange, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::setCondensationControlType(std::string condensationControlType) {
      bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlType, condensationControlType);
      return result;
    }

    void CoilCoolingLowTempRadiantVarFlow_Impl::resetCondensationControlType() {
      bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlType, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::setCondensationControlDewpointOffset(double condensationControlDewpointOffset) {
      bool result =
        setDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlDewpointOffset, condensationControlDewpointOffset);
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingLowTempRadiantVarFlow_Impl::resetCondensationControlDewpointOffset() {
      bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlDewpointOffset, "");
      OS_ASSERT(result);
    }

    std::vector<std::string> CoilCoolingLowTempRadiantVarFlow_Impl::condensationControlTypeValues() const {
      return CoilCoolingLowTempRadiantVarFlow::condensationControlTypeValues();
    }

    boost::optional<ModelObject> CoilCoolingLowTempRadiantVarFlow_Impl::coolingControlTemperatureScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = coolingControlTemperatureSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::setCoolingControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setCoolingControlTemperatureSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetCoolingControlTemperatureSchedule();
      }
      return true;
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->demandComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    boost::optional<ZoneHVACLowTempRadiantVarFlow> CoilCoolingLowTempRadiantVarFlow_Impl::parentZoneHVAC() const {

      boost::optional<ZoneHVACLowTempRadiantVarFlow> result;

      // This coil performance object can only be found in a ZoneHVACLowTempRadiantVarFlow
      // Check all ZoneHVACLowTempRadiantVarFlow in the model, seeing if this is inside of one of them.
      boost::optional<ZoneHVACLowTempRadiantVarFlow> parentCoil;
      auto zoneHVACs = this->model().getConcreteModelObjects<ZoneHVACLowTempRadiantVarFlow>();
      for (const auto& zoneHVAC : zoneHVACs) {
        if (auto cc = zoneHVAC.coolingCoil()) {
          if (cc->handle() == this->handle()) {
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

    std::string CoilCoolingLowTempRadiantVarFlow_Impl::coolingDesignCapacityMethod() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacityMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::setCoolingDesignCapacityMethod(const std::string& coolingDesignCapacityMethod) {
      bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacityMethod, coolingDesignCapacityMethod);
      return result;
    }

    boost::optional<double> CoilCoolingLowTempRadiantVarFlow_Impl::coolingDesignCapacity() const {
      return getDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacity, true);
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::isCoolingDesignCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::setCoolingDesignCapacity(double coolingDesignCapacity) {
      bool result(false);
      if (coolingDesignCapacity) {
        result = setDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacity, coolingDesignCapacity);
      }
      return result;
    }

    void CoilCoolingLowTempRadiantVarFlow_Impl::autosizeCoolingDesignCapacity() {
      bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacity, "Autosize");
      OS_ASSERT(result);
    }

    double CoilCoolingLowTempRadiantVarFlow_Impl::coolingDesignCapacityPerFloorArea() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacityPerFloorArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea) {
      bool result =
        setDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacityPerFloorArea, coolingDesignCapacityPerFloorArea);
      return result;
    }

    double CoilCoolingLowTempRadiantVarFlow_Impl::fractionofAutosizedCoolingDesignCapacity() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::FractionofAutosizedCoolingDesignCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingLowTempRadiantVarFlow_Impl::setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity) {
      bool result = setDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::FractionofAutosizedCoolingDesignCapacity,
                              fractionofAutosizedCoolingDesignCapacity);
      return result;
    }

    boost::optional<double> CoilCoolingLowTempRadiantVarFlow_Impl::autosizedCoolingDesignCapacity() const {

      // TODO!!!
      return boost::none;
    }

    boost::optional<double> CoilCoolingLowTempRadiantVarFlow_Impl::autosizedMaximumColdWaterFlow() const {
      boost::optional<ZoneHVACLowTempRadiantVarFlow> zoneHVAC = parentZoneHVAC();
      boost::optional<double> result;
      if (!zoneHVAC) {
        return result;
      }

      return zoneHVAC->getAutosizedValue("Design Size Maximum Cold Water Flow", "m3/s");
    }

    void CoilCoolingLowTempRadiantVarFlow_Impl::autosize() {
      autosizeMaximumColdWaterFlow();
      autosizeCoolingDesignCapacity();
    }

    void CoilCoolingLowTempRadiantVarFlow_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedMaximumColdWaterFlow();
      if (val) {
        setMaximumColdWaterFlow(val.get());
      }
      val = autosizedCoolingDesignCapacity();
      if (val) {
        setCoolingDesignCapacity(val.get());
      }
    }

  }  // namespace detail

  CoilCoolingLowTempRadiantVarFlow::CoilCoolingLowTempRadiantVarFlow(const Model& model, Schedule& coolingControlTemperatureSchedule)
    : StraightComponent(CoilCoolingLowTempRadiantVarFlow::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>());

    bool ok = setCoolingControlTemperatureSchedule(coolingControlTemperatureSchedule);
    OS_ASSERT(ok);

    ok = setCoolingDesignCapacityMethod("CoolingDesignCapacity");
    OS_ASSERT(ok);
    autosizeCoolingDesignCapacity();
    ok = setCoolingDesignCapacityPerFloorArea(0);
    OS_ASSERT(ok);
    ok = setFractionofAutosizedCoolingDesignCapacity(1.0);
    OS_ASSERT(ok);
  }

  IddObjectType CoilCoolingLowTempRadiantVarFlow::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_VariableFlow);
  }

  std::vector<std::string> CoilCoolingLowTempRadiantVarFlow::condensationControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlType);
  }

  std::vector<std::string> CoilCoolingLowTempRadiantVarFlow::coolingDesignCapacityMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacityMethod);
  }

  boost::optional<Schedule> CoilCoolingLowTempRadiantVarFlow::coolingControlTemperatureSchedule() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->coolingControlTemperatureSchedule();
  }

  boost::optional<double> CoilCoolingLowTempRadiantVarFlow::maximumColdWaterFlow() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->maximumColdWaterFlow();
  }

  bool CoilCoolingLowTempRadiantVarFlow::isMaximumColdWaterFlowDefaulted() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->isMaximumColdWaterFlowDefaulted();
  }

  bool CoilCoolingLowTempRadiantVarFlow::isMaximumColdWaterFlowAutosized() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->isMaximumColdWaterFlowAutosized();
  }

  double CoilCoolingLowTempRadiantVarFlow::coolingControlThrottlingRange() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->coolingControlThrottlingRange();
  }

  bool CoilCoolingLowTempRadiantVarFlow::isCoolingControlThrottlingRangeDefaulted() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->isCoolingControlThrottlingRangeDefaulted();
  }

  std::string CoilCoolingLowTempRadiantVarFlow::condensationControlType() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->condensationControlType();
  }

  bool CoilCoolingLowTempRadiantVarFlow::isCondensationControlTypeDefaulted() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->isCondensationControlTypeDefaulted();
  }

  double CoilCoolingLowTempRadiantVarFlow::condensationControlDewpointOffset() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->condensationControlDewpointOffset();
  }

  bool CoilCoolingLowTempRadiantVarFlow::isCondensationControlDewpointOffsetDefaulted() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->isCondensationControlDewpointOffsetDefaulted();
  }

  bool CoilCoolingLowTempRadiantVarFlow::setMaximumColdWaterFlow(double maximumColdWaterFlow) {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->setMaximumColdWaterFlow(maximumColdWaterFlow);
  }

  void CoilCoolingLowTempRadiantVarFlow::resetMaximumColdWaterFlow() {
    getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->resetMaximumColdWaterFlow();
  }

  void CoilCoolingLowTempRadiantVarFlow::autosizeMaximumColdWaterFlow() {
    getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->autosizeMaximumColdWaterFlow();
  }

  bool CoilCoolingLowTempRadiantVarFlow::setCoolingControlThrottlingRange(double coolingControlThrottlingRange) {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->setCoolingControlThrottlingRange(coolingControlThrottlingRange);
  }

  void CoilCoolingLowTempRadiantVarFlow::resetCoolingControlThrottlingRange() {
    getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->resetCoolingControlThrottlingRange();
  }

  bool CoilCoolingLowTempRadiantVarFlow::setCoolingControlTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->setCoolingControlTemperatureSchedule(schedule);
  }

  void CoilCoolingLowTempRadiantVarFlow::resetCoolingControlTemperatureSchedule() {
    getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->resetCoolingControlTemperatureSchedule();
  }

  bool CoilCoolingLowTempRadiantVarFlow::setCondensationControlType(std::string condensationControlType) {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->setCondensationControlType(condensationControlType);
  }

  void CoilCoolingLowTempRadiantVarFlow::resetCondensationControlType() {
    getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->resetCondensationControlType();
  }

  bool CoilCoolingLowTempRadiantVarFlow::setCondensationControlDewpointOffset(double condensationControlDewpointOffset) {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->setCondensationControlDewpointOffset(condensationControlDewpointOffset);
  }

  void CoilCoolingLowTempRadiantVarFlow::resetCondensationControlDewpointOffset() {
    getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->resetCondensationControlDewpointOffset();
  }

  boost::optional<double> CoilCoolingLowTempRadiantVarFlow::autosizedMaximumColdWaterFlow() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->autosizedMaximumColdWaterFlow();
  }

  boost::optional<ZoneHVACLowTempRadiantVarFlow> CoilCoolingLowTempRadiantVarFlow::parentZoneHVAC() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->parentZoneHVAC();
  }

  std::string CoilCoolingLowTempRadiantVarFlow::coolingDesignCapacityMethod() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->coolingDesignCapacityMethod();
  }

  bool CoilCoolingLowTempRadiantVarFlow::setCoolingDesignCapacityMethod(const std::string& coolingDesignCapacityMethod) {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->setCoolingDesignCapacityMethod(coolingDesignCapacityMethod);
  }

  boost::optional<double> CoilCoolingLowTempRadiantVarFlow::coolingDesignCapacity() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->coolingDesignCapacity();
  }

  bool CoilCoolingLowTempRadiantVarFlow::isCoolingDesignCapacityAutosized() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->isCoolingDesignCapacityAutosized();
  }

  bool CoilCoolingLowTempRadiantVarFlow::setCoolingDesignCapacity(double coolingDesignCapacity) {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->setCoolingDesignCapacity(coolingDesignCapacity);
  }

  void CoilCoolingLowTempRadiantVarFlow::autosizeCoolingDesignCapacity() {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->autosizeCoolingDesignCapacity();
  }

  boost::optional<double> CoilCoolingLowTempRadiantVarFlow::autosizedCoolingDesignCapacity() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->autosizedCoolingDesignCapacity();
  }

  double CoilCoolingLowTempRadiantVarFlow::coolingDesignCapacityPerFloorArea() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->coolingDesignCapacityPerFloorArea();
  }

  bool CoilCoolingLowTempRadiantVarFlow::setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea) {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->setCoolingDesignCapacityPerFloorArea(coolingDesignCapacityPerFloorArea);
  }

  double CoilCoolingLowTempRadiantVarFlow::fractionofAutosizedCoolingDesignCapacity() const {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->fractionofAutosizedCoolingDesignCapacity();
  }

  bool CoilCoolingLowTempRadiantVarFlow::setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity) {
    return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->setFractionofAutosizedCoolingDesignCapacity(
      fractionofAutosizedCoolingDesignCapacity);
  }

  /// @cond
  CoilCoolingLowTempRadiantVarFlow::CoilCoolingLowTempRadiantVarFlow(std::shared_ptr<detail::CoilCoolingLowTempRadiantVarFlow_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
