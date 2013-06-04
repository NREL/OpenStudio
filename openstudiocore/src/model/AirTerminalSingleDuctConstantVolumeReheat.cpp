/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/AirTerminalSingleDuctConstantVolumeReheat.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeReheat_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Connection.hpp>
#include <model/Connection_Impl.hpp>
#include <model/Connection.hpp>
#include <model/Connection_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/PortList.hpp>
#include <model/PortList_Impl.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/AirLoopHVACZoneMixer.hpp>
#include <model/AirLoopHVACZoneMixer_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Model.hpp>

#include <utilities/idd/OS_AirTerminal_SingleDuct_ConstantVolume_Reheat_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <boost/foreach.hpp>


namespace openstudio {
namespace model {

namespace detail {

  AirTerminalSingleDuctConstantVolumeReheat_Impl::AirTerminalSingleDuctConstantVolumeReheat_Impl(const IdfObject& idfObject,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctConstantVolumeReheat::iddObjectType());
  }

  AirTerminalSingleDuctConstantVolumeReheat_Impl::AirTerminalSingleDuctConstantVolumeReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == AirTerminalSingleDuctConstantVolumeReheat::iddObjectType());
  }

  AirTerminalSingleDuctConstantVolumeReheat_Impl::AirTerminalSingleDuctConstantVolumeReheat_Impl(const AirTerminalSingleDuctConstantVolumeReheat_Impl& other,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirTerminalSingleDuctConstantVolumeReheat_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirTerminalSingleDuctConstantVolumeReheat_Impl::iddObjectType() const {
    return AirTerminalSingleDuctConstantVolumeReheat::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalSingleDuctConstantVolumeReheat_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctConstantVolumeReheat","Availability"));
    }
    return result;
  }

  Schedule AirTerminalSingleDuctConstantVolumeReheat_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      BOOST_ASSERT(value);
      const_cast<AirTerminalSingleDuctConstantVolumeReheat_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    BOOST_ASSERT(value);
    return value.get();
  }

  unsigned AirTerminalSingleDuctConstantVolumeReheat_Impl::inletPort() 
  {
    return OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AirInletNodeName;
  }

  unsigned AirTerminalSingleDuctConstantVolumeReheat_Impl::outletPort() 
  {
    return OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AirOutletNodeName;
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::addToNode(Node & node)
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
                AirTerminalSingleDuctConstantVolumeReheat mo = this->getObject<AirTerminalSingleDuctConstantVolumeReheat>();

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

  std::vector<IdfObject> AirTerminalSingleDuctConstantVolumeReheat_Impl::remove()
  {
    Model _model = this->model();
    ModelObject thisObject = this->getObject<ModelObject>();

    std::vector<IdfObject> result;

    HVACComponent _reheatCoil = reheatCoil();

    boost::optional<ModelObject> sourceModelObject = this->inletModelObject();
    boost::optional<unsigned> sourcePort = this->connectedObjectPort(this->inletPort());
    
    boost::optional<ModelObject> targetModelObject = this->outletModelObject();
    boost::optional<unsigned> targetPort = this->connectedObjectPort(this->outletPort());

    std::vector<ThermalZone> thermalZones = _model.getModelObjects<ThermalZone>();
    for( std::vector<ThermalZone>::iterator it = thermalZones.begin();
         it != thermalZones.end();
         it++ )
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

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::isRemovable() const
  {
    return true;
  }

  std::vector<ModelObject> AirTerminalSingleDuctConstantVolumeReheat_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (OptionalHVACComponent intermediate = reheatCoil()) {
      result.push_back(*intermediate);
    }
    return result;
  }

  ModelObject AirTerminalSingleDuctConstantVolumeReheat_Impl::clone(Model model) const
  {
    AirTerminalSingleDuctConstantVolumeReheat modelObjectClone = StraightComponent_Impl::clone(model).cast<AirTerminalSingleDuctConstantVolumeReheat>();

    HVACComponent coil = this->reheatCoil();

    HVACComponent coilClone = coil.clone(model).cast<HVACComponent>();
    
    modelObjectClone.setReheatCoil(coilClone);

    return modelObjectClone;
  }

  Connection AirTerminalSingleDuctConstantVolumeReheat_Impl::airOutletNode() const {
    boost::optional<Connection> value = optionalAirOutletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Air Outlet Node attached.");
    }
    return value.get();
  }

  Connection AirTerminalSingleDuctConstantVolumeReheat_Impl::airInletNode() const {
    boost::optional<Connection> value = optionalAirInletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Air Inlet Node attached.");
    }
    return value.get();
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeReheat_Impl::maximumAirFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate,true);
  }

  OSOptionalQuantity AirTerminalSingleDuctConstantVolumeReheat_Impl::getMaximumAirFlowRate(bool returnIP) const {
    OptionalDouble value = maximumAirFlowRate();
    return getQuantityFromDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate, value, returnIP);
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::isMaximumAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<HVACComponent> AirTerminalSingleDuctConstantVolumeReheat_Impl::reheatCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ReheatCoilName);
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeReheat_Impl::maximumHotWaterorSteamFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate,true);
  }

  OSOptionalQuantity AirTerminalSingleDuctConstantVolumeReheat_Impl::getMaximumHotWaterorSteamFlowRate(bool returnIP) const {
    OptionalDouble value = maximumHotWaterorSteamFlowRate();
    return getQuantityFromDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate, value, returnIP);
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::isMaximumHotWaterorSteamFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirTerminalSingleDuctConstantVolumeReheat_Impl::minimumHotWaterorSteamFlowRate() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MinimumHotWaterorSteamFlowRate,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  Quantity AirTerminalSingleDuctConstantVolumeReheat_Impl::getMinimumHotWaterorSteamFlowRate(bool returnIP) const {
    OptionalDouble value = minimumHotWaterorSteamFlowRate();
    OSOptionalQuantity result = getQuantityFromDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MinimumHotWaterorSteamFlowRate, value, returnIP);
    BOOST_ASSERT(result.isSet());
    return result.get();
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::isMinimumHotWaterorSteamFlowRateDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MinimumHotWaterorSteamFlowRate);
  }

  double AirTerminalSingleDuctConstantVolumeReheat_Impl::convergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ConvergenceTolerance,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  Quantity AirTerminalSingleDuctConstantVolumeReheat_Impl::getConvergenceTolerance(bool returnIP) const {
    OptionalDouble value = convergenceTolerance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ConvergenceTolerance, value, returnIP);
    BOOST_ASSERT(result.isSet());
    return result.get();
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::isConvergenceToleranceDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ConvergenceTolerance);
  }

  double AirTerminalSingleDuctConstantVolumeReheat_Impl::maximumReheatAirTemperature() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumReheatAirTemperature,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  Quantity AirTerminalSingleDuctConstantVolumeReheat_Impl::getMaximumReheatAirTemperature(bool returnIP) const {
    OptionalDouble value = maximumReheatAirTemperature();
    OSOptionalQuantity result = getQuantityFromDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumReheatAirTemperature, value, returnIP);
    BOOST_ASSERT(result.isSet());
    return result.get();
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::isMaximumReheatAirTemperatureDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumReheatAirTemperature);
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AvailabilityScheduleName,
                              "AirTerminalSingleDuctConstantVolumeReheat",
                              "Availability",
                              schedule);
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setAirOutletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AirOutletNodeName, connection.handle());
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setAirInletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AirInletNodeName, connection.handle());
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate) {
    bool result(false);
    if (maximumAirFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate, maximumAirFlowRate.get());
    }
    else {
      resetMaximumAirFlowRate();
      result = true;
    }
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setMaximumAirFlowRate(const OSOptionalQuantity& maximumAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (maximumAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate,maximumAirFlowRate.get());
      if (value) {
        result = setMaximumAirFlowRate(value);
      }
    }
    else {
      result = setMaximumAirFlowRate(value);
    }
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeReheat_Impl::resetMaximumAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate, "");
    BOOST_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeReheat_Impl::autosizeMaximumAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate, "autosize");
    BOOST_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setReheatCoil(const boost::optional<HVACComponent>& heatingCoilName) {
    bool result(false);
    if (heatingCoilName) {
      result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ReheatCoilName, heatingCoilName.get().handle());
    }
    else {
      resetReheatCoil();
      result = true;
    }
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeReheat_Impl::resetReheatCoil() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ReheatCoilName, "");
    BOOST_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setMaximumHotWaterorSteamFlowRate(boost::optional<double> maximumHotWaterorSteamFlowRate) {
    bool result(false);
    if (maximumHotWaterorSteamFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate, maximumHotWaterorSteamFlowRate.get());
    }
    else {
      resetMaximumHotWaterorSteamFlowRate();
      result = true;
    }
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setMaximumHotWaterorSteamFlowRate(const OSOptionalQuantity& maximumHotWaterorSteamFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (maximumHotWaterorSteamFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate,maximumHotWaterorSteamFlowRate.get());
      if (value) {
        result = setMaximumHotWaterorSteamFlowRate(value);
      }
    }
    else {
      result = setMaximumHotWaterorSteamFlowRate(value);
    }
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeReheat_Impl::resetMaximumHotWaterorSteamFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate, "");
    BOOST_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeReheat_Impl::autosizeMaximumHotWaterorSteamFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate, "autosize");
    BOOST_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MinimumHotWaterorSteamFlowRate, minimumHotWaterorSteamFlowRate);
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setMinimumHotWaterorSteamFlowRate(const Quantity& minimumHotWaterorSteamFlowRate) {
    OptionalDouble value = getDoubleFromQuantity(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MinimumHotWaterorSteamFlowRate,minimumHotWaterorSteamFlowRate);
    if (!value) {
      return false;
    }
    return setMinimumHotWaterorSteamFlowRate(value.get());
  }

  void AirTerminalSingleDuctConstantVolumeReheat_Impl::resetMinimumHotWaterorSteamFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MinimumHotWaterorSteamFlowRate, "");
    BOOST_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setConvergenceTolerance(double convergenceTolerance) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ConvergenceTolerance, convergenceTolerance);
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setConvergenceTolerance(const Quantity& convergenceTolerance) {
    OptionalDouble value = getDoubleFromQuantity(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ConvergenceTolerance,convergenceTolerance);
    if (!value) {
      return false;
    }
    return setConvergenceTolerance(value.get());
  }

  void AirTerminalSingleDuctConstantVolumeReheat_Impl::resetConvergenceTolerance() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ConvergenceTolerance, "");
    BOOST_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setMaximumReheatAirTemperature(double maximumReheatAirTemperature) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumReheatAirTemperature, maximumReheatAirTemperature);
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setMaximumReheatAirTemperature(const Quantity& maximumReheatAirTemperature) {
    OptionalDouble value = getDoubleFromQuantity(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumReheatAirTemperature,maximumReheatAirTemperature);
    if (!value) {
      return false;
    }
    return setMaximumReheatAirTemperature(value.get());
  }

  void AirTerminalSingleDuctConstantVolumeReheat_Impl::resetMaximumReheatAirTemperature() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumReheatAirTemperature, "");
    BOOST_ASSERT(result);
  }

  boost::optional<Schedule> AirTerminalSingleDuctConstantVolumeReheat_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AvailabilityScheduleName);
  }

  boost::optional<Connection> AirTerminalSingleDuctConstantVolumeReheat_Impl::optionalAirOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AirOutletNodeName);
  }

  boost::optional<Connection> AirTerminalSingleDuctConstantVolumeReheat_Impl::optionalAirInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AirInletNodeName);
  }

  openstudio::OSOptionalQuantity AirTerminalSingleDuctConstantVolumeReheat_Impl::maximumAirFlowRate_SI() const {
    return getMaximumAirFlowRate(false);
  }

  openstudio::OSOptionalQuantity AirTerminalSingleDuctConstantVolumeReheat_Impl::maximumAirFlowRate_IP() const {
    return getMaximumAirFlowRate(true);
  }

  openstudio::OSOptionalQuantity AirTerminalSingleDuctConstantVolumeReheat_Impl::maximumHotWaterorSteamFlowRate_SI() const {
    return getMaximumHotWaterorSteamFlowRate(false);
  }

  openstudio::OSOptionalQuantity AirTerminalSingleDuctConstantVolumeReheat_Impl::maximumHotWaterorSteamFlowRate_IP() const {
    return getMaximumHotWaterorSteamFlowRate(true);
  }

  openstudio::Quantity AirTerminalSingleDuctConstantVolumeReheat_Impl::minimumHotWaterorSteamFlowRate_SI() const {
    return getMinimumHotWaterorSteamFlowRate(false);
  }

  openstudio::Quantity AirTerminalSingleDuctConstantVolumeReheat_Impl::minimumHotWaterorSteamFlowRate_IP() const {
    return getMinimumHotWaterorSteamFlowRate(true);
  }

  openstudio::Quantity AirTerminalSingleDuctConstantVolumeReheat_Impl::convergenceTolerance_SI() const {
    return getConvergenceTolerance(false);
  }

  openstudio::Quantity AirTerminalSingleDuctConstantVolumeReheat_Impl::convergenceTolerance_IP() const {
    return getConvergenceTolerance(true);
  }

  openstudio::Quantity AirTerminalSingleDuctConstantVolumeReheat_Impl::maximumReheatAirTemperature_SI() const {
    return getMaximumReheatAirTemperature(false);
  }

  openstudio::Quantity AirTerminalSingleDuctConstantVolumeReheat_Impl::maximumReheatAirTemperature_IP() const {
    return getMaximumReheatAirTemperature(true);
  }

  boost::optional<ModelObject> AirTerminalSingleDuctConstantVolumeReheat_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  boost::optional<ModelObject> AirTerminalSingleDuctConstantVolumeReheat_Impl::airOutletNodeAsModelObject() const {
    OptionalModelObject result = airOutletNode();
    return result;
  }

  boost::optional<ModelObject> AirTerminalSingleDuctConstantVolumeReheat_Impl::airInletNodeAsModelObject() const {
    OptionalModelObject result = airInletNode();
    return result;
  }

  boost::optional<ModelObject> AirTerminalSingleDuctConstantVolumeReheat_Impl::reheatCoilAsModelObject() const {
    OptionalModelObject result;
    OptionalHVACComponent intermediate = reheatCoil();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setAirOutletNodeAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalConnection intermediate = modelObject->optionalCast<Connection>();
      if (intermediate) {
        Connection connection(*intermediate);
        return setAirOutletNode(connection);
      }
    }
    return false;
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setAirInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalConnection intermediate = modelObject->optionalCast<Connection>();
      if (intermediate) {
        Connection connection(*intermediate);
        return setAirInletNode(connection);
      }
    }
    return false;
  }

  bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setReheatCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        HVACComponent heatingCoilName(*intermediate);
        return setReheatCoil(heatingCoilName);
      }
      else {
        return false;
      }
    }
    else {
      resetReheatCoil();
    }
    return true;
  }

} // detail

