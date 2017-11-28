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

#include "MeterCustom.hpp"
#include "MeterCustom_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Meter_Custom_FieldEnums.hxx>

// Needed for extensible groups
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  MeterCustom_Impl::MeterCustom_Impl(const IdfObject& idfObject,
                                     Model_Impl* model,
                                     bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == MeterCustom::iddObjectType());
  }

  MeterCustom_Impl::MeterCustom_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : ModelObject_Impl(other, model, keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == MeterCustom::iddObjectType());
  }

  MeterCustom_Impl::MeterCustom_Impl(const MeterCustom_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  // This logically doesn't produce any output variables
  const std::vector<std::string>& MeterCustom_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    //LOG(Info, "Meter:Custom does not produce any output variables.");
    return result;
  }

  IddObjectType MeterCustom_Impl::iddObjectType() const {
    return MeterCustom::iddObjectType();
  }

  // fuel Type
  boost::optional<std::string> MeterCustom_Impl::fuelType() const {
    return getString(OS_Meter_CustomFields::FuelType,true);
  }

  bool MeterCustom_Impl::setFuelType(const std::string& fuelType) {
    return setString(OS_Meter_CustomFields::FuelType, fuelType);
  }

  void MeterCustom_Impl::resetFuelType() {
    bool result = setString(OS_Meter_CustomFields::FuelType, "");
    OS_ASSERT(result);
  }


  // Add a new (Key, Var) group
  bool MeterCustom_Impl::addKeyVarGroup(const std::string& keyName, const std::string& outputVariableorMeterName) {
    WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();

    bool temp = eg.setString(OS_Meter_CustomExtensibleFields::KeyName, keyName);
    bool ok = eg.setString(OS_Meter_CustomExtensibleFields::OutputVariableorMeterName, outputVariableorMeterName);

    if (temp) {
      temp = ok;
    }

    if (!temp) {
      getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      return temp;
    }
    return temp;
  }

  // Remove the (Key, Var) group at given index
  bool MeterCustom_Impl::removeKeyVarGroup(unsigned groupIndex) {
    bool ok = false;
    unsigned numberofDataPairs = numExtensibleGroups();
    // Only delete if the index is smaller than the number of groups
    if (groupIndex < numberofDataPairs) {
      getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
      ok = true;
    }

    return ok;
  }

  // Remove all the (Key, Var) groups
  void MeterCustom_Impl::removeAllKeyVarGroups() {
    getObject<ModelObject>().clearExtensibleGroups();
  }

  // Return a vector of (Key, Var) pairs
  std::vector< std::pair<std::string, std::string> > MeterCustom_Impl::keyVarGroups() {
    std::vector< std::pair<std::string, std::string> > result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for (const auto & group : groups) {
      boost::optional<std::string> keyName = group.cast<WorkspaceExtensibleGroup>().getString(OS_Meter_CustomExtensibleFields::KeyName);
      boost::optional<std::string> outputVariableorMeterName = group.cast<WorkspaceExtensibleGroup>().getString(OS_Meter_CustomExtensibleFields::OutputVariableorMeterName);

      if (keyName && outputVariableorMeterName) {
        result.push_back(std::make_pair(keyName.get(), outputVariableorMeterName.get()));
      }
    }

    return result;
  }

  // Return the number of (KeyName, OutputVariableorMeterName) groups
  unsigned MeterCustom_Impl::numKeyVarGroups() const {
    return numExtensibleGroups();
  }


  // Lower level functions
  /** Get the Key Name at index. Indexing starts at 0. */
  boost::optional<std::string> MeterCustom_Impl::keyName(unsigned index) const {
    IdfExtensibleGroup eg = getExtensibleGroup(index);
    if (!eg.empty()) {
      return eg.getString(OS_Meter_CustomExtensibleFields::KeyName, true);
    }
    return boost::none;
  }

  /** Set the Key Name at index. Indexing starts at 0. */
  bool MeterCustom_Impl::setKeyName(unsigned index, const std::string& str) {
    IdfExtensibleGroup eg = getExtensibleGroup(index);
    if (!eg.empty()) {
      return eg.setString(OS_Meter_CustomExtensibleFields::KeyName, str);
    } else {
      StringVector values(2u);
      values[OS_Meter_CustomExtensibleFields::KeyName] = str;
      return !insertExtensibleGroup(index, values).empty();
    }
    OS_ASSERT(false);
    return false;
  }

  /** Get the Output Variable of Meter Name at index. Indexing starts at 0. */
  boost::optional<std::string> MeterCustom_Impl::outputVariableorMeterName(unsigned index) const {
    IdfExtensibleGroup eg = getExtensibleGroup(index);
    if (!eg.empty()) {
      return eg.getString(OS_Meter_CustomExtensibleFields::OutputVariableorMeterName, true);
    }
    return boost::none;
  }

  /** Set the Output Variable of Meter Name at index. Indexing starts at 0. */
  bool MeterCustom_Impl::setOutputVariableorMeterName(unsigned index, const std::string& str) {
    IdfExtensibleGroup eg = getExtensibleGroup(index);
    if (!eg.empty()) {
      return eg.setString(OS_Meter_CustomExtensibleFields::OutputVariableorMeterName, str);
    } else {
      StringVector values(2u);
      values[OS_Meter_CustomExtensibleFields::OutputVariableorMeterName] = str;
      return !insertExtensibleGroup(index, values).empty();
    }
    OS_ASSERT(false);
    return false;
  }




} // detail

