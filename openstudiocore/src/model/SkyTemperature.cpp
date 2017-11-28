/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "SkyTemperature.hpp"
#include "SkyTemperature_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"
#include "RunPeriod.hpp"
#include "RunPeriod_Impl.hpp"
#include "SizingPeriod.hpp"
#include "SizingPeriod_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_WeatherProperty_SkyTemperature_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model  {

namespace detail {

  SkyTemperature_Impl::SkyTemperature_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SkyTemperature::iddObjectType());
  }

  SkyTemperature_Impl::SkyTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SkyTemperature::iddObjectType());
  }

  SkyTemperature_Impl::SkyTemperature_Impl(const SkyTemperature_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> SkyTemperature_Impl::parent() const
  {
    // if Name field not blank, that is the parent
    OptionalParentObject result = getObject<ModelObject>().getModelObjectTarget<ParentObject>(
        OS_WeatherProperty_SkyTemperatureFields::Name);
    // if it is blank, parent is Site
    if (!result) {
      result = OptionalParentObject(model().getOptionalUniqueModelObject<Site>());
    }
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool SkyTemperature_Impl::setParent(ParentObject& newParent)
  {
    if (!(newParent.model() == model())) { return false; }

    if (newParent.optionalCast<Site>()) {
      // nullify the field
      return setPointer(OS_WeatherProperty_SkyTemperatureFields::Name,Handle());
    }
    else if (newParent.optionalCast<RunPeriod>() || newParent.optionalCast<SizingPeriod>()) {
      return setPointer(OS_WeatherProperty_SkyTemperatureFields::Name,newParent.handle());
    }

    return false;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& SkyTemperature_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SkyTemperature_Impl::iddObjectType() const {
    return SkyTemperature::iddObjectType();
  }

} // detail

/// constructor
SkyTemperature::SkyTemperature(const Model& model)
  : ModelObject(SkyTemperature::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SkyTemperature_Impl>());
}

// constructor
SkyTemperature::SkyTemperature(std::shared_ptr<detail::SkyTemperature_Impl> impl)
  : ModelObject(std::move(impl))
{}

IddObjectType SkyTemperature::iddObjectType() {
  IddObjectType result(IddObjectType::OS_WeatherProperty_SkyTemperature);
  return result;
}

} // model
} // openstudio
