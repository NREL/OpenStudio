/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <model/SetpointManagerMultiZoneHumidityMinimum.hpp>
#include <model/SetpointManagerMultiZoneHumidityMinimum_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_MultiZone_Humidity_Minimum_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerMultiZoneHumidityMinimum_Impl::SetpointManagerMultiZoneHumidityMinimum_Impl(const IdfObject& idfObject,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerMultiZoneHumidityMinimum::iddObjectType());
  }

  SetpointManagerMultiZoneHumidityMinimum_Impl::SetpointManagerMultiZoneHumidityMinimum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerMultiZoneHumidityMinimum::iddObjectType());
  }

  SetpointManagerMultiZoneHumidityMinimum_Impl::SetpointManagerMultiZoneHumidityMinimum_Impl(const SetpointManagerMultiZoneHumidityMinimum_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerMultiZoneHumidityMinimum_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerMultiZoneHumidityMinimum_Impl::iddObjectType() const {
    return SetpointManagerMultiZoneHumidityMinimum::iddObjectType();
  }

  double SetpointManagerMultiZoneHumidityMinimum_Impl::minimumSetpointHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MinimumSetpointHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerMultiZoneHumidityMinimum_Impl::isMinimumSetpointHumidityRatioDefaulted() const {
    return isEmpty(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MinimumSetpointHumidityRatio);
  }

  double SetpointManagerMultiZoneHumidityMinimum_Impl::maximumSetpointHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MaximumSetpointHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerMultiZoneHumidityMinimum_Impl::isMaximumSetpointHumidityRatioDefaulted() const {
    return isEmpty(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MaximumSetpointHumidityRatio);
  }

  Node SetpointManagerMultiZoneHumidityMinimum_Impl::setpointNodeorNodeList() const {
    boost::optional<Node> value = optionalSetpointNodeorNodeList();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Setpoint Nodeor Node List attached.");
    }
    return value.get();
  }

  std::string SetpointManagerMultiZoneHumidityMinimum_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerMultiZoneHumidityMinimum_Impl::isControlVariableDefaulted() const {
    return isEmpty(OS_SetpointManager_MultiZone_Humidity_MinimumFields::ControlVariable);
  }

  bool SetpointManagerMultiZoneHumidityMinimum_Impl::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
    bool result = setDouble(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MinimumSetpointHumidityRatio, minimumSetpointHumidityRatio);
    return result;
  }

  void SetpointManagerMultiZoneHumidityMinimum_Impl::resetMinimumSetpointHumidityRatio() {
    bool result = setString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MinimumSetpointHumidityRatio, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerMultiZoneHumidityMinimum_Impl::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
    bool result = setDouble(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MaximumSetpointHumidityRatio, maximumSetpointHumidityRatio);
    return result;
  }

  void SetpointManagerMultiZoneHumidityMinimum_Impl::resetMaximumSetpointHumidityRatio() {
    bool result = setString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MaximumSetpointHumidityRatio, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerMultiZoneHumidityMinimum_Impl::setSetpointNodeorNodeList(const Node& node) {
    bool result = setPointer(OS_SetpointManager_MultiZone_Humidity_MinimumFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  bool SetpointManagerMultiZoneHumidityMinimum_Impl::setControlVariable(std::string controlVariable) {
    bool result = setString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::ControlVariable, controlVariable);
    return result;
  }

  void SetpointManagerMultiZoneHumidityMinimum_Impl::resetControlVariable() {
    bool result = setString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::ControlVariable, "");
    OS_ASSERT(result);
  }

  boost::optional<Node> SetpointManagerMultiZoneHumidityMinimum_Impl::optionalSetpointNodeorNodeList() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MultiZone_Humidity_MinimumFields::SetpointNodeorNodeListName);
  }

} // detail

SetpointManagerMultiZoneHumidityMinimum::SetpointManagerMultiZoneHumidityMinimum(const Model& model)
  : SetpointManager(SetpointManagerMultiZoneHumidityMinimum::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_SetpointManager_MultiZone_Humidity_MinimumFields::SetpointNodeorNodeListName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setSetpointNodeorNodeList();
  OS_ASSERT(ok);
}

IddObjectType SetpointManagerMultiZoneHumidityMinimum::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_MultiZone_Humidity_Minimum);
}

std::vector<std::string> SetpointManagerMultiZoneHumidityMinimum::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_MultiZone_Humidity_MinimumFields::ControlVariable);
}

double SetpointManagerMultiZoneHumidityMinimum::minimumSetpointHumidityRatio() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->minimumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneHumidityMinimum::isMinimumSetpointHumidityRatioDefaulted() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->isMinimumSetpointHumidityRatioDefaulted();
}

double SetpointManagerMultiZoneHumidityMinimum::maximumSetpointHumidityRatio() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->maximumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneHumidityMinimum::isMaximumSetpointHumidityRatioDefaulted() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->isMaximumSetpointHumidityRatioDefaulted();
}

Node SetpointManagerMultiZoneHumidityMinimum::setpointNodeorNodeList() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->setpointNodeorNodeList();
}

std::string SetpointManagerMultiZoneHumidityMinimum::controlVariable() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->controlVariable();
}

bool SetpointManagerMultiZoneHumidityMinimum::isControlVariableDefaulted() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->isControlVariableDefaulted();
}

bool SetpointManagerMultiZoneHumidityMinimum::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->setMinimumSetpointHumidityRatio(minimumSetpointHumidityRatio);
}

void SetpointManagerMultiZoneHumidityMinimum::resetMinimumSetpointHumidityRatio() {
  getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->resetMinimumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneHumidityMinimum::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->setMaximumSetpointHumidityRatio(maximumSetpointHumidityRatio);
}

void SetpointManagerMultiZoneHumidityMinimum::resetMaximumSetpointHumidityRatio() {
  getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->resetMaximumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneHumidityMinimum::setSetpointNodeorNodeList(const Node& node) {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->setSetpointNodeorNodeList(node);
}

bool SetpointManagerMultiZoneHumidityMinimum::setControlVariable(std::string controlVariable) {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->setControlVariable(controlVariable);
}

void SetpointManagerMultiZoneHumidityMinimum::resetControlVariable() {
  getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->resetControlVariable();
}

/// @cond
SetpointManagerMultiZoneHumidityMinimum::SetpointManagerMultiZoneHumidityMinimum(boost::shared_ptr<detail::SetpointManagerMultiZoneHumidityMinimum_Impl> impl)
  : SetpointManager(impl)
{}
/// @endcond

} // model
} // openstudio

