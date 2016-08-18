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

#include "MeterCustomDecrement.hpp"
#include "MeterCustomDecrement_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Meter_CustomDecrement_FieldEnums.hxx>

// Needed for extensible groups
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  MeterCustomDecrement_Impl::MeterCustomDecrement_Impl(const IdfObject& idfObject,
                                     Model_Impl* model,
                                     bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == MeterCustomDecrement::iddObjectType());
  }

  MeterCustomDecrement_Impl::MeterCustomDecrement_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : ModelObject_Impl(other, model, keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == MeterCustomDecrement::iddObjectType());
  }

  MeterCustomDecrement_Impl::MeterCustomDecrement_Impl(const MeterCustomDecrement_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  // This logically doesn't produce any output variables
  const std::vector<std::string>& MeterCustomDecrement_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    //LOG(Info, "Meter:Custom does not produce any output variables.");
    return result;
  }

  IddObjectType MeterCustomDecrement_Impl::iddObjectType() const {
    return MeterCustomDecrement::iddObjectType();
  }

  // fuel Type
  boost::optional<std::string> MeterCustomDecrement_Impl::fuelType() const {
    return getString(OS_Meter_CustomDecrementFields::FuelType,true);
  }

  bool MeterCustomDecrement_Impl::setFuelType(const std::string& fuelType) {
    return setString(OS_Meter_CustomDecrementFields::FuelType, fuelType);
  }

  void MeterCustomDecrement_Impl::resetFuelType() {
    bool result = setString(OS_Meter_CustomDecrementFields::FuelType, "");
    OS_ASSERT(result);
  }
  
  // Source Meter Name
  std::string MeterCustomDecrement_Impl::sourceMeterName() const {
    return getString(OS_Meter_CustomDecrementFields::SourceMeterName,true).get();
  }
  
  bool MeterCustomDecrement_Impl::setSourceMeterName(const std::string& sourceMeterName) {
    return setString(OS_Meter_CustomDecrementFields::SourceMeterName, sourceMeterName);
  }

  // Add a new (Key, Var) group
  bool MeterCustomDecrement_Impl::addKeyVarGroup(const std::string& keyName, const std::string& outputVariableorMeterName) {
    WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();

    bool temp = eg.setString(OS_Meter_CustomDecrementExtensibleFields::KeyName, keyName);
    bool ok = eg.setString(OS_Meter_CustomDecrementExtensibleFields::OutputVariableorMeterName, outputVariableorMeterName);

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
  bool MeterCustomDecrement_Impl::removeKeyVarGroup(unsigned groupIndex) {
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
  void MeterCustomDecrement_Impl::removeAllKeyVarGroups() {
    getObject<ModelObject>().clearExtensibleGroups();
  }

  // Return a vector of (Key, Var) pairs
  std::vector< std::pair<std::string, std::string> > MeterCustomDecrement_Impl::keyVarGroups() {
    std::vector< std::pair<std::string, std::string> > result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for (const auto & group : groups) {
      boost::optional<std::string> keyName = group.cast<WorkspaceExtensibleGroup>().getString(OS_Meter_CustomDecrementExtensibleFields::KeyName);
      boost::optional<std::string> outputVariableorMeterName = group.cast<WorkspaceExtensibleGroup>().getString(OS_Meter_CustomDecrementExtensibleFields::OutputVariableorMeterName);

      if (keyName && outputVariableorMeterName) {
        result.push_back(std::make_pair(keyName.get(), outputVariableorMeterName.get()));
      }
    }

    return result;
  }

  // Return the number of (KeyName, OutputVariableorMeterName) groups
  unsigned MeterCustomDecrement_Impl::numKeyVarGroups() const {
    return numExtensibleGroups();
  }


  // Lower level functions
  /** Get the Key Name at index. Indexing starts at 0. */
  boost::optional<std::string> MeterCustomDecrement_Impl::keyName(unsigned index) const {
    IdfExtensibleGroup eg = getExtensibleGroup(index);
    if (!eg.empty()) {
      return eg.getString(OS_Meter_CustomDecrementExtensibleFields::KeyName, true);
    }
    return boost::none;
  }

  /** Set the Key Name at index. Indexing starts at 0. */
  bool MeterCustomDecrement_Impl::setKeyName(unsigned index, const std::string& str) {
    IdfExtensibleGroup eg = getExtensibleGroup(index);
    if (!eg.empty()) {
      return eg.setString(OS_Meter_CustomDecrementExtensibleFields::KeyName, str);
    } else {
      StringVector values(2u);
      values[OS_Meter_CustomDecrementExtensibleFields::KeyName] = str;
      return !insertExtensibleGroup(index, values).empty();
    }
    OS_ASSERT(false);
    return false;
  }

  /** Get the Output Variable of Meter Name at index. Indexing starts at 0. */
  boost::optional<std::string> MeterCustomDecrement_Impl::outputVariableorMeterName(unsigned index) const {
    IdfExtensibleGroup eg = getExtensibleGroup(index);
    if (!eg.empty()) {
      return eg.getString(OS_Meter_CustomDecrementExtensibleFields::OutputVariableorMeterName, true);
    }
    return boost::none;
  }

  /** Set the Output Variable of Meter Name at index. Indexing starts at 0. */
  bool MeterCustomDecrement_Impl::setOutputVariableorMeterName(unsigned index, const std::string& str) {
    IdfExtensibleGroup eg = getExtensibleGroup(index);
    if (!eg.empty()) {
      return eg.setString(OS_Meter_CustomDecrementExtensibleFields::OutputVariableorMeterName, str);
    } else {
      StringVector values(2u);
      values[OS_Meter_CustomDecrementExtensibleFields::OutputVariableorMeterName] = str;
      return !insertExtensibleGroup(index, values).empty();
    }
    OS_ASSERT(false);
    return false;
  }




} // detail

MeterCustomDecrement::MeterCustomDecrement(const Model& model, const std::string& sourceMeterName)
  : ModelObject(MeterCustomDecrement::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::MeterCustomDecrement_Impl>());
  
  setSourceMeterName(sourceMeterName);

  // Default the fuelType to Electricity (maybe "Generic"?)
  setFuelType("Electricity");

}

