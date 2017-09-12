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

#include "ExteriorLightsDefinition.hpp"
#include "ExteriorLightsDefinition_Impl.hpp"

#include <utilities/idd/OS_Exterior_Lights_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ExteriorLightsDefinition_Impl::ExteriorLightsDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ExteriorLightsDefinition::iddObjectType());
  }

  ExteriorLightsDefinition_Impl::ExteriorLightsDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ExteriorLightsDefinition::iddObjectType());
  }

  ExteriorLightsDefinition_Impl::ExteriorLightsDefinition_Impl(const ExteriorLightsDefinition_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ExteriorLightsDefinition_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ExteriorLightsDefinition_Impl::iddObjectType() const {
    return ExteriorLightsDefinition::iddObjectType();
  }

  double ExteriorLightsDefinition_Impl::designLevel() const {
    boost::optional<double> value = getDouble(OS_Exterior_Lights_DefinitionFields::DesignLevel,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ExteriorLightsDefinition_Impl::setDesignLevel(double designLevel) {
    bool result = setDouble(OS_Exterior_Lights_DefinitionFields::DesignLevel, designLevel);
    return result;
  }

} // detail

ExteriorLightsDefinition::ExteriorLightsDefinition(const Model& model)
  : ResourceObject(ExteriorLightsDefinition::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ExteriorLightsDefinition_Impl>());

  bool ok = setDesignLevel(0.0);
  OS_ASSERT(ok);
}

IddObjectType ExteriorLightsDefinition::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Exterior_Lights_Definition);
  return result;
}

double ExteriorLightsDefinition::designLevel() const {
  return getImpl<detail::ExteriorLightsDefinition_Impl>()->designLevel();
}

bool ExteriorLightsDefinition::setDesignLevel(double designLevel) {
  return getImpl<detail::ExteriorLightsDefinition_Impl>()->setDesignLevel(designLevel);
}

/// @cond
ExteriorLightsDefinition::ExteriorLightsDefinition(std::shared_ptr<detail::ExteriorLightsDefinition_Impl> impl)
  : ResourceObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

