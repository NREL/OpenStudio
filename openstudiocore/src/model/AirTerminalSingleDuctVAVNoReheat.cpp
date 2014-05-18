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

#include <model/AirTerminalSingleDuctVAVNoReheat.hpp>
#include <model/AirTerminalSingleDuctVAVNoReheat_Impl.hpp>

#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/DesignSpecificationOutdoorAir.hpp>
#include <model/DesignSpecificationOutdoorAir_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <utilities/idd/IddFactory.hxx>

#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/PortList.hpp>
#include <model/PortList_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/AirLoopHVACZoneMixer.hpp>
#include <model/AirLoopHVACZoneMixer_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Model.hpp>
#include <utilities/idd/OS_AirTerminal_SingleDuct_VAV_NoReheat_FieldEnums.hxx>
//#include <utilities/units/Unit.hpp>
//#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
//#include <boost/foreach.hpp>

namespace openstudio {
namespace model {

namespace detail {

  AirTerminalSingleDuctVAVNoReheat_Impl::AirTerminalSingleDuctVAVNoReheat_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctVAVNoReheat::iddObjectType());
  }

  AirTerminalSingleDuctVAVNoReheat_Impl::AirTerminalSingleDuctVAVNoReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctVAVNoReheat::iddObjectType());
  }

  AirTerminalSingleDuctVAVNoReheat_Impl::AirTerminalSingleDuctVAVNoReheat_Impl(const AirTerminalSingleDuctVAVNoReheat_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirTerminalSingleDuctVAVNoReheat_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirTerminalSingleDuctVAVNoReheat_Impl::iddObjectType() const {
    return AirTerminalSingleDuctVAVNoReheat::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalSingleDuctVAVNoReheat_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_SingleDuct_VAV_NoReheatFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctVAVNoReheat","Availability"));
    }
    if (std::find(b,e,OS_AirTerminal_SingleDuct_VAV_NoReheatFields::MinimumAirFlowFractionScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctVAVNoReheat","Minimum Air Flow Fraction"));
    }
    return result;
  }

  Schedule AirTerminalSingleDuctVAVNoReheat_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<AirTerminalSingleDuctVAVNoReheat_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> AirTerminalSingleDuctVAVNoReheat_Impl::maximumAirFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate,true);
  }

  bool AirTerminalSingleDuctVAVNoReheat_Impl::isMaximumAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<std::string> AirTerminalSingleDuctVAVNoReheat_Impl::zoneMinimumAirFlowInputMethod() const {
    return getString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::ZoneMinimumAirFlowInputMethod,true);
  }

  boost::optional<double> AirTerminalSingleDuctVAVNoReheat_Impl::constantMinimumAirFlowFraction() const {
    return getDouble(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::ConstantMinimumAirFlowFraction,true);
  }

  bool AirTerminalSingleDuctVAVNoReheat_Impl::isConstantMinimumAirFlowFractionDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::ConstantMinimumAirFlowFraction);
  }

  boost::optional<double> AirTerminalSingleDuctVAVNoReheat_Impl::fixedMinimumAirFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::FixedMinimumAirFlowRate,true);
  }

  bool AirTerminalSingleDuctVAVNoReheat_Impl::isFixedMinimumAirFlowRateDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::FixedMinimumAirFlowRate);
  }

  boost::optional<Schedule> AirTerminalSingleDuctVAVNoReheat_Impl::minimumAirFlowFractionSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::MinimumAirFlowFractionScheduleName);
  }

  boost::optional<DesignSpecificationOutdoorAir> AirTerminalSingleDuctVAVNoReheat_Impl::designSpecificationOutdoorAirObject() const {
    return getObject<ModelObject>().getModelObjectTarget<DesignSpecificationOutdoorAir>(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::DesignSpecificationOutdoorAirObjectName);
  }

  bool AirTerminalSingleDuctVAVNoReheat_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::AvailabilityScheduleName,
                              "AirTerminalSingleDuctVAVNoReheat",
                              "Availability",
                              schedule);
    return result;
  }

    unsigned AirTerminalSingleDuctVAVNoReheat_Impl::inletPort()
  {
    return OS_AirTerminal_SingleDuct_VAV_NoReheatFields::AirInletNodeName;
  }

  unsigned AirTerminalSingleDuctVAVNoReheat_Impl::outletPort()
  {
    return OS_AirTerminal_SingleDuct_VAV_NoReheatFields::AirOutletNodeName;
  }

  bool AirTerminalSingleDuctVAVNoReheat_Impl::setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate) {
    bool result(false);
    if (maximumAirFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate, maximumAirFlowRate.get());
    }
    else {
      resetMaximumAirFlowRate();
      result = true;
    }
    return result;
  }

  void AirTerminalSingleDuctVAVNoReheat_Impl::resetMaximumAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctVAVNoReheat_Impl::autosizeMaximumAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctVAVNoReheat_Impl::setZoneMinimumAirFlowInputMethod(boost::optional<std::string> zoneMinimumAirFlowInputMethod) {
    bool result(false);
    if (zoneMinimumAirFlowInputMethod) {
      result = setString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::ZoneMinimumAirFlowInputMethod, zoneMinimumAirFlowInputMethod.get());
    }
    else {
      resetZoneMinimumAirFlowInputMethod();
      result = true;
    }
    return result;
  }

  void AirTerminalSingleDuctVAVNoReheat_Impl::resetZoneMinimumAirFlowInputMethod() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::ZoneMinimumAirFlowInputMethod, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctVAVNoReheat_Impl::setConstantMinimumAirFlowFraction(boost::optional<double> constantMinimumAirFlowFraction) {
    bool result(false);
    if (constantMinimumAirFlowFraction) {
      result = setDouble(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::ConstantMinimumAirFlowFraction, constantMinimumAirFlowFraction.get());
    }
    else {
      resetConstantMinimumAirFlowFraction();
      result = true;
    }
    OS_ASSERT(result);
    return result;
  }

  void AirTerminalSingleDuctVAVNoReheat_Impl::resetConstantMinimumAirFlowFraction() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::ConstantMinimumAirFlowFraction, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctVAVNoReheat_Impl::setFixedMinimumAirFlowRate(boost::optional<double> fixedMinimumAirFlowRate) {
    bool result(false);
    if (fixedMinimumAirFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::FixedMinimumAirFlowRate, fixedMinimumAirFlowRate.get());
    }
    else {
      resetFixedMinimumAirFlowRate();
      result = true;
    }
    OS_ASSERT(result);
    return result;
  }

  void AirTerminalSingleDuctVAVNoReheat_Impl::resetFixedMinimumAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::FixedMinimumAirFlowRate, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctVAVNoReheat_Impl::setMinimumAirFlowFractionSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::MinimumAirFlowFractionScheduleName,
                              "AirTerminalSingleDuctVAVNoReheat",
                              "Minimum Air Flow Fraction",
                              schedule);
    return result;
  }

  void AirTerminalSingleDuctVAVNoReheat_Impl::resetMinimumAirFlowFractionSchedule() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::MinimumAirFlowFractionScheduleName, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctVAVNoReheat_Impl::setDesignSpecificationOutdoorAirObject(const boost::optional<DesignSpecificationOutdoorAir>& designSpecificationOutdoorAir) {
    bool result(false);
    if (designSpecificationOutdoorAir) {
      result = setPointer(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::DesignSpecificationOutdoorAirObjectName, designSpecificationOutdoorAir.get().handle());
    }
    else {
      resetDesignSpecificationOutdoorAirObject();
      result = true;
    }
    return result;
  }

  void AirTerminalSingleDuctVAVNoReheat_Impl::resetDesignSpecificationOutdoorAirObject() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::DesignSpecificationOutdoorAirObjectName, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> AirTerminalSingleDuctVAVNoReheat_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::AvailabilityScheduleName);
  }

  std::vector<std::string> AirTerminalSingleDuctVAVNoReheat_Impl::zoneMinimumAirFlowInputMethodValues() const {
    return AirTerminalSingleDuctVAVNoReheat::zoneMinimumAirFlowInputMethodValues();
  }

  boost::optional<ModelObject> AirTerminalSingleDuctVAVNoReheat_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  boost::optional<ModelObject> AirTerminalSingleDuctVAVNoReheat_Impl::minimumAirFlowFractionScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = minimumAirFlowFractionSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> AirTerminalSingleDuctVAVNoReheat_Impl::designSpecificationOutdoorAirObjectAsModelObject() const {
    OptionalModelObject result;
    OptionalDesignSpecificationOutdoorAir intermediate = designSpecificationOutdoorAirObject();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool AirTerminalSingleDuctVAVNoReheat_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

bool AirTerminalSingleDuctVAVNoReheat_Impl::addToNode(Node & node)
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
                AirTerminalSingleDuctVAVNoReheat mo = this->getObject<AirTerminalSingleDuctVAVNoReheat>();

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

  std::vector<IdfObject> AirTerminalSingleDuctVAVNoReheat_Impl::remove()
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

  bool AirTerminalSingleDuctVAVNoReheat_Impl::isRemovable() const
  {
    return true;
  }

  bool AirTerminalSingleDuctVAVNoReheat_Impl::setMinimumAirFlowFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setMinimumAirFlowFractionSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetMinimumAirFlowFractionSchedule();
    }
    return true;
  }

  bool AirTerminalSingleDuctVAVNoReheat_Impl::setDesignSpecificationOutdoorAirObjectAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalDesignSpecificationOutdoorAir intermediate = modelObject->optionalCast<DesignSpecificationOutdoorAir>();
      if (intermediate) {
        DesignSpecificationOutdoorAir designSpecificationOutdoorAir(*intermediate);
        return setDesignSpecificationOutdoorAirObject(designSpecificationOutdoorAir);
      }
      else {
        return false;
      }
    }
    else {
      resetDesignSpecificationOutdoorAirObject();
    }
    return true;
  }

} // detail

