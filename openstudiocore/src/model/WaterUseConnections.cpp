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

#include "Model.hpp"
#include "WaterUseConnections.hpp"
#include "WaterUseConnections_Impl.hpp"
#include "WaterUseEquipment.hpp"
#include "WaterUseEquipment_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_WaterUse_Connections_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {

namespace model {

namespace detail {

  WaterUseConnections_Impl::WaterUseConnections_Impl(const IdfObject& idfObject,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == WaterUseConnections::iddObjectType());
  }

  WaterUseConnections_Impl::WaterUseConnections_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == WaterUseConnections::iddObjectType());
  }

  WaterUseConnections_Impl::WaterUseConnections_Impl(const WaterUseConnections_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& WaterUseConnections_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  std::vector<ScheduleTypeKey> WaterUseConnections_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_WaterUse_ConnectionsFields::HotWaterSupplyTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterUseConnections","Hot Water Supply Temperature"));
    }
    if (std::find(b,e,OS_WaterUse_ConnectionsFields::ColdWaterSupplyTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterUseConnections","Cold Water Supply Temperature"));
    }
    return result;
  }

  IddObjectType WaterUseConnections_Impl::iddObjectType() const {
    return WaterUseConnections::iddObjectType();
  }

  boost::optional<Schedule> WaterUseConnections_Impl::hotWaterSupplyTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterUse_ConnectionsFields::HotWaterSupplyTemperatureScheduleName);
  }

  boost::optional<Schedule> WaterUseConnections_Impl::coldWaterSupplyTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterUse_ConnectionsFields::ColdWaterSupplyTemperatureScheduleName);
  }

  bool WaterUseConnections_Impl::setHotWaterSupplyTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterUse_ConnectionsFields::HotWaterSupplyTemperatureScheduleName,
                              "WaterUseConnections",
                              "Hot Water Supply Temperature",
                              schedule);
    return result;
  }

  void WaterUseConnections_Impl::resetHotWaterSupplyTemperatureSchedule() {
    bool result = setString(OS_WaterUse_ConnectionsFields::HotWaterSupplyTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterUseConnections_Impl::setColdWaterSupplyTemperatureSchedule(Schedule & schedule) {
    bool result = setSchedule(OS_WaterUse_ConnectionsFields::ColdWaterSupplyTemperatureScheduleName,
                              "WaterUseConnections",
                              "Cold Water Supply Temperature",
                              schedule);
    return result;
  }

  void WaterUseConnections_Impl::resetColdWaterSupplyTemperatureSchedule() {
    bool result = setString(OS_WaterUse_ConnectionsFields::ColdWaterSupplyTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<ModelObject> WaterUseConnections_Impl::hotWaterSupplyTemperatureScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = hotWaterSupplyTemperatureSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> WaterUseConnections_Impl::coldWaterSupplyTemperatureScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = coldWaterSupplyTemperatureSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool WaterUseConnections_Impl::setHotWaterSupplyTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        return setHotWaterSupplyTemperatureSchedule(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetHotWaterSupplyTemperatureSchedule();
    }
    return true;
  }

  bool WaterUseConnections_Impl::setColdWaterSupplyTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        return setColdWaterSupplyTemperatureSchedule(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetColdWaterSupplyTemperatureSchedule();
    }
    return true;
  }

  unsigned WaterUseConnections_Impl::inletPort()
  {
    return OS_WaterUse_ConnectionsFields::InletNodeName;
  }
  
  unsigned WaterUseConnections_Impl::outletPort()
  {
    return OS_WaterUse_ConnectionsFields::OutletNodeName;
  }

  std::vector<WaterUseEquipment> WaterUseConnections_Impl::waterUseEquipment() const
  {
    std::vector<WaterUseEquipment> result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for( const auto & elem : groups )
    {
      WorkspaceExtensibleGroup group = elem.cast<WorkspaceExtensibleGroup>();

      boost::optional<WorkspaceObject> wo = group.getTarget(OS_WaterUse_ConnectionsExtensibleFields::WaterUseEquipmentName);
       
      if( wo )
      {
        WaterUseEquipment equipment = wo->cast<WaterUseEquipment>();

        result.push_back(equipment);
      }
    }

    return result;
  }
  
  bool WaterUseConnections_Impl::addWaterUseEquipment(const WaterUseEquipment & waterUseEquipment)
  {
    bool result = false;
  
    if( waterUseEquipment.model() == model() )
    {
      WorkspaceExtensibleGroup group = getObject<WaterUseConnections>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();

      group.setPointer(OS_WaterUse_ConnectionsExtensibleFields::WaterUseEquipmentName,waterUseEquipment.handle());

      result = true;
    }

    return result;
  }
  
  bool WaterUseConnections_Impl::removeWaterUseEquipment(WaterUseEquipment & _waterUseEquipment)
  {
    std::vector<WaterUseEquipment> equipment = waterUseEquipment();

    int i = 0;

    for( const auto & elem : equipment )
    {
      if( elem.handle() == _waterUseEquipment.handle() )
      {
        eraseExtensibleGroup(i);

        return true;
      }

      i++;
    }

    return false;
  }

  bool WaterUseConnections_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      if( plant->demandComponent(node.handle()) )
      {
        return StraightComponent_Impl::addToNode(node);
      }
    }

    return false;
  }

} // detail

