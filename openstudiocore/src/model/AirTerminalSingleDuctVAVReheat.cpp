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

#include <model/AirTerminalSingleDuctVAVReheat.hpp>
#include <model/AirTerminalSingleDuctVAVReheat_Impl.hpp>
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
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <utilities/idd/OS_AirTerminal_SingleDuct_VAV_Reheat_FieldEnums.hxx>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <boost/foreach.hpp>

namespace openstudio {

namespace model {

namespace detail{

  AirTerminalSingleDuctVAVReheat_Impl::AirTerminalSingleDuctVAVReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctVAVReheat::iddObjectType());
  }

  AirTerminalSingleDuctVAVReheat_Impl::AirTerminalSingleDuctVAVReheat_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctVAVReheat::iddObjectType());
  }

  AirTerminalSingleDuctVAVReheat_Impl::AirTerminalSingleDuctVAVReheat_Impl(
      const AirTerminalSingleDuctVAVReheat_Impl& other, 
      Model_Impl* model, 
      bool keepHandle) 
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  // Get all output variable names that could be associated with this object. 
  const std::vector<std::string>& AirTerminalSingleDuctVAVReheat_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirTerminalSingleDuctVAVReheat_Impl::iddObjectType() const {
    return AirTerminalSingleDuctVAVReheat::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalSingleDuctVAVReheat_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_SingleDuct_VAV_ReheatFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctVAVReheat","Availability"));
    }
    if (std::find(b,e,OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctVAVReheat","Minimum Air Flow Fraction"));
    }
    return result;
  }

  Schedule AirTerminalSingleDuctVAVReheat_Impl::availabilitySchedule()  const
  {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<AirTerminalSingleDuctVAVReheat_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_VAV_ReheatFields::AvailabilityScheduleName,
                              "AirTerminalSingleDuctVAVReheat",
                              "Availability",
                              schedule);
    return result;
  }

  unsigned AirTerminalSingleDuctVAVReheat_Impl::inletPort() 
  {
    return OS_AirTerminal_SingleDuct_VAV_ReheatFields::AirInletNodeName;
  }

  unsigned AirTerminalSingleDuctVAVReheat_Impl::outletPort() 
  {
    return OS_AirTerminal_SingleDuct_VAV_ReheatFields::AirOutletNodeName;
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::addToNode(Node & node)
  {
    Model _model = node.model();

    if( boost::optional<ModelObject> outlet = node.outletModelObject() )
    {
      boost::optional<ThermalZone> thermalZone;

      if( boost::optional<PortList> portList = outlet->optionalCast<PortList>()  )
      {
        thermalZone = portList->thermalZone();
      }

      if( thermalZone || outlet->optionalCast<AirLoopHVACZoneMixer>() )
      {
        if( boost::optional<ModelObject> inlet = node.inletModelObject() )
        {
          if( boost::optional<AirLoopHVACZoneSplitter> splitter = inlet->optionalCast<AirLoopHVACZoneSplitter>() )
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
                AirTerminalSingleDuctVAVReheat mo = this->getObject<AirTerminalSingleDuctVAVReheat>();

                thermalZone->addEquipment(mo);
                thermalZone->setHeatingPriority(mo,1);
                thermalZone->setCoolingPriority(mo,1);
              }

              return true; 
            }
          }
        }
      }
    }

    return false;
  }

  std::vector<IdfObject> AirTerminalSingleDuctVAVReheat_Impl::remove()
  {
    Model _model = this->model();
    ModelObject thisObject = this->getObject<ModelObject>();

    HVACComponent _reheatCoil = reheatCoil();

    boost::optional<ModelObject> sourceModelObject = this->inletModelObject();
    boost::optional<unsigned> sourcePort = this->connectedObjectPort(this->inletPort());
    
    boost::optional<ModelObject> targetModelObject = this->outletModelObject();
    boost::optional<unsigned> targetPort = this->connectedObjectPort(this->outletPort());

    std::vector<ThermalZone> thermalZones = _model.getModelObjects<ThermalZone>();
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

            if( boost::optional<PlantLoop> loop = _reheatCoil.plantLoop() )
            {
              loop->removeDemandBranchWithComponent(_reheatCoil);
            }

            return StraightComponent_Impl::remove();
          }
        }
      }
    }

    model().disconnect(getObject<ModelObject>(),inletPort());
    model().disconnect(getObject<ModelObject>(),outletPort());

    if( boost::optional<PlantLoop> loop = _reheatCoil.plantLoop() )
    {
      loop->removeDemandBranchWithComponent(_reheatCoil);
    }

    return StraightComponent_Impl::remove();
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::isRemovable() const
  {
    return true;
  }

  std::vector<ModelObject> AirTerminalSingleDuctVAVReheat_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (OptionalHVACComponent intermediate = optionalReheatCoil()) {
      result.push_back(*intermediate);
    }
    return result;
  }

  ModelObject AirTerminalSingleDuctVAVReheat_Impl::clone(Model model) const
  {
    AirTerminalSingleDuctVAVReheat modelObjectClone = StraightComponent_Impl::clone(model).cast<AirTerminalSingleDuctVAVReheat>();

    HVACComponent coil = this->reheatCoil();

    HVACComponent coilClone = coil.clone(model).cast<HVACComponent>();
    
    modelObjectClone.setReheatCoil(coilClone);

    return modelObjectClone;
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::maximumAirFlowRate() const
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate);
  }

  void AirTerminalSingleDuctVAVReheat_Impl::setMaximumAirFlowRate( double value )
  {
    this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate,value);
  }

  void AirTerminalSingleDuctVAVReheat_Impl::autosizeMaximumAirFlowRate()
  {
    this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate,"Autocalculate");
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::isMaximumAirFlowRateAutosized() const
  {
    boost::optional<std::string> text = this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate);
    if( text )
    {
      if( istringEqual(text.get(),"Autocalculate") )
      {
        return true;
      }
      else if( istringEqual(text.get(),"AutoSize") )
      {
        return true;
      }
    }

    return false;
  }

  std::string AirTerminalSingleDuctVAVReheat_Impl::zoneMinimumAirFlowMethod()
  {
    return this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod).get();
  }

  void AirTerminalSingleDuctVAVReheat_Impl::setZoneMinimumAirFlowMethod( std::string value )
  {
    if( istringEqual(value,"Constant") )
    {
      this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod,"Constant");
    }
    else if( istringEqual(value,"FixedFlowRate") )
    {
      this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod,"FixedFlowRate");
    }
    else if( istringEqual(value,"Scheduled") )
    {
      this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod,"Scheduled");
    }
  }

  double AirTerminalSingleDuctVAVReheat_Impl::constantMinimumAirFlowFraction()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ConstantMinimumAirFlowFraction).get();
  }

  void AirTerminalSingleDuctVAVReheat_Impl::setConstantMinimumAirFlowFraction( double value )
  {
    this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ConstantMinimumAirFlowFraction,value);
  }

  double AirTerminalSingleDuctVAVReheat_Impl::fixedMinimumAirFlowRate()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::FixedMinimumAirFlowRate).get();
  }

  void AirTerminalSingleDuctVAVReheat_Impl::setFixedMinimumAirFlowRate( double value )
  {
    this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::FixedMinimumAirFlowRate,value);
  }

  boost::optional<Schedule> AirTerminalSingleDuctVAVReheat_Impl::minimumAirFlowFractionSchedule() const
  {
    return this->getObject<AirTerminalSingleDuctVAVReheat>().getModelObjectTarget<Schedule>(
        OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName);
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setMinimumAirFlowFractionSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName,
                              "AirTerminalSingleDuctVAVReheat",
                              "Minimum Air Flow Fraction",
                              schedule);
    return result;
  }

  void AirTerminalSingleDuctVAVReheat_Impl::resetMinimumAirFlowFractionSchedule() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::maximumHotWaterOrSteamFlowRate()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate);
  }

  void AirTerminalSingleDuctVAVReheat_Impl::setMaximumHotWaterOrSteamFlowRate( double value )
  {
    this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate,value);
  }

  void AirTerminalSingleDuctVAVReheat_Impl::autosizeMaximumHotWaterOrSteamFlowRate()
  {
    this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate,"Autocalculate");
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::isMaximumHotWaterOrSteamFlowRateAutosized() const
  {
    boost::optional<std::string> text = this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate);
    if( text )
    {
      if( istringEqual(text.get(),"Autocalculate") )
      {
        return true;
      }
      else if( istringEqual(text.get(),"AutoSize") )
      {
        return true;
      }
    }

    return false;
  }

  double AirTerminalSingleDuctVAVReheat_Impl::minimumHotWaterOrSteamFlowRate()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumHotWaterorSteamFlowRate).get();
  }

  void AirTerminalSingleDuctVAVReheat_Impl::setMinimumHotWaterOrStreamFlowRate( double value )
  {
    this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumHotWaterorSteamFlowRate,value);
  }

  double AirTerminalSingleDuctVAVReheat_Impl::convergenceTolerance()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ConvergenceTolerance).get();
  }

  void AirTerminalSingleDuctVAVReheat_Impl::setConvergenceTolerance( double value )
  {
    this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ConvergenceTolerance,value);
  }

  std::string AirTerminalSingleDuctVAVReheat_Impl::damperHeatingAction()
  {
    return this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::DamperHeatingAction).get();
  }

  void AirTerminalSingleDuctVAVReheat_Impl::setDamperHeatingAction( std::string value )
  {
    if( istringEqual(value,"Normal") )
    {
      this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::DamperHeatingAction,"Normal");
    }
    else if( istringEqual(value,"Reverse") )
    {
      this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::DamperHeatingAction,"Reverse");
    }
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::maximumFlowPerZoneFloorAreaDuringReheat()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat);
  }

  void AirTerminalSingleDuctVAVReheat_Impl::setMaximumFlowPerZoneFloorAreaDuringReheat( double value )
  {
    this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat,value);
  }

  void AirTerminalSingleDuctVAVReheat_Impl::autosizeMaximumFlowPerZoneFloorAreaDuringReheat()
  {
    this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat,"Autocalculate");
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::isMaximumFlowPerZoneFloorAreaDuringReheatAutosized() const
  {
    boost::optional<std::string> text = this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat);
    if( text )
    {
      if( istringEqual(text.get(),"Autocalculate") )
      {
        return true;
      }
      else if( istringEqual(text.get(),"AutoSize") )
      {
        return true;
      }
    }

    return false;
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::maximumFlowFractionDuringReheat()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat);
  }

  void AirTerminalSingleDuctVAVReheat_Impl::setMaximumFlowFractionDuringReheat( double value )  
  {
    this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat,value);
  }

  void AirTerminalSingleDuctVAVReheat_Impl::autosizeMaximumFlowFractionDuringReheat()
  {
    this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat,"Autocalculate");
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::isMaximumFlowFractionDuringReheatAutosized() const
  {
    boost::optional<std::string> text = this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat);
    if( text )
    {
      if( istringEqual(text.get(),"Autocalculate") )
      {
        return true;
      }
      else if( istringEqual(text.get(),"AutoSize") )
      {
        return true;
      }
    }

    return false;
  }

  double AirTerminalSingleDuctVAVReheat_Impl::maximumReheatAirTemperature()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumReheatAirTemperature).get();
  }

  void AirTerminalSingleDuctVAVReheat_Impl::setMaximumReheatAirTemperature( double value )
  {
    this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumReheatAirTemperature,value);
  }

  HVACComponent AirTerminalSingleDuctVAVReheat_Impl::reheatCoil() const
  {
    boost::optional<HVACComponent> coil = optionalReheatCoil();
    OS_ASSERT(coil);
    return coil.get();
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setReheatCoil(HVACComponent & coil)
  {
    bool result = false;

    switch(coil.iddObject().type().value())
    {
    case openstudio::IddObjectType::OS_Coil_Heating_Gas :
      {
        result = true;

        break;
      }
    case openstudio::IddObjectType::OS_Coil_Heating_Water :
      {
        result = true;

        break;
      }
    case openstudio::IddObjectType::OS_Coil_Heating_Electric :
      {
        result = true;

        break;
      }
    default:
      {
        LOG(Warn, "Unsupported or invalid IddObjectType: '" << coil.iddObject().name() << "'");

        result = false;
      }
    }

    if( result )
    {
      result = this->setPointer(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ReheatCoilName,coil.handle());
    } 

    return result;
  }

  boost::optional<HVACComponent> AirTerminalSingleDuctVAVReheat_Impl::optionalReheatCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ReheatCoilName);
  }

  boost::optional<Schedule> AirTerminalSingleDuctVAVReheat_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_VAV_ReheatFields::AvailabilityScheduleName);
  }

  boost::optional<ModelObject> AirTerminalSingleDuctVAVReheat_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  boost::optional<ModelObject> AirTerminalSingleDuctVAVReheat_Impl::minimumAirFlowFractionScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = minimumAirFlowFractionSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setMinimumAirFlowFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

} // detail

