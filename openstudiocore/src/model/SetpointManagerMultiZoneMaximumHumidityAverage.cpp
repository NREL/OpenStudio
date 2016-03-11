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

#include "SetpointManagerMultiZoneMaximumHumidityAverage.hpp"
#include "SetpointManagerMultiZoneMaximumHumidityAverage_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_MultiZone_MaximumHumidity_Average_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerMultiZoneMaximumHumidityAverage_Impl::SetpointManagerMultiZoneMaximumHumidityAverage_Impl(const IdfObject& idfObject,
                                                                                                           Model_Impl* model,
                                                                                                           bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerMultiZoneMaximumHumidityAverage::iddObjectType());
  }

  SetpointManagerMultiZoneMaximumHumidityAverage_Impl::SetpointManagerMultiZoneMaximumHumidityAverage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                           Model_Impl* model,
                                                                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerMultiZoneMaximumHumidityAverage::iddObjectType());
  }

  SetpointManagerMultiZoneMaximumHumidityAverage_Impl::SetpointManagerMultiZoneMaximumHumidityAverage_Impl(const SetpointManagerMultiZoneMaximumHumidityAverage_Impl& other,
                                                                                                           Model_Impl* model,
                                                                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerMultiZoneMaximumHumidityAverage_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerMultiZoneMaximumHumidityAverage_Impl::iddObjectType() const {
    return SetpointManagerMultiZoneMaximumHumidityAverage::iddObjectType();
  }

  std::string SetpointManagerMultiZoneMaximumHumidityAverage_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_MultiZone_MaximumHumidity_AverageFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerMultiZoneMaximumHumidityAverage_Impl::minimumSetpointHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_MaximumHumidity_AverageFields::MinimumSetpointHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerMultiZoneMaximumHumidityAverage_Impl::maximumSetpointHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_MaximumHumidity_AverageFields::MaximumSetpointHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Node> SetpointManagerMultiZoneMaximumHumidityAverage_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MultiZone_MaximumHumidity_AverageFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerMultiZoneMaximumHumidityAverage_Impl::setControlVariable(const std::string& controlVariable) {
    bool result = setString(OS_SetpointManager_MultiZone_MaximumHumidity_AverageFields::ControlVariable, controlVariable);
    return result;
  }

  bool SetpointManagerMultiZoneMaximumHumidityAverage_Impl::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
    bool result = setDouble(OS_SetpointManager_MultiZone_MaximumHumidity_AverageFields::MinimumSetpointHumidityRatio, minimumSetpointHumidityRatio);
    return result;
  }

  bool SetpointManagerMultiZoneMaximumHumidityAverage_Impl::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
    bool result = setDouble(OS_SetpointManager_MultiZone_MaximumHumidity_AverageFields::MaximumSetpointHumidityRatio, maximumSetpointHumidityRatio);
    return result;
  }

  bool SetpointManagerMultiZoneMaximumHumidityAverage_Impl::setSetpointNode(const Node& node) {
    bool result = setPointer(OS_SetpointManager_MultiZone_MaximumHumidity_AverageFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  void SetpointManagerMultiZoneMaximumHumidityAverage_Impl::resetSetpointNode() {
    bool result = setString(OS_SetpointManager_MultiZone_MaximumHumidity_AverageFields::SetpointNodeorNodeListName, "");
    OS_ASSERT(result);
  }

} // detail

SetpointManagerMultiZoneMaximumHumidityAverage::SetpointManagerMultiZoneMaximumHumidityAverage(const Model& model)
  : SetpointManager(SetpointManagerMultiZoneMaximumHumidityAverage::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl>());

  setControlVariable("MaximumHumidityRatio");
  setMinimumSetpointHumidityRatio(0.008);
  setMaximumSetpointHumidityRatio(0.015);
}

IddObjectType SetpointManagerMultiZoneMaximumHumidityAverage::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_MultiZone_MaximumHumidity_Average);
}

std::vector<std::string> SetpointManagerMultiZoneMaximumHumidityAverage::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_MultiZone_MaximumHumidity_AverageFields::ControlVariable);
}

std::string SetpointManagerMultiZoneMaximumHumidityAverage::controlVariable() const {
  return getImpl<detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl>()->controlVariable();
}

double SetpointManagerMultiZoneMaximumHumidityAverage::minimumSetpointHumidityRatio() const {
  return getImpl<detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl>()->minimumSetpointHumidityRatio();
}

double SetpointManagerMultiZoneMaximumHumidityAverage::maximumSetpointHumidityRatio() const {
  return getImpl<detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl>()->maximumSetpointHumidityRatio();
}

boost::optional<Node> SetpointManagerMultiZoneMaximumHumidityAverage::setpointNode() const {
  return getImpl<detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl>()->setpointNode();
}

bool SetpointManagerMultiZoneMaximumHumidityAverage::setControlVariable(const std::string& controlVariable) {
  return getImpl<detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl>()->setControlVariable(controlVariable);
}

bool SetpointManagerMultiZoneMaximumHumidityAverage::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
  return getImpl<detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl>()->setMinimumSetpointHumidityRatio(minimumSetpointHumidityRatio);
}

bool SetpointManagerMultiZoneMaximumHumidityAverage::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
  return getImpl<detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl>()->setMaximumSetpointHumidityRatio(maximumSetpointHumidityRatio);
}

/// @cond
SetpointManagerMultiZoneMaximumHumidityAverage::SetpointManagerMultiZoneMaximumHumidityAverage(std::shared_ptr<detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl> impl)
  : SetpointManager(impl)
{}
/// @endcond

} // model
} // openstudio