AirTerminalSingleDuctConstantVolumeReheat::AirTerminalSingleDuctConstantVolumeReheat(const Model& model)
  : StraightComponent(AirTerminalSingleDuctConstantVolumeReheat::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AvailabilityScheduleName
  //     OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AirOutletNodeName
  //     OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AirInletNodeName
  bool ok = true;
  // ok = setHandle();
  BOOST_ASSERT(ok);
  // ok = setAvailabilitySchedule();
  BOOST_ASSERT(ok);
  // ok = setAirOutletNode();
  BOOST_ASSERT(ok);
  // ok = setAirInletNode();
  BOOST_ASSERT(ok);
}

IddObjectType AirTerminalSingleDuctConstantVolumeReheat::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_Reheat);
}

Schedule AirTerminalSingleDuctConstantVolumeReheat::availabilitySchedule() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->availabilitySchedule();
}

Connection AirTerminalSingleDuctConstantVolumeReheat::airOutletNode() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->airOutletNode();
}

Connection AirTerminalSingleDuctConstantVolumeReheat::airInletNode() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->airInletNode();
}

boost::optional<double> AirTerminalSingleDuctConstantVolumeReheat::maximumAirFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->maximumAirFlowRate();
}

OSOptionalQuantity AirTerminalSingleDuctConstantVolumeReheat::getMaximumAirFlowRate(bool returnIP) const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->getMaximumAirFlowRate(returnIP);
}

