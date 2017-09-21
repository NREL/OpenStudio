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

#include "ScheduleConstant.hpp"
#include "ScheduleConstant_Impl.hpp"
#include "Model.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/OS_Schedule_Constant_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

using openstudio::Handle;
using openstudio::OptionalHandle;
using openstudio::HandleVector;
using openstudio::IdfObject;
using openstudio::WorkspaceObject;
using openstudio::OptionalWorkspaceObject;
using openstudio::WorkspaceObjectVector;
using openstudio::Workspace;

namespace openstudio {
namespace model {

namespace detail {

  ScheduleConstant_Impl::ScheduleConstant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Schedule_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ScheduleConstant::iddObjectType());
  }

  ScheduleConstant_Impl::ScheduleConstant_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : Schedule_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ScheduleConstant::iddObjectType());
  }

  ScheduleConstant_Impl::ScheduleConstant_Impl(const ScheduleConstant_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : Schedule_Impl(other,model,keepHandle)
  {}

  // return the parent object in the hierarchy
  boost::optional<ParentObject> ScheduleConstant_Impl::parent() const
  {
    return boost::optional<ParentObject>();
  }

  // return any children objects in the hierarchy
  std::vector<ModelObject> ScheduleConstant_Impl::children() const
  {
    std::vector<ModelObject> result;
    return result;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& ScheduleConstant_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ScheduleConstant_Impl::iddObjectType() const {
    return ScheduleConstant::iddObjectType();
  }

  boost::optional<ScheduleTypeLimits> ScheduleConstant_Impl::scheduleTypeLimits() const {
    return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_Schedule_ConstantFields::ScheduleTypeLimitsName);
  }

  std::vector<double> ScheduleConstant_Impl::values() const {
    return DoubleVector(1u,value());
  }

  double ScheduleConstant_Impl::value() const {
    boost::optional<double> result = getDouble(OS_Schedule_ConstantFields::Value, true);
    if(!result){
      return 0.0;
    }
    return *result;
  }

  boost::optional<Quantity> ScheduleConstant_Impl::getValue(bool returnIP) const {
    return toQuantity(value(),returnIP);
  }

  bool ScheduleConstant_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
    if (scheduleTypeLimits.model() != model()) {
      return false;
    }
    if (!candidateIsCompatibleWithCurrentUse(scheduleTypeLimits)) {
      return false;
    }
    return setPointer(OS_Schedule_ConstantFields::ScheduleTypeLimitsName,scheduleTypeLimits.handle());
  }

  bool ScheduleConstant_Impl::resetScheduleTypeLimits() {
    if (okToResetScheduleTypeLimits()) {
      return setString(OS_Schedule_ConstantFields::ScheduleTypeLimitsName,"");
    }
    return false;
  }

  void ScheduleConstant_Impl::setValue(double value) {
    bool ok = setDouble(OS_Schedule_ConstantFields::Value, value);
    OS_ASSERT(ok);
  }

  bool ScheduleConstant_Impl::setValue(const Quantity& value) {
    OptionalDouble dval = toDouble(value);
    if (dval) {
      setValue(*dval);
      return true;
    }
    return false;
  }

  void ScheduleConstant_Impl::ensureNoLeapDays()
  {
    // nothing to do
  }

} // detail

// create a new ScheduleConstant object in the model's workspace
ScheduleConstant::ScheduleConstant(const Model& model)
  : Schedule(ScheduleConstant::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ScheduleConstant_Impl>());
  setValue(0.0);
}

// constructor
ScheduleConstant::ScheduleConstant(std::shared_ptr<detail::ScheduleConstant_Impl> impl)
  : Schedule(std::move(impl))
{}

IddObjectType ScheduleConstant::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Schedule_Constant);
  return result;
}

boost::optional<Quantity> ScheduleConstant::getValue(bool returnIP) const {
  return getImpl<detail::ScheduleConstant_Impl>()->getValue(returnIP);
}

double ScheduleConstant::value() const {
  return getImpl<detail::ScheduleConstant_Impl>()->value();
}

void ScheduleConstant::setValue(double value) {
  getImpl<detail::ScheduleConstant_Impl>()->setValue(value);
}

bool ScheduleConstant::setValue(const Quantity& value) {
  return getImpl<detail::ScheduleConstant_Impl>()->setValue(value);
}

} // model
} // openstudio
