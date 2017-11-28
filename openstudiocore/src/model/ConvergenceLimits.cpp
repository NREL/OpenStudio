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

#include "ConvergenceLimits.hpp"
#include "ConvergenceLimits_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/OS_ConvergenceLimits_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

namespace detail {

  ConvergenceLimits_Impl::ConvergenceLimits_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ConvergenceLimits::iddObjectType());
  }

  ConvergenceLimits_Impl::ConvergenceLimits_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ConvergenceLimits::iddObjectType());
  }

  ConvergenceLimits_Impl::ConvergenceLimits_Impl(const ConvergenceLimits_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ConvergenceLimits_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ConvergenceLimits_Impl::iddObjectType() const {
    return ConvergenceLimits::iddObjectType();
  }

  boost::optional<ParentObject> ConvergenceLimits_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
    return result;
  }

  bool ConvergenceLimits_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  boost::optional<int> ConvergenceLimits_Impl::minimumSystemTimestep() const {
    return getInt(OS_ConvergenceLimitsFields::MinimumSystemTimestep,true);
  }

  int ConvergenceLimits_Impl::maximumHVACIterations() const {
    boost::optional<int> value = getInt(OS_ConvergenceLimitsFields::MaximumHVACIterations,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ConvergenceLimits_Impl::isMaximumHVACIterationsDefaulted() const {
    return isEmpty(OS_ConvergenceLimitsFields::MaximumHVACIterations);
  }

  int ConvergenceLimits_Impl::minimumPlantIterations() const {
    boost::optional<int> value = getInt(OS_ConvergenceLimitsFields::MinimumPlantIterations,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ConvergenceLimits_Impl::isMinimumPlantIterationsDefaulted() const {
    return isEmpty(OS_ConvergenceLimitsFields::MinimumPlantIterations);
  }

  int ConvergenceLimits_Impl::maximumPlantIterations() const {
    boost::optional<int> value = getInt(OS_ConvergenceLimitsFields::MaximumPlantIterations,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ConvergenceLimits_Impl::isMaximumPlantIterationsDefaulted() const {
    return isEmpty(OS_ConvergenceLimitsFields::MaximumPlantIterations);
  }

  bool ConvergenceLimits_Impl::setMinimumSystemTimestep(boost::optional<int> minimumSystemTimestep) {
    bool result = false;
    if (minimumSystemTimestep) {
      result = setInt(OS_ConvergenceLimitsFields::MinimumSystemTimestep, minimumSystemTimestep.get());
    } else {
      result = setString(OS_ConvergenceLimitsFields::MinimumSystemTimestep, "");
    }
    return result;
  }

  void ConvergenceLimits_Impl::resetMinimumSystemTimestep() {
    bool result = setString(OS_ConvergenceLimitsFields::MinimumSystemTimestep, "");
    OS_ASSERT(result);
  }

  bool ConvergenceLimits_Impl::setMaximumHVACIterations(int maximumHVACIterations) {
    bool result = setInt(OS_ConvergenceLimitsFields::MaximumHVACIterations, maximumHVACIterations);
    return result;
  }

  void ConvergenceLimits_Impl::resetMaximumHVACIterations() {
    bool result = setString(OS_ConvergenceLimitsFields::MaximumHVACIterations, "");
    OS_ASSERT(result);
  }

  bool ConvergenceLimits_Impl::setMinimumPlantIterations(int minimumPlantIterations) {
    bool result = setInt(OS_ConvergenceLimitsFields::MinimumPlantIterations, minimumPlantIterations);
    return result;
  }

  void ConvergenceLimits_Impl::resetMinimumPlantIterations() {
    bool result = setString(OS_ConvergenceLimitsFields::MinimumPlantIterations, "");
    OS_ASSERT(result);
  }

  bool ConvergenceLimits_Impl::setMaximumPlantIterations(int maximumPlantIterations) {
    bool result = setInt(OS_ConvergenceLimitsFields::MaximumPlantIterations, maximumPlantIterations);
    return result;
  }

  void ConvergenceLimits_Impl::resetMaximumPlantIterations() {
    bool result = setString(OS_ConvergenceLimitsFields::MaximumPlantIterations, "");
    OS_ASSERT(result);
  }

} // detail

IddObjectType ConvergenceLimits::iddObjectType() {
  IddObjectType result(IddObjectType::OS_ConvergenceLimits);
  return result;
}

boost::optional<int> ConvergenceLimits::minimumSystemTimestep() const {
  return getImpl<detail::ConvergenceLimits_Impl>()->minimumSystemTimestep();
}

int ConvergenceLimits::maximumHVACIterations() const {
  return getImpl<detail::ConvergenceLimits_Impl>()->maximumHVACIterations();
}

bool ConvergenceLimits::isMaximumHVACIterationsDefaulted() const {
  return getImpl<detail::ConvergenceLimits_Impl>()->isMaximumHVACIterationsDefaulted();
}

int ConvergenceLimits::minimumPlantIterations() const {
  return getImpl<detail::ConvergenceLimits_Impl>()->minimumPlantIterations();
}

bool ConvergenceLimits::isMinimumPlantIterationsDefaulted() const {
  return getImpl<detail::ConvergenceLimits_Impl>()->isMinimumPlantIterationsDefaulted();
}

int ConvergenceLimits::maximumPlantIterations() const {
  return getImpl<detail::ConvergenceLimits_Impl>()->maximumPlantIterations();
}

bool ConvergenceLimits::isMaximumPlantIterationsDefaulted() const {
  return getImpl<detail::ConvergenceLimits_Impl>()->isMaximumPlantIterationsDefaulted();
}

bool ConvergenceLimits::setMinimumSystemTimestep(int minimumSystemTimestep) {
  return getImpl<detail::ConvergenceLimits_Impl>()->setMinimumSystemTimestep(minimumSystemTimestep);
}

void ConvergenceLimits::resetMinimumSystemTimestep() {
  getImpl<detail::ConvergenceLimits_Impl>()->resetMinimumSystemTimestep();
}

bool ConvergenceLimits::setMaximumHVACIterations(int maximumHVACIterations) {
  return getImpl<detail::ConvergenceLimits_Impl>()->setMaximumHVACIterations(maximumHVACIterations);
}

void ConvergenceLimits::resetMaximumHVACIterations() {
  getImpl<detail::ConvergenceLimits_Impl>()->resetMaximumHVACIterations();
}

bool ConvergenceLimits::setMinimumPlantIterations(int minimumPlantIterations) {
  return getImpl<detail::ConvergenceLimits_Impl>()->setMinimumPlantIterations(minimumPlantIterations);
}

void ConvergenceLimits::resetMinimumPlantIterations() {
  getImpl<detail::ConvergenceLimits_Impl>()->resetMinimumPlantIterations();
}

bool ConvergenceLimits::setMaximumPlantIterations(int maximumPlantIterations) {
  return getImpl<detail::ConvergenceLimits_Impl>()->setMaximumPlantIterations(maximumPlantIterations);
}

void ConvergenceLimits::resetMaximumPlantIterations() {
  getImpl<detail::ConvergenceLimits_Impl>()->resetMaximumPlantIterations();
}

/// @cond
ConvergenceLimits::ConvergenceLimits(std::shared_ptr<detail::ConvergenceLimits_Impl> impl)
  : ModelObject(std::move(impl))
{}

ConvergenceLimits::ConvergenceLimits(Model& model)
  : ModelObject(ConvergenceLimits::iddObjectType(),model)
{
  setMinimumSystemTimestep(1);
}
/// @endcond

} // model
} // openstudio