MeterCustom::MeterCustom(const Model& model)
  : ModelObject(MeterCustom::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::MeterCustom_Impl>());

  // Default the fuelType to Electricity (maybe "Generic"?)
  setFuelType("Electricity");

}

IddObjectType MeterCustom::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Meter_Custom);
}

std::vector<std::string> MeterCustom::fuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Meter_CustomFields::FuelType);
}

boost::optional<std::string> MeterCustom::fuelType() const {
  return getImpl<detail::MeterCustom_Impl>()->fuelType();
}

bool MeterCustom::setFuelType(const std::string& fuelType) {
  return getImpl<detail::MeterCustom_Impl>()->setFuelType(fuelType);
}

void MeterCustom::resetFuelType() {
  getImpl<detail::MeterCustom_Impl>()->resetFuelType();
}

// Add a new (Key, Var) group
bool MeterCustom::addKeyVarGroup(const std::string& keyName, const std::string& outputVariableorMeterName) {
  return getImpl<detail::MeterCustom_Impl>()->addKeyVarGroup(keyName, outputVariableorMeterName);
}

// Remove the (Key, Var) group at given index
bool MeterCustom::removeKeyVarGroup(unsigned groupIndex) {
  return getImpl<detail::MeterCustom_Impl>()->removeKeyVarGroup(groupIndex);
}

// Remove all the (Key, Var) groups
void MeterCustom::removeAllKeyVarGroups() {
  getImpl<detail::MeterCustom_Impl>()->removeAllKeyVarGroups();
}

// // Return a vector of (Key, Var) pairs
std::vector< std::pair<std::string, std::string> > MeterCustom::keyVarGroups() {
  return getImpl<detail::MeterCustom_Impl>()->keyVarGroups();
}

// Return the number of (KeyName, OutputVariableorMeterName) groups
unsigned MeterCustom::numKeyVarGroups() const {
  return getImpl<detail::MeterCustom_Impl>()->numKeyVarGroups();
}


// Lower level functions
/** Get the Key Name at index. Indexing starts at 0. */
boost::optional<std::string> MeterCustom::keyName(unsigned index) const {
  return getImpl<detail::MeterCustom_Impl>()->keyName(index);
}
/** Set the Key Name at index. Indexing starts at 0. */
bool MeterCustom::setKeyName(unsigned index, const std::string& str) {
  return getImpl<detail::MeterCustom_Impl>()->setKeyName(index, str);
}

/** Get the Output Variable of Meter Name at index. Indexing starts at 0. */
boost::optional<std::string> MeterCustom::outputVariableorMeterName(unsigned index) const {
  return getImpl<detail::MeterCustom_Impl>()->outputVariableorMeterName(index);
}
/** Set the Output Variable of Meter Name at index. Indexing starts at 0. */
bool MeterCustom::setOutputVariableorMeterName(unsigned index, const std::string& str) {
  return getImpl<detail::MeterCustom_Impl>()->setOutputVariableorMeterName(index, str);
}

/// @cond
MeterCustom::MeterCustom(std::shared_ptr<detail::MeterCustom_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

