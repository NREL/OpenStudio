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

#include <model/AirTerminalSingleDuctParallelPIUReheat.hpp>
#include <model/AirTerminalSingleDuctParallelPIUReheat_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/PortList.hpp>
#include <model/PortList_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/AirLoopHVACZoneMixer.hpp>
#include <model/AirLoopHVACZoneMixer_Impl.hpp>
#include <model/Model.hpp>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_AirTerminal_SingleDuct_ParallelPIU_Reheat_FieldEnums.hxx>
#include <utilities/core/Assert.hpp>

namespace openstudio {

namespace model {

namespace detail {

  AirTerminalSingleDuctParallelPIUReheat_Impl::AirTerminalSingleDuctParallelPIUReheat_Impl(
      const IdfObject& idfObject, 
      Model_Impl* model, 
      bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctParallelPIUReheat::iddObjectType());
  }

  AirTerminalSingleDuctParallelPIUReheat_Impl::AirTerminalSingleDuctParallelPIUReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctParallelPIUReheat::iddObjectType());
  }

  AirTerminalSingleDuctParallelPIUReheat_Impl::AirTerminalSingleDuctParallelPIUReheat_Impl(const AirTerminalSingleDuctParallelPIUReheat_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirTerminalSingleDuctParallelPIUReheat_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirTerminalSingleDuctParallelPIUReheat_Impl::iddObjectType() const {
    return AirTerminalSingleDuctParallelPIUReheat::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalSingleDuctParallelPIUReheat_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctParallelPIUReheat","Availability"));
    }
    return result;
  }

  boost::optional<double> AirTerminalSingleDuctParallelPIUReheat_Impl::maximumPrimaryAirFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumPrimaryAirFlowRate,true);
  }

  bool AirTerminalSingleDuctParallelPIUReheat_Impl::isMaximumPrimaryAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumPrimaryAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> AirTerminalSingleDuctParallelPIUReheat_Impl::maximumSecondaryAirFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumSecondaryAirFlowRate,true);
  }

  bool AirTerminalSingleDuctParallelPIUReheat_Impl::isMaximumSecondaryAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumSecondaryAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> AirTerminalSingleDuctParallelPIUReheat_Impl::minimumPrimaryAirFlowFraction() const {
    return getDouble(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MinimumPrimaryAirFlowFraction,true);
  }

  bool AirTerminalSingleDuctParallelPIUReheat_Impl::isMinimumPrimaryAirFlowFractionAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MinimumPrimaryAirFlowFraction, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> AirTerminalSingleDuctParallelPIUReheat_Impl::fanOnFlowFraction() const {
    return getDouble(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::FanOnFlowFraction,true);
  }

  bool AirTerminalSingleDuctParallelPIUReheat_Impl::isFanOnFlowFractionAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::FanOnFlowFraction, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> AirTerminalSingleDuctParallelPIUReheat_Impl::maximumHotWaterorSteamFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumHotWaterorSteamFlowRate,true);
  }

  bool AirTerminalSingleDuctParallelPIUReheat_Impl::isMaximumHotWaterorSteamFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumHotWaterorSteamFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  double AirTerminalSingleDuctParallelPIUReheat_Impl::minimumHotWaterorSteamFlowRate() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MinimumHotWaterorSteamFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctParallelPIUReheat_Impl::isMinimumHotWaterorSteamFlowRateDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MinimumHotWaterorSteamFlowRate);
  }

  double AirTerminalSingleDuctParallelPIUReheat_Impl::convergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::ConvergenceTolerance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctParallelPIUReheat_Impl::isConvergenceToleranceDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::ConvergenceTolerance);
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::setMaximumPrimaryAirFlowRate(boost::optional<double> maximumPrimaryAirFlowRate) {
    bool result = false;
    if (maximumPrimaryAirFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumPrimaryAirFlowRate, maximumPrimaryAirFlowRate.get());
    } else {
      result = setString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumPrimaryAirFlowRate, "");
    }
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::autosizeMaximumPrimaryAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumPrimaryAirFlowRate, "Autosize");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::setMaximumSecondaryAirFlowRate(boost::optional<double> maximumSecondaryAirFlowRate) {
    bool result = false;
    if (maximumSecondaryAirFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumSecondaryAirFlowRate, maximumSecondaryAirFlowRate.get());
    } else {
      result = setString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumSecondaryAirFlowRate, "");
    }
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::autosizeMaximumSecondaryAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumSecondaryAirFlowRate, "Autosize");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::setMinimumPrimaryAirFlowFraction(boost::optional<double> minimumPrimaryAirFlowFraction) {
    bool result = false;
    if (minimumPrimaryAirFlowFraction) {
      result = setDouble(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MinimumPrimaryAirFlowFraction, minimumPrimaryAirFlowFraction.get());
    } else {
      result = setString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MinimumPrimaryAirFlowFraction, "");
    }
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::autosizeMinimumPrimaryAirFlowFraction() {
    bool result = setString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MinimumPrimaryAirFlowFraction, "Autosize");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::setFanOnFlowFraction(boost::optional<double> fanOnFlowFraction) {
    bool result = false;
    if (fanOnFlowFraction) {
      result = setDouble(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::FanOnFlowFraction, fanOnFlowFraction.get());
    } else {
      result = setString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::FanOnFlowFraction, "");
    }
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::autosizeFanOnFlowFraction() {
    bool result = setString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::FanOnFlowFraction, "Autosize");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::setMaximumHotWaterorSteamFlowRate(boost::optional<double> maximumHotWaterorSteamFlowRate) {
    bool result = false;
    if (maximumHotWaterorSteamFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumHotWaterorSteamFlowRate, maximumHotWaterorSteamFlowRate.get());
    } else {
      result = setString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumHotWaterorSteamFlowRate, "");
    }
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::resetMaximumHotWaterorSteamFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumHotWaterorSteamFlowRate, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::autosizeMaximumHotWaterorSteamFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumHotWaterorSteamFlowRate, "Autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctParallelPIUReheat_Impl::setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MinimumHotWaterorSteamFlowRate, minimumHotWaterorSteamFlowRate);
    return result;
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::resetMinimumHotWaterorSteamFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MinimumHotWaterorSteamFlowRate, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctParallelPIUReheat_Impl::setConvergenceTolerance(double convergenceTolerance) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::ConvergenceTolerance, convergenceTolerance);
    return result;
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::resetConvergenceTolerance() {
    bool result = setString(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::ConvergenceTolerance, "");
    OS_ASSERT(result);
  }

  unsigned AirTerminalSingleDuctParallelPIUReheat_Impl::inletPort() 
  {
    return OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::SupplyAirInletNodeName;
  }

  unsigned AirTerminalSingleDuctParallelPIUReheat_Impl::outletPort() 
  {
    return OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::OutletNodeName;
  }
    
  HVACComponent AirTerminalSingleDuctParallelPIUReheat_Impl::reheatCoil() const
  {
    boost::optional<HVACComponent> hvacComponent = optionalReheatCoil();
    OS_ASSERT(hvacComponent);
    return hvacComponent.get();
  }

  HVACComponent AirTerminalSingleDuctParallelPIUReheat_Impl::fan() const
  {
    boost::optional<HVACComponent> hvacComponent = optionalFan();
    OS_ASSERT(hvacComponent);
    return hvacComponent.get();
  }

  Schedule AirTerminalSingleDuctParallelPIUReheat_Impl::availabilitySchedule() const
  {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<AirTerminalSingleDuctParallelPIUReheat_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::setReheatCoil( HVACComponent & hvacComponent )
  {
    bool isTypeCorrect = false;

    if( hvacComponent.iddObjectType() == IddObjectType::OS_Coil_Heating_Electric )
    {
      isTypeCorrect = true;
    }
    else if( hvacComponent.iddObjectType() == IddObjectType::OS_Coil_Heating_Gas )
    {
      isTypeCorrect = true;
    }
    else if( hvacComponent.iddObjectType() == IddObjectType::OS_Coil_Heating_Water )
    {
      isTypeCorrect = true;
    }

    if( isTypeCorrect )
    {
      setPointer(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::ReheatCoilName,hvacComponent.handle());
    }
  }

  void AirTerminalSingleDuctParallelPIUReheat_Impl::setFan( HVACComponent & hvacComponent )
  {
    bool isTypeCorrect = false;

    if( hvacComponent.iddObjectType() == IddObjectType::OS_Fan_ConstantVolume )
    {
      isTypeCorrect = true;
    }

    if( isTypeCorrect )
    {
      setPointer(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::FanName,hvacComponent.handle());
    }
  }

  bool AirTerminalSingleDuctParallelPIUReheat_Impl::setAvailabilitySchedule( Schedule & schedule ) {
    return setSchedule(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::AvailabilityScheduleName,
                       "AirTerminalSingleDuctParallelPIUReheat",
                       "Availability",
                       schedule);
  }

  std::vector<ModelObject> AirTerminalSingleDuctParallelPIUReheat_Impl::children() const {
    std::vector<ModelObject> result;
    if (OptionalHVACComponent intermediate = optionalReheatCoil()) {
      result.push_back(*intermediate);
    }
    if (OptionalHVACComponent intermediate = optionalFan()) {
      result.push_back(*intermediate);
    }
    return result;
  }

  ModelObject AirTerminalSingleDuctParallelPIUReheat_Impl::clone(Model model) const
  {
    AirTerminalSingleDuctParallelPIUReheat modelObjectClone = StraightComponent_Impl::clone(model).cast<AirTerminalSingleDuctParallelPIUReheat>();

    modelObjectClone.setString(modelObjectClone.secondaryAirInletPort(),""); 

    // clone coil

    HVACComponent coil = this->reheatCoil();

    HVACComponent coilClone = coil.clone(model).cast<HVACComponent>();
    
    modelObjectClone.setReheatCoil(coilClone);

    // clone fan

    HVACComponent fan = this->fan();

    HVACComponent fanClone = fan.clone(model).cast<HVACComponent>();

    modelObjectClone.setFan(fanClone);


    return modelObjectClone;
  }

  bool AirTerminalSingleDuctParallelPIUReheat_Impl::addToNode(Node & node)
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

              ModelObject thisObject = getObject<ModelObject>();

              _model.connect( sourceModelObject.get(),
                              sourcePort.get(),
                              inletNode,
                              inletNode.inletPort() );
              
              _model.connect( inletNode,
                              inletNode.outletPort(),
                              thisObject,
                              inletPort() );

              _model.connect( thisObject,
                              outletPort(),
                              node,
                              node.inletPort() );

              if( thermalZone )
              {
                Node secondaryInletNode(_model);

                PortList exhaustPortList = thermalZone->exhaustPortList();

                _model.connect( exhaustPortList,
                                exhaustPortList.nextPort(),
                                secondaryInletNode,
                                secondaryInletNode.inletPort() );

                _model.connect( secondaryInletNode,
                                secondaryInletNode.outletPort(),
                                thisObject,
                                secondaryAirInletPort() );

                ModelObject mo = this->getObject<ModelObject>();

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

  std::vector<IdfObject> AirTerminalSingleDuctParallelPIUReheat_Impl::remove()
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

    if( boost::optional<Node> secondaryAirInletNode = this->secondaryAirInletNode() )
    {
      secondaryAirInletNode->remove();
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

  boost::optional<Node> AirTerminalSingleDuctParallelPIUReheat_Impl::secondaryAirInletNode() const
  {
    boost::optional<ModelObject> mo = connectedObject(secondaryAirInletPort());

    if( mo )
    {
      if( boost::optional<Node> node = mo->optionalCast<Node>() )
      {
        return node;
      }
    }

    return boost::none;
  }

  unsigned AirTerminalSingleDuctParallelPIUReheat_Impl::secondaryAirInletPort() const
  {
    return OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::SecondaryAirInletNodeName;
  }

  boost::optional<Schedule> AirTerminalSingleDuctParallelPIUReheat_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::AvailabilityScheduleName);
  }

  boost::optional<HVACComponent> AirTerminalSingleDuctParallelPIUReheat_Impl::optionalFan() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::FanName);
  }

  boost::optional<HVACComponent> AirTerminalSingleDuctParallelPIUReheat_Impl::optionalReheatCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_ParallelPIU_ReheatFields::ReheatCoilName);
  }

  boost::optional<ModelObject> AirTerminalSingleDuctParallelPIUReheat_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  bool AirTerminalSingleDuctParallelPIUReheat_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

} // detail

