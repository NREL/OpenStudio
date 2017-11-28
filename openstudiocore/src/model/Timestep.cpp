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

#include "Timestep.hpp"
#include "Timestep_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/OS_Timestep_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

namespace model  {

namespace detail {

  Timestep_Impl::Timestep_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Timestep::iddObjectType());
  }

  Timestep_Impl::Timestep_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Timestep::iddObjectType());
  }

  Timestep_Impl::Timestep_Impl(const Timestep_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
  }

  IddObjectType Timestep_Impl::iddObjectType() const {
    return Timestep::iddObjectType();
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> Timestep_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool Timestep_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& Timestep_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  int Timestep_Impl::numberOfTimestepsPerHour() const {
    boost::optional<int> value = getInt(OS_TimestepFields::NumberofTimestepsperHour,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Timestep_Impl::isNumberOfTimestepsPerHourDefaulted() const {
    return isEmpty(OS_TimestepFields::NumberofTimestepsperHour);
  }

  bool Timestep_Impl::setNumberOfTimestepsPerHour(int numberOfTimestepsPerHour) {
    bool result = setInt(OS_TimestepFields::NumberofTimestepsperHour, numberOfTimestepsPerHour);
    return result;
  }

  void Timestep_Impl::resetNumberOfTimestepsPerHour() {
    bool result = setString(OS_TimestepFields::NumberofTimestepsperHour, "");
    OS_ASSERT(result);
  }

} // detail

/// constructor
Timestep::Timestep(const Model& model)
  : ModelObject(Timestep::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::Timestep_Impl>());

  setNumberOfTimestepsPerHour(6);
}

// constructor
Timestep::Timestep(std::shared_ptr<detail::Timestep_Impl> impl)
  : ModelObject(std::move(impl))
{}

IddObjectType Timestep::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Timestep);
  return result;
}

int Timestep::numberOfTimestepsPerHour() const {
  return getImpl<detail::Timestep_Impl>()->numberOfTimestepsPerHour();
}

bool Timestep::isNumberOfTimestepsPerHourDefaulted() const {
  return getImpl<detail::Timestep_Impl>()->isNumberOfTimestepsPerHourDefaulted();
}

bool Timestep::setNumberOfTimestepsPerHour(int numberOfTimestepsPerHour) {
  return getImpl<detail::Timestep_Impl>()->setNumberOfTimestepsPerHour(numberOfTimestepsPerHour);
}

void Timestep::resetNumberOfTimestepsPerHour() {
  getImpl<detail::Timestep_Impl>()->resetNumberOfTimestepsPerHour();
}

} // model
} // openstudio