bool AirTerminalSingleDuctConstantVolumeReheat::isMaximumAirFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->isMaximumAirFlowRateAutosized();
}

boost::optional<HVACComponent> AirTerminalSingleDuctConstantVolumeReheat::reheatCoil() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->reheatCoil();
}

boost::optional<double> AirTerminalSingleDuctConstantVolumeReheat::maximumHotWaterorSteamFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->maximumHotWaterorSteamFlowRate();
}

OSOptionalQuantity AirTerminalSingleDuctConstantVolumeReheat::getMaximumHotWaterorSteamFlowRate(bool returnIP) const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->getMaximumHotWaterorSteamFlowRate(returnIP);
}

bool AirTerminalSingleDuctConstantVolumeReheat::isMaximumHotWaterorSteamFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->isMaximumHotWaterorSteamFlowRateAutosized();
}

double AirTerminalSingleDuctConstantVolumeReheat::minimumHotWaterorSteamFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->minimumHotWaterorSteamFlowRate();
}

Quantity AirTerminalSingleDuctConstantVolumeReheat::getMinimumHotWaterorSteamFlowRate(bool returnIP) const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->getMinimumHotWaterorSteamFlowRate(returnIP);
}

bool AirTerminalSingleDuctConstantVolumeReheat::isMinimumHotWaterorSteamFlowRateDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->isMinimumHotWaterorSteamFlowRateDefaulted();
}