AirTerminalSingleDuctVAVNoReheat::AirTerminalSingleDuctVAVNoReheat(const Model& model, Schedule& schedule)
  : StraightComponent(AirTerminalSingleDuctVAVNoReheat::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>());

  bool test = setAvailabilitySchedule(schedule);
  if (!test) {
    remove();
    LOG_AND_THROW("Unable to construct " << briefDescription() << ", because cannot set its "
        << "availability schedule to " << schedule.briefDescription() << ".");
  }

  this->autosizeMaximumAirFlowRate();
  this->setZoneMinimumAirFlowInputMethod("Constant");
  this->setConstantMinimumAirFlowFraction(0.3);
  this->setString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::DesignSpecificationOutdoorAirObjectName,"");

}

IddObjectType AirTerminalSingleDuctVAVNoReheat::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_SingleDuct_VAV_NoReheat);
}

std::vector<std::string> AirTerminalSingleDuctVAVNoReheat::zoneMinimumAirFlowInputMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirTerminal_SingleDuct_VAV_NoReheatFields::ZoneMinimumAirFlowInputMethod);
}

Schedule AirTerminalSingleDuctVAVNoReheat::availabilitySchedule() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->availabilitySchedule();
}

boost::optional<double> AirTerminalSingleDuctVAVNoReheat::maximumAirFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->maximumAirFlowRate();
}