AirTerminalSingleDuctVAVReheat::AirTerminalSingleDuctVAVReheat( const Model& model, 
                                                                Schedule & availabilitySchedule,
                                                                HVACComponent & coil )
  : StraightComponent(AirTerminalSingleDuctVAVReheat::iddObjectType(),model) 
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>());

  bool test = setAvailabilitySchedule(availabilitySchedule);
  if (!test) {
    remove();
    LOG_AND_THROW("Unable to construct " << briefDescription() << ", because cannot set its "
        << "availability schedule to " << availabilitySchedule.briefDescription() << ".");
  }

  this->setReheatCoil(coil);

  this->autosizeMaximumAirFlowRate();

  this->setZoneMinimumAirFlowMethod("Constant");

  this->setConstantMinimumAirFlowFraction(0.3);

  this->setFixedMinimumAirFlowRate(0.0);

  this->setMaximumFlowPerZoneFloorAreaDuringReheat(0.0);

  this->setMinimumHotWaterOrStreamFlowRate(0.0);

  this->setConvergenceTolerance(0.001);

  this->setDamperHeatingAction("Normal");

  this->autosizeMaximumFlowPerZoneFloorAreaDuringReheat();

  this->autosizeMaximumFlowFractionDuringReheat();

  this->setMaximumReheatAirTemperature(35.0);

  this->autosizeMaximumHotWaterOrSteamFlowRate();

  this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::DesignSpecificationOutdoorAirObjectName,"");
}

