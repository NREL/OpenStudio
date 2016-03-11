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

#include "AirTerminalSingleDuctVAVHeatAndCoolNoReheat.hpp"
#include "AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(const IdfObject& idfObject,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType());
  }

  AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType());
  }

  AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(const AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl& other,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::iddObjectType() const {
    return AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctVAVHeatAndCoolNoReheat","Availability Schedule"));
    }
    return result;
  }

  boost::optional<Schedule> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AvailabilitySchedule);
  }

  boost::optional<double> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::maximumAirFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate,true);
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::isMaximumAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::zoneMinimumAirFlowFraction() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::ZoneMinimumAirFlowFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AvailabilitySchedule,
                              "AirTerminalSingleDuctVAVHeatAndCoolNoReheat",
                              "Availability Schedule",
                              schedule);
    return result;
  }

  void AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AvailabilitySchedule, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate) {
    bool result(false);
    if (maximumAirFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate, maximumAirFlowRate.get());
    }
    return result;
  }

  void AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::autosizeMaximumAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::ZoneMinimumAirFlowFraction, zoneMinimumAirFlowFraction);
    return result;
  }

  unsigned AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::inletPort() 
  {
    return OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AirInlet;
  }

  unsigned AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::outletPort() 
  {
    return OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AirOutlet;
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::addToNode(Node & node)
  {
    Model _model = node.model();

    if( boost::optional<ModelObject> outlet = node.outletModelObject() )
    {
      boost::optional<ThermalZone> thermalZone;

      if( boost::optional<PortList> portList = outlet->optionalCast<PortList>()  )
      {
        thermalZone = portList->thermalZone();
      }

      if( thermalZone || (outlet->optionalCast<Mixer>() && node.airLoopHVAC()) )
      {
        if( boost::optional<ModelObject> inlet = node.inletModelObject() )
        {
          if( boost::optional<Splitter> splitter = inlet->optionalCast<Splitter>() )
          {
            boost::optional<ModelObject> sourceModelObject = inlet;
            boost::optional<unsigned> sourcePort = node.connectedObjectPort(node.inletPort());

            if( sourcePort && sourceModelObject )
            {
              Node inletNode(_model);

              _model.connect( sourceModelObject.get(),
                              sourcePort.get(),
                              inletNode,
                              inletNode.inletPort() );
              
              _model.connect( inletNode,
                              inletNode.outletPort(),
                              this->getObject<ModelObject>(),
                              this->inletPort() );

              _model.connect( this->getObject<ModelObject>(),
                              outletPort(),
                              node,
                              node.inletPort() );

              if( thermalZone )
              {
                AirTerminalSingleDuctVAVHeatAndCoolNoReheat mo = this->getObject<AirTerminalSingleDuctVAVHeatAndCoolNoReheat>();

                thermalZone->addEquipment(mo);
              }

              return true; 
            }
          }
        }
      }
    }

    return false;
  }

  std::vector<IdfObject> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::remove()
  {
    Model _model = this->model();
    ModelObject thisObject = this->getObject<ModelObject>();

    boost::optional<ModelObject> sourceModelObject = this->inletModelObject();
    boost::optional<unsigned> sourcePort = this->connectedObjectPort(this->inletPort());
    
    boost::optional<ModelObject> targetModelObject = this->outletModelObject();
    boost::optional<unsigned> targetPort = this->connectedObjectPort(this->outletPort());

    std::vector<ThermalZone> thermalZones = _model.getConcreteModelObjects<ThermalZone>();
    for( auto & thermalZone : thermalZones )
    {
      std::vector<ModelObject> equipment = thermalZone.equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
      {
        thermalZone.removeEquipment(thisObject);

        break;
      }
    }

    if( sourcePort && sourceModelObject
        && targetPort && targetModelObject )
    {
      if( boost::optional<Node> inletNode = sourceModelObject->optionalCast<Node>() )
      {
        if( boost::optional<ModelObject> source2ModelObject = inletNode->inletModelObject() )
        {
          if( boost::optional<unsigned> source2Port = inletNode->connectedObjectPort(inletNode->inletPort()) )
          {
            _model.connect( source2ModelObject.get(),
                            source2Port.get(),
                            targetModelObject.get(),
                            targetPort.get() );

            inletNode->disconnect();
            inletNode->remove();

            return StraightComponent_Impl::remove();
          }
        }
      }
    }

    model().disconnect(getObject<ModelObject>(),inletPort());
    model().disconnect(getObject<ModelObject>(),outletPort());

    return StraightComponent_Impl::remove();
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::isRemovable() const
  {
    return true;
  }

} // detail

AirTerminalSingleDuctVAVHeatAndCoolNoReheat::AirTerminalSingleDuctVAVHeatAndCoolNoReheat(const Model& model)
  : StraightComponent(AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>());

  autosizeMaximumAirFlowRate();
  setZoneMinimumAirFlowFraction(0.0);
}

IddObjectType AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat);
}

boost::optional<Schedule> AirTerminalSingleDuctVAVHeatAndCoolNoReheat::availabilitySchedule() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->availabilitySchedule();
}

boost::optional<double> AirTerminalSingleDuctVAVHeatAndCoolNoReheat::maximumAirFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->maximumAirFlowRate();
}

bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat::isMaximumAirFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->isMaximumAirFlowRateAutosized();
}

double AirTerminalSingleDuctVAVHeatAndCoolNoReheat::zoneMinimumAirFlowFraction() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->zoneMinimumAirFlowFraction();
}

bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->setAvailabilitySchedule(schedule);
}

void AirTerminalSingleDuctVAVHeatAndCoolNoReheat::resetAvailabilitySchedule() {
  getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->resetAvailabilitySchedule();
}

bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat::setMaximumAirFlowRate(double maximumAirFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
}

void AirTerminalSingleDuctVAVHeatAndCoolNoReheat::autosizeMaximumAirFlowRate() {
  getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->autosizeMaximumAirFlowRate();
}

bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat::setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction) {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->setZoneMinimumAirFlowFraction(zoneMinimumAirFlowFraction);
}

/// @cond
AirTerminalSingleDuctVAVHeatAndCoolNoReheat::AirTerminalSingleDuctVAVHeatAndCoolNoReheat(std::shared_ptr<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

