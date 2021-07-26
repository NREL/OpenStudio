/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "DaylightingDeviceTubular.hpp"
#include "DaylightingDeviceTubular_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"

#include <utilities/idd/OS_DaylightingDevice_Tubular_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    DaylightingDeviceTubular_Impl::DaylightingDeviceTubular_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == DaylightingDeviceTubular::iddObjectType());
    }

    DaylightingDeviceTubular_Impl::DaylightingDeviceTubular_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == DaylightingDeviceTubular::iddObjectType());
    }

    DaylightingDeviceTubular_Impl::DaylightingDeviceTubular_Impl(const DaylightingDeviceTubular_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& DaylightingDeviceTubular_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType DaylightingDeviceTubular_Impl::iddObjectType() const {
      return DaylightingDeviceTubular::iddObjectType();
    }

  }  // namespace detail

  DaylightingDeviceTubular::DaylightingDeviceTubular(const SubSurface& subSurface)
    : ModelObject(DaylightingDeviceTubular::iddObjectType(), subSurface.model()) {
    OS_ASSERT(getImpl<detail::DaylightingDeviceTubular_Impl>());

    bool subSurfaceOk = false;
    if (subSurface.allowDaylightingDeviceTubular()) {
      if (!subSurface.daylightingDeviceTubular()) {
        subSurfaceOk = true;
      }
    }

    if (!subSurfaceOk) {
      this->remove();
      LOG_AND_THROW("Cannot create DaylightingDeviceTubular for SubSurface '" << subSurface.name().get() << "'");
    }

    bool test;
    std::string subSurfaceType = subSurface.subSurfaceType();
    if (istringEqual("TubularDaylightDome", subSurfaceType)) {
      test = setPointer(OS_DaylightingDevice_TubularFields::DomeName, subSurface.handle());
    } else if (istringEqual("TubularDaylightDiffuser", subSurfaceType)) {
      test = setPointer(OS_DaylightingDevice_TubularFields::DiffuserName, subSurface.handle());
    }
    OS_ASSERT(test);
  }

  IddObjectType DaylightingDeviceTubular::iddObjectType() {
    IddObjectType result(IddObjectType::OS_DaylightingDevice_Tubular);
    return result;
  }

  /// @cond
  DaylightingDeviceTubular::DaylightingDeviceTubular(std::shared_ptr<detail::DaylightingDeviceTubular_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