Schedule AirTerminalSingleDuctVAVReheat::availabilitySchedule() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->availabilitySchedule();
}

bool AirTerminalSingleDuctVAVReheat::setAvailabilitySchedule(Schedule& schedule)
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setAvailabilitySchedule(schedule);
}

AirTerminalSingleDuctVAVReheat::AirTerminalSingleDuctVAVReheat(boost::shared_ptr<detail::AirTerminalSingleDuctVAVReheat_Impl> p)
  : StraightComponent(p)
{}

IddObjectType AirTerminalSingleDuctVAVReheat::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_SingleDuct_VAV_Reheat);
}

boost::optional<double> AirTerminalSingleDuctVAVReheat::maximumAirFlowRate() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumAirFlowRate();
}

void AirTerminalSingleDuctVAVReheat::setMaximumAirFlowRate( double value )
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumAirFlowRate(value);
}

void AirTerminalSingleDuctVAVReheat::autosizeMaximumAirFlowRate()
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizeMaximumAirFlowRate();
}

bool AirTerminalSingleDuctVAVReheat::isMaximumAirFlowRateAutosized() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->isMaximumAirFlowRateAutosized();
}

std::string AirTerminalSingleDuctVAVReheat::zoneMinimumAirFlowMethod()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->zoneMinimumAirFlowMethod();
}

