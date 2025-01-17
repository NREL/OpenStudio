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

#include "AirLoopHVACExhaustSystem.hpp"
#include "AirLoopHVACExhaustSystem_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "FanSystemModel.hpp"
#include "FanSystemModel_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_ExhaustSystem_FieldEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    AirLoopHVACExhaustSystem_Impl::AirLoopHVACExhaustSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACExhaustSystem::iddObjectType());
    }

    AirLoopHVACExhaustSystem_Impl::AirLoopHVACExhaustSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirLoopHVACExhaustSystem::iddObjectType());
    }

    AirLoopHVACExhaustSystem_Impl::AirLoopHVACExhaustSystem_Impl(const AirLoopHVACExhaustSystem_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirLoopHVACExhaustSystem_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType AirLoopHVACExhaustSystem_Impl::iddObjectType() const {
      return AirLoopHVACExhaustSystem::iddObjectType();
    }

    ComponentType AirLoopHVACExhaustSystem_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> AirLoopHVACExhaustSystem_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> AirLoopHVACExhaustSystem_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> AirLoopHVACExhaustSystem_Impl::appGHeatingFuelTypes() const {
      return {};
    }

    HVACComponent AirLoopHVACExhaustSystem_Impl::fan() const {
      boost::optional<HVACComponent> value = optionalFan();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Fan attached.");
      }
      return value.get();
    }

    bool AirLoopHVACExhaustSystem_Impl::setFan(const HVACComponent& fan) {
      const bool result = setPointer(OS_AirLoopHVAC_ExhaustSystemFields::FanName, fan.handle());
      return result;
    }

    boost::optional<HVACComponent> AirLoopHVACExhaustSystem_Impl::optionalFan() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_ExhaustSystemFields::FanName);
    }

  }  // namespace detail

  AirLoopHVACExhaustSystem::AirLoopHVACExhaustSystem(const Model& model) : HVACComponent(AirLoopHVACExhaustSystem::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirLoopHVACExhaustSystem_Impl>());

    bool ok = true;

    FanSystemModel fan(model);
    ok = setFan(fan);
    OS_ASSERT(ok);
  }

  IddObjectType AirLoopHVACExhaustSystem::iddObjectType() {
    return {IddObjectType::OS_AirLoopHVAC_ExhaustSystem};
  }

  HVACComponent AirLoopHVACExhaustSystem::fan() const {
    return getImpl<detail::AirLoopHVACExhaustSystem_Impl>()->fan();
  }

  bool AirLoopHVACExhaustSystem::setFan(const HVACComponent& fan) {
    return getImpl<detail::AirLoopHVACExhaustSystem_Impl>()->setFan(fan);
  }

  /// @cond
  AirLoopHVACExhaustSystem::AirLoopHVACExhaustSystem(std::shared_ptr<detail::AirLoopHVACExhaustSystem_Impl> impl) : HVACComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
