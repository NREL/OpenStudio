/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

