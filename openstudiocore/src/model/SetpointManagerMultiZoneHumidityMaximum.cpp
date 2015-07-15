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

#include <model/SetpointManagerMultiZoneHumidityMaximum.hpp>
#include <model/SetpointManagerMultiZoneHumidityMaximum_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_MultiZone_Humidity_Maximum_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerMultiZoneHumidityMaximum_Impl::SetpointManagerMultiZoneHumidityMaximum_Impl(const IdfObject& idfObject,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerMultiZoneHumidityMaximum::iddObjectType());
  }

  SetpointManagerMultiZoneHumidityMaximum_Impl::SetpointManagerMultiZoneHumidityMaximum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerMultiZoneHumidityMaximum::iddObjectType());
  }

  SetpointManagerMultiZoneHumidityMaximum_Impl::SetpointManagerMultiZoneHumidityMaximum_Impl(const SetpointManagerMultiZoneHumidityMaximum_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerMultiZoneHumidityMaximum_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerMultiZoneHumidityMaximum_Impl::iddObjectType() const {
    return SetpointManagerMultiZoneHumidityMaximum::iddObjectType();
  }

  std::string SetpointManagerMultiZoneHumidityMaximum_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_MultiZone_Humidity_MaximumFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerMultiZoneHumidityMaximum_Impl::minimumSetpointHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Humidity_MaximumFields::MinimumSetpointHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerMultiZoneHumidityMaximum_Impl::maximumSetpointHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Humidity_MaximumFields::MaximumSetpointHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Node> SetpointManagerMultiZoneHumidityMaximum_Impl::setpointNodeorNodeList() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MultiZone_Humidity_MaximumFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerMultiZoneHumidityMaximum_Impl::setControlVariable(std::string controlVariable) {
    bool result = setString(OS_SetpointManager_MultiZone_Humidity_MaximumFields::ControlVariable, controlVariable);
    return result;
  }

  bool SetpointManagerMultiZoneHumidityMaximum_Impl::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
    bool result = setDouble(OS_SetpointManager_MultiZone_Humidity_MaximumFields::MinimumSetpointHumidityRatio, minimumSetpointHumidityRatio);
    return result;
  }

  bool SetpointManagerMultiZoneHumidityMaximum_Impl::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
    bool result = setDouble(OS_SetpointManager_MultiZone_Humidity_MaximumFields::MaximumSetpointHumidityRatio, maximumSetpointHumidityRatio);
    return result;
  }

  bool SetpointManagerMultiZoneHumidityMaximum_Impl::setSetpointNodeorNodeList(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_SetpointManager_MultiZone_Humidity_MaximumFields::SetpointNodeorNodeListName, node.get().handle());
    }
    else {
      resetSetpointNodeorNodeList();
      result = true;
    }
    return result;
  }

  void SetpointManagerMultiZoneHumidityMaximum_Impl::resetSetpointNodeorNodeList() {
    bool result = setString(OS_SetpointManager_MultiZone_Humidity_MaximumFields::SetpointNodeorNodeListName, "");
    OS_ASSERT(result);
  }

} // detail

SetpointManagerMultiZoneHumidityMaximum::SetpointManagerMultiZoneHumidityMaximum(const Model& model)
  : SetpointManager(SetpointManagerMultiZoneHumidityMaximum::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setControlVariable();
  OS_ASSERT(ok);
  // ok = setMinimumSetpointHumidityRatio();
  OS_ASSERT(ok);
  // ok = setMaximumSetpointHumidityRatio();
  OS_ASSERT(ok);
}

IddObjectType SetpointManagerMultiZoneHumidityMaximum::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_MultiZone_Humidity_Maximum);
}

std::vector<std::string> SetpointManagerMultiZoneHumidityMaximum::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_MultiZone_Humidity_MaximumFields::ControlVariable);
}

std::string SetpointManagerMultiZoneHumidityMaximum::controlVariable() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->controlVariable();
}

double SetpointManagerMultiZoneHumidityMaximum::minimumSetpointHumidityRatio() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->minimumSetpointHumidityRatio();
}

double SetpointManagerMultiZoneHumidityMaximum::maximumSetpointHumidityRatio() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->maximumSetpointHumidityRatio();
}

boost::optional<Node> SetpointManagerMultiZoneHumidityMaximum::setpointNodeorNodeList() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->setpointNodeorNodeList();
}

bool SetpointManagerMultiZoneHumidityMaximum::setControlVariable(std::string controlVariable) {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->setControlVariable(controlVariable);
}

bool SetpointManagerMultiZoneHumidityMaximum::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->setMinimumSetpointHumidityRatio(minimumSetpointHumidityRatio);
}

bool SetpointManagerMultiZoneHumidityMaximum::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->setMaximumSetpointHumidityRatio(maximumSetpointHumidityRatio);
}

bool SetpointManagerMultiZoneHumidityMaximum::setSetpointNodeorNodeList(const Node& node) {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->setSetpointNodeorNodeList(node);
}

void SetpointManagerMultiZoneHumidityMaximum::resetSetpointNodeorNodeList() {
  getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->resetSetpointNodeorNodeList();
}

/// @cond
SetpointManagerMultiZoneHumidityMaximum::SetpointManagerMultiZoneHumidityMaximum(std::shared_ptr<detail::SetpointManagerMultiZoneHumidityMaximum_Impl> impl)
  : SetpointManager(impl)
{}
/// @endcond

} // model
} // openstudio

