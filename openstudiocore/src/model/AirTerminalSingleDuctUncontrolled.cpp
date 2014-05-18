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

#include <model/AirTerminalSingleDuctUncontrolled.hpp>
#include <model/AirTerminalSingleDuctUncontrolled_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/AirLoopHVACZoneMixer.hpp>
#include <model/AirLoopHVACZoneMixer_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/PortList.hpp>
#include <model/PortList_Impl.hpp>
#include <model/Model.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <utilities/idd/OS_AirTerminal_SingleDuct_Uncontrolled_FieldEnums.hxx>
#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail{

  AirTerminalSingleDuctUncontrolled_Impl::AirTerminalSingleDuctUncontrolled_Impl(
      const IdfObject& idfObject,
      Model_Impl* model,
      bool keepHandle)
        : StraightComponent_Impl(idfObject, model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctUncontrolled::iddObjectType());
  }

  AirTerminalSingleDuctUncontrolled_Impl::AirTerminalSingleDuctUncontrolled_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
        : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctUncontrolled::iddObjectType());
  }

  AirTerminalSingleDuctUncontrolled_Impl::AirTerminalSingleDuctUncontrolled_Impl(
      const AirTerminalSingleDuctUncontrolled_Impl& other,
      Model_Impl* model,
      bool keepHandle)
        : StraightComponent_Impl(other,model,keepHandle)
  {}

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& AirTerminalSingleDuctUncontrolled_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirTerminalSingleDuctUncontrolled_Impl::iddObjectType() const {
    return AirTerminalSingleDuctUncontrolled::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalSingleDuctUncontrolled_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_SingleDuct_UncontrolledFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctUncontrolled","Availability"));
    }
    return result;
  }

  Schedule AirTerminalSingleDuctUncontrolled_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<AirTerminalSingleDuctUncontrolled_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctUncontrolled_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_UncontrolledFields::AvailabilityScheduleName,
                              "AirTerminalSingleDuctUncontrolled",
                              "Availability",
                              schedule);
    return result;
  }

  unsigned AirTerminalSingleDuctUncontrolled_Impl::inletPort()
  {
    return OS_AirTerminal_SingleDuct_UncontrolledFields::InletAirNodeName;
  }

  unsigned AirTerminalSingleDuctUncontrolled_Impl::outletPort()
  {
    return OS_AirTerminal_SingleDuct_UncontrolledFields::ZoneSupplyAirNodeName;
  }

  bool AirTerminalSingleDuctUncontrolled_Impl::addToNode(Node & node)
  {
    Model _model = node.model();

    if( boost::optional<ModelObject> outlet = node.outletModelObject() )
    {
      boost::optional<ThermalZone> thermalZone;

      if( boost::optional<PortList> portList = outlet->optionalCast<PortList>() )
      {
        thermalZone = portList->thermalZone(); 
      }

      if( thermalZone || (outlet->optionalCast<Mixer>() && node.airLoopHVAC()) )
      {
        if( boost::optional<ModelObject> inlet = node.inletModelObject() )
        {
          if( boost::optional<Splitter> splitter = inlet->optionalCast<Splitter>() )
          {
            boost::optional<ModelObject> sourceModelObject = node.inletModelObject();
            boost::optional<unsigned> sourcePort = node.connectedObjectPort(node.inletPort());

            if( sourcePort && sourceModelObject )
            {
              _model.connect( sourceModelObject.get(),
                              sourcePort.get(),
                              this->getObject<ModelObject>(),
                              inletPort() );

              _model.connect( this->getObject<ModelObject>(),
                              outletPort(),
                              node,
                              node.inletPort() );

              if( thermalZone )
              {
                AirTerminalSingleDuctUncontrolled mo = this->getObject<AirTerminalSingleDuctUncontrolled>();

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

  std::vector<IdfObject> AirTerminalSingleDuctUncontrolled_Impl::remove()
  {
    Model _model = this->model();
    ModelObject thisObject = this->getObject<ModelObject>();

    boost::optional<ModelObject> sourceModelObject = this->inletModelObject();
    boost::optional<unsigned> sourcePort = this->connectedObjectPort(this->inletPort());

    boost::optional<ModelObject> targetModelObject = this->outletModelObject();
    boost::optional<unsigned> targetPort = this->connectedObjectPort(this->outletPort());

    std::vector<ThermalZone> thermalZones = _model.getConcreteModelObjects<ThermalZone>();
    for( std::vector<ThermalZone>::iterator it = thermalZones.begin();
         it != thermalZones.end();
         ++it )
    {
      std::vector<ModelObject> equipment = it->equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
      {
        it->removeEquipment(thisObject);

        break;
      }
    }

    if( sourcePort && sourceModelObject
        && targetPort && targetModelObject )
    {
      _model.connect( sourceModelObject.get(),
                      sourcePort.get(),
                      targetModelObject.get(),
                      targetPort.get() );

      return ModelObject_Impl::remove();
    }
    else
    {
      model().disconnect(getObject<ModelObject>(),inletPort());
      model().disconnect(getObject<ModelObject>(),outletPort());

      return ModelObject_Impl::remove();
    }
  }

  bool AirTerminalSingleDuctUncontrolled_Impl::isRemovable() const
  {
    return true;
  }

  boost::optional<Schedule> AirTerminalSingleDuctUncontrolled_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_UncontrolledFields::AvailabilityScheduleName);
  }

  boost::optional<ModelObject> AirTerminalSingleDuctUncontrolled_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  bool AirTerminalSingleDuctUncontrolled_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  boost::optional<double> AirTerminalSingleDuctUncontrolled_Impl::maximumAirFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_UncontrolledFields::MaximumAirFlowRate,true);
  }

  OSOptionalQuantity AirTerminalSingleDuctUncontrolled_Impl::getMaximumAirFlowRate(bool returnIP) const {
    OptionalDouble value = maximumAirFlowRate();
    return getQuantityFromDouble(OS_AirTerminal_SingleDuct_UncontrolledFields::MaximumAirFlowRate, value, returnIP);
  }

  bool AirTerminalSingleDuctUncontrolled_Impl::isMaximumAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_UncontrolledFields::MaximumAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }


  bool AirTerminalSingleDuctUncontrolled_Impl::setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate) {
    bool result(false);
    if (maximumAirFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_UncontrolledFields::MaximumAirFlowRate, maximumAirFlowRate.get());
    }
    return result;
  }

  bool AirTerminalSingleDuctUncontrolled_Impl::setMaximumAirFlowRate(const OSOptionalQuantity& maximumAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (maximumAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_AirTerminal_SingleDuct_UncontrolledFields::MaximumAirFlowRate,maximumAirFlowRate.get());
      if (value) {
        result = setMaximumAirFlowRate(value);
      }
    }
    else {
      result = setMaximumAirFlowRate(value);
    }
    return result;
  }

  void AirTerminalSingleDuctUncontrolled_Impl::autosizeMaximumAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_UncontrolledFields::MaximumAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

} // detail

