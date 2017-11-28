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

#include "SurfacePropertyOtherSideConditionsModel.hpp"
#include "SurfacePropertyOtherSideConditionsModel_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SurfaceProperty_OtherSideConditionsModel_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SurfacePropertyOtherSideConditionsModel_Impl::SurfacePropertyOtherSideConditionsModel_Impl(const IdfObject& idfObject,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SurfacePropertyOtherSideConditionsModel::iddObjectType());
  }

  SurfacePropertyOtherSideConditionsModel_Impl::SurfacePropertyOtherSideConditionsModel_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SurfacePropertyOtherSideConditionsModel::iddObjectType());
  }

  SurfacePropertyOtherSideConditionsModel_Impl::SurfacePropertyOtherSideConditionsModel_Impl(const SurfacePropertyOtherSideConditionsModel_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SurfacePropertyOtherSideConditionsModel_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SurfacePropertyOtherSideConditionsModel_Impl::iddObjectType() const {
    return SurfacePropertyOtherSideConditionsModel::iddObjectType();
  }

  std::string SurfacePropertyOtherSideConditionsModel_Impl::typeOfModeling() const {
    boost::optional<std::string> value = getString(OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SurfacePropertyOtherSideConditionsModel_Impl::isTypeOfModelingDefaulted() const {
    return isEmpty(OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling);
  }

  bool SurfacePropertyOtherSideConditionsModel_Impl::setTypeOfModeling(const std::string& typeOfModeling) {
    bool result = setString(OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling, typeOfModeling);
    return result;
  }

  void SurfacePropertyOtherSideConditionsModel_Impl::resetTypeOfModeling() {
    bool result = setString(OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling, "");
    OS_ASSERT(result);
  }

} // detail

SurfacePropertyOtherSideConditionsModel::SurfacePropertyOtherSideConditionsModel(const Model& model)
  : ResourceObject(SurfacePropertyOtherSideConditionsModel::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
}

IddObjectType SurfacePropertyOtherSideConditionsModel::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SurfaceProperty_OtherSideConditionsModel);
}

std::vector<std::string> SurfacePropertyOtherSideConditionsModel::typeOfModelingValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling);
}

std::string SurfacePropertyOtherSideConditionsModel::typeOfModeling() const {
  return getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>()->typeOfModeling();
}

bool SurfacePropertyOtherSideConditionsModel::isTypeOfModelingDefaulted() const {
  return getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>()->isTypeOfModelingDefaulted();
}

bool SurfacePropertyOtherSideConditionsModel::setTypeOfModeling(const std::string& typeOfModeling) {
  return getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>()->setTypeOfModeling(typeOfModeling);
}

void SurfacePropertyOtherSideConditionsModel::resetTypeOfModeling() {
  getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>()->resetTypeOfModeling();
}

/// @cond
SurfacePropertyOtherSideConditionsModel::SurfacePropertyOtherSideConditionsModel(std::shared_ptr<detail::SurfacePropertyOtherSideConditionsModel_Impl> impl)
  : ResourceObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

