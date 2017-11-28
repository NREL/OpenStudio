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

#include "EnergyManagementSystemConstructionIndexVariable.hpp"
#include "EnergyManagementSystemConstructionIndexVariable_Impl.hpp"

#include "Construction.hpp"
#include "Construction_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_ConstructionIndexVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemConstructionIndexVariable_Impl::EnergyManagementSystemConstructionIndexVariable_Impl(const IdfObject& idfObject,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemConstructionIndexVariable::iddObjectType());
  }

  EnergyManagementSystemConstructionIndexVariable_Impl::EnergyManagementSystemConstructionIndexVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemConstructionIndexVariable::iddObjectType());
  }

  EnergyManagementSystemConstructionIndexVariable_Impl::EnergyManagementSystemConstructionIndexVariable_Impl(const EnergyManagementSystemConstructionIndexVariable_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemConstructionIndexVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemConstructionIndexVariable_Impl::iddObjectType() const {
    return EnergyManagementSystemConstructionIndexVariable::iddObjectType();
  }

  ModelObject EnergyManagementSystemConstructionIndexVariable_Impl::constructionObject() const {
    boost::optional<ModelObject> value = optionalConstructionObject();
    if (!value) {
      LOG(Info, " does not have an Construction Object attached.");
    }
    return value.get();
  }

  bool EnergyManagementSystemConstructionIndexVariable_Impl::setConstructionObject(const ModelObject& construction) {
    bool result = setPointer(OS_EnergyManagementSystem_ConstructionIndexVariableFields::ConstructionObjectName, construction.handle());
    return result;
  }

  boost::optional<ModelObject> EnergyManagementSystemConstructionIndexVariable_Impl::optionalConstructionObject() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_EnergyManagementSystem_ConstructionIndexVariableFields::ConstructionObjectName);
  }

} // detail

EnergyManagementSystemConstructionIndexVariable::EnergyManagementSystemConstructionIndexVariable(const Model& model, const Construction& construction)
  : ModelObject(EnergyManagementSystemConstructionIndexVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemConstructionIndexVariable_Impl>());
  bool ok = setConstructionObject(construction);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s Construction to "
      << construction.briefDescription() << ".");
  }
}

EnergyManagementSystemConstructionIndexVariable::EnergyManagementSystemConstructionIndexVariable(const Model& model)
  : ModelObject(EnergyManagementSystemConstructionIndexVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemConstructionIndexVariable_Impl>());
  Construction construction = Construction(model);
  construction.setName("Generic Construction");
  setConstructionObject(construction);
}

IddObjectType EnergyManagementSystemConstructionIndexVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_ConstructionIndexVariable);
}

ModelObject EnergyManagementSystemConstructionIndexVariable::constructionObject() const {
  return getImpl<detail::EnergyManagementSystemConstructionIndexVariable_Impl>()->constructionObject();
}

bool EnergyManagementSystemConstructionIndexVariable::setConstructionObject(const ModelObject& construction) {
  return getImpl<detail::EnergyManagementSystemConstructionIndexVariable_Impl>()->setConstructionObject(construction);
}

/// @cond
EnergyManagementSystemConstructionIndexVariable::EnergyManagementSystemConstructionIndexVariable(std::shared_ptr<detail::EnergyManagementSystemConstructionIndexVariable_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