AirTerminalSingleDuctParallelPIUReheat::AirTerminalSingleDuctParallelPIUReheat( const Model& model,
                                                                                Schedule & schedule,
                                                                                HVACComponent & fan,
                                                                                HVACComponent & reheatCoil )
  : StraightComponent(AirTerminalSingleDuctParallelPIUReheat::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>());

  bool test = setAvailabilitySchedule(schedule);
  if (!test) {
    remove();
    LOG_AND_THROW("Could not construct " << briefDescription() << ", because could not set its "
        << "availability schedule to " << schedule.briefDescription() << ".");
  }

  setFan(fan);

  setReheatCoil(reheatCoil);

  autosizeMaximumHotWaterorSteamFlowRate();

  setMinimumHotWaterorSteamFlowRate(0.0);

  setConvergenceTolerance(0.001);

  autosizeMaximumPrimaryAirFlowRate();

  autosizeMaximumSecondaryAirFlowRate();

  autosizeMinimumPrimaryAirFlowFraction();

  autosizeFanOnFlowFraction();
}

IddObjectType AirTerminalSingleDuctParallelPIUReheat::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_SingleDuct_ParallelPIU_Reheat);
}

boost::optional<double> AirTerminalSingleDuctParallelPIUReheat::maximumPrimaryAirFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->maximumPrimaryAirFlowRate();
}

