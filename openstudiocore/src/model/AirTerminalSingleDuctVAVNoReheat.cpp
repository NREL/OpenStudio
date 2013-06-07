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

#include <model/AirTerminalSingleDuctVAVNoReheat.hpp>
#include <model/AirTerminalSingleDuctVAVNoReheat_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
//#include <model/Connection.hpp>
//#include <model/Connection_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/DesignSpecificationOutdoorAir.hpp>
#include <model/DesignSpecificationOutdoorAir_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_AirTerminal_SingleDuct_VAV_NoReheat_FieldEnums.hxx>
#include <utilities/units/Unit.hpp>
#include <utilities/core/Assert.hpp>
#include <model/ScheduleCompact.hpp>

//copy from AirTerminal_SingleDuct_VAV_Reheat
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
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <boost/foreach.hpp>

namespace openstudio {
namespace model {

namespace detail {

  AirTerminalSingleDuctVAVNoReheat_Impl::AirTerminalSingleDuctVAVNoReheat_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctVAVNoReheat::iddObjectType());
  }

  AirTerminalSingleDuctVAVNoReheat_Impl::AirTerminalSingleDuctVAVNoReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == AirTerminalSingleDuctVAVNoReheat::iddObjectType());
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
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
    }
    return value.get();
  }

  //Connection AirTerminalSingleDuctVAVNoReheat_Impl::airOutletNode() const {
  //  boost::optional<Connection> value = optionalAirOutletNode();
  //  if (!value) {
  //    LOG_AND_THROW(briefDescription() << " does not have an Air Outlet Node attached.");
  //  }
  //  return value.get();
  //}

  //Connection AirTerminalSingleDuctVAVNoReheat_Impl::airInletNode() const {
  //  boost::optional<Connection> value = optionalAirInletNode();
  //  if (!value) {
  //    LOG_AND_THROW(briefDescription() << " does not have an Air Inlet Node attached.");
  //  }
  //  return value.get();
  //}

  boost::optional<double> AirTerminalSingleDuctVAVNoReheat_Impl::maximumAirFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate,true);
  }

//  OSOptionalQuantity AirTerminalSingleDuctVAVNoReheat_Impl::getMaximumAirFlowRate(bool returnIP) const {
//    OptionalDouble value = maximumAirFlowRate();
//    return getQuantityFromDouble(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate, value, returnIP);
//  }

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

//  OSOptionalQuantity AirTerminalSingleDuctVAVNoReheat_Impl::getConstantMinimumAirFlowFraction(bool returnIP) const {
//    OptionalDouble value = constantMinimumAirFlowFraction();
//    return getQuantityFromDouble(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::ConstantMinimumAirFlowFraction, value, returnIP);
//  }

  boost::optional<double> AirTerminalSingleDuctVAVNoReheat_Impl::fixedMinimumAirFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::FixedMinimumAirFlowRate,true);
  }

//  OSOptionalQuantity AirTerminalSingleDuctVAVNoReheat_Impl::getFixedMinimumAirFlowRate(bool returnIP) const {
//    OptionalDouble value = fixedMinimumAirFlowRate();
//    return getQuantityFromDouble(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::FixedMinimumAirFlowRate, value, returnIP);
//  }

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

 ///* bool AirTerminalSingleDuctVAVNoReheat_Impl::setAirOutletNode(const Connection& connection) {
 //   bool result = setPointer(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::AirOutletNodeName, connection.handle());
 //   return result;
 // }

 // bool AirTerminalSingleDuctVAVNoReheat_Impl::setAirInletNode(const Connection& connection) {
 //   bool result = setPointer(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::AirInletNodeName, connection.handle());
 //   return result;
 // }*/

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