WaterUseConnections::WaterUseConnections(const Model& model)
  : StraightComponent(WaterUseConnections::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::WaterUseConnections_Impl>());
}

IddObjectType WaterUseConnections::iddObjectType() {
  static IddObjectType result(IddObjectType::OS_WaterUse_Connections);
  return result;
}

boost::optional<Schedule> WaterUseConnections::hotWaterSupplyTemperatureSchedule() const {
  return getImpl<detail::WaterUseConnections_Impl>()->hotWaterSupplyTemperatureSchedule();
}

boost::optional<Schedule> WaterUseConnections::coldWaterSupplyTemperatureSchedule() const {
  return getImpl<detail::WaterUseConnections_Impl>()->coldWaterSupplyTemperatureSchedule();
}

bool WaterUseConnections::setHotWaterSupplyTemperatureSchedule(Schedule& hotWaterSupplyTemperatureSchedule) {
  return getImpl<detail::WaterUseConnections_Impl>()->setHotWaterSupplyTemperatureSchedule(hotWaterSupplyTemperatureSchedule);
}

void WaterUseConnections::resetHotWaterSupplyTemperatureSchedule() {
  getImpl<detail::WaterUseConnections_Impl>()->resetHotWaterSupplyTemperatureSchedule();
}

bool WaterUseConnections::setColdWaterSupplyTemperatureSchedule(Schedule& coldWaterSupplyTemperatureSchedule) {
  return getImpl<detail::WaterUseConnections_Impl>()->setColdWaterSupplyTemperatureSchedule(coldWaterSupplyTemperatureSchedule);
}

void WaterUseConnections::resetColdWaterSupplyTemperatureSchedule() {
  getImpl<detail::WaterUseConnections_Impl>()->resetColdWaterSupplyTemperatureSchedule();
}

unsigned WaterUseConnections::inletPort()
{
  return getImpl<detail::WaterUseConnections_Impl>()->inletPort();
}

unsigned WaterUseConnections::outletPort()
{
  return getImpl<detail::WaterUseConnections_Impl>()->outletPort();
}

std::vector<WaterUseEquipment> WaterUseConnections::waterUseEquipment() const
{
  return getImpl<detail::WaterUseConnections_Impl>()->waterUseEquipment();
}

bool WaterUseConnections::addWaterUseEquipment(const WaterUseEquipment & waterUseEquipment)
{
  return getImpl<detail::WaterUseConnections_Impl>()->addWaterUseEquipment(waterUseEquipment);
}

bool WaterUseConnections::removeWaterUseEquipment(WaterUseEquipment & waterUseEquipment)
{
  return getImpl<detail::WaterUseConnections_Impl>()->removeWaterUseEquipment(waterUseEquipment);
}

/// @cond
WaterUseConnections::WaterUseConnections(std::shared_ptr<detail::WaterUseConnections_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