double AirTerminalSingleDuctConstantVolumeReheat::convergenceTolerance() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->convergenceTolerance();
}

Quantity AirTerminalSingleDuctConstantVolumeReheat::getConvergenceTolerance(bool returnIP) const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->getConvergenceTolerance(returnIP);
}

bool AirTerminalSingleDuctConstantVolumeReheat::isConvergenceToleranceDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->isConvergenceToleranceDefaulted();
}

double AirTerminalSingleDuctConstantVolumeReheat::maximumReheatAirTemperature() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->maximumReheatAirTemperature();
}

Quantity AirTerminalSingleDuctConstantVolumeReheat::getMaximumReheatAirTemperature(bool returnIP) const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->getMaximumReheatAirTemperature(returnIP);
}

bool AirTerminalSingleDuctConstantVolumeReheat::isMaximumReheatAirTemperatureDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->isMaximumReheatAirTemperatureDefaulted();
}

bool AirTerminalSingleDuctConstantVolumeReheat::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setAvailabilitySchedule(schedule);
}

bool AirTerminalSingleDuctConstantVolumeReheat::setAirOutletNode(const Connection& connection) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setAirOutletNode(connection);
}

bool AirTerminalSingleDuctConstantVolumeReheat::setAirInletNode(const Connection& connection) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setAirInletNode(connection);
}