AirTerminalSingleDuctUncontrolled::AirTerminalSingleDuctUncontrolled(const Model& model,
                                                                     Schedule & availabilitySchedule)
  : StraightComponent(AirTerminalSingleDuctUncontrolled::iddObjectType(),model) 
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctUncontrolled_Impl>());

  setString(openstudio::OS_AirTerminal_SingleDuct_UncontrolledFields::MaximumAirFlowRate,"AutoSize" );

  setAvailabilitySchedule(availabilitySchedule);
}

Schedule AirTerminalSingleDuctUncontrolled::availabilitySchedule() const
{
  return getImpl<detail::AirTerminalSingleDuctUncontrolled_Impl>()->availabilitySchedule();
}

bool AirTerminalSingleDuctUncontrolled::setAvailabilitySchedule(Schedule & schedule)
{
  return getImpl<detail::AirTerminalSingleDuctUncontrolled_Impl>()->setAvailabilitySchedule(schedule);
}

AirTerminalSingleDuctUncontrolled::AirTerminalSingleDuctUncontrolled(boost::shared_ptr<detail::AirTerminalSingleDuctUncontrolled_Impl> p)
  : StraightComponent(p)
{}

IddObjectType AirTerminalSingleDuctUncontrolled::iddObjectType() {
  IddObjectType result(IddObjectType::OS_AirTerminal_SingleDuct_Uncontrolled);
  return result;
}

boost::optional<double> AirTerminalSingleDuctUncontrolled::maximumAirFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctUncontrolled_Impl>()->maximumAirFlowRate();
}

OSOptionalQuantity AirTerminalSingleDuctUncontrolled::getMaximumAirFlowRate(bool returnIP) const {
  return getImpl<detail::AirTerminalSingleDuctUncontrolled_Impl>()->getMaximumAirFlowRate(returnIP);
}

bool AirTerminalSingleDuctUncontrolled::isMaximumAirFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctUncontrolled_Impl>()->isMaximumAirFlowRateAutosized();
}

bool AirTerminalSingleDuctUncontrolled::setMaximumAirFlowRate(double maximumAirFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctUncontrolled_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
}

bool AirTerminalSingleDuctUncontrolled::setMaximumAirFlowRate(const Quantity& maximumAirFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctUncontrolled_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
}

void AirTerminalSingleDuctUncontrolled::autosizeMaximumAirFlowRate() {
  getImpl<detail::AirTerminalSingleDuctUncontrolled_Impl>()->autosizeMaximumAirFlowRate();
}

} // model
} // openstudio