IddObjectType MeterCustomDecrement::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Meter_CustomDecrement);
}

std::vector<std::string> MeterCustomDecrement::fuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Meter_CustomDecrementFields::FuelType);
}

boost::optional<std::string> MeterCustomDecrement::fuelType() const {
  return getImpl<detail::MeterCustomDecrement_Impl>()->fuelType();
}

bool MeterCustomDecrement::setFuelType(const std::string& fuelType) {
  return getImpl<detail::MeterCustomDecrement_Impl>()->setFuelType(fuelType);
}

void MeterCustomDecrement::resetFuelType() {
  getImpl<detail::MeterCustomDecrement_Impl>()->resetFuelType();
}

std::string MeterCustomDecrement::sourceMeterName() const {
  return getImpl<detail::MeterCustomDecrement_Impl>()->sourceMeterName();
}

bool MeterCustomDecrement::setSourceMeterName(const std::string& sourceMeterName) {
  return getImpl<detail::MeterCustomDecrement_Impl>()->setSourceMeterName(sourceMeterName);
}


// Add a new (Key, Var) group
bool MeterCustomDecrement::addKeyVarGroup(const std::string& keyName, const std::string& outputVariableorMeterName) {
  return getImpl<detail::MeterCustomDecrement_Impl>()->addKeyVarGroup(keyName, outputVariableorMeterName);
}

// Remove the (Key, Var) group at given index
bool MeterCustomDecrement::removeKeyVarGroup(unsigned groupIndex) {
  return getImpl<detail::MeterCustomDecrement_Impl>()->removeKeyVarGroup(groupIndex);
}

// Remove all the (Key, Var) groups
void MeterCustomDecrement::removeAllKeyVarGroups() {
  getImpl<detail::MeterCustomDecrement_Impl>()->removeAllKeyVarGroups();
}

// // Return a vector of (Key, Var) pairs
std::vector< std::pair<std::string, std::string> > MeterCustomDecrement::keyVarGroups() {
  return getImpl<detail::MeterCustomDecrement_Impl>()->keyVarGroups();
}

// Return the number of (KeyName, OutputVariableorMeterName) groups
unsigned MeterCustomDecrement::numKeyVarGroups() const {
  return getImpl<detail::MeterCustomDecrement_Impl>()->numKeyVarGroups();
}


// Lower level functions
/** Get the Key Name at index. Indexing starts at 0. */
boost::optional<std::string> MeterCustomDecrement::keyName(unsigned index) const {
  return getImpl<detail::MeterCustomDecrement_Impl>()->keyName(index);
}
/** Set the Key Name at index. Indexing starts at 0. */
bool MeterCustomDecrement::setKeyName(unsigned index, const std::string& str) {
  return getImpl<detail::MeterCustomDecrement_Impl>()->setKeyName(index, str);
}

/** Get the Output Variable of Meter Name at index. Indexing starts at 0. */
boost::optional<std::string> MeterCustomDecrement::outputVariableorMeterName(unsigned index) const {
  return getImpl<detail::MeterCustomDecrement_Impl>()->outputVariableorMeterName(index);
}
/** Set the Output Variable of Meter Name at index. Indexing starts at 0. */
bool MeterCustomDecrement::setOutputVariableorMeterName(unsigned index, const std::string& str) {
  return getImpl<detail::MeterCustomDecrement_Impl>()->setOutputVariableorMeterName(index, str);
}

/// @cond
MeterCustomDecrement::MeterCustomDecrement(std::shared_ptr<detail::MeterCustomDecrement_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

