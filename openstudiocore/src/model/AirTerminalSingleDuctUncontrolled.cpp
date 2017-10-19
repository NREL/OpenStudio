/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "AirTerminalSingleDuctUncontrolled.hpp"
#include "AirTerminalSingleDuctUncontrolled_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "Model.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include <utilities/idd/OS_AirTerminal_SingleDuct_Uncontrolled_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

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

AirTerminalSingleDuctUncontrolled::AirTerminalSingleDuctUncontrolled(std::shared_ptr<detail::AirTerminalSingleDuctUncontrolled_Impl> p)
  : StraightComponent(std::move(p))
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
