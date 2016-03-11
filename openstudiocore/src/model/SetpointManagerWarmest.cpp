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

#include "SetpointManagerWarmest.hpp"
#include "SetpointManagerWarmest_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_Warmest_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

  SetpointManagerWarmest_Impl::SetpointManagerWarmest_Impl(const IdfObject& idfObject,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerWarmest::iddObjectType());
  }

  SetpointManagerWarmest_Impl::SetpointManagerWarmest_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerWarmest::iddObjectType());
  }

  SetpointManagerWarmest_Impl::SetpointManagerWarmest_Impl(const SetpointManagerWarmest_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerWarmest_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerWarmest_Impl::iddObjectType() const {
    return SetpointManagerWarmest::iddObjectType();
  }

  std::string SetpointManagerWarmest_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_WarmestFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerWarmest_Impl::minimumSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_WarmestFields::MinimumSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerWarmest_Impl::maximumSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_WarmestFields::MaximumSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string SetpointManagerWarmest_Impl::strategy() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_WarmestFields::Strategy,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Node> SetpointManagerWarmest_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_WarmestFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerWarmest_Impl::setControlVariable(const std::string& controlVariable) {
    bool result = setString(OS_SetpointManager_WarmestFields::ControlVariable, controlVariable);
    return result;
  }

  bool SetpointManagerWarmest_Impl::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_WarmestFields::MinimumSetpointTemperature, minimumSetpointTemperature);
    return result;
  }

  bool SetpointManagerWarmest_Impl::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_WarmestFields::MaximumSetpointTemperature, maximumSetpointTemperature);
    return result;
  }

  bool SetpointManagerWarmest_Impl::setStrategy(const std::string& strategy) {
    bool result = setString(OS_SetpointManager_WarmestFields::Strategy, strategy);
    return result;
  }

  bool SetpointManagerWarmest_Impl::setSetpointNode(const Node& node) {
    bool result = setPointer(OS_SetpointManager_WarmestFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  void SetpointManagerWarmest_Impl::resetSetpointNode()
  {
    bool result = setString(OS_SetpointManager_WarmestFields::SetpointNodeorNodeListName,"");
    OS_ASSERT(result);
  }

} // detail

SetpointManagerWarmest::SetpointManagerWarmest(const Model& model)
  : SetpointManager(SetpointManagerWarmest::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerWarmest_Impl>());

  setControlVariable("Temperature");
  setMinimumSetpointTemperature(12.2);
  setMaximumSetpointTemperature(15.6);
  setStrategy("MaximumTemperature");
}

IddObjectType SetpointManagerWarmest::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_Warmest);
}

std::vector<std::string> SetpointManagerWarmest::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_WarmestFields::ControlVariable);
}

std::vector<std::string> SetpointManagerWarmest::strategyValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_WarmestFields::Strategy);
}

std::string SetpointManagerWarmest::controlVariable() const {
  return getImpl<detail::SetpointManagerWarmest_Impl>()->controlVariable();
}

double SetpointManagerWarmest::minimumSetpointTemperature() const {
  return getImpl<detail::SetpointManagerWarmest_Impl>()->minimumSetpointTemperature();
}

double SetpointManagerWarmest::maximumSetpointTemperature() const {
  return getImpl<detail::SetpointManagerWarmest_Impl>()->maximumSetpointTemperature();
}

std::string SetpointManagerWarmest::strategy() const {
  return getImpl<detail::SetpointManagerWarmest_Impl>()->strategy();
}

boost::optional<Node> SetpointManagerWarmest::setpointNode() const {
  return getImpl<detail::SetpointManagerWarmest_Impl>()->setpointNode();
}

bool SetpointManagerWarmest::setControlVariable(const std::string& controlVariable) {
  return getImpl<detail::SetpointManagerWarmest_Impl>()->setControlVariable(controlVariable);
}

bool SetpointManagerWarmest::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
  return getImpl<detail::SetpointManagerWarmest_Impl>()->setMinimumSetpointTemperature(minimumSetpointTemperature);
}

bool SetpointManagerWarmest::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
  return getImpl<detail::SetpointManagerWarmest_Impl>()->setMaximumSetpointTemperature(maximumSetpointTemperature);
}

bool SetpointManagerWarmest::setStrategy(const std::string& strategy) {
  return getImpl<detail::SetpointManagerWarmest_Impl>()->setStrategy(strategy);
}

/// @cond
SetpointManagerWarmest::SetpointManagerWarmest(std::shared_ptr<detail::SetpointManagerWarmest_Impl> impl)
  : SetpointManager(impl)
{}
/// @endcond

} // model
} // openstudio