void AirTerminalSingleDuctVAVReheat::setZoneMinimumAirFlowMethod( std::string value )
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setZoneMinimumAirFlowMethod(value);
}

double AirTerminalSingleDuctVAVReheat::constantMinimumAirFlowFraction()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->constantMinimumAirFlowFraction();
}

void AirTerminalSingleDuctVAVReheat::setConstantMinimumAirFlowFraction( double value )
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setConstantMinimumAirFlowFraction(value);
}

double AirTerminalSingleDuctVAVReheat::fixedMinimumAirFlowRate()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->fixedMinimumAirFlowRate();
}

void AirTerminalSingleDuctVAVReheat::setFixedMinimumAirFlowRate( double value )
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setFixedMinimumAirFlowRate(value);
}

boost::optional<Schedule> AirTerminalSingleDuctVAVReheat::minimumAirFlowFractionSchedule() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->minimumAirFlowFractionSchedule();
}

bool AirTerminalSingleDuctVAVReheat::setMinimumAirFlowFractionSchedule(Schedule & schedule )
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMinimumAirFlowFractionSchedule(schedule);
}

void AirTerminalSingleDuctVAVReheat::resetMinimumAirFlowFractionSchedule() {
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->resetMinimumAirFlowFractionSchedule();
}

boost::optional<double> AirTerminalSingleDuctVAVReheat::maximumHotWaterOrSteamFlowRate()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumHotWaterOrSteamFlowRate();
}