//  bool AirTerminalSingleDuctVAVNoReheat_Impl::setMaximumAirFlowRate(const OSOptionalQuantity& maximumAirFlowRate) {
//    bool result(false);
//    OptionalDouble value;
//    if (maximumAirFlowRate.isSet()) {
//      value = getDoubleFromQuantity(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate,maximumAirFlowRate.get());
//      if (value) {
//        result = setMaximumAirFlowRate(value);
//      }
//    }
//    else {
//      result = setMaximumAirFlowRate(value);
//    }
//    return result;
//  }

  void AirTerminalSingleDuctVAVNoReheat_Impl::resetMaximumAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate, "");
    BOOST_ASSERT(result);
  }

  void AirTerminalSingleDuctVAVNoReheat_Impl::autosizeMaximumAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate, "autosize");
    BOOST_ASSERT(result);
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
    BOOST_ASSERT(result);
  }

  void AirTerminalSingleDuctVAVNoReheat_Impl::setConstantMinimumAirFlowFraction(boost::optional<double> constantMinimumAirFlowFraction) {
    bool result(false);
    if (constantMinimumAirFlowFraction) {
      result = setDouble(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::ConstantMinimumAirFlowFraction, constantMinimumAirFlowFraction.get());
    }
    else {
      resetConstantMinimumAirFlowFraction();
      result = true;
    }
    BOOST_ASSERT(result);
  }

//  bool AirTerminalSingleDuctVAVNoReheat_Impl::setConstantMinimumAirFlowFraction(const OSOptionalQuantity& constantMinimumAirFlowFraction) {
//    bool result(false);
//    OptionalDouble value;
//    if (constantMinimumAirFlowFraction.isSet()) {
//      value = getDoubleFromQuantity(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::ConstantMinimumAirFlowFraction,constantMinimumAirFlowFraction.get());
//      if (value) {
//        setConstantMinimumAirFlowFraction(value);
//		result = true;
//      }
//    }
//    else {
//      setConstantMinimumAirFlowFraction(value);
//      result = true;
//    }
//    return result;
//  }

  void AirTerminalSingleDuctVAVNoReheat_Impl::resetConstantMinimumAirFlowFraction() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::ConstantMinimumAirFlowFraction, "");
    BOOST_ASSERT(result);
  }

  void AirTerminalSingleDuctVAVNoReheat_Impl::setFixedMinimumAirFlowRate(boost::optional<double> fixedMinimumAirFlowRate) {
    bool result(false);
    if (fixedMinimumAirFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::FixedMinimumAirFlowRate, fixedMinimumAirFlowRate.get());
    }
    else {
      resetFixedMinimumAirFlowRate();
      result = true;
    }
    BOOST_ASSERT(result);
  }

//  bool AirTerminalSingleDuctVAVNoReheat_Impl::setFixedMinimumAirFlowRate(const OSOptionalQuantity& fixedMinimumAirFlowRate) {
//    bool result(false);
//    OptionalDouble value;
//    if (fixedMinimumAirFlowRate.isSet()) {
//      value = getDoubleFromQuantity(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::FixedMinimumAirFlowRate,fixedMinimumAirFlowRate.get());
//      if (value) {
//        setFixedMinimumAirFlowRate(value);
//		result = true;
//      }
//    }
//    else {
//      setFixedMinimumAirFlowRate(value);
//      result = true;
//    }
//    return result;
//  }

  void AirTerminalSingleDuctVAVNoReheat_Impl::resetFixedMinimumAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::FixedMinimumAirFlowRate, "");
    BOOST_ASSERT(result);
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
    BOOST_ASSERT(result);
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
    BOOST_ASSERT(result);
  }

  boost::optional<Schedule> AirTerminalSingleDuctVAVNoReheat_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::AvailabilityScheduleName);
  }

 ///* boost::optional<Connection> AirTerminalSingleDuctVAVNoReheat_Impl::optionalAirOutletNode() const {
 //   return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::AirOutletNodeName);
 // }

 // boost::optional<Connection> AirTerminalSingleDuctVAVNoReheat_Impl::optionalAirInletNode() const {
 //   return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_SingleDuct_VAV_NoReheatFields::AirInletNodeName);
 // }*/

