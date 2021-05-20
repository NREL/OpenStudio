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

#include "HeatExchangerDesiccantBalancedFlow.hpp"
#include "HeatExchangerDesiccantBalancedFlow_Impl.hpp"
#include "CoilSystemCoolingWaterHeatExchangerAssisted.hpp"
#include "CoilSystemCoolingWaterHeatExchangerAssisted_Impl.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "AirflowNetworkEquivalentDuct.hpp"
#include "AirflowNetworkEquivalentDuct_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_HeatExchanger_Desiccant_BalancedFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    HeatExchangerDesiccantBalancedFlow_Impl::HeatExchangerDesiccantBalancedFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AirToAirComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatExchangerDesiccantBalancedFlow::iddObjectType());
    }

    HeatExchangerDesiccantBalancedFlow_Impl::HeatExchangerDesiccantBalancedFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : AirToAirComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatExchangerDesiccantBalancedFlow::iddObjectType());
    }

    HeatExchangerDesiccantBalancedFlow_Impl::HeatExchangerDesiccantBalancedFlow_Impl(const HeatExchangerDesiccantBalancedFlow_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : AirToAirComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeatExchangerDesiccantBalancedFlow_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{};
      return result;
    }

    IddObjectType HeatExchangerDesiccantBalancedFlow_Impl::iddObjectType() const {
      return HeatExchangerDesiccantBalancedFlow::iddObjectType();
    }

    std::vector<ModelObject> HeatExchangerDesiccantBalancedFlow_Impl::children() const {
      std::vector<ModelObject> result;
      std::vector<AirflowNetworkEquivalentDuct> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());
      return result;
    }

    std::vector<ScheduleTypeKey> HeatExchangerDesiccantBalancedFlow_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_HeatExchanger_Desiccant_BalancedFlowFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("HeatExchangerDesiccantBalancedFlow", "Availability"));
      }
      return result;
    }

    Schedule HeatExchangerDesiccantBalancedFlow_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    boost::optional<HVACComponent> HeatExchangerDesiccantBalancedFlow_Impl::containingHVACComponent() const {
      // CoilSystemCoolingWaterHeatExchangerAssisted
      {
        auto coilSystems = model().getConcreteModelObjects<CoilSystemCoolingWaterHeatExchangerAssisted>();
        for (const auto& coilSystem : coilSystems) {
          if (coilSystem.heatExchanger().handle() == handle()) {
            return coilSystem;
          }
        }
      }

      // CoilSystemCoolingDXHeatExchangerAssisted
      {
        auto coilSystems = model().getConcreteModelObjects<CoilSystemCoolingDXHeatExchangerAssisted>();
        for (const auto& coilSystem : coilSystems) {
          if (coilSystem.heatExchanger().handle() == handle()) {
            return coilSystem;
          }
        }
      }

      return boost::none;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::addToNode(Node& node) {
      bool success(false);

      auto t_containingHVACComponent = containingHVACComponent();

      if (t_containingHVACComponent) {
        if (t_containingHVACComponent->optionalCast<CoilSystemCoolingWaterHeatExchangerAssisted>()) {
          LOG(Warn,
              this->briefDescription() << " cannot be connected directly when it's part of a parent CoilSystemCoolingWaterHeatExchangerAssisted. "
                                          "Please call CoilSystemCoolingWaterHeatExchangerAssisted::addToNode instead");
        } else if (t_containingHVACComponent->optionalCast<CoilSystemCoolingDXHeatExchangerAssisted>()) {
          LOG(Warn, this->briefDescription() << " cannot be connected directly when it's part of a parent CoilSystemCoolingDXHeatExchangerAssisted. "
                                                "Please call CoilSystemCoolingDXHeatExchangerAssisted::addToNode instead");
        } else {
          // Shouldn't happen
          OS_ASSERT(false);
        }
      } else {
        success = AirToAirComponent_Impl::addToNode(node);
      }

      return success;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_HeatExchanger_Desiccant_BalancedFlowFields::AvailabilitySchedule, "HeatExchangerDesiccantBalancedFlow",
                                "Availability", schedule);
      return result;
    }

    boost::optional<Schedule> HeatExchangerDesiccantBalancedFlow_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_HeatExchanger_Desiccant_BalancedFlowFields::AvailabilitySchedule);
    }

    boost::optional<ModelObject> HeatExchangerDesiccantBalancedFlow_Impl::availabilityScheduleAsModelObject() const {
      OptionalModelObject result = availabilitySchedule();
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setAvailabilitySchedule(schedule);
        }
      }
      return false;
    }

    unsigned HeatExchangerDesiccantBalancedFlow_Impl::primaryAirInletPort() const {
      return OS_HeatExchanger_Desiccant_BalancedFlowFields::RegenerationAirInletNode;
    }

    unsigned HeatExchangerDesiccantBalancedFlow_Impl::primaryAirOutletPort() const {
      return OS_HeatExchanger_Desiccant_BalancedFlowFields::RegenerationAirOutletNode;
    }

    unsigned HeatExchangerDesiccantBalancedFlow_Impl::secondaryAirInletPort() const {
      return OS_HeatExchanger_Desiccant_BalancedFlowFields::ProcessAirInletNode;
    }

    unsigned HeatExchangerDesiccantBalancedFlow_Impl::secondaryAirOutletPort() const {
      return OS_HeatExchanger_Desiccant_BalancedFlowFields::ProcessAirOutletNode;
    }

    AirflowNetworkEquivalentDuct HeatExchangerDesiccantBalancedFlow_Impl::getAirflowNetworkEquivalentDuct(double length, double diameter) {
      boost::optional<AirflowNetworkEquivalentDuct> opt = airflowNetworkEquivalentDuct();
      if (opt) {
        if (opt->airPathLength() != length) {
          opt->setAirPathLength(length);
        }
        if (opt->airPathHydraulicDiameter() != diameter) {
          opt->setAirPathHydraulicDiameter(diameter);
        }
      }
      return AirflowNetworkEquivalentDuct(model(), length, diameter, handle());
    }

    boost::optional<AirflowNetworkEquivalentDuct> HeatExchangerDesiccantBalancedFlow_Impl::airflowNetworkEquivalentDuct() const {
      std::vector<AirflowNetworkEquivalentDuct> myAFN =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
      auto count = myAFN.size();
      if (count == 1) {
        return myAFN[0];
      } else if (count > 1) {
        LOG(Warn, briefDescription() << " has more than one AirflowNetwork EquivalentDuct attached, returning first.");
        return myAFN[0];
      }
      return boost::none;
    }

  }  // namespace detail

  HeatExchangerDesiccantBalancedFlow::HeatExchangerDesiccantBalancedFlow(const Model& model)
    : AirToAirComponent(HeatExchangerDesiccantBalancedFlow::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>());

    Schedule schedule = model.alwaysOnDiscreteSchedule();
    setAvailabilitySchedule(schedule);
  }

  IddObjectType HeatExchangerDesiccantBalancedFlow::iddObjectType() {
    return IddObjectType(IddObjectType::OS_HeatExchanger_Desiccant_BalancedFlow);
  }

  Schedule HeatExchangerDesiccantBalancedFlow::availabilitySchedule() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->availabilitySchedule();
  }

  bool HeatExchangerDesiccantBalancedFlow::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setAvailabilitySchedule(schedule);
  }

  AirflowNetworkEquivalentDuct HeatExchangerDesiccantBalancedFlow::getAirflowNetworkEquivalentDuct(double length, double diameter) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->getAirflowNetworkEquivalentDuct(length, diameter);
  }

  boost::optional<AirflowNetworkEquivalentDuct> HeatExchangerDesiccantBalancedFlow::airflowNetworkEquivalentDuct() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->airflowNetworkEquivalentDuct();
  }

  /// @cond
  HeatExchangerDesiccantBalancedFlow::HeatExchangerDesiccantBalancedFlow(std::shared_ptr<detail::HeatExchangerDesiccantBalancedFlow_Impl> impl)
    : AirToAirComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio