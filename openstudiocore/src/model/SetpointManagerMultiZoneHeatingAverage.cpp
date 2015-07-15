/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include <model/SetpointManagerMultiZoneHeatingAverage.hpp>
#include <model/SetpointManagerMultiZoneHeatingAverage_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_MultiZone_Heating_Average_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerMultiZoneHeatingAverage_Impl::SetpointManagerMultiZoneHeatingAverage_Impl(const IdfObject& idfObject,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerMultiZoneHeatingAverage::iddObjectType());
  }

  SetpointManagerMultiZoneHeatingAverage_Impl::SetpointManagerMultiZoneHeatingAverage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerMultiZoneHeatingAverage::iddObjectType());
  }

  SetpointManagerMultiZoneHeatingAverage_Impl::SetpointManagerMultiZoneHeatingAverage_Impl(const SetpointManagerMultiZoneHeatingAverage_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerMultiZoneHeatingAverage_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerMultiZoneHeatingAverage_Impl::iddObjectType() const {
    return SetpointManagerMultiZoneHeatingAverage::iddObjectType();
  }

  std::string SetpointManagerMultiZoneHeatingAverage_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_MultiZone_Heating_AverageFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerMultiZoneHeatingAverage_Impl::minimumSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Heating_AverageFields::MinimumSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerMultiZoneHeatingAverage_Impl::maximumSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Heating_AverageFields::MaximumSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Node> SetpointManagerMultiZoneHeatingAverage_Impl::setpointNodeorNodeList() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MultiZone_Heating_AverageFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerMultiZoneHeatingAverage_Impl::setControlVariable(std::string controlVariable) {
    bool result = setString(OS_SetpointManager_MultiZone_Heating_AverageFields::ControlVariable, controlVariable);
    return result;
  }

  bool SetpointManagerMultiZoneHeatingAverage_Impl::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_MultiZone_Heating_AverageFields::MinimumSetpointTemperature, minimumSetpointTemperature);
    return result;
  }

  bool SetpointManagerMultiZoneHeatingAverage_Impl::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_MultiZone_Heating_AverageFields::MaximumSetpointTemperature, maximumSetpointTemperature);
    return result;
  }

  bool SetpointManagerMultiZoneHeatingAverage_Impl::setSetpointNodeorNodeList(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_SetpointManager_MultiZone_Heating_AverageFields::SetpointNodeorNodeListName, node.get().handle());
    }
    else {
      resetSetpointNodeorNodeList();
      result = true;
    }
    return result;
  }

  void SetpointManagerMultiZoneHeatingAverage_Impl::resetSetpointNodeorNodeList() {
    bool result = setString(OS_SetpointManager_MultiZone_Heating_AverageFields::SetpointNodeorNodeListName, "");
    OS_ASSERT(result);
  }

} // detail

SetpointManagerMultiZoneHeatingAverage::SetpointManagerMultiZoneHeatingAverage(const Model& model)
  : SetpointManager(SetpointManagerMultiZoneHeatingAverage::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setControlVariable();
  OS_ASSERT(ok);
  // ok = setMinimumSetpointTemperature();
  OS_ASSERT(ok);
  // ok = setMaximumSetpointTemperature();
  OS_ASSERT(ok);
}

IddObjectType SetpointManagerMultiZoneHeatingAverage::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_MultiZone_Heating_Average);
}

std::vector<std::string> SetpointManagerMultiZoneHeatingAverage::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_MultiZone_Heating_AverageFields::ControlVariable);
}

std::string SetpointManagerMultiZoneHeatingAverage::controlVariable() const {
  return getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>()->controlVariable();
}

double SetpointManagerMultiZoneHeatingAverage::minimumSetpointTemperature() const {
  return getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>()->minimumSetpointTemperature();
}

double SetpointManagerMultiZoneHeatingAverage::maximumSetpointTemperature() const {
  return getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>()->maximumSetpointTemperature();
}

boost::optional<Node> SetpointManagerMultiZoneHeatingAverage::setpointNodeorNodeList() const {
  return getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>()->setpointNodeorNodeList();
}

bool SetpointManagerMultiZoneHeatingAverage::setControlVariable(std::string controlVariable) {
  return getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>()->setControlVariable(controlVariable);
}

bool SetpointManagerMultiZoneHeatingAverage::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
  return getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>()->setMinimumSetpointTemperature(minimumSetpointTemperature);
}

bool SetpointManagerMultiZoneHeatingAverage::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
  return getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>()->setMaximumSetpointTemperature(maximumSetpointTemperature);
}

bool SetpointManagerMultiZoneHeatingAverage::setSetpointNodeorNodeList(const Node& node) {
  return getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>()->setSetpointNodeorNodeList(node);
}

void SetpointManagerMultiZoneHeatingAverage::resetSetpointNodeorNodeList() {
  getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>()->resetSetpointNodeorNodeList();
}

/// @cond
SetpointManagerMultiZoneHeatingAverage::SetpointManagerMultiZoneHeatingAverage(std::shared_ptr<detail::SetpointManagerMultiZoneHeatingAverage_Impl> impl)
  : SetpointManager(impl)
{}
/// @endcond

} // model
} // openstudio