//  openstudio::OSOptionalQuantity AirTerminalSingleDuctVAVNoReheat_Impl::maximumAirFlowRate_SI() const {
//    return getMaximumAirFlowRate(false);
//  }

//  openstudio::OSOptionalQuantity AirTerminalSingleDuctVAVNoReheat_Impl::maximumAirFlowRate_IP() const {
//    return getMaximumAirFlowRate(true);
//  }

  std::vector<std::string> AirTerminalSingleDuctVAVNoReheat_Impl::zoneMinimumAirFlowInputMethodValues() const {
    return AirTerminalSingleDuctVAVNoReheat::zoneMinimumAirFlowInputMethodValues();
  }

//  openstudio::OSOptionalQuantity AirTerminalSingleDuctVAVNoReheat_Impl::constantMinimumAirFlowFraction_SI() const {
//    return getConstantMinimumAirFlowFraction(false);
//  }

//  openstudio::OSOptionalQuantity AirTerminalSingleDuctVAVNoReheat_Impl::constantMinimumAirFlowFraction_IP() const {
//    return getConstantMinimumAirFlowFraction(true);
//  }

//  openstudio::OSOptionalQuantity AirTerminalSingleDuctVAVNoReheat_Impl::fixedMinimumAirFlowRate_SI() const {
//    return getFixedMinimumAirFlowRate(false);
//  }

//  openstudio::OSOptionalQuantity AirTerminalSingleDuctVAVNoReheat_Impl::fixedMinimumAirFlowRate_IP() const {
//    return getFixedMinimumAirFlowRate(true);
//  }

  boost::optional<ModelObject> AirTerminalSingleDuctVAVNoReheat_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  //boost::optional<ModelObject> AirTerminalSingleDuctVAVNoReheat_Impl::airOutletNodeAsModelObject() const {
  //  OptionalModelObject result = airOutletNode();
  //  return result;
  //}

  //boost::optional<ModelObject> AirTerminalSingleDuctVAVNoReheat_Impl::airInletNodeAsModelObject() const {
  //  OptionalModelObject result = airInletNode();
  //  return result;
  //}

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

  ///*bool AirTerminalSingleDuctVAVNoReheat_Impl::setAirOutletNodeAsModelObject(const boost::optional<ModelObject>& modelObject) {
  //  if (modelObject) {
  //    OptionalConnection intermediate = modelObject->optionalCast<Connection>();
  //    if (intermediate) {
  //      Connection connection(*intermediate);
  //      return setAirOutletNode(connection);
  //    }
  //  }
  //  return false;
  //}*/

 ///* bool AirTerminalSingleDuctVAVNoReheat_Impl::setAirInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject) {
 //   if (modelObject) {
 //     OptionalConnection intermediate = modelObject->optionalCast<Connection>();
 //     if (intermediate) {
 //       Connection connection(*intermediate);
 //       return setAirInletNode(connection);
 //     }
 //   }
 //   return false;
 // }*/

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
                AirTerminalSingleDuctVAVNoReheat mo = this->getObject<AirTerminalSingleDuctVAVNoReheat>();

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

AirTerminalSingleDuctVAVNoReheat::AirTerminalSingleDuctVAVNoReheat(const Model& model)
  : StraightComponent(AirTerminalSingleDuctVAVNoReheat::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_AirTerminal_SingleDuct_VAV_NoReheatFields::AvailabilityScheduleName
  //     OS_AirTerminal_SingleDuct_VAV_NoReheatFields::AirOutletNodeName
  //     OS_AirTerminal_SingleDuct_VAV_NoReheatFields::AirInletNodeName
  bool ok = true;
  // ok = setHandle();
  BOOST_ASSERT(ok);

  model::ScheduleCompact availabilitySchedule(model);
  setAvailabilitySchedule(availabilitySchedule); 

  BOOST_ASSERT(ok);
  // ok = setAirOutletNode();
  BOOST_ASSERT(ok);
  // ok = setAirInletNode();
  BOOST_ASSERT(ok);
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

//Connection AirTerminalSingleDuctVAVNoReheat::airOutletNode() const {
//  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->airOutletNode();
//}
//
//Connection AirTerminalSingleDuctVAVNoReheat::airInletNode() const {
//  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->airInletNode();
//}

boost::optional<double> AirTerminalSingleDuctVAVNoReheat::maximumAirFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->maximumAirFlowRate();
}

