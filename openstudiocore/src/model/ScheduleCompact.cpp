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

#include "ScheduleCompact.hpp"
#include "ScheduleCompact_Impl.hpp"
#include "Model.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/OS_Schedule_Compact_FieldEnums.hxx>
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

  ScheduleCompact_Impl::ScheduleCompact_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Schedule_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ScheduleCompact::iddObjectType());
  }

  ScheduleCompact_Impl::ScheduleCompact_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : Schedule_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ScheduleCompact::iddObjectType());
  }

  ScheduleCompact_Impl::ScheduleCompact_Impl(const ScheduleCompact_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : Schedule_Impl(other,model,keepHandle)
  {}

  // return the parent object in the hierarchy
  boost::optional<ParentObject> ScheduleCompact_Impl::parent() const
  {
    return boost::optional<ParentObject>();
  }

  // return any children objects in the hierarchy
  std::vector<ModelObject> ScheduleCompact_Impl::children() const
  {
    std::vector<ModelObject> result;
    return result;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& ScheduleCompact_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ScheduleCompact_Impl::iddObjectType() const {
    return ScheduleCompact::iddObjectType();
  }

  std::vector<double> ScheduleCompact_Impl::values() const {
    DoubleVector result;
    for (const IdfExtensibleGroup& eg : extensibleGroups()) {
      std::string str = eg.getString(0,true).get();
      boost::trim(str);
      if (!str.empty()) {
        char firstChar = str[0];
        switch (firstChar) {
        case 'T':
          break;
        case 'F':
          break;
        case 'U':
          break;
        default :
        {
          try {
            double value = boost::lexical_cast<double>(str);
            result.push_back(value);
          }
          catch (...) {}
        }
        }
      }
    }
    return result;
  }

  boost::optional<ScheduleTypeLimits> ScheduleCompact_Impl::scheduleTypeLimits() const {
    return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_Schedule_CompactFields::ScheduleTypeLimitsName);
  }

  bool ScheduleCompact_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
    if (scheduleTypeLimits.model() != model()) {
      return false;
    }
    if (!candidateIsCompatibleWithCurrentUse(scheduleTypeLimits)) {
      return false;
    }
    return setPointer(OS_Schedule_CompactFields::ScheduleTypeLimitsName,scheduleTypeLimits.handle());
  }

  bool ScheduleCompact_Impl::resetScheduleTypeLimits() {
    if (okToResetScheduleTypeLimits()) {
      return setString(OS_Schedule_CompactFields::ScheduleTypeLimitsName,"");
    }
    return false;
  }

  void ScheduleCompact_Impl::setToConstantValue(double value) {
    clearExtensibleGroups();
    StringVector values;
    values.push_back("Through: 12/31");
    IdfExtensibleGroup eg = pushExtensibleGroup(values);
    OS_ASSERT(!eg.empty());
    values[0] = "For: AllDays";
    eg = pushExtensibleGroup(values);
    OS_ASSERT(!eg.empty());
    values[0] = "Until: 24:00";
    eg = pushExtensibleGroup(values);
    OS_ASSERT(!eg.empty());
    values[0] = "";
    eg = pushExtensibleGroup(values);
    OS_ASSERT(!eg.empty());
    bool ok = eg.setDouble(0,value);
    OS_ASSERT(ok);
  }

  bool ScheduleCompact_Impl::setToConstantValue(const Quantity& value) {
    OptionalDouble dval = toDouble(value);
    if (dval) {
      setToConstantValue(*dval);
      return true;
    }
    return false;
  }

  void ScheduleCompact_Impl::ensureNoLeapDays()
  {
    LOG(Warn, "Ensure no leap days is not yet implemented for schedule compact");
  }

  bool ScheduleCompact_Impl::isConstantValue() const {
    IdfExtensibleGroupVector scheduleData = extensibleGroups();
    if (scheduleData.size() == 4u) {
      if (scheduleData[3].getDouble(0)) { return true; }
    }
    return false;
  }

  boost::optional<double> ScheduleCompact_Impl::constantValue() const {
    IdfExtensibleGroupVector scheduleData = extensibleGroups();
    if (scheduleData.size() == 4u) {
      return scheduleData[3].getDouble(0);
    }
    return boost::none;
  }

  boost::optional<Quantity> ScheduleCompact_Impl::getConstantValue(bool returnIP) const {
    OptionalQuantity result;
    if (OptionalDouble value = constantValue()) {
      result = toQuantity(*value,returnIP);
    }
    return result;
  }

} // detail

// create a new ScheduleCompact object in the model's workspace
ScheduleCompact::ScheduleCompact(const Model& model)
  : Schedule(ScheduleCompact::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ScheduleCompact_Impl>());
}

ScheduleCompact::ScheduleCompact(const Model& model,double constantValue)
  : Schedule(ScheduleCompact::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ScheduleCompact_Impl>());
  setToConstantValue(constantValue);
}

ScheduleCompact::ScheduleCompact(const Model& model,const Quantity& constantValue)
  : Schedule(ScheduleCompact::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ScheduleCompact_Impl>());
  setToConstantValue(constantValue);
}

// constructor
ScheduleCompact::ScheduleCompact(std::shared_ptr<detail::ScheduleCompact_Impl> impl)
  : Schedule(std::move(impl))
{}

void ScheduleCompact::setToConstantValue(double value) {
  getImpl<detail::ScheduleCompact_Impl>()->setToConstantValue(value);
}

bool ScheduleCompact::setToConstantValue(const Quantity& value) {
  return getImpl<detail::ScheduleCompact_Impl>()->setToConstantValue(value);
}

bool ScheduleCompact::isConstantValue() const {
  return getImpl<detail::ScheduleCompact_Impl>()->isConstantValue();
}

boost::optional<double> ScheduleCompact::constantValue() const {
  return getImpl<detail::ScheduleCompact_Impl>()->constantValue();
}

boost::optional<Quantity> ScheduleCompact::getConstantValue(bool returnIP) const {
  return getImpl<detail::ScheduleCompact_Impl>()->getConstantValue(returnIP);
}

IddObjectType ScheduleCompact::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Schedule_Compact);
  return result;
}

} // model
} // openstudio