bool AirTerminalSingleDuctParallelPIUReheat::isMaximumPrimaryAirFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->isMaximumPrimaryAirFlowRateAutosized();
}

boost::optional<double> AirTerminalSingleDuctParallelPIUReheat::maximumSecondaryAirFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->maximumSecondaryAirFlowRate();
}

bool AirTerminalSingleDuctParallelPIUReheat::isMaximumSecondaryAirFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->isMaximumSecondaryAirFlowRateAutosized();
}

boost::optional<double> AirTerminalSingleDuctParallelPIUReheat::minimumPrimaryAirFlowFraction() const {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->minimumPrimaryAirFlowFraction();
}

bool AirTerminalSingleDuctParallelPIUReheat::isMinimumPrimaryAirFlowFractionAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->isMinimumPrimaryAirFlowFractionAutosized();
}

boost::optional<double> AirTerminalSingleDuctParallelPIUReheat::fanOnFlowFraction() const {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->fanOnFlowFraction();
}

bool AirTerminalSingleDuctParallelPIUReheat::isFanOnFlowFractionAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->isFanOnFlowFractionAutosized();
}

boost::optional<double> AirTerminalSingleDuctParallelPIUReheat::maximumHotWaterorSteamFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->maximumHotWaterorSteamFlowRate();
}

