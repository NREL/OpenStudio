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

#include "SetpointManagerColdest.hpp"
#include "SetpointManagerColdest_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_Coldest_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerColdest_Impl::SetpointManagerColdest_Impl(const IdfObject& idfObject,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerColdest::iddObjectType());
  }

  SetpointManagerColdest_Impl::SetpointManagerColdest_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerColdest::iddObjectType());
  }

  SetpointManagerColdest_Impl::SetpointManagerColdest_Impl(const SetpointManagerColdest_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerColdest_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerColdest_Impl::iddObjectType() const {
    return SetpointManagerColdest::iddObjectType();
  }

  std::string SetpointManagerColdest_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_ColdestFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerColdest_Impl::minimumSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_ColdestFields::MinimumSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerColdest_Impl::maximumSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_ColdestFields::MaximumSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string SetpointManagerColdest_Impl::strategy() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_ColdestFields::Strategy,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Node> SetpointManagerColdest_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_ColdestFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerColdest_Impl::setControlVariable(const std::string& controlVariable) {
    bool result = setString(OS_SetpointManager_ColdestFields::ControlVariable, controlVariable);
    return result;
  }

  bool SetpointManagerColdest_Impl::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_ColdestFields::MinimumSetpointTemperature, minimumSetpointTemperature);
    return result;
  }

  bool SetpointManagerColdest_Impl::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_ColdestFields::MaximumSetpointTemperature, maximumSetpointTemperature);
    return result;
  }

  bool SetpointManagerColdest_Impl::setStrategy(const std::string& strategy) {
    bool result = setString(OS_SetpointManager_ColdestFields::Strategy, strategy);
    return result;
  }

  bool SetpointManagerColdest_Impl::setSetpointNode(const Node& node) {
    bool result = setPointer(OS_SetpointManager_ColdestFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  void SetpointManagerColdest_Impl::resetSetpointNode()
  {
    bool result = setString(OS_SetpointManager_ColdestFields::SetpointNodeorNodeListName,"");
    OS_ASSERT(result);
  }

} // detail

SetpointManagerColdest::SetpointManagerColdest(const Model& model)
  : SetpointManager(SetpointManagerColdest::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerColdest_Impl>());

  setControlVariable("Temperature");
  setMinimumSetpointTemperature(20.);
  setMaximumSetpointTemperature(50.);
  setStrategy("MinimumTemperature");
}

IddObjectType SetpointManagerColdest::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_Coldest);
}

std::vector<std::string> SetpointManagerColdest::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_ColdestFields::ControlVariable);
}

std::vector<std::string> SetpointManagerColdest::strategyValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_ColdestFields::Strategy);
}

std::string SetpointManagerColdest::controlVariable() const {
  return getImpl<detail::SetpointManagerColdest_Impl>()->controlVariable();
}

double SetpointManagerColdest::minimumSetpointTemperature() const {
  return getImpl<detail::SetpointManagerColdest_Impl>()->minimumSetpointTemperature();
}

double SetpointManagerColdest::maximumSetpointTemperature() const {
  return getImpl<detail::SetpointManagerColdest_Impl>()->maximumSetpointTemperature();
}

std::string SetpointManagerColdest::strategy() const {
  return getImpl<detail::SetpointManagerColdest_Impl>()->strategy();
}

boost::optional<Node> SetpointManagerColdest::setpointNode() const {
  return getImpl<detail::SetpointManagerColdest_Impl>()->setpointNode();
}

bool SetpointManagerColdest::setControlVariable(const std::string& controlVariable) {
  return getImpl<detail::SetpointManagerColdest_Impl>()->setControlVariable(controlVariable);
}

bool SetpointManagerColdest::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
  return getImpl<detail::SetpointManagerColdest_Impl>()->setMinimumSetpointTemperature(minimumSetpointTemperature);
}

bool SetpointManagerColdest::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
  return getImpl<detail::SetpointManagerColdest_Impl>()->setMaximumSetpointTemperature(maximumSetpointTemperature);
}

bool SetpointManagerColdest::setStrategy(const std::string& strategy) {
  return getImpl<detail::SetpointManagerColdest_Impl>()->setStrategy(strategy);
}

/// @cond
SetpointManagerColdest::SetpointManagerColdest(std::shared_ptr<detail::SetpointManagerColdest_Impl> impl)
  : SetpointManager(impl)
{}
/// @endcond

} // model
} // openstudio