bool AirTerminalSingleDuctConstantVolumeReheat::setMaximumAirFlowRate(double maximumAirFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
}

bool AirTerminalSingleDuctConstantVolumeReheat::setMaximumAirFlowRate(const Quantity& maximumAirFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
}

void AirTerminalSingleDuctConstantVolumeReheat::resetMaximumAirFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->resetMaximumAirFlowRate();
}

void AirTerminalSingleDuctConstantVolumeReheat::autosizeMaximumAirFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->autosizeMaximumAirFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeReheat::setReheatCoil(const HVACComponent& heatingCoilName) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setReheatCoil(heatingCoilName);
}

void AirTerminalSingleDuctConstantVolumeReheat::resetReheatCoil() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->resetReheatCoil();
}

bool AirTerminalSingleDuctConstantVolumeReheat::setMaximumHotWaterorSteamFlowRate(double maximumHotWaterorSteamFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setMaximumHotWaterorSteamFlowRate(maximumHotWaterorSteamFlowRate);
}

bool AirTerminalSingleDuctConstantVolumeReheat::setMaximumHotWaterorSteamFlowRate(const Quantity& maximumHotWaterorSteamFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setMaximumHotWaterorSteamFlowRate(maximumHotWaterorSteamFlowRate);
}

void AirTerminalSingleDuctConstantVolumeReheat::resetMaximumHotWaterorSteamFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->resetMaximumHotWaterorSteamFlowRate();
}

void AirTerminalSingleDuctConstantVolumeReheat::autosizeMaximumHotWaterorSteamFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->autosizeMaximumHotWaterorSteamFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeReheat::setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setMinimumHotWaterorSteamFlowRate(minimumHotWaterorSteamFlowRate);
}

bool AirTerminalSingleDuctConstantVolumeReheat::setMinimumHotWaterorSteamFlowRate(const Quantity& minimumHotWaterorSteamFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setMinimumHotWaterorSteamFlowRate(minimumHotWaterorSteamFlowRate);
}

void AirTerminalSingleDuctConstantVolumeReheat::resetMinimumHotWaterorSteamFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->resetMinimumHotWaterorSteamFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeReheat::setConvergenceTolerance(double convergenceTolerance) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setConvergenceTolerance(convergenceTolerance);
}

bool AirTerminalSingleDuctConstantVolumeReheat::setConvergenceTolerance(const Quantity& convergenceTolerance) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setConvergenceTolerance(convergenceTolerance);
}

void AirTerminalSingleDuctConstantVolumeReheat::resetConvergenceTolerance() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->resetConvergenceTolerance();
}

bool AirTerminalSingleDuctConstantVolumeReheat::setMaximumReheatAirTemperature(double maximumReheatAirTemperature) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setMaximumReheatAirTemperature(maximumReheatAirTemperature);
}

bool AirTerminalSingleDuctConstantVolumeReheat::setMaximumReheatAirTemperature(const Quantity& maximumReheatAirTemperature) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setMaximumReheatAirTemperature(maximumReheatAirTemperature);
}

void AirTerminalSingleDuctConstantVolumeReheat::resetMaximumReheatAirTemperature() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->resetMaximumReheatAirTemperature();
}

/// @cond
AirTerminalSingleDuctConstantVolumeReheat::AirTerminalSingleDuctConstantVolumeReheat(boost::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