void AirTerminalSingleDuctVAVReheat::setMaximumHotWaterOrSteamFlowRate( double value )
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumHotWaterOrSteamFlowRate(value);
}

void AirTerminalSingleDuctVAVReheat::autosizeMaximumHotWaterOrSteamFlowRate()
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizeMaximumHotWaterOrSteamFlowRate();
}

bool AirTerminalSingleDuctVAVReheat::isMaximumHotWaterOrSteamFlowRateAutosized() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->isMaximumHotWaterOrSteamFlowRateAutosized();
}

double AirTerminalSingleDuctVAVReheat::minimumHotWaterOrSteamFlowRate()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->minimumHotWaterOrSteamFlowRate();
}

void AirTerminalSingleDuctVAVReheat::setMinimumHotWaterOrStreamFlowRate( double value )
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMinimumHotWaterOrStreamFlowRate(value);
}

double AirTerminalSingleDuctVAVReheat::convergenceTolerance()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->convergenceTolerance();
}

void AirTerminalSingleDuctVAVReheat::setConvergenceTolerance( double value )
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setConvergenceTolerance(value);
}

std::string AirTerminalSingleDuctVAVReheat::damperHeatingAction()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->damperHeatingAction();
}

void AirTerminalSingleDuctVAVReheat::setDamperHeatingAction( std::string value )
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setDamperHeatingAction(value);
}

boost::optional<double> AirTerminalSingleDuctVAVReheat::maximumFlowPerZoneFloorAreaDuringReheat()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumFlowPerZoneFloorAreaDuringReheat();
}

void AirTerminalSingleDuctVAVReheat::setMaximumFlowPerZoneFloorAreaDuringReheat( double value )
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumFlowPerZoneFloorAreaDuringReheat(value);
}

void AirTerminalSingleDuctVAVReheat::autosizeMaximumFlowPerZoneFloorAreaDuringReheat()
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizeMaximumFlowPerZoneFloorAreaDuringReheat();
}

bool AirTerminalSingleDuctVAVReheat::isMaximumFlowPerZoneFloorAreaDuringReheatAutosized() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->isMaximumFlowPerZoneFloorAreaDuringReheatAutosized();
}

boost::optional<double> AirTerminalSingleDuctVAVReheat::maximumFlowFractionDuringReheat()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumFlowFractionDuringReheat();
}

void AirTerminalSingleDuctVAVReheat::setMaximumFlowFractionDuringReheat( double value )  
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumFlowFractionDuringReheat(value);
}

void AirTerminalSingleDuctVAVReheat::autosizeMaximumFlowFractionDuringReheat()
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizeMaximumFlowFractionDuringReheat();
}

bool AirTerminalSingleDuctVAVReheat::isMaximumFlowFractionDuringReheatAutosized() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->isMaximumFlowFractionDuringReheatAutosized();
}

double AirTerminalSingleDuctVAVReheat::maximumReheatAirTemperature()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumReheatAirTemperature();
}

void AirTerminalSingleDuctVAVReheat::setMaximumReheatAirTemperature( double value )
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumReheatAirTemperature(value);
}

HVACComponent AirTerminalSingleDuctVAVReheat::reheatCoil() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->reheatCoil();
}

bool AirTerminalSingleDuctVAVReheat::setReheatCoil(HVACComponent & coil)
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setReheatCoil(coil);
}

} // model

} // openstudio