bool AirTerminalSingleDuctVAVNoReheat::isMaximumAirFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->isMaximumAirFlowRateAutosized();
}

boost::optional<std::string> AirTerminalSingleDuctVAVNoReheat::zoneMinimumAirFlowInputMethod() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->zoneMinimumAirFlowInputMethod();
}

boost::optional<double> AirTerminalSingleDuctVAVNoReheat::constantMinimumAirFlowFraction() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->constantMinimumAirFlowFraction();
}
bool AirTerminalSingleDuctVAVNoReheat::isConstantMinimumAirFlowFractionDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->isConstantMinimumAirFlowFractionDefaulted();
}

boost::optional<double> AirTerminalSingleDuctVAVNoReheat::fixedMinimumAirFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->fixedMinimumAirFlowRate();
}

bool AirTerminalSingleDuctVAVNoReheat::isFixedMinimumAirFlowRateDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->isFixedMinimumAirFlowRateDefaulted();
}

boost::optional<Schedule> AirTerminalSingleDuctVAVNoReheat::minimumAirFlowFractionSchedule() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->minimumAirFlowFractionSchedule();
}

boost::optional<DesignSpecificationOutdoorAir> AirTerminalSingleDuctVAVNoReheat::designSpecificationOutdoorAirObject() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->designSpecificationOutdoorAirObject();
}

bool AirTerminalSingleDuctVAVNoReheat::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setAvailabilitySchedule(schedule);
}

bool AirTerminalSingleDuctVAVNoReheat::setMaximumAirFlowRate(double maximumAirFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
}

void AirTerminalSingleDuctVAVNoReheat::resetMaximumAirFlowRate() {
  getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->resetMaximumAirFlowRate();
}

void AirTerminalSingleDuctVAVNoReheat::autosizeMaximumAirFlowRate() {
  getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->autosizeMaximumAirFlowRate();
}

bool AirTerminalSingleDuctVAVNoReheat::setZoneMinimumAirFlowInputMethod(std::string zoneMinimumAirFlowInputMethod) {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setZoneMinimumAirFlowInputMethod(zoneMinimumAirFlowInputMethod);
}

void AirTerminalSingleDuctVAVNoReheat::resetZoneMinimumAirFlowInputMethod() {
  getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->resetZoneMinimumAirFlowInputMethod();
}

bool AirTerminalSingleDuctVAVNoReheat::setConstantMinimumAirFlowFraction(double constantMinimumAirFlowFraction) {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setConstantMinimumAirFlowFraction(constantMinimumAirFlowFraction);
}

void AirTerminalSingleDuctVAVNoReheat::resetConstantMinimumAirFlowFraction() {
  getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->resetConstantMinimumAirFlowFraction();
}

bool AirTerminalSingleDuctVAVNoReheat::setFixedMinimumAirFlowRate(double fixedMinimumAirFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setFixedMinimumAirFlowRate(fixedMinimumAirFlowRate);
}

void AirTerminalSingleDuctVAVNoReheat::resetFixedMinimumAirFlowRate() {
  getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->resetFixedMinimumAirFlowRate();
}

bool AirTerminalSingleDuctVAVNoReheat::setMinimumAirFlowFractionSchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setMinimumAirFlowFractionSchedule(schedule);
}

void AirTerminalSingleDuctVAVNoReheat::resetMinimumAirFlowFractionSchedule() {
  getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->resetMinimumAirFlowFractionSchedule();
}

bool AirTerminalSingleDuctVAVNoReheat::setDesignSpecificationOutdoorAirObject(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir) {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setDesignSpecificationOutdoorAirObject(designSpecificationOutdoorAir);
}

void AirTerminalSingleDuctVAVNoReheat::resetDesignSpecificationOutdoorAirObject() {
  getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->resetDesignSpecificationOutdoorAirObject();
}

/// @cond
AirTerminalSingleDuctVAVNoReheat::AirTerminalSingleDuctVAVNoReheat(boost::shared_ptr<detail::AirTerminalSingleDuctVAVNoReheat_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

