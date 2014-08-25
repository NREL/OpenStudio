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

#include <model/SetpointManagerMultiZoneMinimumHumidityAverage.hpp>
#include <model/SetpointManagerMultiZoneMinimumHumidityAverage_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_MultiZone_MinimumHumidity_Average_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerMultiZoneMinimumHumidityAverage_Impl::SetpointManagerMultiZoneMinimumHumidityAverage_Impl(const IdfObject& idfObject,
                                                                                                           Model_Impl* model,
                                                                                                           bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType());
  }

  SetpointManagerMultiZoneMinimumHumidityAverage_Impl::SetpointManagerMultiZoneMinimumHumidityAverage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                           Model_Impl* model,
                                                                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType());
  }

  SetpointManagerMultiZoneMinimumHumidityAverage_Impl::SetpointManagerMultiZoneMinimumHumidityAverage_Impl(const SetpointManagerMultiZoneMinimumHumidityAverage_Impl& other,
                                                                                                           Model_Impl* model,
                                                                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerMultiZoneMinimumHumidityAverage_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerMultiZoneMinimumHumidityAverage_Impl::iddObjectType() const {
    return SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType();
  }

  double SetpointManagerMultiZoneMinimumHumidityAverage_Impl::minimumSetpointHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MinimumSetpointHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::isMinimumSetpointHumidityRatioDefaulted() const {
    return isEmpty(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MinimumSetpointHumidityRatio);
  }

  double SetpointManagerMultiZoneMinimumHumidityAverage_Impl::maximumSetpointHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MaximumSetpointHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::isMaximumSetpointHumidityRatioDefaulted() const {
    return isEmpty(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MaximumSetpointHumidityRatio);
  }

  Node SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setpointNodeorNodeList() const {
    boost::optional<Node> value = optionalSetpointNodeorNodeList();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Setpoint Nodeor Node List attached.");
    }
    return value.get();
  }

  std::string SetpointManagerMultiZoneMinimumHumidityAverage_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::isControlVariableDefaulted() const {
    return isEmpty(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::ControlVariable);
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
    bool result = setDouble(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MinimumSetpointHumidityRatio, minimumSetpointHumidityRatio);
    return result;
  }

  void SetpointManagerMultiZoneMinimumHumidityAverage_Impl::resetMinimumSetpointHumidityRatio() {
    bool result = setString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MinimumSetpointHumidityRatio, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
    bool result = setDouble(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MaximumSetpointHumidityRatio, maximumSetpointHumidityRatio);
    return result;
  }

  void SetpointManagerMultiZoneMinimumHumidityAverage_Impl::resetMaximumSetpointHumidityRatio() {
    bool result = setString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MaximumSetpointHumidityRatio, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setSetpointNodeorNodeList(const Node& node) {
    bool result = setPointer(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setControlVariable(std::string controlVariable) {
    bool result = setString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::ControlVariable, controlVariable);
    return result;
  }

  void SetpointManagerMultiZoneMinimumHumidityAverage_Impl::resetControlVariable() {
    bool result = setString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::ControlVariable, "");
    OS_ASSERT(result);
  }

  boost::optional<Node> SetpointManagerMultiZoneMinimumHumidityAverage_Impl::optionalSetpointNodeorNodeList() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::SetpointNodeorNodeListName);
  }

} // detail

SetpointManagerMultiZoneMinimumHumidityAverage::SetpointManagerMultiZoneMinimumHumidityAverage(const Model& model)
  : SetpointManager(SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::SetpointNodeorNodeListName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setSetpointNodeorNodeList();
  OS_ASSERT(ok);
}

IddObjectType SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_MultiZone_MinimumHumidity_Average);
}

std::vector<std::string> SetpointManagerMultiZoneMinimumHumidityAverage::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::ControlVariable);
}

double SetpointManagerMultiZoneMinimumHumidityAverage::minimumSetpointHumidityRatio() const {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->minimumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneMinimumHumidityAverage::isMinimumSetpointHumidityRatioDefaulted() const {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->isMinimumSetpointHumidityRatioDefaulted();
}

double SetpointManagerMultiZoneMinimumHumidityAverage::maximumSetpointHumidityRatio() const {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->maximumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneMinimumHumidityAverage::isMaximumSetpointHumidityRatioDefaulted() const {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->isMaximumSetpointHumidityRatioDefaulted();
}

Node SetpointManagerMultiZoneMinimumHumidityAverage::setpointNodeorNodeList() const {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->setpointNodeorNodeList();
}

std::string SetpointManagerMultiZoneMinimumHumidityAverage::controlVariable() const {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->controlVariable();
}

bool SetpointManagerMultiZoneMinimumHumidityAverage::isControlVariableDefaulted() const {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->isControlVariableDefaulted();
}

bool SetpointManagerMultiZoneMinimumHumidityAverage::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->setMinimumSetpointHumidityRatio(minimumSetpointHumidityRatio);
}

void SetpointManagerMultiZoneMinimumHumidityAverage::resetMinimumSetpointHumidityRatio() {
  getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->resetMinimumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneMinimumHumidityAverage::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->setMaximumSetpointHumidityRatio(maximumSetpointHumidityRatio);
}

void SetpointManagerMultiZoneMinimumHumidityAverage::resetMaximumSetpointHumidityRatio() {
  getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->resetMaximumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneMinimumHumidityAverage::setSetpointNodeorNodeList(const Node& node) {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->setSetpointNodeorNodeList(node);
}

bool SetpointManagerMultiZoneMinimumHumidityAverage::setControlVariable(std::string controlVariable) {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->setControlVariable(controlVariable);
}

void SetpointManagerMultiZoneMinimumHumidityAverage::resetControlVariable() {
  getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->resetControlVariable();
}

/// @cond
SetpointManagerMultiZoneMinimumHumidityAverage::SetpointManagerMultiZoneMinimumHumidityAverage(boost::shared_ptr<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl> impl)
  : SetpointManager(impl)
{}
/// @endcond

} // model
} // openstudio

