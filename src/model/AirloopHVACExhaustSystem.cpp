/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "AirloopHVACExhaustSystem.hpp"
#include "AirloopHVACExhaustSystem_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "ZoneMixers.hpp"
#include "ZoneMixers_Impl.hpp"
#include "FansSystemModel.hpp"
#include "FansSystemModel_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_ExhaustSystem_FieldEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    AirloopHVACExhaustSystem_Impl::AirloopHVACExhaustSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirloopHVACExhaustSystem::iddObjectType());
    }

    AirloopHVACExhaustSystem_Impl::AirloopHVACExhaustSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirloopHVACExhaustSystem::iddObjectType());
    }

    AirloopHVACExhaustSystem_Impl::AirloopHVACExhaustSystem_Impl(const AirloopHVACExhaustSystem_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirloopHVACExhaustSystem_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType AirloopHVACExhaustSystem_Impl::iddObjectType() const {
      return AirloopHVACExhaustSystem::iddObjectType();
    }

    ComponentType AirloopHVACExhaustSystem_Impl::componentType() const {
      // TODO
      return ComponentType::None;
    }

    std::vector<FuelType> AirloopHVACExhaustSystem_Impl::coolingFuelTypes() const {
      // TODO
      return {};
    }

    std::vector<FuelType> AirloopHVACExhaustSystem_Impl::heatingFuelTypes() const {
      // TODO
      return {};
    }

    std::vector<AppGFuelType> AirloopHVACExhaustSystem_Impl::appGHeatingFuelTypes() const {

      // TODO
      return {};
    }

    ZoneMixers AirloopHVACExhaustSystem_Impl::zoneMixer() const {
      boost::optional<ZoneMixers> value = optionalZoneMixer();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Zone Mixer attached.");
      }
      return value.get();
    }

    FansSystemModel AirloopHVACExhaustSystem_Impl::fan() const {
      boost::optional<FansSystemModel> value = optionalFan();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Fan attached.");
      }
      return value.get();
    }

    bool AirloopHVACExhaustSystem_Impl::setZoneMixer(const ZoneMixers& zoneMixers) {
      const bool result = setPointer(OS_AirLoopHVAC_ExhaustSystemFields::ZoneMixerName, zoneMixers.handle());
      return result;
    }

    bool AirloopHVACExhaustSystem_Impl::setFan(const FansSystemModel& fansSystemModel) {
      const bool result = setPointer(OS_AirLoopHVAC_ExhaustSystemFields::FanName, fansSystemModel.handle());
      return result;
    }

    boost::optional<ZoneMixers> AirloopHVACExhaustSystem_Impl::optionalZoneMixer() const {
      return getObject<ModelObject>().getModelObjectTarget<ZoneMixers>(OS_AirLoopHVAC_ExhaustSystemFields::ZoneMixerName);
    }

    boost::optional<FansSystemModel> AirloopHVACExhaustSystem_Impl::optionalFan() const {
      return getObject<ModelObject>().getModelObjectTarget<FansSystemModel>(OS_AirLoopHVAC_ExhaustSystemFields::FanName);
    }

  }  // namespace detail

  AirloopHVACExhaustSystem::AirloopHVACExhaustSystem(const Model& model) : StraightComponent(AirloopHVACExhaustSystem::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirloopHVACExhaustSystem_Impl>());

    // TODO: consider adding (overloaded or not) explicit ctors taking required objects as argument

    // TODO: Appropriately handle the following required object-list fields.
    //     OS_AirLoopHVAC_ExhaustSystemFields::ZoneMixerName
    //     OS_AirLoopHVAC_ExhaustSystemFields::FanName
    bool ok = true;
    // ok = setZoneMixer();
    OS_ASSERT(ok);
    // ok = setFan();
    OS_ASSERT(ok);
  }

  IddObjectType AirloopHVACExhaustSystem::iddObjectType() {
    return {IddObjectType::OS_AirLoopHVAC_ExhaustSystem};
  }

  ZoneMixers AirloopHVACExhaustSystem::zoneMixer() const {
    return getImpl<detail::AirloopHVACExhaustSystem_Impl>()->zoneMixer();
  }

  FansSystemModel AirloopHVACExhaustSystem::fan() const {
    return getImpl<detail::AirloopHVACExhaustSystem_Impl>()->fan();
  }

  bool AirloopHVACExhaustSystem::setZoneMixer(const ZoneMixers& zoneMixers) {
    return getImpl<detail::AirloopHVACExhaustSystem_Impl>()->setZoneMixer(zoneMixers);
  }

  bool AirloopHVACExhaustSystem::setFan(const FansSystemModel& fansSystemModel) {
    return getImpl<detail::AirloopHVACExhaustSystem_Impl>()->setFan(fansSystemModel);
  }

  /// @cond
  AirloopHVACExhaustSystem::AirloopHVACExhaustSystem(std::shared_ptr<detail::AirloopHVACExhaustSystem_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