bool AirTerminalSingleDuctParallelPIUReheat::isMaximumHotWaterorSteamFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->isMaximumHotWaterorSteamFlowRateAutosized();
}

double AirTerminalSingleDuctParallelPIUReheat::minimumHotWaterorSteamFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->minimumHotWaterorSteamFlowRate();
}

bool AirTerminalSingleDuctParallelPIUReheat::isMinimumHotWaterorSteamFlowRateDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->isMinimumHotWaterorSteamFlowRateDefaulted();
}

double AirTerminalSingleDuctParallelPIUReheat::convergenceTolerance() const {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->convergenceTolerance();
}

bool AirTerminalSingleDuctParallelPIUReheat::isConvergenceToleranceDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->isConvergenceToleranceDefaulted();
}

void AirTerminalSingleDuctParallelPIUReheat::setMaximumPrimaryAirFlowRate(double maximumPrimaryAirFlowRate) {
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setMaximumPrimaryAirFlowRate(maximumPrimaryAirFlowRate);
}

void AirTerminalSingleDuctParallelPIUReheat::autosizeMaximumPrimaryAirFlowRate() {
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->autosizeMaximumPrimaryAirFlowRate();
}

void AirTerminalSingleDuctParallelPIUReheat::setMaximumSecondaryAirFlowRate(double maximumSecondaryAirFlowRate) {
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setMaximumSecondaryAirFlowRate(maximumSecondaryAirFlowRate);
}

