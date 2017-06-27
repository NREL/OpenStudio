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

#include "OutsideSurfaceConvectionAlgorithm.hpp"
#include "OutsideSurfaceConvectionAlgorithm_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SurfaceConvectionAlgorithm_Outside_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model  {

namespace detail {

  OutsideSurfaceConvectionAlgorithm_Impl::OutsideSurfaceConvectionAlgorithm_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == OutsideSurfaceConvectionAlgorithm::iddObjectType());
  }

  OutsideSurfaceConvectionAlgorithm_Impl::OutsideSurfaceConvectionAlgorithm_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == OutsideSurfaceConvectionAlgorithm::iddObjectType());
  }

  OutsideSurfaceConvectionAlgorithm_Impl::OutsideSurfaceConvectionAlgorithm_Impl(
      const OutsideSurfaceConvectionAlgorithm_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> OutsideSurfaceConvectionAlgorithm_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool OutsideSurfaceConvectionAlgorithm_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& OutsideSurfaceConvectionAlgorithm_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType OutsideSurfaceConvectionAlgorithm_Impl::iddObjectType() const {
    return OutsideSurfaceConvectionAlgorithm::iddObjectType();
  }

  std::string OutsideSurfaceConvectionAlgorithm_Impl::algorithm() const {
    boost::optional<std::string> value = getString(OS_SurfaceConvectionAlgorithm_OutsideFields::Algorithm,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool OutsideSurfaceConvectionAlgorithm_Impl::isAlgorithmDefaulted() const {
    return isEmpty(OS_SurfaceConvectionAlgorithm_OutsideFields::Algorithm);
  }

  bool OutsideSurfaceConvectionAlgorithm_Impl::setAlgorithm(std::string algorithm) {
    bool result = setString(OS_SurfaceConvectionAlgorithm_OutsideFields::Algorithm, algorithm);
    return result;
  }

  void OutsideSurfaceConvectionAlgorithm_Impl::resetAlgorithm() {
    bool result = setString(OS_SurfaceConvectionAlgorithm_OutsideFields::Algorithm, "");
    OS_ASSERT(result);
  }

} // detail

/// constructor
OutsideSurfaceConvectionAlgorithm::OutsideSurfaceConvectionAlgorithm(const Model& model)
  : ModelObject(OutsideSurfaceConvectionAlgorithm::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::OutsideSurfaceConvectionAlgorithm_Impl>());

  setAlgorithm("DOE-2");
}

// constructor
OutsideSurfaceConvectionAlgorithm::OutsideSurfaceConvectionAlgorithm(
    std::shared_ptr<detail::OutsideSurfaceConvectionAlgorithm_Impl> impl)
  : ModelObject(std::move(impl))
{}

IddObjectType OutsideSurfaceConvectionAlgorithm::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SurfaceConvectionAlgorithm_Outside);
  return result;
}

std::vector<std::string> OutsideSurfaceConvectionAlgorithm::validAlgorithmValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceConvectionAlgorithm_OutsideFields::Algorithm);
}

std::string OutsideSurfaceConvectionAlgorithm::algorithm() const {
  return getImpl<detail::OutsideSurfaceConvectionAlgorithm_Impl>()->algorithm();
}

bool OutsideSurfaceConvectionAlgorithm::isAlgorithmDefaulted() const {
  return getImpl<detail::OutsideSurfaceConvectionAlgorithm_Impl>()->isAlgorithmDefaulted();
}

bool OutsideSurfaceConvectionAlgorithm::setAlgorithm(std::string algorithm) {
  return getImpl<detail::OutsideSurfaceConvectionAlgorithm_Impl>()->setAlgorithm(algorithm);
}

void OutsideSurfaceConvectionAlgorithm::resetAlgorithm() {
  getImpl<detail::OutsideSurfaceConvectionAlgorithm_Impl>()->resetAlgorithm();
}

} // model
} // openstudio