//OSOptionalQuantity AirTerminalSingleDuctVAVNoReheat::getMaximumAirFlowRate(bool returnIP) const {
//  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->getMaximumAirFlowRate(returnIP);
//}

bool AirTerminalSingleDuctVAVNoReheat::isMaximumAirFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->isMaximumAirFlowRateAutosized();
}

boost::optional<std::string> AirTerminalSingleDuctVAVNoReheat::zoneMinimumAirFlowInputMethod() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->zoneMinimumAirFlowInputMethod();
}

boost::optional<double> AirTerminalSingleDuctVAVNoReheat::constantMinimumAirFlowFraction() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->constantMinimumAirFlowFraction();
}

//OSOptionalQuantity AirTerminalSingleDuctVAVNoReheat::getConstantMinimumAirFlowFraction(bool returnIP) const {
//  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->getConstantMinimumAirFlowFraction(returnIP);
//}

boost::optional<double> AirTerminalSingleDuctVAVNoReheat::fixedMinimumAirFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->fixedMinimumAirFlowRate();
}

//OSOptionalQuantity AirTerminalSingleDuctVAVNoReheat::getFixedMinimumAirFlowRate(bool returnIP) const {
//  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->getFixedMinimumAirFlowRate(returnIP);
//}

boost::optional<Schedule> AirTerminalSingleDuctVAVNoReheat::minimumAirFlowFractionSchedule() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->minimumAirFlowFractionSchedule();
}

boost::optional<DesignSpecificationOutdoorAir> AirTerminalSingleDuctVAVNoReheat::designSpecificationOutdoorAirObject() const {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->designSpecificationOutdoorAirObject();
}

bool AirTerminalSingleDuctVAVNoReheat::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setAvailabilitySchedule(schedule);
}

//bool AirTerminalSingleDuctVAVNoReheat::setAirOutletNode(const Connection& connection) {
//  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setAirOutletNode(connection);
//}
//
//bool AirTerminalSingleDuctVAVNoReheat::setAirInletNode(const Connection& connection) {
//  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setAirInletNode(connection);
//}

bool AirTerminalSingleDuctVAVNoReheat::setMaximumAirFlowRate(double maximumAirFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
}

//bool AirTerminalSingleDuctVAVNoReheat::setMaximumAirFlowRate(const Quantity& maximumAirFlowRate) {
//  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
//}

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

void AirTerminalSingleDuctVAVNoReheat::setConstantMinimumAirFlowFraction(double constantMinimumAirFlowFraction) {
  getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setConstantMinimumAirFlowFraction(constantMinimumAirFlowFraction);
}

//bool AirTerminalSingleDuctVAVNoReheat::setConstantMinimumAirFlowFraction(const Quantity& constantMinimumAirFlowFraction) {
//  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setConstantMinimumAirFlowFraction(constantMinimumAirFlowFraction);
//}

void AirTerminalSingleDuctVAVNoReheat::resetConstantMinimumAirFlowFraction() {
  getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->resetConstantMinimumAirFlowFraction();
}

void AirTerminalSingleDuctVAVNoReheat::setFixedMinimumAirFlowRate(double fixedMinimumAirFlowRate) {
  getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setFixedMinimumAirFlowRate(fixedMinimumAirFlowRate);
}

//bool AirTerminalSingleDuctVAVNoReheat::setFixedMinimumAirFlowRate(const Quantity& fixedMinimumAirFlowRate) {
//  return getImpl<detail::AirTerminalSingleDuctVAVNoReheat_Impl>()->setFixedMinimumAirFlowRate(fixedMinimumAirFlowRate);
//}

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