void AirTerminalSingleDuctParallelPIUReheat::autosizeMaximumSecondaryAirFlowRate() {
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->autosizeMaximumSecondaryAirFlowRate();
}

void AirTerminalSingleDuctParallelPIUReheat::setMinimumPrimaryAirFlowFraction(double minimumPrimaryAirFlowFraction) {
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setMinimumPrimaryAirFlowFraction(minimumPrimaryAirFlowFraction);
}

void AirTerminalSingleDuctParallelPIUReheat::autosizeMinimumPrimaryAirFlowFraction() {
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->autosizeMinimumPrimaryAirFlowFraction();
}

void AirTerminalSingleDuctParallelPIUReheat::setFanOnFlowFraction(double fanOnFlowFraction) {
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setFanOnFlowFraction(fanOnFlowFraction);
}

void AirTerminalSingleDuctParallelPIUReheat::autosizeFanOnFlowFraction() {
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->autosizeFanOnFlowFraction();
}

void AirTerminalSingleDuctParallelPIUReheat::setMaximumHotWaterorSteamFlowRate(double maximumHotWaterorSteamFlowRate) {
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setMaximumHotWaterorSteamFlowRate(maximumHotWaterorSteamFlowRate);
}

void AirTerminalSingleDuctParallelPIUReheat::resetMaximumHotWaterorSteamFlowRate() {
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->resetMaximumHotWaterorSteamFlowRate();
}

void AirTerminalSingleDuctParallelPIUReheat::autosizeMaximumHotWaterorSteamFlowRate() {
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->autosizeMaximumHotWaterorSteamFlowRate();
}

bool AirTerminalSingleDuctParallelPIUReheat::setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setMinimumHotWaterorSteamFlowRate(minimumHotWaterorSteamFlowRate);
}

void AirTerminalSingleDuctParallelPIUReheat::resetMinimumHotWaterorSteamFlowRate() {
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->resetMinimumHotWaterorSteamFlowRate();
}

bool AirTerminalSingleDuctParallelPIUReheat::setConvergenceTolerance(double convergenceTolerance) {
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setConvergenceTolerance(convergenceTolerance);
}

void AirTerminalSingleDuctParallelPIUReheat::resetConvergenceTolerance() {
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->resetConvergenceTolerance();
}
  
HVACComponent AirTerminalSingleDuctParallelPIUReheat::reheatCoil() const
{
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->reheatCoil();
}

HVACComponent AirTerminalSingleDuctParallelPIUReheat::fan() const
{
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->fan();
}

Schedule AirTerminalSingleDuctParallelPIUReheat::availabilitySchedule() const
{
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->availabilitySchedule();
}

void AirTerminalSingleDuctParallelPIUReheat::setReheatCoil( HVACComponent & hvacComponent )
{
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setReheatCoil(hvacComponent);
}

void AirTerminalSingleDuctParallelPIUReheat::setFan( HVACComponent & hvacComponent )
{
  getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setFan(hvacComponent);
}

bool AirTerminalSingleDuctParallelPIUReheat::setAvailabilitySchedule(Schedule& schedule )
{
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setAvailabilitySchedule(schedule);
}

boost::optional<Node> AirTerminalSingleDuctParallelPIUReheat::secondaryAirInletNode() const
{
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->secondaryAirInletNode();
}

unsigned AirTerminalSingleDuctParallelPIUReheat::secondaryAirInletPort() const
{
  return getImpl<detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->secondaryAirInletPort();
}

/// @cond

AirTerminalSingleDuctParallelPIUReheat::AirTerminalSingleDuctParallelPIUReheat(boost::shared_ptr<detail::AirTerminalSingleDuctParallelPIUReheat_Impl> impl)
  : StraightComponent(impl)
{}

/// @endcond

} // model

} // openstudio

